/*
  Production Test code for the SparkFun Qwiic Sound Trigger VM1010
  By: Paul Clark
  SparkFun Electronics
  Date: March 22nd, 2021
  License: please see LICENSE.md for details

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/17979

  Hardware Connections:
  Use a Qwiic cable to connect the Sound Trigger to your board.

  LED_BUILTIN will light up if the PCA9536 is detected correctly.
  Snap your fingers to create a sound.
  The TRIG LED on the sound trigger will light up for a second before being reset by the code.
  Snap again for another flash.

*/

#include <SparkFun_PCA9536_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_PCA9536

PCA9536 myTrigger;

unsigned long lastTrigger = 0; // Keep a record of when the last trigger took place (millis)
boolean soundTriggered = false; // Flag when a new sound event is detected

#define VM1010_MODE 0 // The VM1010 mode pin is connected to GPIO0 on the PCA9536
#define VM1010_TRIG 1 // The VM1010 trigger pin (Dout) is connected to GPIO1 on the PCA9536

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Qwiic Sound Trigger Production Test"));

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  delay(250); // Try every 0.25 seconds

  Wire.begin();

  Wire.beginTransmission(0x41); // Detect PCA9536 on address 0x41
  if (Wire.endTransmission() != 0)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: nothing detected on address 0x41"));
    return;
  }
  
  if (myTrigger.begin() == false) // Initialize the Sound Trigger PCA9536 with a begin function
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: .begin failed"));
    return;
  }

  // Configure VM1010_TRIG (GPIO1) as an input for the VM1010 trigger signal (Dout)
  PCA9536_error_t result = myTrigger.pinMode(VM1010_TRIG, INPUT);
  if (result != PCA9536_ERROR_SUCCESS)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: .pinMode(VM1010_TRIG, INPUT) failed"));
    return;
  }

  // Configure VM1010_MODE (GPIO0) as an input for now.
  // The pull-up resistor on the sound trigger will hold the VM1010 in "Wake On Sound" mode.
  // We will configure VM1010_MODE as an output when we want to pull the MODE pin low to clear the wake-up event.
  result = myTrigger.pinMode(VM1010_MODE, INPUT);
  if (result != PCA9536_ERROR_SUCCESS)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: .pinMode(VM1010_MODE, INPUT) failed"));
    return;
  }

  digitalWrite(LED_BUILTIN, HIGH); // PCA9536 detected OK - light up LED_BUILTIN

  // Check for a new sound (wake-up event) by reading the VM1010 Trigger (Dout) pin
  // Dout will go high when a sound is detected
  if (myTrigger.digitalRead(VM1010_TRIG) != HIGH)
  {
    Wire.end();
    Serial.println(F("All is well, sound trigger is connected, no sound detected..."));
    return; // Return leaving LED_BUILTIN lit up
  }

  // A sound has been detected!
  if (soundTriggered == false)
  {
    soundTriggered = true; // Set soundTriggered
    lastTrigger = millis(); // Update lastTrigger
  }
  
  // Check if it is time to reset the sound trigger
  if (millis() < (lastTrigger + 1000))
  {
    Wire.end();
    Serial.println(F("All is well, sound trigger is connected, sound detected, but it is not yet time to reset the trigger..."));
    return; // Return leaving LED_BUILTIN lit up
  }

  // Reset the trigger
  myTrigger.digitalWrite(VM1010_MODE, LOW); // Get ready to pull the VM1010 MODE pin low
  myTrigger.pinMode(VM1010_MODE, OUTPUT); // Change the PCA9536 GPIO0 pin to an output. It will pull the VM1010 MODE pin low
  myTrigger.pinMode(VM1010_MODE, INPUT); // Change the PCA9536 GPIO0 pin back to an input (with pull-up), so it will not 'fight' the mode button

  soundTriggered = false; // Clear soundTriggered
  
  Wire.end();
  Serial.println(F("Test passed! Sound trigger is connected. Sound detected and reset."));
  
  // Return, leaving LED_BUILTIN lit up
}
