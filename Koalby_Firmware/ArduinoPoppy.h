#ifndef ArduinoPoppy_h
#define ArduinoPoppy_h
#include <Herkulex.h>
// #include <DynamixelShield.h>
//#include <SoftwareSerial.h>
#include "set.h" //Includes set datastructure

//define this to use serial2 as output, comment out this definition to use USB
//#define DYNAMIXEL_CONTROL

//#define HUMAN_CONTROL
//#define DEBUG

#ifdef DYNAMIXEL_CONTROL
  #define SERIAL_MONITOR Serial2 //output to USB-Serial adapter
#else
  #define SERIAL_MONITOR Serial  //output to upload, USB
#endif

//Motor types
// #define HERK 0
// #define DYN  1

//List of possible numeric commands to send - cannot use 0 as a command
enum Commands { Init = 1, GetPosition = 5, SetPosition = 10,
                SetPositionT/*Set position with ID and time of motion*/ = 11,
                ArmMirror = 15, /*set arms to mirror each other*/
                SetTorque = 20,/*Set torque to "on" or "off" based on ID*/
                SetCompliant = 21,/*Set torque to "on" or "off" based on ID*/
                ReadBatteryLevel = 30,
                InitIMU = 40,
                ReadIMUData = 41,
                InitTFLuna = 50,
                ReadTFLunaData = 51,
                InitHuskyLens = 60,
                ReadHuskyLens = 61,
                SetRotationOn = 74,
                SetRotationOff = 76,
                Shutdown = 100
              };


struct Motor{
  int hexID;
  int minPos;
  int maxPos;
  int homePos;
  int is0601;
};


/**
   ArduinoPoppy Class for the addition of an Arduino Mega 2560 to control Herkulex and Dynamixel motors.
*/
class ArduinoPoppy {
  public:
    //Constructor
//    ArduinoPoppy(SoftwareSerial * TFLuna);
    ArduinoPoppy();

    // Global Methods
    void Setup();
    void SetupIMU();
    void SetupTFLuna();
    void SetupHuskyLens();
    int ReadCommand();
    void Initialize();
    void Shutdown();
    void GetPosition();
    void SetPosition();
    void SetRotationOn();
    void SetRotationOff();
    void SetPositionT();
    void SetTorque();
    void SetCompliant();
    void ReadBatteryLevel();
    void ReadIMUData();
    void ReadHuskyLensData();
    void ReadTFLunaData();
    void ArmMirror();
    void PuppetMaster();
    void UpdateRobot();

    // Global Constants
    int command = 0;
//    SoftwareSerial tf;

  private:
    unsigned int compliancePWMCounter = 0;
    // Private Methods
    int getIntFromSerial();
    int getIntFromSerial(char* msg);

    //Private Objects
    // DynamixelShield dxl;
    Set compliantMotorSet;
    
    //Private Variables
    bool armMirrorModeOn = false;
    int lastMirror = 0;

    //Private Constants - This defines the robot's motor setup
    const static int MOTOR_COUNT = 26;//sizeOF(IdArr) not working right, using manual definition

    //List of motors
    // TODO Update, not accurate
    Motor Right_Shoulder_Rotator_Joint =          {0x01,     6,    -118,     0,        false};    //0   * Motor 1 - Herkulex, Right Forearm
    Motor RightArmZ     =         {0x02,    97,    -90,       1,        false};    //1   * Motor 2 - Herkulex, Right Upper Arm  *** Wrong Limit Motor Needs to be adjusted ????
    Motor RightShoulderX =        {0x03,    133,     -17,      2,        false};    //2   * Motor 3 - Herkulex, Right Arm Connector
    Motor RightShoulderY =        {0x0F,    -95,   90,     3,       false};    //3   * Motor F - Herkulex, Right Shoulder

    Motor LeftForearm =           {0x0B,     131,    -3,      4,        false};    //4   * Motor B - Herkulex, Left Forearm ???
    Motor LeftUpperArm =          {0x0A,     90,    -90,     5,        false};    //5   * Motor A - Herkulex, Left Upper Arm
    Motor LeftUpperConnector =    {0x06,     10,   -142,     6,        false};    //6   * Motor 6 - Herkulex, Left Arm Connector
    Motor LeftShoulder =          {0x07,     93,    -85,   7,        false};    //7   * Motor F - Herkulex, Left Shoulder

    Motor TorsoDRRear =           {0x11,   64,    -66,    17,        false};    //8   * Motor 11 - Herkulex, Torso Double Rotation Backside
    Motor TorsoDRFront =          {0x12,    23,     -49,    16,        false};    //9   * Motor 12 - Herkulex, Torso Double Rotation Frontside
    Motor Abdomen =               {0x13,   95,    -71,      8,        false};    //10  * Motor 13 - Herkluex, Abdomen

    Motor LHipX    =              {0x09,   3,    -13,      15,        false};    //11  * Herkluex, right hip 1
    Motor LHipZ    =              {0x0E,   17,    -13,      -4,        false};    //12  * Herkluex, right hip 2 - NEEDS ADJUSTMENT
    Motor LHipY    =              {0x1F,   297,    297,      0,        false};    //13  * Dynamixel, left hip 3 //2 ,146, 77  changed to 31
    Motor LKnee    =              {0x0C,   -82,  3,      -8,        false};    //14  * Herkluex, right knee
    Motor LAnkle   =              {0x05,  43,   -20,      4,        false};    //15  * Herkluex, right ankle

    Motor RHipX    =              {0x08,   12,    -20,      19,        false};    //16  * Herkluex, right  hip 1
    Motor RHipZ    =              {0x04,   -62, 40 , 20,        false};    //17  * Herkluex, right hip 2 **<------------ check this one with someone whose code isn't changed**
    Motor RHipY    =              {0x1E,   116,    116,      11,        false};    //18  * Dynamixel, right hip - changed to 30
    Motor RKnee    =              {0x14,    52,      -2,      12,        false};    //19  * Herkluex, right knee
    Motor RAnkle   =              {0x0D,    46,     -21,      14,        false};    //20  * Herkluex, right ankle - unplugged ???

    Motor AbsY =            {0x15,   170,    170,      0,        false};     //21  * Motor X - Dynamixel, test motor
    Motor AbsX =             {0x16,   230,    230,     0,        false};     //22  * Motor X - Dynamixel, test motor

    Motor HeadZ =            {0x7,   60,    60,      4,        false};     //23  * Neck rotation 10,    110,      60
    Motor HeadY =             {0x4,   155,    155,      0,        false};     //24  * Head nodding 120,    140,      140

    // Motor dxlTest =               {0x100,   -100,    100,      0,        DYN};     //25  * Motor X - Dynamixel, test motor
    
  ///1,3,4,5,6,7,8,9,A,C,E,F,11,12,13,14 //171, 4,90
    //Hips out:
    //10,6,100,  10,2,90,  10,11,100, 10,16,100
    //Knees and arms back
    //10,14,0,  10,19,0,  10,0,100,  10,4,100
  
    //Remaining motor is 0x10
    
    Motor idArr[MOTOR_COUNT] = {Right_Shoulder_Rotator_Joint, RightArmZ, RightShoulderX, RightShoulderY,
                       LeftForearm,  LeftUpperArm,  LeftUpperConnector,  LeftShoulder,
                       TorsoDRRear,  TorsoDRFront, Abdomen,
                       LHipX, LHipZ, LHipY, LKnee, LAnkle,
                       RHipX, RHipZ, RHipY, RKnee, RAnkle,
                       AbsY,AbsX,
                       HeadZ,HeadY,
                       };

    //Pairs of motors to mirror - 2nd motor sets position to match first motor
    int mirrorArray[4][2] = {
      {0, 4},
      {1, 5},
      {2, 6},
      {3, 7}
    };


};

#endif
