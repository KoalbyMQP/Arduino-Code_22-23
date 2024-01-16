#include "ArduinoPoppy.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#define BNO055_SAMPLERATE_PERIOD_MS 10

//#include <SoftwareSerial.h>
#include <HuskyLensProtocolCore.h>
#include <HUSKYLENSMindPlus.h>
#include <DFRobot_HuskyLens.h>
#include <HUSKYLENS.h>
//#define SoftwareSerial tfluna

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);
//SoftwareSerial tf(12, 13);


ArduinoPoppy::ArduinoPoppy() {
}


void ArduinoPoppy::Setup() {
  delay(2000);  //a delay to have time for serial monitor opening
  SERIAL_MONITOR.begin(115200);    // Open serial communications
#ifdef HUMAN_CONTROL
  SERIAL_MONITOR.println("Begin");
#endif
  //Start Dynamixel shield
  // Set Port baudrate to 115200. This has to match with DYNAMIXEL baudrate.
#ifdef DYNAMIXEL_CONTROL
  dxl.begin(115200);
#endif

  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version - 1.0 for our motors
  dxl.setPortProtocolVersion(1.0); //This differs for Dynamixel 320's, will need to check before each command

  //Start HerkuleX
  Herkulex.beginSerial1(115200); //open serial port 1 for HerkuleX's
  delay(100);
}

void ArduinoPoppy::SetupIMU() {


}

void ArduinoPoppy::SetupTFLuna() {
  //  tf.begin(115200);
  SERIAL_MONITOR.println ("Initializing...");

}

//void printResult(HUSKYLENSResult result){
//    if (result.command == COMMAND_RETURN_BLOCK){
//        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
//    }
//   // else if (result.command == COMMAND_RETURN_ARROW){
//   //     Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
//    }
//    else{
//        Serial.println("Object unknown!");
//    }
//}


//void printResult(HUSKYLENSResult result);

HUSKYLENS huskylens;
SoftwareSerial huskySerial(10, 11);
//int ledPin = 13;
void printResult(HUSKYLENSResult result);

void ArduinoPoppy::SetupHuskyLens() {

  //    Serial.begin(115200);
  huskySerial.begin(9600);
  pinMode (2, OUTPUT);
  pinMode (3, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (7, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (6, OUTPUT);
  //    pinMode(ledPin, OUTPUT);
  //    analogWrite(5, 180); //motor1 enable pin
  //    analogWrite(6, 180); //motor2 enable pin
  while (!huskylens.begin(huskySerial))
  {
    //Serial.println(F("Begin failed!"));
    //Serial.println(F("1.Please recheck the "Protocol Type" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    //Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
}

int ArduinoPoppy::ReadCommand() {
  int readCommand = -1;
  if (SERIAL_MONITOR.available() != 0) {
    readCommand = SERIAL_MONITOR.parseInt();
#ifdef HUMAN_CONTROL
    SERIAL_MONITOR.print("Command: ");
    SERIAL_MONITOR.println(readCommand);
#endif
  }
  return readCommand;
}

void ArduinoPoppy::Initialize() {
  Herkulex.initialize(); //initialize motors
  for (int i = 0; i < MOTOR_COUNT; i++)
    if (idArr[i].type == HERK)
      Herkulex.reboot(idArr[i].hexID);
    else//Dynamixel
    {
      // Turn off torque when configuring items in EEPROM area
      dxl.torqueOff(idArr[i].hexID);
      dxl.setOperatingMode(idArr[i].hexID, OP_POSITION);
      dxl.torqueOn(idArr[i].hexID);
      dxl.ledOn(idArr[i].hexID);
    }
  delay(500);
  //Move each motor to initialized position
  for (int i = 0; i < MOTOR_COUNT; i++) {
    if (idArr[i].type == HERK) {
      Herkulex.torqueON(idArr[i].hexID);
      Herkulex.moveOneAngle(idArr[i].hexID, idArr[i].homePos, 1000, LED_GREEN);
      //I cannot explain why this line is needed, but I swear on my life removing it makes the motor stop working right in init
      Herkulex.getAngle(idArr[i].hexID);
    } else {
      //      dxl.setGoalPosition(idArr[i].hexID, idArr[i].homePos, UNIT_DEGREE);
    }
  }
}

//TODO - update for DXL
void ArduinoPoppy::Shutdown() {
#ifdef HUMAN_CONTROL
  SERIAL_MONITOR.println("Robot Shutdown");
#endif

  for (int motorNum = 0; motorNum < MOTOR_COUNT; motorNum++) {
    if (idArr[motorNum].type == HERK) {
      Herkulex.torqueOFF(idArr[motorNum].hexID);
      Herkulex.setLed(idArr[motorNum].hexID, LED_RED);
    } else {
      dxl.torqueOff(idArr[motorNum].hexID);
    }
  }
}

// Return position in the same range as setPosition
void ArduinoPoppy::GetPosition() {
  //SERIAL_MONITOR.println("get position");
  // Get motor id
  int motorNum =  getIntFromSerial("Enter Motor Id");

  //Print the Angle
  //UPDATE: returns distance from home
  if (idArr[motorNum].type == HERK) {

    float angle = 0;
    int attempt = 0;
    do {
      angle = Herkulex.getAngle(idArr[motorNum].hexID);
      attempt++;
    } while (angle < -164 && attempt < 10);
    //Serial2.println(angle);
    //Serial2.println(map(angle,    idArr[motorNum].minPos,idArr[motorNum].maxPos,  0,100));
    if (idArr[motorNum].minPos < idArr[motorNum].maxPos)
      SERIAL_MONITOR.println((int)(angle - idArr[motorNum].homePos));
    else
      SERIAL_MONITOR.println((int)(-angle + idArr[motorNum].homePos));

  } else {
    float angle = 0;
    angle = dxl.getPresentPosition(idArr[motorNum].hexID, UNIT_DEGREE);

    //Serial2.println(angle);
    //Serial2.println(map(angle,    idArr[motorNum].minPos,idArr[motorNum].maxPos,  0,100));
    if (idArr[motorNum].minPos < idArr[motorNum].maxPos)
      SERIAL_MONITOR.println((int)(angle - idArr[motorNum].homePos));
    else
      SERIAL_MONITOR.println((int)(-angle + idArr[motorNum].homePos));
  }
}

void ArduinoPoppy::SetPosition() { //Set position, use default time of motion
  //Read motor number
  int motorNum = getIntFromSerial("Enter Motor Index ");
  /*SERIAL_MONITOR.print("out: ");
    SERIAL_MONITOR.println(motorNum);*/

  //Read motor target position
  int positionPerc = getIntFromSerial("Enter Motor Position ");

  //Send parsed command to the motor
  int mappedTarget = 0;
  //Account for motor direction when setting limits
  if (positionPerc, idArr[motorNum].minPos < idArr[motorNum].maxPos) {
    positionPerc = positionPerc + idArr[motorNum].homePos;
    mappedTarget = min(max(positionPerc, idArr[motorNum].minPos), idArr[motorNum].maxPos);
  } else {
    positionPerc = -positionPerc + idArr[motorNum].homePos;
    mappedTarget = max(min(positionPerc, idArr[motorNum].minPos), idArr[motorNum].maxPos);
    /*SERIAL_MONITOR.print("Val2: ");
      SERIAL_MONITOR.print(positionPerc);
      SERIAL_MONITOR.print("actual: ");
      SERIAL_MONITOR.println(mappedTarget);*/
  }

  if (idArr[motorNum].type == HERK)
    Herkulex.moveOneAngle(idArr[motorNum].hexID, mappedTarget, 1000, LED_BLUE); //move motor with 300 speed
  else //Dynamixel
    dxl.setGoalPosition(idArr[motorNum].hexID, mappedTarget, UNIT_DEGREE);
}

void ArduinoPoppy::SetRotationOn() {
  int motorNum = getIntFromSerial("Enter Motor Index ");
  int goalSpeed = getIntFromSerial("Enter Motor Position ");
  Herkulex.moveSpeedOne(idArr[motorNum].hexID, goalSpeed, 1000, LED_BLUE);
}

void ArduinoPoppy::SetRotationOff() {
  int motorNum = getIntFromSerial("Enter Motor Index ");
  Herkulex.moveSpeedOne(idArr[motorNum].hexID, 0, 1000, LED_BLUE); //set speed to 0s
}

void ArduinoPoppy::SetPositionT() { //Set position with time of motion
  //Read motor number}
  int motorNum = getIntFromSerial("Enter Motor Index ");

  //Read motor target position
  int positionPerc = getIntFromSerial("Enter Motor Position ");

  //Read time of motion
  int tTime = getIntFromSerial("Enter travel time (millis) ");

  //Send parsed command to the motor
  int mappedTarget = 0;
  //Account for motor direction when setting limits
  if (positionPerc, idArr[motorNum].minPos < idArr[motorNum].maxPos) {
    positionPerc = positionPerc + idArr[motorNum].homePos;
    mappedTarget = min(max(positionPerc, idArr[motorNum].minPos), idArr[motorNum].maxPos);
    /*SERIAL_MONITOR.print("Val: ");
      SERIAL_MONITOR.print(positionPerc);
      SERIAL_MONITOR.print("actual: ");
      SERIAL_MONITOR.println(mappedTarget);*/
  } else {
    positionPerc = -positionPerc + idArr[motorNum].homePos;
    mappedTarget = max(min(positionPerc, idArr[motorNum].minPos), idArr[motorNum].maxPos);
    /*SERIAL_MONITOR.print("Val2: ");
      SERIAL_MONITOR.print(positionPerc);
      SERIAL_MONITOR.print("actual: ");
      SERIAL_MONITOR.println(mappedTarget);*/
  }

  if (idArr[motorNum].type == HERK)
    Herkulex.moveOneAngle(idArr[motorNum].hexID, mappedTarget, tTime, LED_BLUE); //move motor with 300 speed
  else //Dynamixel
    //TODO: timed motion for Dynamixel
    dxl.setGoalPosition(idArr[motorNum].hexID, mappedTarget, UNIT_DEGREE);
}

//Caution: might move FAST when starting, no safties currently implemented for that
void ArduinoPoppy::ArmMirror(/*int mirrorArray[4][2], bool armMirrorModeOn, int lastMirror*/) { //Set position, use default time of motion
  //Assumes all arm motors being used in this function are HerkuleX motors, not Dynamixels
  //Read setting number
  armMirrorModeOn = getIntFromSerial("Enter Value (0 off, 1 on) ");

  if (armMirrorModeOn) {
    for (int i = 0; i < 4; i++) {
      Herkulex.torqueOFF(idArr[mirrorArray[i][0]].hexID);
    }
  } else {
    for (int i = 0; i < 4; i++) {
      Herkulex.torqueON(idArr[mirrorArray[i][0]].hexID);
    }
  }
}

void ArduinoPoppy::SetTorque() { //Set position, use default time of motion
  //Read motor number
#ifdef HUMAN_CONTROL
  SERIAL_MONITOR.println("Enter Motor Index ");        //Prompt User for input
#endif

  while (SERIAL_MONITOR.available() == 0) {}          // wait for user input
  int motorNum = SERIAL_MONITOR.parseInt();                    //Read user input and hold it in a variable

  //Read motor target position
  int setTorqueOn = getIntFromSerial("Enter Motor Torque(0 = off, 1 = on) ");

  //Send parsed command to the motor
  if (idArr[motorNum].type == HERK)
    if (setTorqueOn)
      Herkulex.torqueON(idArr[motorNum].hexID);
    else
      Herkulex.torqueOFF(idArr[motorNum].hexID);
  else //Dynamixel
    if (setTorqueOn)
      dxl.torqueOn(idArr[motorNum].hexID);
    else
      dxl.torqueOff(idArr[motorNum].hexID);
}

void ArduinoPoppy::SetCompliant() { //Set position, use default time of motion
  //Read motor number
#ifdef HUMAN_CONTROL
  SERIAL_MONITOR.println("Enter Motor Index ");        //Prompt User for input
#endif

  while (SERIAL_MONITOR.available() == 0) {}          // wait for user input
  int motorNum = SERIAL_MONITOR.parseInt();                    //Read user input and hold it in a variable

  //Read value
  int setTorqueOn = getIntFromSerial("Enter Motor Compliance (0 = off, 1 = on) ");

  //Send parsed command to the motor
  if (idArr[motorNum].type == HERK)
    if (setTorqueOn) {
      //      SERIAL_MONITOR.print("Set to compliant: ");
      SERIAL_MONITOR.println(motorNum);
      compliantMotorSet.add(motorNum);
    } else
      compliantMotorSet.sub(motorNum);
  else //Dynamixel
  {
    //TODO implement
  }
}


void ArduinoPoppy::ReadBatteryLevel() {
  float analogValue = analogRead(A10);
  float voltage = 0.0048 * analogValue;
  SERIAL_MONITOR.println(voltage);
}

// Return position in the same range as setPosition
void ArduinoPoppy::ReadIMUData() {

  //  long int t1 = millis();
  //  SERIAL_MONITOR.print(t1);
  //  SERIAL_MONITOR.println("");

  /* Get a new sensor event */
  imu::Vector<3> gyroscope = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);

  /* Display the floating point gyroscope data */
  SERIAL_MONITOR.print(gyroscope.x());
  SERIAL_MONITOR.print(",");
  SERIAL_MONITOR.print(gyroscope.y());
  SERIAL_MONITOR.print(",");
  SERIAL_MONITOR.print(gyroscope.z());
  SERIAL_MONITOR.print(",");

  imu::Vector<3> acceleration = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);


  /* Display the floating point accelerometr data */
  SERIAL_MONITOR.print(acceleration.x());
  SERIAL_MONITOR.print(",");
  SERIAL_MONITOR.print(acceleration.y());
  SERIAL_MONITOR.print(",");
  SERIAL_MONITOR.print(acceleration.z());
  SERIAL_MONITOR.print(",");

  imu::Vector<3> magnometer = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);


  /* Display the floating point magnetometer data */
  SERIAL_MONITOR.print(magnometer.x());
  SERIAL_MONITOR.print(",");
  SERIAL_MONITOR.print(magnometer.y());
  SERIAL_MONITOR.print(",");
  SERIAL_MONITOR.print(magnometer.z());
  SERIAL_MONITOR.println("");

}


void ArduinoPoppy::ReadTFLunaData() {
  //  tf.listen();
  //  SERIAL_MONITOR.println(tf.read());


  //const int HEADER = 0x59;
  //const int HEADER = 173;
  //int uart[9]; //save data measured by LiDAR
  //int i;
  //int dist; //actual distance measurements of LiDAR
  //int check; //save check value

  //  if (tf.available()) { //check if serial port has data input
  //    if (tf.read() == HEADER) { //assess data package frame header 0x59
  ////      Serial.println(HEADER);
  //      uart[0] = HEADER;
  //      if (tf.read() == HEADER) { //assess data package frame header 0x59
  //        uart[1] = HEADER;
  //        for (i = 2; i < 9; i++) { //save data in array
  //          uart[i] = tf.read();
  //        }
  //        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
  //        if (uart[8] == (check & 0xff)) { //verify the received data as per protocol
  ////          uart[2] = uart[2] + 84;
  ////          uart[3] = uart[3] + 84;
  //          dist = uart[2] + uart[3] * 256; //calculate distance value
  ////          Serial.print("2 = ");
  ////          Serial.print(uart[2]); //output measure distance value of LiDAR
  ////          Serial.print('\n');
  ////          Serial.print("3 = ");
  ////          Serial.print(uart[3]); //output measure distance value of LiDAR
  ////          Serial.print('\n');
  //
  //          Serial.print("dist = ");
  //          Serial.print(dist); //output measure distance value of LiDAR
  //          Serial.print('\n');
  //        }
  //      }
  //    }
  //  }

}

void ArduinoPoppy::ReadHuskyLensData() {
  Serial.print(huskylens.request());
  if (!huskylens.request())
    SERIAL_MONITOR.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if (!huskylens.isLearned())
    SERIAL_MONITOR.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if (!huskylens.available())
    SERIAL_MONITOR.println(F("No block or arrow appears on the screen!"));
  else
  {
    SERIAL_MONITOR.println(F("###########"));
    while (huskylens.available())
    {
      HUSKYLENSResult result = huskylens.read();
      if (result.command == COMMAND_RETURN_BLOCK) {
        SERIAL_MONITOR.print(result.xCenter);
        SERIAL_MONITOR.print(",");
        SERIAL_MONITOR.print(result.yCenter);
        SERIAL_MONITOR.print(",");
        SERIAL_MONITOR.print(result.width);
        SERIAL_MONITOR.print(",");
        SERIAL_MONITOR.print(result.height);
        SERIAL_MONITOR.print(",");
        SERIAL_MONITOR.print(result.ID);
        SERIAL_MONITOR.println("");
      }
      else {
        SERIAL_MONITOR.println("Object unknown!");
      }
      //driveBot(result);
    }
  }


}
void ArduinoPoppy::UpdateRobot() {
  if (armMirrorModeOn) {
    lastMirror = millis();
    for (int i = 0; i < 4; i++) {
      int rowRead = mirrorArray[i][0];
      int rowSet = mirrorArray[i][1];
      int pos1 = Herkulex.getAngle(idArr[rowRead].hexID) - idArr[rowRead].homePos + idArr[rowSet].homePos;

      if (pos1 > -164)
      {

        //Send parsed command to the motor
        int mappedTarget = min(max(pos1, idArr[rowSet].minPos), idArr[rowSet].maxPos);
        Herkulex.moveOneAngle(idArr[rowSet].hexID, mappedTarget, 200, LED_BLUE); //move motor
      }
    }
  }

  //Iterate over compliant motors
  int n = compliantMotorSet.first();

  while (n != -1)
  {
    if (compliancePWMCounter % 10 < 2)
      Herkulex.torqueOFF(idArr[n].hexID);
    else
      Herkulex.torqueON(idArr[n].hexID);
    Herkulex.moveOneAngle(idArr[n].hexID, Herkulex.getAngle(idArr[n].hexID), 200, 2);
    n = compliantMotorSet.next();
  }
  compliancePWMCounter++;
}

//Return an integer entered over serial - options with and without a message
int ArduinoPoppy::getIntFromSerial() {
  while (SERIAL_MONITOR.available() == 0) {}
  return SERIAL_MONITOR.parseInt();
}

int ArduinoPoppy::getIntFromSerial(char* msg) {
#ifdef HUMAN_CONTROL
  SERIAL_MONITOR.println(msg);
#endif

  return getIntFromSerial();
}
