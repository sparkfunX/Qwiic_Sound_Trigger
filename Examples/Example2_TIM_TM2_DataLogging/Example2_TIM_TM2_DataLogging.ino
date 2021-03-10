/*
  Qwiic Sound Trigger (VM1010) Example: accurate sound event logging with the u-blox ZED-F9P and UBX TIM_TM2 messages
  By: Paul Clark
  SparkFun Electronics
  Date: March 10th, 2021
  License: MIT. Please see LICENSE.md for more details.

  Feel like supporting our work? Buy a board from SparkFun!
  Qwiic Sound Trigger:                 https://www.sparkfun.com/products/17979
  ZED-F9P RTK SMA:                     https://www.sparkfun.com/products/16481
  ZED-F9P RTK2:                        https://www.sparkfun.com/products/15136
  MicroMod Data Logging Carrier Board: https://www.sparkfun.com/products/16829
  MicroMod Artemis Processor Board:    https://www.sparkfun.com/products/16401

  Based on the u-blox GNSS library example DataLoggingExample2_TIM_TM2:
  Configuring the ZED-F9P GNSS to automatically send TIM TM2 reports over I2C and log them to file on SD card
  
  This example shows how to configure the u-blox ZED-F9P GNSS to send TIM TM2 reports when a sound event is detected
  and automatically log the data to SD card in UBX format.

  This code is intended to be run on the MicroMod Data Logging Carrier Board using the Artemis Processor
  but can be adapted to any board by changing the chip select pin and SPI definitions.

  Hardware Connections:
  Solder a 6-way row of header pins to the Qwiic Sound Trigger so you can access the TRIG pin.
  Please see: https://learn.sparkfun.com/tutorials/micromod-data-logging-carrier-board-hookup-guide
  Insert the Artemis Processor into the MicroMod Data Logging Carrier Board and secure with the screw.
  Connect your ZED-F9P GNSS breakout to the Carrier Board using a Qwiic cable.
  Connect an antenna to your GNSS board.
  Insert a formatted micro-SD card into the socket on the Carrier Board.
  Connect the Qwiic Sound Trigger to the ZED-F9P using a second Qwiic cable.
  Use a jumper cable to connect the TRIG pin on the Qwiic Sound Trigger to the INT pin on the ZED-F9P breakout.
  Connect the Carrier Board to your computer using a USB-C cable.
  
  Ensure you have the SparkFun Apollo3 boards installed: http://boardsmanager/All#SparkFun_Apollo3
  This code has been tested using version 1.2.1 of the Apollo3 boards on Arduino IDE 1.8.13.
  Select "SparkFun Artemis MicroMod" as the board type.
  Press upload to upload the code onto the Artemis.
  Open the Serial Monitor at 115200 baud to see the output.

  To minimise I2C bus errors, it is a good idea to open the I2C pull-up split pad links on
  the MicroMod Data Logging Carrier Board and the u-blox module breakout and the Qwiic Sound Trigger.

  Each time the Qwiic Sound Trigger detects a sound, it pulls its TRIG pin high. The ZED-F9P will
  detect this on its INT pin and generate a TIM TM2 message. The Artemis will log the TIM TM2 message
  to SD card. You can then study the timing of the sound pulse with nanosecond resolution!
  The code will "debounce" the sound event and reset the VM1010 for the next sound event after 250ms.

  Note: TIM TM2 can only capture the timing of one rising edge and one falling edge per
  navigation solution. So with setNavigationFrequency set to 4Hz, we can only see the timing
  of one rising edge every 250ms. The code "debounces" each sound event to make sure there will
  only be one rising edge event per navigation solution.

  TIM TM2 messages are only produced when a rising or falling edge is detected on the INT pin.
  If you disconnect your TRIG to INT jumper wire, the messages will stop.

  Data is logged in u-blox UBX format. Please see the u-blox protocol specification for more details.
  You can replay and analyze the data using u-center:
  https://www.u-blox.com/en/product/u-center

*/

#include <SparkFun_PCA9536_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_PCA9536
PCA9536 myTrigger;

#include <SPI.h>
#include <SD.h>
#include <Wire.h> //Needed for I2C to GNSS and PCA9536

#include <SparkFun_u-blox_GNSS_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_u-blox_GNSS
SFE_UBLOX_GNSS myGNSS;

File myFile; //File that all GNSS data is written to

// This is hopefully a temporary fix for the MicroMod SAMD51 Processor Board
#if defined(ARDUINO_ARCH_SAMD)
#define BATTVIN3 BATT_VIN
#endif

// This is hopefully a temporary fix for the MicroMod ESP32 Processor Board
#if defined(ARDUINO_ARCH_ESP32)
#define CS SS
#define BATTVIN3 BATT_VIN
#endif

// This is hopefully a temporary fix for the MicroMod nRF52840 Processor Board
#if defined(ARDUINO_ARDUINO_NANO33BLE)
#define CS SS
#endif

#define sdChipSelect CS //Primary SPI Chip Select is CS for the MicroMod Artemis Processor. Adjust for your processor if necessary.
#define packetLength 36 // TIM TM2 is 28 + 8 bytes in length (including the sync chars, class, id, length and checksum bytes)
#define VM1010_MODE 0 // The VM1010 mode pin is connected to GPIO0 on the PCA9536
#define VM1010_TRIG 1 // The VM1010 trigger pin (Dout) is connected to GPIO1 on the PCA9536
#define LOW_BATT 3.2 // Define the low battery voltage as 3.2V

// Uncomment the next line to keep the SD file open between writes:
//  This will result in much faster writes, but there is a risk of the log file being left open when the power is disconnected
//  and the data being lost
//#define KEEP_FILE_OPEN // Uncomment this line to keep the SD file open between writes

int dotsPrinted = 0; // Print dots in rows of 50 while waiting for a TIM TM2 message

// Callback: printTIMTM2data will be called when new TIM TM2 data arrives
// See u-blox_structs.h for the full definition of UBX_TIM_TM2_data_t
//         _____  You can use any name you like for the callback. Use the same name when you call setAutoTIMTM2callback
//        /                  _____  This _must_ be UBX_TIM_TM2_data_t
//        |                 /                   _____ You can use any name you like for the struct
//        |                 |                  /
//        |                 |                  |
void printTIMTM2data(UBX_TIM_TM2_data_t ubxDataStruct)
{
  // It is the rising edge of the sound event (TRIG) which is important
  // The falling edge is less useful, as it will be "debounced" by the loop code

  if (ubxDataStruct.flags.bits.newRisingEdge) // 1 if a new rising edge was detected
  {
    Serial.println();
    Serial.print(F("Sound Event Detected!"));
  
    Serial.print(F("  Rising Edge Counter: ")); // Rising edge counter
    Serial.print(ubxDataStruct.count);
  
    Serial.print(F("  Time Of Week: "));
    Serial.print(ubxDataStruct.towMsR); // Time Of Week of rising edge (ms)
    Serial.print(F(" ms + "));
    Serial.print(ubxDataStruct.towSubMsR); // Millisecond fraction of Time Of Week of rising edge in nanoseconds
    Serial.println(F(" ns"));
  
    dotsPrinted = 0; // Reset dotsPrinted
  }
}

void setup()
{
  Serial.begin(115200);
  while (!Serial); //Wait for user to open terminal
  Serial.println("SparkFun Qwiic Sound Trigger Example");

  Wire.begin(); // Start I2C communication with the GNSS and the Qwiic Sound Trigger

  #if defined(AM_PART_APOLLO3)
  Wire.setPullups(0); // On the Artemis, we can disable the internal I2C pull-ups too to help reduce bus errors
  #endif

  pinMode(LED_BUILTIN, OUTPUT); // We will use LED_BUILTIN to indicate when data is being written to the SD card
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(BATTVIN3, INPUT); // Use VIN/3 to detect low battery

  // Initialize the Sound Trigger PCA9536 with a begin function
  if (myTrigger.begin() == false)
  {
    Serial.println(F("Sound Trigger (PCA9536) not detected. Please check wiring. Freezing..."));
    while (1)
      ;
  }

  // Configure VM1010_TRIG (GPIO1) as an input for the VM1010 trigger signal (Dout)
  myTrigger.pinMode(VM1010_TRIG, INPUT);

  // Configure VM1010_MODE (GPIO0) as an input for now.
  // The pull-up resistor on the sound trigger will hold the VM1010 in "Wake On Sound" mode.
  // We will configure VM1010_MODE as an output when we want to pull the MODE pin low to clear the wake-up event.
  myTrigger.pinMode(VM1010_MODE, INPUT);

  Serial.println("Initializing SD card...");

  // See if the card is present and can be initialized:
  if (!SD.begin(sdChipSelect))
  {
    Serial.println("Card failed, or not present. Freezing...");
    // don't do anything more:
    while (1);
  }
  Serial.println("SD card initialized.");

  // Create or open a file called "TIM_TM2.ubx" on the SD card.
  // If the file already exists, the new data is appended to the end of the file.
  myFile = SD.open("TIM_TM2.ubx", FILE_WRITE);
  if(!myFile)
  {
    Serial.println(F("Failed to create UBX data file! Freezing..."));
    while (1);
  }
  #ifndef KEEP_FILE_OPEN
  myFile.close(); // Close the log file. It will be reopened when a sound event is detected.
  #endif

  //myGNSS.enableDebugging(); // Uncomment this line to enable helpful GNSS debug messages on Serial

  // TIM TM2 messages are 36 bytes long.
  // In this example, the data will arrive no faster than four messages per second.
  // So, setting the file buffer size to 361 bytes should be more than adequate.
  // I.e. room for ten messages plus an empty tail byte.
  myGNSS.setFileBufferSize(361); // setFileBufferSize must be called _before_ .begin

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    Serial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing..."));
    while (1);
  }

  // Uncomment the next line if you want to reset your GNSS module back to the default settings with 1Hz navigation rate
  // (This will also disable any "auto" messages that were enabled and saved by other examples and reduce the load on the I2C bus)
  //myGNSS.factoryDefault(); delay(5000);

  myGNSS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGNSS.saveConfigSelective(VAL_CFG_SUBSEC_IOPORT); //Save (only) the communications port settings to flash and BBR

  myGNSS.setNavigationFrequency(4); //Produce four navigation solutions per second

  myGNSS.setAutoTIMTM2callback(&printTIMTM2data); // Enable automatic TIM TM2 messages with callback to printTIMTM2data

  myGNSS.logTIMTM2(); // Enable TIM TM2 data logging

  while (Serial.available()) // Make sure the Serial buffer is empty
  {
    Serial.read();
  }

  Serial.println(F("Press any key to stop logging."));

  resetSoundTrigger(); // Reset the sound trigger - in case it has already been triggered
}

void resetSoundTrigger()
{
  // Reset the sound event by:
  myTrigger.digitalWrite(VM1010_MODE, LOW); // Get ready to pull the VM1010 MODE pin low
  myTrigger.pinMode(VM1010_MODE, OUTPUT); // Change the PCA9536 GPIO0 pin to an output. It will pull the VM1010 MODE pin low
  myTrigger.pinMode(VM1010_MODE, INPUT); // Change the PCA9536 GPIO0 pin back to an input (with pull-up), so it will not 'fight' the mode button  
}

void loop()
{
  myGNSS.checkUblox(); // Check for the arrival of new data and process it.
  myGNSS.checkCallbacks(); // Check if any callbacks are waiting to be processed.

  // Check to see if a new packetLength-byte TIM TM2 message has been stored
  // This indicates if a sound event was detected
  if (myGNSS.fileBufferAvailable() >= packetLength)
  {
    uint8_t myBuffer[packetLength]; // Create our own buffer to hold the data while we write it to SD card

    myGNSS.extractFileBufferData((uint8_t *)&myBuffer, packetLength); // Extract exactly packetLength bytes from the UBX file buffer and put them into myBuffer

    digitalWrite(LED_BUILTIN, HIGH); // We will use LED_BUILTIN to indicate when data is being written to the SD card

    #ifndef KEEP_FILE_OPEN
    myFile = SD.open("TIM_TM2.ubx", FILE_WRITE); // Reopen the file
    #endif
    
    myFile.write(myBuffer, packetLength); // Write exactly packetLength bytes from myBuffer to the ubxDataFile on the SD card
    
    #ifndef KEEP_FILE_OPEN
    myFile.close(); // Close the log file after the write. Slower but safer...
    #endif

    digitalWrite(LED_BUILTIN, LOW); // Turn the LED off

    //printBuffer(myBuffer); // Uncomment this line to print the data

    // "debounce" the sound event by 250ms so we only capture one rising edge per navigation solution
    for (int i = 0; i < 250; i++)
    {
      if (readVIN() < LOW_BATT) // Check if the battery voltage is low
      {
        #ifdef KEEP_FILE_OPEN
        myFile.close(); // Close the log file immediately
        #endif
        
        break; // Stop the debounce if the battery is low
      }
      delay(1); // Wait 250 * 1ms
    }

    resetSoundTrigger(); // Reset the sound trigger. This will generate a falling edge TIM_TM2 message
  }

  // Check if we should stop logging:
  // Has the user sent serial data?
  // Or is the battery voltage low?
  // This preserves the log file, preventing it from being left open accidentally when the power is disconnected
  if ((Serial.available()) || (readVIN() < LOW_BATT))
  {
    #ifdef KEEP_FILE_OPEN
    myFile.close(); // Close the log file
    #endif
    
    Serial.println();
    Serial.println(F("Logging stopped. Freezing..."));
    while(1); // Do nothing more
  }

  Serial.print("."); // Print dots in rows of 50 while we wait for a sound event
  delay(50);
  if (++dotsPrinted > 50)
  {
    Serial.println();
    dotsPrinted = 0;
  }
}

// Print the buffer contents as Hexadecimal
// You should see:
// SYNC CHAR 1: 0xB5
// SYNC CHAR 2: 0x62
// CLASS: 0x0D for TIM
// ID: 0x03 for TM2
// LENGTH: 2-bytes Little Endian (0x1C00 = 28 bytes for TIM TM2)
// PAYLOAD: LENGTH bytes
// CHECKSUM_A
// CHECKSUM_B
// Please see the u-blox protocol specification for more details
void printBuffer(uint8_t *ptr)
{
  for (int i = 0; i < packetLength; i++)
  {
    if (ptr[i] < 16) Serial.print("0"); // Print a leading zero if required
    Serial.print(ptr[i], HEX); // Print the byte as Hexadecimal
    Serial.print(" ");
  }
  Serial.println();
}

// Read VIN / 3
// Return the true voltage (compensating for processor type)
float readVIN()
{
  float vin = analogRead(BATTVIN3);
#if defined(ARDUINO_ARDUINO_NANO33BLE)
  // nRF52840 (Arduino NANO 33 BLE) is 3.3V and defaults to 10-bit
  // BUT the Schottky diode D2 on the 3.3V line reduces VDD to 3.05V
  vin *= 3.05 / 1023.0;
  vin *= 3.0; // Correct for resistor divider
#elif defined(ARDUINO_AM_AP3_SFE_ARTEMIS_MICROMOD)
  vin *= 2.0 / 1023.0; // Artemis (APOLLO3) is 2.0V and defaults to 10-bit
  vin *= 2.5 / 1.5; // Artemis PB has a built-in 150k/100k divider
  vin *= 1.41; // Correction factor to compensate for the divider resistance
  vin *= 3.0; // Correct for resistor divider
#elif defined(ARDUINO_ARCH_ESP32)
  // ESP32 is 3.3V and defaults to 12-bit
  // Manual measurements:
  // VIN  ADC
  // 3.5V 1150
  // 4.0V 1350
  // 4.5V 1535
  // 5.0V 1735
  // 5.5V 1930
  // 6.0V 2130
  // so, VIN = (ADC / 392) + 0.565
  vin /= 392;
  vin += 0.565;
#elif defined(ARDUINO_ARCH_SAMD)
  // SAMD51 is 3.3V and defaults to 10-bit
  // BUT the Schottky diode D3 on the 3.3V line reduces VDD to 3.02V
  vin *= 3.02 / 1023.0;
  vin *= 3.0; // Correct for resistor divider
#else
  vin *= 3.0; // Undefined PB!
#endif
  return (vin);
}
