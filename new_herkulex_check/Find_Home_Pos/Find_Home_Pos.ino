#include <Herkulex.h>

#define MOTOR_ID 7
#define IS_0601 true

void setup() {
  
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Herkulex.beginSerial2(115200); //open serial port 2
  Serial.println("Begin");

  Herkulex.reboot(0xfe); //reboot first motor
  delay(500); 
  delay(10);
  Herkulex.initialize(); //initialize motors
  delay(10);
  Herkulex.torqueOFF(0xfe);
  delay(10);
  Herkulex.clearError(0xfe);
 

  // Serial.print("Moving Motor ");
  // Serial.print(currMotor.hexID);
  // Serial.print(" to ");
  // // Serial.println(currMotor.homePos);
  // Herkulex.torqueON(motorID);
  Herkulex.torqueOFF(MOTOR_ID);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(Herkulex.getAngle(MOTOR_ID, IS_0601));
  delay(10);

}
