
//Defines for Reading Commander
Commander command = Commander();
#define ButtonRight1   0
#define ButtonRight2   1
#define ButtonRight3   2
#define ButtonLeft4    3
#define ButtonLeft5    4
#define ButtonLeft6    5
#define ButtonRightTop 6
#define ButtonLeftTop  7

//What input is which----------------------------------------
//Move Mode(for analog)---------------
#define MOVELR lookH
#define MOVEFB lookV
#define MOVEYAW walkH
#define MOVEUD walkV
//      CamPan  EXT1 (in = 0) = 0 (in > 0) -= 8
//      CamTilt EXT2 (in = 0) = 0 (in > 0) -= 8
//Body Mode(for analog)----------------
#define BODYLR lookH
#define BODYFB lookV 
#define BODYYAW walkH
#define BODYPITCH walkV
//      BODYHEIGHT EXT1 (in = 0) = 0 (in > 0) = 8
//      BODYROLL   EXT2 (in = 0) = 0 (in > 0) = 8
//Buttons(mode independant)
#define ANALOGMODEBIT ButtonRightTop
#define ANALOGHOLDBIT ButtonLeftTop



//reminder of variables for gait generation control
/*
  //All variables range from -127 to 127 under normal situations
  MoveMode         what gait to use --uint_8t
  GaitPeriod       how long one cycle takes
  GaitMoveX        how far to step each cycle
  GaitMoveY        how far to step each cycle
  GaitStepHeight   how far to lift for each step
  GaitMoveZrot     how far to rotate each cycle
  GaitBodyX        how far the body should be translated relative to the legs/direction of travel
  GaitBodyY        how far the body should be translated relative to the legs/direction of travel
  GaitBodyZ        how far the body should be translated vertically from the default height
  GaitBodyPitch     how far the body should be rotated relative to the legs/direction of travel
  GaitBodyRoll     how far the body should be rotated relative to the legs/direction of travel
  GaitBodyYaw     how far the body should be rotated relative to the legs/direction of travel
*/

boolean AnalogInMode = 0;
boolean AnalogHold = 0;
uint8_t Ext1 = 0;
uint8_t Ext2 = 0;

//ReadCommanderData
void GetCommander(){
  //Read Buttons
  while(command.ReadMsgs() > 0){// was an if statement
    uint8_t Buttons = command.buttons;
    AnalogInMode = bitRead(Buttons, ANALOGMODEBIT);
    AnalogHold = bitRead(Buttons, ANALOGHOLDBIT);
    //insert other buttons here
    
    
    //Read Extended Byte
    uint8_t Extended = command.ext;
    uint8_t FirstHalfExtTemp = Extended;//will become x's (xxxx0000) of Extended
    FirstHalfExtTemp &= B11110000;//Clear lower nibble
    int8_t FirstHalfExt = FirstHalfExtTemp/16;//Shift upper nibble to lower nibble
    
    uint8_t SecondHalfExtTemp = Extended;//will become 0's (xxxx0000) of Extended
    SecondHalfExtTemp &= B00001111;//Clear upper nibble
    int8_t SecondHalfExt = SecondHalfExtTemp;
    
    switch(FirstHalfExt){
      case 0://Reset
        Ext1 = 0;
        Ext2 = 0;
        MoveMode = 0;
        break;
      case 1://Nothing
        //Nothing
        break;
      case 2://Ext1 Follows
        Ext1 = SecondHalfExt;
        if (Ext1  != 0){//Scale Ext1 if not 0
          Ext1 -= 8;
        }
        break;
      case 3://Ext2 Follows
        Ext2 = SecondHalfExt;
        if (Ext2  != 0){//Scale Ext2 if not 0
          Ext2 -= 8;
        }
        break;
      case 4://Leg Position mode
        //Code here
        break;
      case 5://Select Mode
        MoveMode = SecondHalfExt;
        break;
      default:
        //Things
        break;
    }
   
    //apply holds
    if (AnalogHold == 0){
      GaitMoveY = 0;
      GaitMoveX = 0;
      GaitMoveZrot = 0;
      CamPan = 0;
      CamTilt = 0;
      GaitBodyY = 0;
      GaitBodyX = 0;
      GaitBodyYaw = 0;
      GaitBodyPitch = 0;
      GaitBodyRoll = 0;
      GaitBodyZ = 0;
    }
    //Check Mode (body or move)
    if (AnalogInMode == 0){//MoveMode
      GaitMoveY = command.MOVELR;
      GaitMoveX = command.MOVEFB;
      GaitMoveZrot = command.MOVEYAW;
      GaitStepHeight = command.MOVEUD;
      CamPan = map(Ext1,-7,7,-127,127);
      CamTilt = map(Ext2,-7,7,-127,127);
    }
    else if (AnalogInMode == 1){
      GaitBodyY = command.BODYLR;
      GaitBodyX = command.BODYFB;
      GaitBodyYaw = command.BODYYAW;
      GaitBodyPitch = command.BODYPITCH;
      GaitBodyZ = map(Ext1,-7,7,-127,127);
      GaitBodyRoll = map(Ext2,-7,7,-127,127);
    }
    
    
    //Scale the outputs --- now done in functuions
    
    
  }  
}

