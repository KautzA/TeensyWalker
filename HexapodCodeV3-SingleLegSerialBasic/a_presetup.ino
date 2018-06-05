//Serial Ports
#define UserSerial Serial
#define AXSerial Serial1
#define USBSerial Serial
#define COMMANDSerail Serial


//Libraries
#include<ax12Serial.h>   //KurtE's bioloid library https://github.com/KurtE/BioloidSerial
#include<BioloidSerial.h> //KurtE's bioloid library https://github.com/KurtE/BioloidSerial
#include <CommanderUserSerial.h>  //TrossenRobotics Commander Lib modified by me
#include <i2c_t3.h> //Teensy3.x I2C library to use WIRE1


#define ServoI2C Wire1

#define NUM_LEGS 4
#define NUM_SERVOS_PER_LEG 5


#define MOVE_MODE_WALK 0
#define MOVE_MODE_SWERVE 1
#define MOVE_MODE_OTHER 2

uint8_t MoveMode = MOVE_MODE_WALK;

//GaitGenControlVars
int GaitPeriod = 5000; //walk period
int GaitMoveX = 0;
int GaitMoveY = 0;
int GaitStepHeight = 0;
float GaitMoveZrot = 0;
int GaitBodyX = 0;
int GaitBodyY = 0;
int GaitBodyZ = 0;
float GaitBodyPitch = 0;
float GaitBodyRoll = 0;
float GaitBodyYaw = 0;



int pan;
int tilt;

int CamPan = 0;//Unused by GG
int CamTilt = 0;//Unused by GG




//Defines for BioloidSerial
#define NumServos 16
BioloidControllerEx bioloid = BioloidControllerEx();

//Limits and id for each motor in the design in order  min, max, ID
//ServoLimits
const int DXLServoLimits[NUM_LEGS][NUM_SERVOS_PER_LEG][3] = {
  {{538,759,2},{183,813,3},{39,941,4},{205,820,5},{205,820,6}},
  {{280,469,7},{193,809,8},{56,930,9},{211,814,10},{211,814,11}},
  {{543,756,12},{186,814,13},{44,926,14},{188,817,15},{188,817,16}},
  {{262,468,17},{182,824,18},{69,941,19},{208,812,20},{211,814,21}}
  };
  
#define PWM_SERVOS_MIN 150 //min pulse out of 4096
#define PWM_SERVOS_MAX 600 //max pulse out of 4096

#define NUM_PWM_SERVOS 6
const int PWMServoLimits[NUM_PWM_SERVOS][3] = {//order min,max,id
  {0,180,0},
  {0,180,1},
  {0,180,2},
  {0,180,3},
  {0,180,4},
  {0,180,5}};

float LegDynamixels[NUM_LEGS][NUM_SERVOS_PER_LEG]{0};

float PWMServoPos[NUM_PWM_SERVOS]={0,0,0,0,0,0};//First 2 are pan/tilt into the gimbal

//Initial Positions of legs, used in GaitGen
//Leg 0 (front left)
#define Leg0InitX -120
#define Leg0InitY 120
#define Leg0InitZ -100
//Leg1 (front right)
#define Leg1InitX 120
#define Leg1InitY 120
#define Leg1InitZ -100
//Leg2 (back right)
#define Leg2InitX 120
#define Leg2InitY -120
#define Leg2InitZ -100
//Leg3 (back left
#define Leg3InitX -120
#define Leg3InitY -120
#define Leg3InitZ -100

//Dimensions used in leg calculations
#define CoxaLength 50
#define FemurLength 93
#define TibiaLength 97
#define TharsusLength 25

//Dimensions used to convert to local leg coords
#define CogX 52 // distance that the legs are in left and right of the COG
#define CogY 65 // distance that the legs are in front and behind the COG

//Variable used to output from functions
unsigned long Tim1;
int GaitGenOut[NUM_LEGS][3]; // For GaitGen
int BodyModOut[NUM_LEGS][3]; // for BodyMod
int LegCoordsOut[NUM_LEGS][3];// For LegCoordsOut
float LegCalculateOut[NUM_LEGS][NUM_SERVOS_PER_LEG];// For LegCalculateOut
