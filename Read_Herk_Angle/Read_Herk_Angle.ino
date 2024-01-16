//Disable torque for a specific motor and constantly print its position, used to figure out limits and home position
#include <Herkulex.h>
int n=20; //motor ID - verify your ID !!!!
int is0601 = false;
// -60,  -161, -154

//Left E right 4
//R arm 1, 2, 3

void setup()  
{
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin");
  Herkulex.beginSerial2(115200); //open serial port 1 
  Herkulex.reboot(n); //reboot first motor
  delay(500); 
  Herkulex.initialize(); //initialize motors
  delay(200);
}

void loop() {
  Herkulex.torqueOFF(n);
  Serial.print("Get servo Angle:");
  Serial.println(Herkulex.getAngle(n, is0601));
  Herkulex.setLed(n,LED_GREEN);
  //Herkulex.moveOneAngle(n, 0, 1000, LED_BLUE, is0601); //move motor with 300 speed  
  delay(1200);
  
}
