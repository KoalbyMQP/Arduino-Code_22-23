#include <Arduino.h>
#include <Herkulex.h>

// NEVER RUN THIS PROGRAM ON THE ACTUAL ROBOT IT WILL RESET EVERYTHING
int n = 0xfd;     // set the motor ID - cant be FE
int newId = 0x1F; // set the motor ID

void setup()
{
  delay(2000);          // a delay to have time for serial monitor opening
  Serial.begin(115200); // Open serial communications
  Serial.println("Begin");
  Herkulex.beginSerial1(115200); // open serial port 1
  Herkulex.reboot(n);            // reboot
  delay(500);
  Herkulex.initialize(); // initialize motors
  delay(500);

  for (int i = 0; i < 0xFE; i++)
  {
    Herkulex.set_ID(i, newId);
    Serial.println(i);
    Herkulex.setLed(i, LED_PINK);
    //    delay(1200);
  }

  Herkulex.reboot(newId); // reboot first motor

  delay(500);
  Herkulex.initialize(); // initialize motors
}

void loop()
{
  //  Serial.println("Move Angle: -100 degrees");
  ////  Herkulex.setLed(newId, LED_GREEN);
  //  Herkulex.moveOneAngle(newId, -25, 1000, LED_GREEN); //move motor with 300 speed
  //  delay(1200);
  //  Serial.print("Get servo Angle:");
  //  Serial.println(Herkulex.getAngle(newId));
  //  Serial.println("Move Angle: 100 degrees");
  //  Herkulex.moveOneAngle(newId, 25, 1000, LED_GREEN); //move motor with 300 speed
  //  delay(1200);
  //  Serial.print("Get servo Angle:");

  Herkulex.moveOneAngle(newId, 0, 1000, LED_GREEN);
  Serial.println(Herkulex.getAngle(newId));
}
