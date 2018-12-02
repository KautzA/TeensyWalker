//presetup
//Enable return data for userSerial
#define UserSerialTransmit

//Enable OLED use
#define OLED_ENABLE

//Enable Gimbal use
//#define GIMBAL_ENABLE

//Serial Ports
#define UserSerial Serial
#define AXSerial Serial1
#define USBSerial Serial
#define CommandSerial Serial2


#if defined(__AVR__)
#error "Sorry, This code requires Teensy 3.x"
#endif

//Libraries
#include<ax12Serial.h>   //KurtE's bioloid library https://github.com/KurtE/BioloidSerial
#include<BioloidSerial.h> //KurtE's bioloid library https://github.com/KurtE/BioloidSerial

#include<math.h> //enable use of cosf and sinf to use FPU

#include <i2c_t3.h> //Teensy3.x I2C library to use hardware i2c

#if defined(GIMBAL_ENABLE)//Gimbal Library
#include <PulsePosition.h>
#endif // end gimbal library

#include <SPI.h>
//#include <Wire.h>//Conflicts with i2c_t3

#if defined(OLED_ENABLE) //Libraries for OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif //End OLED libraries





#define LED_RED 24
#define LED_YEL 12


#define ServoI2C Wire1

#define NUM_LEGS 4
#define NUM_SERVOS_PER_LEG 5

//Move Mode --------------------------------------
#define MOVE_MODE_WALK_PERIODIC 0
#define MOVE_MODE_CRAWL_PERIODIC 1
#define MOVE_MODE_SWERVE 2
#define MOVE_MODE_LEG_PLACE 3
#define MOVE_MODE_WALK_RULE 4 //Not written yet

uint8_t MoveMode = MOVE_MODE_WALK_PERIODIC;
//uint8_t MoveMode = MOVE_MODE_CRAWL_PERIODIC;

//Error States
uint8_t ErrorState = 0;//Error state is the current error
#define ERROR_DXL 0 //an error with dynamixel communications, indicated by bit 0 of ErrorState
#define ERROR_CMD 1 //more than set amount of time passed between command packets, bit 1 of ErrorState




//Variable used to check servo states
int ServoCheckVal[NUM_LEGS][NUM_SERVOS_PER_LEG] = {
  0};


//Variables used for command Data in Inputs
int8_t InputX;
int8_t InputY;
int8_t InputZ;
int8_t InputRx;
int8_t InputRy;
int8_t InputRz;
uint8_t InputButtons;
uint8_t InputExtend1;
uint8_t InputExtend2;



//GaitGenControlVars
int GaitPeriod = 2000; //walk period
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

//Gimbal definitions
#if defined GIMBAL_ENABLE
PulsePositionOutput GimbalOut;
#define GIMBAL_PPM_PIN 5
#endif
int CamPan = 0;//Unused by GG
int CamTilt = 0;//Unused by GG
int GimbalMode = 0;


//OLED definitions
#if defined(OLED_ENABLE)//Open OLED Definitions
#define OLED_RESET 27
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//My OLED definitions
#define OLED_HEIGHT 32
#define OLED_WIDTH 128
#define OLED_I2C Wire

#endif//End OLED Definitions

//Defines for BioloidSerial
#define NumServos 16
BioloidControllerEx bioloid = BioloidControllerEx();

//Limits and id for each motor in the design in order  min, max, ID
//ServoLimits
const int DXLServoLimits[NUM_LEGS][NUM_SERVOS_PER_LEG][3] = {
  {
    {
      538,759,2            }
    ,{
      183,813,3            }
    ,{
      39,941,4            }
    ,{
      205,820,5            }
    ,{
      205,820,6            }
  }
  ,
  {
    {
      280,469,7            }
    ,{
      193,809,8            }
    ,{
      56,930,9            }
    ,{
      211,814,10            }
    ,{
      211,814,11            }
  }
  ,
  {
    {
      543,756,12            }
    ,{
      186,814,13            }
    ,{
      44,926,14            }
    ,{
      188,817,15            }
    ,{
      188,817,16            }
  }
  ,
  {
    {
      262,468,17            }
    ,{
      182,824,18            }
    ,{
      69,941,19            }
    ,{
      208,812,20            }
    ,{
      211,814,21            }
  }
};

#define PWM_SERVOS_MIN 150 //min pulse out of 4096
#define PWM_SERVOS_MAX 600 //max pulse out of 4096

#define NUM_PWM_SERVOS 6
const int PWMServoLimits[NUM_PWM_SERVOS][3] = {//order min,max,id
  {
    0,180,0      }
  ,
  {
    0,180,1      }
  ,
  {
    0,180,2      }
  ,
  {
    0,180,3      }
  ,
  {
    0,180,4      }
  ,
  {
    0,180,5      }
};

float LegDynamixels[NUM_LEGS][NUM_SERVOS_PER_LEG]{
  0};

float PWMServoPos[NUM_PWM_SERVOS]={
  0,0,0,0,0,0};//First 2 are pan/tilt into the gimbal

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

const int InitialPositions [NUM_LEGS] [3] = {//x,y,z
  {
    -120,  120, -100  }
  ,//Leg0
  { 
    120,  120, -100  }
  ,//Leg1
  { 
    120, -120, -100  }
  ,//Leg2
  {
    -120, -120, -100  } 
};//Leg3


//Dimensions used in leg calculations
#define COXA_LENGTH 50
#define FEMUR_LENGTH 93
#define TIBIA_LENGTH 97
#define TARSUS_LENGTH 25

//Dimensions used to convert to local leg coords
#define CogX 52 // distance that the legs are in left and right of the COG
#define CogY 65 // distance that the legs are in front and behind the COG

//Variable used to output from functions
unsigned long Tim1;
int GaitGenOut[NUM_LEGS][3]; // For GaitGen [leg] [x,y,z]
int BodyModOut[NUM_LEGS][3]; // for BodyMod [leg] [x,y,z]
int LegCoordsOut[NUM_LEGS][3];// For LegCoordsOut, [leg] [x,y,z]
float LegCalculateOut[NUM_LEGS][NUM_SERVOS_PER_LEG];// For LegCalculateOut, [leg] [servo]
float LegGlobalSpherical[NUM_LEGS][3]; //For ???, [leg] [Azimuth, Elevation, Bank]
float LegLocalSpherical[NUM_LEGS][3];  //For LocalSpherical, [leg] [Azimuth, Elevation, Bank]



