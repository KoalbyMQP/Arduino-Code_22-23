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
Motor Left_Elbow                        = {1, -135, 160, -125, true, "Left_Elbow"}; // Index 1
Motor Left_Arm_Rotator                  = {2, -160, 100, 11, false, "Left_Arm_Rotator"}; // Index 2
Motor Left_Arm_Abductor                 = {3, -160, 160, 36, true, "Left_Arm_Abductor"}; // Index 3

// Right Arm (Wrist to Shoulder)
Motor Right_Wrist_Abductor              = {25, -135, 35, -20, false, "Right_Wrist_Abductor"}; // Index 4
Motor Right_Elbow                       = {11, -70, 130, -40, true, "Right_Elbow"}; // Index 5
Motor Right_Arm_Rotator                 = {10, -160, 160, 20, false, "Right_Arm_Rotator"}; // Index 6
Motor Right_Arm_Abductor                = {6, -30, 80, 15, true, "Right_Arm_Abductor"};  // Index 7 Limited by 0601 wire

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
Motor Right_Leg_Rotator                 = {4, -160, 160, 147, false, "Right_Leg_Rotator"}; // Index 23
Motor Right_Leg_Abductor_Side_To_Side   = {31, -160, 110, 0, true, "Right_Leg_Abductor_Side_To_Side"}; // Index 24
Motor Right_Knee                        = {20, -10, 120, 15, false, "Right_Knee"}; // Index 25
Motor Right_Ankle                       = {5, -40, 40, 0, false, "Right_Ankle"}; // Index 26

// Ordered by id
// FYI ids 16, 23-24, and 29 are unused
int motorsLen = 27;
Motor motors[27] = {
  Left_Elbow, Left_Arm_Rotator, Left_Arm_Abductor, Right_Leg_Rotator, Right_Ankle, // 1-5
  Right_Arm_Abductor, Left_Shoulder, Right_Leg_Abductor_Front_To_Back, Left_Leg_Abductor_Front_To_Back, Right_Arm_Rotator, // 6-10
  Right_Elbow, Left_Knee, Left_Ankle, Left_Leg_Rotator, Right_Shoulder, // 11-15
  Back_Chest, Front_Chest, Hips_Rotate_Upper_Body, Right_Knee, Hips_Lean_Side_To_Side, // 17 - 21
  Hips_Bend_Over, Right_Wrist_Abductor, Left_Wrist_Abductor, Bottom_Neck, Top_Neck, // 22, 25-28
  Left_Leg_Abductor_Side_To_Side, Right_Leg_Abductor_Side_To_Side // 30-31
};

//Checks which motors are attached and reading real values
void setup()  
{
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Serial.println("Begin"); 
  Herkulex.beginSerial3(115200); //open serial port 2 

  for(int i = 0; i < motorsLen; i++)
    Herkulex.reboot(motors[i].hexID); //reboot first motor

  delay(500); 
  Herkulex.initialize(); //initialize motors
  Herkulex.torqueOFF(0xfe);
}

void loop() {
  Herkulex.torqueOFF(0xfe);
  // int count = 0;

  for(int i = 0; i < motorsLen; i++){
    int angle = Herkulex.getAngle(motors[i].hexID, motors[i].is0601);
    Serial.print("Servo ");
    Serial.print(motors[i].hexID);
    Serial.print(" Angle: ");
    Serial.println(angle);
    Herkulex.setLed(motors[i].hexID, LED_BLUE);
    // count++;
  }
  // Serial.print("Found ");
  // Serial.print(count);
  // Serial.println(" motors.");
  Serial.println("Cycle");
  delay(1000);
}