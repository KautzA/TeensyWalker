
//Defines for Reading Commander
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
#define MOVELR InputX
#define MOVEFB -InputY
#define MOVEYAW InputRz
#define MOVEUD -InputZ
//      CamPan  EXT1 (in = 0) = 0 (in > 0) -= 8
//      CamTilt EXT2 (in = 0) = 0 (in > 0) -= 8
//Body Mode(for analog)----------------
#define BODYLR InputX
#define BODYFB -InputY 
#define BODYUD -InputZ
#define BODYYAW -InputRz
#define BODYPITCH -InputRx
#define BODYROLL InputRy
//      BODYHEIGHT EXT1 (in = 0) = 0 (in > 0) = 8
//      BODYROLL   EXT2 (in = 0) = 0 (in > 0) = 8
//Buttons(mode independant)
#define ANALOGMODEBIT ButtonRightTop
#define ANALOGHOLDBIT ButtonLeftTop



//reminder of variables for gait generation control
/*
  //All variables range from -125 to 125 under normal situations
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

//Variables used to read incoming packets
uint8_t IncomingBuffer[12];
uint16_t CommandChecksum;

//Variables used for command Data
int8_t InputX;
int8_t InputY;
int8_t InputZ;
int8_t InputRx;
int8_t InputRy;
int8_t InputRz;
uint8_t InputButtons;
uint8_t InputExtend1;
uint8_t InputExtend2;



boolean AnalogInMode = 0;
boolean AnalogHold = 0;
uint8_t Ext1 = 0;
uint8_t Ext2 = 0;

int ReadPacket(){//Read a packet from CommandSerial
  //Serial.print("\n");
  while((CommandSerial.available()>0)&&(CommandSerial.peek()!=255)){
    CommandSerial.read();
  }

  if (CommandSerial.available() > 11){
    CommandSerial.readBytes(IncomingBuffer, 12);
    CommandChecksum = 0;
    for(int i = 2;i<12;i++){
      CommandChecksum += IncomingBuffer[i];
    }
    CommandChecksum %= 256;

    if(IncomingBuffer[0] != 255){//Invalid Packet
      return -1;
    }
    if(CommandChecksum != 255){//Invalid checksum

#if defined(UserSerialTransmit)
      UserSerial.println("\nChecksumError");
#endif
      return -2;
    }
    else{//ValidChecksum
      InputX = (int8_t)( (int)IncomingBuffer[2]-128 );
      InputY = (int8_t)( (int)IncomingBuffer[3]-128 );
      InputZ = (int8_t)( (int)IncomingBuffer[4]-128 );
      InputRx = (int8_t)( (int)IncomingBuffer[5]-128 );
      InputRy = (int8_t)( (int)IncomingBuffer[6]-128 );
      InputRz = (int8_t)( (int)IncomingBuffer[7]-128 );
      InputButtons = (uint8_t)(IncomingBuffer[8]);
      InputExtend1 = (uint8_t)(IncomingBuffer[9]);
      InputExtend2 = (uint8_t)(IncomingBuffer[10]);
      digitalWrite(LED_RED,HIGH);
      return 1;
    }
  }
  return 0;
}


//ReadCommanderData
void GetInputs(){
  unsigned long InputTimeout = millis();
  while((CommandSerial.available()>12)&&(millis()-InputTimeout<100)){
    ReadPacket();
    //ReadButtons
    AnalogInMode = bitRead(InputButtons, ANALOGMODEBIT);
    AnalogHold = bitRead(InputButtons, ANALOGHOLDBIT);
    //insert other buttons here




    //apply holds
    if (AnalogHold == 0){
      GaitStepHeight = -128;
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
      GaitMoveY = MOVELR;
      GaitMoveX = MOVEFB;
      GaitMoveZrot = MOVEYAW;
      GaitStepHeight = MOVEUD;
      //CamPan = map(Ext1,-7,7,-127,127);
      //CamTilt = map(Ext2,-7,7,-127,127);
    }
    else if (AnalogInMode == 1){//BodyMode
      GaitBodyY = BODYLR;
      GaitBodyX = BODYFB;
      GaitBodyYaw = BODYYAW;
      GaitBodyPitch = BODYPITCH;
      GaitBodyZ = BODYUD;
      GaitBodyRoll = BODYROLL;
    }
  }
}
