// Main firmware for Koalby humanoid robot

// #include <Herkulex.h>
#include <Arduino.h>
#include "ArduinoPoppy.h"

// Defined in ArduinoPoppy.h, motor control methods
ArduinoPoppy robot;

#include <SoftwareSerial.h> //header file of software serial port
SoftwareSerial tf(12, 13);  // define software serial port name as Serial1 and define pin2 as RX and pin3

// Set up
void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing");
  tf.begin(115200);
  robot.Setup();
}

void loop()
{

  robot.command = robot.ReadCommand();
#ifdef DEBUG
  if (robot.command != -1)
    Serial.println(robot.command);
#endif

  switch (robot.command)
  {
  case Init:
    robot.Initialize();
#ifdef DEBUG
    Serial.println("\nINIT");
#endif
    break;

  case InitIMU:
    robot.SetupIMU();
    break;

  case InitTFLuna:
    robot.SetupTFLuna();
    break;

  case InitHuskyLens:
    robot.SetupHuskyLens();

  case GetPosition:
    robot.GetPosition();
#ifdef DEBUG
    Serial.println("GET POSITION");
#endif
    break;

  case SetPosition:
    robot.SetPosition();
#ifdef DEBUG
    Serial.println("SET POSITION");
#endif
    break;

  case SetRotationOn:
    robot.SetRotationOn();
    break;

  case SetRotationOff:
    robot.SetRotationOff();
    break;

  case SetPositionT:
    robot.SetPositionT();
    break;

  case ArmMirror:
    robot.ArmMirror();
    break;

  case SetTorque:
    robot.SetTorque();
    break;

  case SetCompliant:
    robot.SetCompliant();
    break;

  case ReadBatteryLevel:
    robot.ReadBatteryLevel();
    break;

  case ReadIMUData:
    robot.ReadIMUData();
    break;

  case ReadTFLunaData:
    Serial.println(tf.read());
    robot.ReadTFLunaData();
    break;

  case ReadHuskyLens:
    robot.ReadHuskyLensData();
    break;

  case Shutdown:
    robot.Shutdown();
    break;

  default:
    break;
  }

  //  Do other tasks
  robot.UpdateRobot();
  delay(5);
}

// #include <SoftwareSerial.h> //header file of software serial port
// SoftwareSerial tf(12, 13); //define software serial port name as Serial1 and define pin2 as RX and pin3
////Serial tf = Serial2;
//
///* For Arduinoboards with multiple serial ports like DUEboard, interpret above two pieces of code and
//  directly use Serial1 serial port*/
// int dist; //actual distance measurements of LiDAR
// int check; //save check value
// int i;
// int uart[9]; //save data measured by LiDAR
// const int HEADER = 0x59; //frame header of data package
//
// void setup() {
//
//  Serial.begin(115200); //set bit rate of serial port connecting Arduino with computer
//  tf.begin(115200); //set bit rate of serial port connecting LiDAR with Arduino
//  Serial.println("Init");
//}
// void loop() {
////  Serial.println("hii");
////  Serial.println(tf.read());
//
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
//}
