#include <Herkulex.h>

struct Motor{
  int hexID;
  int minPos;
  int maxPos;
  int homePos;
  int is0601;
  String description;
};

// Left Arm (Wrist to Shoulder)
Motor Left_Wrist_Abductor               = {26, -40, 130, 20, false, "Left_Wrist_Abductor"}; // Index 0
Motor Left_Elbow                        = {1, -160, 160, -70, true, "Left_Elbow"}; // Index 1
Motor Left_Arm_Rotator                  = {2, -160, 100, -100, false, "Left_Arm_Rotator"}; // Index 2
Motor Left_Arm_Abductor                 = {3, -160, 160, -64, false, "Left_Arm_Abductor"}; // Index 3

// Right Arm (Wrist to Shoulder)
Motor Right_Wrist_Abductor              = {25, -135, 35, -20, false, "Right_Wrist_Abductor"}; // Index 4
Motor Right_Elbow                       = {11, -70, 130, -40, true, "Right_Elbow"}; // Index 5
Motor Right_Arm_Rotator                 = {10, -160, 160, 20, false, "Right_Arm_Rotator"}; // Index 6
Motor Right_Arm_Abductor                = {6, -30, 80, 15, false, "Right_Arm_Abductor"};  // Index 7 Limited by 0601 wire

// Chest/Neck (Top to Bottom, Left to Right, Front to Back)
Motor Top_Neck                          = {28, 35, 110, 90, false, "Top_Neck"}; // Index 8
Motor Bottom_Neck                       = {27, -20, 160, 90, false, "Bottom_Neck"}; // Index 9
Motor Left_Shoulder                     = {7, -160, 160, -50, true, "Left_Shoulder"}; // Index 10
Motor Right_Shoulder                    = {15, -160, 90, 43, true, "Right_Shoulder"}; // Index 11
Motor Front_Chest                       = {18, -155, 160, -112, false, "Front_Chest"}; // Index 12
Motor Back_Chest                        = {17, -155, 20, -60, false, "Back_Chest"}; // Index 13

// Pelvis (Top to Bottom, Left to Right, Front to Back
Motor Hips_Rotate_Upper_Body            = {19, -160, 120, -90, false, "Hips_Rotate_Upper_Body"}; // Index 14
Motor Hips_Lean_Side_To_Side            = {21, -160, 110, 100, true, "Hips_Lean_Side_To_Side"}; // Index 15
Motor Hips_Bend_Over                    = {22, -90, 40, 25, true, "Hips_Bend_Over"}; // Index 16
Motor Left_Leg_Abductor_Front_To_Back   = {9, -20, 35, 4, false, "Left_Leg_Abductor_Front_To_Back"}; // Index 17
Motor Right_Leg_Abductor_Front_To_Back  = {8, -60, -5, -45, false, "Right_Leg_Abductor_Front_To_Back"}; // Index 18

// Left Leg (Foot to Hip)
Motor Left_Leg_Rotator                  = {14, 20, 160, -10, false, "Left_Leg_Rotator"}; // Index 22
Motor Left_Leg_Abductor_Side_To_Side    = {30, -130, 160, 30, true, "Left_Leg_Abductor_Side_To_Side"}; // Index 21
Motor Left_Knee                         = {12, -130, 0, -40, false, "Left_Knee"}; // Index 20
Motor Left_Ankle                        = {13, -25, 50, 14, false, "Left_Ankle"}; // Index 19

// Right Leg (Foot to Hip
Motor Right_Leg_Rotator                 = {4, -160, 160, 60, false, "Right_Leg_Rotator"}; // Index 23
Motor Right_Leg_Abductor_Side_To_Side   = {31, -160, 110, 0, true, "Right_Leg_Abductor_Side_To_Side"}; // Index 24
Motor Right_Knee                        = {20, -10, 120, 15, false, "Right_Knee"}; // Index 25
Motor Right_Ankle                       = {5, -40, 40, 0, false, "Right_Ankle"}; // Index 26

//Checks which motors are attached and reading real values
void setup()  
{
  delay(100);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin");
  Herkulex.beginSerial3(115200); //open serial port 1  

  for(int i =0;i<0xFE;i++)
    Herkulex.reboot(i); //reboot first motor

  delay(500); 
  Herkulex.initialize(); //initialize motors
 
  Herkulex.torqueOFF(0xfe);
  delay(100);

  // non moving
  Herkulex.torqueON(Left_Arm_Abductor.hexID);
  Herkulex.torqueON(Right_Arm_Abductor.hexID);
  Herkulex.torqueON(Left_Arm_Rotator.hexID);
  Herkulex.torqueON(Right_Arm_Rotator.hexID);
  Herkulex.torqueON(Left_Wrist_Abductor.hexID);
  Herkulex.torqueON(Right_Wrist_Abductor.hexID);

  // moving
  Herkulex.torqueON(Left_Shoulder.hexID);
  Herkulex.torqueON(Right_Shoulder.hexID);
  Herkulex.torqueON(Left_Elbow.hexID);
  Herkulex.torqueON(Right_Elbow.hexID);
  delay(100);
}

void loop() {
  Serial.println(Herkulex.getAngle(Left_Shoulder.hexID, Left_Shoulder.is0601));
  Serial.println(Herkulex.getAngle(Right_Shoulder.hexID, Right_Shoulder.is0601));
  Serial.println(Herkulex.getAngle(Left_Elbow.hexID, Left_Elbow.is0601));
  Serial.println(Herkulex.getAngle(Right_Elbow.hexID, Right_Elbow.is0601));
  Serial.println("-----");

  /*
74.75
-16.90
-66.30
-97.82

48.75
-56.55
-55.90
-57.20

  */

  // Pos 1
  Herkulex.moveOneAngle(Left_Shoulder.hexID, 74.75, 2000, LED_GREEN, Left_Shoulder.is0601);
  Herkulex.moveOneAngle(Right_Shoulder.hexID, -16.90, 2000, LED_GREEN, Right_Shoulder.is0601);
  Herkulex.moveOneAngle(Left_Elbow.hexID, -66.30, 2000, LED_GREEN, Left_Elbow.is0601);
  Herkulex.moveOneAngle(Right_Elbow.hexID, -97.82, 2000, LED_GREEN, Right_Elbow.is0601);
  delay(100000);

  // Pos 2
  Herkulex.moveOneAngle(Left_Shoulder.hexID, 48.75, 2000, LED_GREEN, Left_Shoulder.is0601);
  Herkulex.moveOneAngle(Right_Shoulder.hexID, -56.55, 2000, LED_GREEN, Right_Shoulder.is0601);
  Herkulex.moveOneAngle(Left_Elbow.hexID, -55.90, 2000, LED_GREEN, Left_Elbow.is0601);
  Herkulex.moveOneAngle(Right_Elbow.hexID, -57.20, 2000, LED_GREEN, Right_Elbow.is0601);
  delay(2500);
}