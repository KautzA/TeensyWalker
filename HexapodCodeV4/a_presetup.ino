//presetup
//Enable return data for userSerial
#define USER_SERIAL_TRANSMIT

//Enable OLED use
#define OLED_ENABLE

//Enable Gimbal use
//#define GIMBAL_ENABLE  

//Serial Ports
#define USER_SERIAL Serial
#define DXL_SERIAL Serial1
#define USB_SERIAL Serial
#define COMMAND_SERIAL Serial2
#define MAESTRO_SERIAL Serial4


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




//Error LED
#define LED_RUNNING 12 //red led by Xbee
#define LED_ERROR 24 //yellow led by Xbee

//UI LEDs
#define LED_RGB_ON HIGH //On state for LEDs
#define LED_RGB_OFF LOW //Off state for LEDS
#define LED_RGB_RED 23 //red LED on indicator button
#define LED_RGB_GRE 22 //green LED on indicator button
#define LED_RGB_BLU 21 //blue LED on indicator button
#define BUTTON_RGB_PIN 20 //the button on the indicator button
#define BUTTON_RGB_MODE INPUT_PULLUP //pinmode for the indicator button pin


#define ServoI2C Wire1

#define NUM_LEGS 6
#define NUM_SERVOS_PER_LEG 3

//Move Mode --------------------------------------
#define MOVE_MODE_WALK_PERIODIC 0
#define MOVE_MODE_CRAWL_PERIODIC 1
#define MOVE_MODE_SWERVE 2
#define MOVE_MODE_LEG_PLACE 3
#define MOVE_MODE_WALK_RULE 4 //Not written yet

uint8_t move_mode = MOVE_MODE_WALK_PERIODIC;
//uint8_t MoveMode = MOVE_MODE_CRAWL_PERIODIC;

//Error States
uint8_t error_state = 0;//Error state is the current error
#define ERROR_DXL 0 //an error with dynamixel communications, indicated by bit 0 of ErrorState
#define ERROR_CMD 1 //more than set amount of time passed between command packets, bit 1 of ErrorState




//Variable used to check servo states
int servo_check_val[NUM_LEGS][NUM_SERVOS_PER_LEG] = {
  0};


//Variables used for command Data in Inputs
int8_t input_x;
int8_t input_y;
int8_t input_z;
int8_t input_rx;
int8_t input_ry;
int8_t input_rz;
uint8_t input_buttons;
uint8_t input_extend1;
uint8_t input_extend2;



//GaitGenControlVars
int gait_period = 2000; //walk period
int gait_move_x = 0;
int gait_move_y = 0;
int gait_step_height = 0;
float gait_move_z_rot = 0;
int gait_body_x = 0;
int gait_body_y = 0;
int gait_body_z = 0;
float gait_body_pitch = 0;
float gait_body_roll = 0;
float gait_body_yaw = 0;


//Gimbal definitions
#if defined GIMBAL_ENABLE
PulsePositionOutput GimbalOut;
#define GIMBAL_PPM_PIN 5
#endif
int cam_pan = 0;//Unused by GG
int cam_tilt = 0;//Unused by GG
int gimbal_mode = 0;


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
#define NUM_SERVOS 16
BioloidControllerEx bioloid = BioloidControllerEx();

//Limits and id for each motor in the design in order  min, max, ID
//ServoLimits
const int kDXLServoLimits[NUM_LEGS][NUM_SERVOS_PER_LEG][3] = {//min,max,ID
  {//Leg0
    {1000,2000,0}, //Leg0 Coxa
    {1000,2000,1}, //Leg0 Femur
    {1000,2000,2},  //Leg0 Tibia
  },
  {//Leg1
    {1000,2000,3}, //Leg1 Coxa
    {1000,2000,4}, //Leg1 Femur
    {1000,2000,5},  //Leg1 Tibia
  },
  {//Leg2
    {1000,2000,6},//Leg2 Coxa
    {1000,2000,7},//Leg2 Femur
    {1000,2000,8}, //Leg2 Tibia
  },
  {//Leg3
    {1000,2000,9},//Leg3 Coxa
    {1000,2000,10},//Leg3 Femur
    {1000,2000,11}, //Leg3 Tibia
  },
  {//Leg4
    {1000,2000,12},//Leg4 Coxa
    {1000,2000,13},//Leg4 Femur
    {1000,2000,14}, //Leg4 Tibia
  },
  {//Leg5
    {1000,2000,15},//Leg5 Coxa
    {1000,2000,16},//Leg5 Femur
    {1000,2000,17}, //Leg5 Tibia
  }
};

#define PWM_SERVOS_MIN 150 //min pulse out of 4096
#define PWM_SERVOS_MAX 600 //max pulse out of 4096

#define NUM_PWM_SERVOS 6
const int kPWMServoLimits[NUM_PWM_SERVOS][3] = {//order min,max,id
  {0,180,0},//Servo0
  {0,180,1},//Servo1
  {0,180,2},//Servo2
  {0,180,3},//Servo3
  {0,180,4},//Servo4
  {0,180,5} //Servo5
};

float leg_dynamixels[NUM_LEGS][NUM_SERVOS_PER_LEG]{
  0};

float pwm_servo_pos[NUM_PWM_SERVOS]={
  0,0,0,0,0,0};//not used on the robot

//Initial Positions of legs, used in GaitGen
//Leg 0 (front left)
#define LEG0_INIT_X -120
#define LEG0_INIT_Y 120
#define LEG0_INIT_Z -100
//Leg1 (front right)
#define LEG1_INIT_X 120
#define LEG1_INIT_Y 120
#define LEG1_INIT_Z -100
//Leg2 (back right)
#define LEG2_INIT_X 120
#define LEG2_INIT_Y -120
#define LEG2_INIT_Z -100
//Leg3 (back left
#define LEG3_INIT_X -120
#define LEG3_INIT_Y -120
#define LEG3_INIT_Z -100
//Leg4 (back right)
#define LEG4_INIT_X 0
#define LEG4_INIT_Y -120
#define LEG4_INIT_Z -100
//Leg5 (back left
#define LEG5_INIT_X 0
#define LEG5_INIT_Y -120
#define LEG5_INIT_Z -100

const int kInitialPositions [NUM_LEGS] [3] = {//x,y,z
  {LEG0_INIT_X, LEG0_INIT_Y, LEG0_INIT_Z},//Leg0
  {LEG1_INIT_X, LEG1_INIT_Y, LEG1_INIT_Z},//Leg1
  {LEG2_INIT_X, LEG2_INIT_Y, LEG2_INIT_Z},//Leg2
  {LEG3_INIT_X, LEG3_INIT_Y, LEG3_INIT_Z},//Leg3
  {LEG4_INIT_X, LEG4_INIT_Y, LEG4_INIT_Z},//Leg4
  {LEG5_INIT_X, LEG5_INIT_Y, LEG5_INIT_Z} //Leg5
};


//Dimensions used in leg calculations
#define COXA_LENGTH 10
#define FEMUR_LENGTH 80
#define TIBIA_LENGTH 142
#define TARSUS_LENGTH 25


//Dimensions used to convert to local leg coords
//radius is 110mm
#define COG_X 55 // distance that the legs are in left and right of the COG (center of gravity)
#define COG_Y 95 // distance that the legs are in front and behind the COG
#define COG_CENT 110 //distance the middle legs are from the COG in the x axis
//Period of the status LED
unsigned int led_running_period = 2000;

//Variable used to output from functions
uint8_t current_voltage = 0; //battery voltage in tenths
unsigned long last_voltage_read = 0;
unsigned long time_1;
unsigned long last_cycle_start = 0;
unsigned int last_cycle_time = 0;
int gait_gen_out[NUM_LEGS][3]; // For GaitGen [leg] [x,y,z]
int body_mod_out[NUM_LEGS][3]; // for BodyMod [leg] [x,y,z]
int leg_coords_out[NUM_LEGS][3];// For LegCoordsOut, [leg] [x,y,z]
//float LegCalculateOut[NUM_LEGS][NUM_SERVOS_PER_LEG];// For LegCalculateOut, [leg] [servo] //not used
float leg_global_spherical[NUM_LEGS][3]; //For GaitGen/SwerveSteer, [leg] [Azimuth, Elevation, Bank]
float leg_local_spherical[NUM_LEGS][3];  //For LocalSpherical, [leg] [Azimuth, Elevation, Bank]




