#include <Herkulex.h>

#define HAND_LOOP false
#define TEST_WAVE false
#define HOMING false
#define FIND_BROKEN false

struct Motor{
  int hexID;
  int minPos;
  int maxPos;
  int homePos;
  int is0601;
  String description;
};

// Left Arm (Wrist to Shoulder)
Motor Left_Wrist_Abductor               = {26, -11, 147, 117, false, "Left_Wrist_Abductor"};
Motor Left_Elbow                        = {1, -125, -20, -120, true, "Left_Elbow"};
Motor Left_Arm_Rotator                  = {2, -160, 160, -90, false, "Left_Arm_Rotator"};
Motor Left_Arm_Abductor                 = {3, -120, 140, 35, true, "Left_Arm_Abductor"}; // limited by wire

// Right Arm (Wrist to Shoulder)
Motor Right_Wrist_Abductor              = {25, -2, 155, 27, false, "Right_Wrist_Abductor"};
Motor Right_Elbow                       = {11, -116, -10, -110, true, "Right_Elbow"};
Motor Right_Arm_Rotator                 = {10, -160, 160, -8, false, "Right_Arm_Rotator"};
Motor Right_Arm_Abductor                = {6, -73, 136, 32, true, "Right_Arm_Abductor"};

// Chest/Neck (Top to Bottom, Left to Right, Front to Back)
Motor Top_Neck                          = {28, -103, -33, -83, false, "Top_Neck"};
Motor Bottom_Neck                       = {27, -160, 100, -90, false, "Bottom_Neck"};
Motor Left_Shoulder                     = {7, -160, 145, -50, true, "Left_Shoulder"};
Motor Right_Shoulder                    = {15, -160, 160, -79, true, "Right_Shoulder"};
Motor Front_Chest                       = {18, -30, 43, 0, false, "Front_Chest"};
Motor Back_Chest                        = {17, -70, 55, -5, false, "Back_Chest"};

// Pelvis (Top to Bottom, Left to Right, Front to Back
Motor Hips_Rotate_Upper_Body            = {19, -140, 160, 15, false, "Hips_Rotate_Upper_Body"};
Motor Hips_Lean_Side_To_Side            = {21, 25, 160, 107, true, "Hips_Lean_Side_To_Side"};
Motor Hips_Bend_Over                    = {22, -125, -67, -78, true, "Hips_Bend_Over"};
Motor Left_Leg_Abductor_Side_To_Side    = {9, 61, 100, 78, false, "Left_Leg_Abductor_Side_To_Side"};
Motor Right_Leg_Abductor_Side_To_Side   = {8, -14, 16, -2, false, "Right_Leg_Abductor_Side_To_Side"};

// Left Leg (Foot to Hip)
Motor Left_Leg_Rotator                  = {14, -20, 160, 3, false, "Left_Leg_Rotator"};
Motor Left_Leg_Abductor_Front_To_Back   = {30, 19, 155, 122, true, "Left_Leg_Abductor_Front_To_Back"};
Motor Left_Knee                         = {12, -120, 5, 0, false, "Left_Knee"};
Motor Left_Ankle                        = {13, -75, -5, -38, false, "Left_Ankle"};

// Right Leg (Foot to Hip
Motor Right_Leg_Rotator                 = {4, -120, 160, 0, false, "Right_Leg_Rotator"};
Motor Right_Leg_Abductor_Front_To_Back  = {31, -160, 11, -89, true, "Right_Leg_Abductor_Front_To_Back"};
Motor Right_Knee                        = {20, -120, 0, -115, false, "Right_Knee"};
Motor Right_Ankle                       = {5, -35, 40, 0, false, "Right_Ankle"};

// Ordered in motors array by id (first value of struct)
// FYI ids 16, 23-24, and 29 are unused
int motorsLen = 27;
Motor motors[27] = {
  Left_Elbow, Left_Arm_Rotator, Left_Arm_Abductor, Right_Leg_Rotator, Right_Ankle, // 1-5
  Right_Arm_Abductor, Left_Shoulder, Right_Leg_Abductor_Side_To_Side, Left_Leg_Abductor_Side_To_Side, Right_Arm_Rotator, // 6-10
  Right_Elbow, Left_Knee, Left_Ankle, Left_Leg_Rotator, Right_Shoulder, // 11-15
  Back_Chest, Front_Chest, Hips_Rotate_Upper_Body, Right_Knee, Hips_Lean_Side_To_Side, // 17 - 21
  Hips_Bend_Over, Right_Wrist_Abductor, Left_Wrist_Abductor, Bottom_Neck, Top_Neck, // 22, 25-28
  Left_Leg_Abductor_Front_To_Back, Right_Leg_Abductor_Front_To_Back // 30-31
};

int off[50];
int angle = 0;
int index = 16;
Motor currMotor = motors[index];
bool confirmedBroken[27];

// int counter2 = 0;
// int motorID = 1;

//Checks which motors are attached and reading real values
void setup()  
{
  delay(2000);  //a delay to have time for serial monitor opening
  Serial.begin(115200);    // Open serial communications
  Herkulex.beginSerial3(115200); //open serial port 3
  Serial.println("\nBegin");

  // for(int i = 0; i < 27; i++)
  // {
  //   int registerNum = 12;
  //   if(!motors[i].is0601)
  //   {
  //     Herkulex.writeRegistryEEP(motors[i].hexID, registerNum, 10);
  //   }

  // }

  // Herkulex.writeRegistryEEP(26, 12, 92);

  Herkulex.reboot(0xfe); //reboot motors
  delay(500);
  Herkulex.initialize(); //initialize motors
  delay(10);
  Herkulex.torqueOFF(0xfe);
  delay(10);
  Herkulex.clearError(0xfe);

  for (Motor m : motors) {
    Herkulex.torqueOFF(m.hexID);
  }

  if((!HOMING && !FIND_BROKEN) && (TEST_WAVE || HAND_LOOP))
  {
    Serial.print("Moving Motor ");
    Serial.print(currMotor.hexID);
 
    for (Motor m : motors) {
      Herkulex.torqueON(m.hexID);
    }
  }

  // Serial.print("Moving Motor ");
  // Serial.print(currMotor.hexID);
  // Serial.print(" to ");
  // // Serial.println(currMotor.homePos);
  // Herkulex.torqueON(motorID);

  delay(1000);

  if(FIND_BROKEN)
  {
    for(int i = 0; i < 27; i++)
    {
      confirmedBroken[i] = false;
    }
  }

  // Send motors to home position
  // aka make t-pose
//   for (int i = 0; i < motorsLen; i++) {
//     Herkulex.torqueON(motors[i].hexID);
//     Herkulex.moveOneAngle(motors[i].hexID, motors[i].homePos, 1000, LED_GREEN, motors[i].is0601);
//   }
  
//   delay(4000);
}

int counter = 0;
int foundAngle = 0;

void loop() {
  if(HAND_LOOP)
  {
    // left shoulder: -125 (down) to -155 (up)
    // left elbow: -31 (in) to -55 (out)
    // right shoulder: 125 (up) to 95 (down)
    // right elbow: -13 (in) to -50 (out)
    // left rotator: 131 
    // right rotator: 10
    // back chest: -5 (center), 15 (left), -25 (right)

    Herkulex.moveOneAngle(Left_Shoulder.hexID, -125, 1000, LED_GREEN, Left_Shoulder.is0601);
    Herkulex.moveOneAngle(Right_Shoulder.hexID, 125, 1000, LED_GREEN, Right_Shoulder.is0601);
    Herkulex.moveOneAngle(Left_Elbow.hexID, -31, 1000, LED_GREEN, Left_Elbow.is0601);
    Herkulex.moveOneAngle(Right_Elbow.hexID, -50, 1000, LED_GREEN, Right_Elbow.is0601);
    Herkulex.moveOneAngle(Left_Arm_Rotator.hexID, 120, 1000, LED_GREEN, Left_Arm_Rotator.is0601);
    Herkulex.moveOneAngle(Right_Arm_Rotator.hexID, 10, 1000, LED_GREEN, Right_Arm_Rotator.is0601);
    Herkulex.moveOneAngle(Left_Arm_Abductor.hexID, 110, 1000, LED_GREEN, Left_Arm_Abductor.is0601);
    Herkulex.moveOneAngle(Right_Arm_Abductor.hexID, -24, 1000, LED_GREEN, Right_Arm_Abductor.is0601);
    Herkulex.moveOneAngle(Left_Wrist_Abductor.hexID, -30, 1000, LED_GREEN, Left_Arm_Rotator.is0601);
    Herkulex.moveOneAngle(Right_Wrist_Abductor.hexID, -30, 1000, LED_GREEN, Right_Arm_Rotator.is0601);
    Herkulex.moveOneAngle(Front_Chest.hexID, 0, 1000, LED_GREEN, Front_Chest.is0601);
    Herkulex.moveOneAngle(Back_Chest.hexID, -5, 1000, LED_GREEN, Back_Chest.is0601);
    Herkulex.moveOneAngle(Hips_Lean_Side_To_Side.hexID, 90, 1000, LED_GREEN, Hips_Lean_Side_To_Side.is0601);
    delay(1100);

    int motionTime = 500;
    int delayTime = 350;

    while(true)
    {
      Serial.println(Herkulex.getAngle(Left_Arm_Rotator.hexID, Left_Arm_Rotator.is0601));
      Herkulex.moveOneAngle(Left_Shoulder.hexID, -155, motionTime, LED_GREEN, Left_Shoulder.is0601);
      Herkulex.moveOneAngle(Right_Shoulder.hexID, 95, motionTime, LED_GREEN, Right_Shoulder.is0601);
      // Herkulex.moveOneAngle(Back_Chest.hexID, 15, 1000, LED_GREEN, Back_Chest.is0601);
      delay(delayTime);
      Herkulex.moveOneAngle(Left_Elbow.hexID, -55, motionTime, LED_GREEN, Left_Elbow.is0601);
      Herkulex.moveOneAngle(Right_Elbow.hexID, -13, motionTime, LED_GREEN, Right_Elbow.is0601);
      delay(delayTime);
      Herkulex.moveOneAngle(Left_Shoulder.hexID, -125, motionTime, LED_GREEN, Left_Shoulder.is0601);
      Herkulex.moveOneAngle(Right_Shoulder.hexID, 125, motionTime, LED_GREEN, Right_Shoulder.is0601);
      // Herkulex.moveOneAngle(Back_Chest.hexID, 25, 1000, LED_GREEN, Back_Chest.is0601);
      delay(delayTime);
      Herkulex.moveOneAngle(Left_Elbow.hexID, -31, motionTime, LED_GREEN, Left_Elbow.is0601);
      Herkulex.moveOneAngle(Right_Elbow.hexID, -50, motionTime, LED_GREEN, Right_Elbow.is0601);
      delay(delayTime);
    }
  }

  if(TEST_WAVE)
  {
    Herkulex.moveOneAngle(Right_Shoulder.hexID, 110, 1000, LED_GREEN, Right_Shoulder.is0601);
    Herkulex.moveOneAngle(Right_Elbow.hexID, -35, 1000, LED_GREEN, Right_Elbow.is0601);
    Herkulex.moveOneAngle(Right_Wrist_Abductor.hexID, -35, 1000, LED_GREEN, Right_Wrist_Abductor.is0601);
    Herkulex.moveOneAngle(Bottom_Neck.hexID, -120, 1000, LED_GREEN, Bottom_Neck.is0601);
    delay(1100);
    while(true)
    {
      Herkulex.moveOneAngle(Right_Arm_Rotator.hexID, 30, 500, LED_GREEN, Right_Arm_Rotator.is0601);
      Herkulex.moveOneAngle(Right_Arm_Abductor.hexID, -40, 500, LED_GREEN, Right_Arm_Abductor.is0601);
      // Herkulex.moveOneAngle(Right_Wrist_Abductor.hexID, 0, 1000, LED_GREEN, Right_Wrist_Abductor.is0601);
      delay(500);
      Herkulex.moveOneAngle(Right_Arm_Rotator.hexID, 0, 500, LED_GREEN, Right_Arm_Rotator.is0601);
      Herkulex.moveOneAngle(Right_Arm_Abductor.hexID, -55, 500, LED_GREEN, Right_Arm_Abductor.is0601);
      // Herkulex.moveOneAngle(Right_Wrist_Abductor.hexID, 90, 1000, LED_GREEN, Right_Wrist_Abductor.is0601);
      delay(500);
    }
  }

  if(FIND_BROKEN)
  {
    int counter = 0;
    for(index = 0; index < motorsLen; index++)
    {
      currMotor = motors[index];
      foundAngle = Herkulex.getAngle(currMotor.hexID, currMotor.is0601);
      if((foundAngle < -160 && !confirmedBroken[index]))
      {
        Herkulex.setLed(currMotor.hexID, LED_RED);
        confirmedBroken[index] = true;
        Serial.print(currMotor.hexID);
        Serial.print(", DRS-");
        Serial.print(currMotor.is0601 ? "0601" : "0201");
        Serial.print(", " + currMotor.description);
        Serial.print(", Angle: ");
        Serial.println(Herkulex.getAngle(currMotor.hexID, currMotor.is0601));
        Serial.print("Error number: ");
        Serial.println(Herkulex.stat(currMotor.hexID));
        counter++;
      }
      else if(!confirmedBroken[index] || foundAngle >= -160)
      {
        Herkulex.setLed(currMotor.hexID, LED_GREEN);
        confirmedBroken[index] = false;
      }
      delay(10);
    }
    if(counter > 0)
    {
      Serial.print(counter);
      Serial.println(" motors broken");
    }
    delay(2000);
    return;
  }

  // For motor home position testing
  if(HOMING)
  {
    Serial.println(Herkulex.getAngle(currMotor.hexID, currMotor.is0601));
    Serial.println(Herkulex.stat(currMotor.hexID));
    counter++;
    if(counter == 1)
    {
      Serial.println(currMotor.hexID);
      counter = 0;
    }
    delay(150);
    return;
  }

  return;

  // Herkulex.moveOneAngle(currMotor.hexID, currMotor.homePos, 1000, LED_GREEN, currMotor.is0601);

  Herkulex.setLed(currMotor.hexID, LED_RED);
  angle = Herkulex.getAngle(currMotor.hexID, currMotor.is0601);

  // if (off[currMotor.hexID] != 1 && (angle >= currMotor.maxPos || angle <= currMotor.minPos)) {
  //   Herkulex.torqueOFF(currMotor.hexID);
  //   Serial.print("WARNING Motor ");
  //   Serial.print(currMotor.hexID);
  //   Serial.print(" at position ");
  //   Serial.print(angle);
  //   Serial.println(" exceeded bounds and was turned off.");
  //   off[currMotor.hexID] = 1;
  // }

  Serial.print(currMotor.hexID);
  Serial.print(", ");
  Serial.print(currMotor.description);
  Serial.print(", ");
  Serial.print(angle);
  Serial.print(", ");
  Serial.println(Herkulex.stat(currMotor.hexID));
  Herkulex.clearError(0xfe);

  if (Serial.available() != 0) 
  {
    Serial.readString();
    Herkulex.setLed(currMotor.hexID, LED_GREEN);
    // Herkulex.torqueON(currMotor.hexID);

    index++;

    if (index == motorsLen)
      index = 0;

    currMotor = motors[index];

    // Serial.print("Moving Motor ");
    // Serial.print(currMotor.hexID);
    // Serial.print(" to ");
    // Serial.println(currMotor.homePos);
    delay(1500);
  }
}