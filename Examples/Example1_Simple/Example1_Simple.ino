/*
  Simple example for the SparkFun Qwiic Sound Trigger VM1010
  By: Paul Clark
  SparkFun Electronics
  Date: March 10th 2021

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/17979

  This example demonstrates how to check for a sound event (by reading GPIO1 on the PCA9536)
  and how to reset the Sound Trigger for the next event (by toggling GPIO0 low then high)
  
  Hardware Connections:
  Use a Qwiic cable to connect the Sound Trigger to your board.

  Licence: please see LICENSE.md for further details

  Based on:
  Using the PCA9536 -- Digital Output
  By: Jim Lindblom

*/

#include <SparkFun_PCA9536_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_PCA9536

PCA9536 myTrigger;

unsigned long lastTrigger; // Keep a record of when the last trigger took place (millis)

#define VM1010_MODE 0 // The VM1010 mode pin is connected to GPIO0 on the PCA9536
#define VM1010_TRIG 1 // The VM1010 trigger pin (Dout) is connected to GPIO1 on the PCA9536

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Qwiic Sound Trigger Example"));

  Wire.begin(); // We communicate with the PCA9536 via I2C

  pinMode(LED_BUILTIN, OUTPUT); // The LED on the sound trigger will only flash very briefly
  digitalWrite(LED_BUILTIN, LOW); // We will flash LED_BUILTIN for a full second on each sound event

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

  lastTrigger = millis(); // Initialize lastTrigger

  Serial.println(F("Waiting for a sound event..."));
}

void loop()
{
  // Check for a new sound (wake-up event) every millisecond (approx.)
  delay(1);

  // Check for a wake-up event by reading the VM1010 Trigger (Dout) pin
  // Dout will go high when a sound is detected
  if (myTrigger.digitalRead(VM1010_TRIG) == HIGH)
  {
    // We have detected a sound so:

    // Lets tell the user about it:
    Serial.println();
    Serial.print(F("Sound detected! It has been "));
    Serial.print(millis() - lastTrigger);
    Serial.println(F(" ms since the last sound event"));

    lastTrigger = millis(); // Update lastTrigger

    // Flash LED_BUILTIN for a full second. The LED on the sound trigger will only flash briefly
    digitalWrite(LED_BUILTIN, HIGH);

    // Unless your sound is _very_ quick ( ~ 10 milliseconds ), you will see multiple sounds events from the VM1010
    // You can "debounce" those by adding a delay before resetting the VM1010
    //delay(100); // Uncomment this line to "debounce" sound events

    // Now we need to reset the sound event by:
    myTrigger.digitalWrite(VM1010_MODE, LOW); // Get ready to pull the VM1010 MODE pin low
    myTrigger.pinMode(VM1010_MODE, OUTPUT); // Change the PCA9536 GPIO0 pin to an output. It will pull the VM1010 MODE pin low
    myTrigger.pinMode(VM1010_MODE, INPUT); // Change the PCA9536 GPIO0 pin back to an input (with pull-up), so it will not 'fight' the mode button
  }

  // Check if it has been more than one second since the last sound event
  // If it has, turn LED_BUILTIN off
  if (millis() > (lastTrigger + 1000))
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
