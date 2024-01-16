#include <Herkulex.h>

//NEVER RUN THIS PROGRAM ON THE ACTUAL ROBOT IT WILL RESET EVERYTHING
int oldID=20; //set the motor ID - cant be FE
int newID=9; //set the motor ID
bool is0601 = false;

void setup()  
{
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin");
  Herkulex.beginSerial3(115200); //open serial port 2
  Herkulex.reboot(0xfe); //reboot 
  delay(500); 
  Herkulex.initialize(); //initialize motors
  delay(500);
  Herkulex.torqueOFF(0xfe);
  delay(10);

  // for(int i=0;i<0xFE;i++){
  Herkulex.set_ID(oldID, newID);
  delay(10);
    // Serial.println(i);
//    delay(1200);
  // }
    
  for(int i =0;i<0xFE;i++)
    Herkulex.reboot(i); //reboot first motor

  delay(500); 
  Herkulex.initialize(); //initialize motors
  // Herkulex.torqueON(newID);
  Herkulex.setLed(0xfe, LED_RED);
  delay(10);
  Herkulex.setLed(newID, LED_GREEN);
  Serial.println("Done changing id");
}

void loop(){
//  Serial.println("Move Angle: -100 degrees");
////  Herkulex.setLed(newID, LED_GREEN);
//  Herkulex.moveOneAngle(newID, -25, 1000, LED_GREEN, is0601); //move motor with 300 speed  
//  delay(1200);
//  Serial.print("Get servo Angle:");
//  Serial.println(Herkulex.getAngle(newID));
//  Serial.println("Move Angle: 100 degrees");
//  Herkulex.moveOneAngle(newID, 25, 1000, LED_GREEN, is0601); //move motor with 300 speed  
//  delay(1200);
//  Serial.print("Get servo Angle:");
  
  //  Herkulex.moveOneAngle(newID, 200, 1000, LED_GREEN, is0601);
   delay(1500);
   Serial.println(Herkulex.getAngle(newID, is0601));
   delay(100);
  //  Herkulex.moveOneAngle(newID, 1000, 1000, LED_GREEN, is0601);
   delay(1500);
   Serial.println(Herkulex.getAngle(newID, is0601));
   delay(100);
}
