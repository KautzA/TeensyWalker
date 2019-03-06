//inputs
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
#define MOVELR input_x
#define MOVEFB -input_y
#define MOVEYAW input_rz
#define MOVEUD -input_z
//      cam_pan  EXT1 (in = 0) = 0 (in > 0) -= 8
//      cam_tilt EXT2 (in = 0) = 0 (in > 0) -= 8
//Body Mode(for analog)----------------
#define BODYLR input_x
#define BODYFB -input_y 
#define BODYUD -input_z
#define BODYYAW -input_rz
#define BODYPITCH -input_rx
#define BODYROLL input_ry
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

//Error state information
#define CONTROL_PERIOD 1000 //How long before indicating communications problem
unsigned long LastCommand = 0;



//Variables used to read incoming packets
uint8_t IncomingBuffer[12];
uint16_t CommandChecksum;




boolean AnalogInMode = 0;
boolean AnalogHold = 0;
uint8_t Ext1 = 0;
uint8_t Ext2 = 0;

int ReadPacket(){//Read a packet from CommandSerial
  if ((millis() - LastCommand) >= CONTROL_PERIOD){//check for error
    bitWrite(error_state, ERROR_CMD, 1);
  }
  else{
    bitWrite(error_state, ERROR_CMD, 0);
  }

  while((COMMAND_SERIAL.available()>0)&&(COMMAND_SERIAL.peek()!=255)){
    COMMAND_SERIAL.read();
  }

  if (COMMAND_SERIAL.available() > 11){
    COMMAND_SERIAL.readBytes(IncomingBuffer, 12);
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
      input_x = (int8_t)( (int)IncomingBuffer[2]-128 );
      input_y = (int8_t)( (int)IncomingBuffer[3]-128 );
      input_z = (int8_t)( (int)IncomingBuffer[4]-128 );
      input_rx = (int8_t)( (int)IncomingBuffer[5]-128 );
      input_ry = (int8_t)( (int)IncomingBuffer[6]-128 );
      input_rz = (int8_t)( (int)IncomingBuffer[7]-128 );
      input_buttons = (uint8_t)(IncomingBuffer[8]);
      input_extend1 = (uint8_t)(IncomingBuffer[9]);
      input_extend2 = (uint8_t)(IncomingBuffer[10]);
      
      LastCommand = millis();
      return 1;
    }
  }
  return 0;
}


//ReadCommanderData
void GetInputs(){
  unsigned long InputTimeout = millis();
  while((COMMAND_SERIAL.available()>12)&&((millis()-InputTimeout)<100)){
    ReadPacket();
    //ReadButtons 
    AnalogInMode = bitRead(input_buttons, ANALOGMODEBIT);
    AnalogHold = bitRead(input_buttons, ANALOGHOLDBIT);
    //insert other buttons here




    //apply holds
    if (AnalogHold == 0){
      gait_step_height = -128;
      gait_move_y = 0;
      gait_move_x = 0;
      gait_move_z_rot = 0;
      cam_pan = 0;
      cam_tilt = 0;
      gait_body_y = 0;
      gait_body_x = 0;
      gait_body_yaw = 0;
      gait_body_pitch = 0;
      gait_body_roll = 0;
      gait_body_z = 0;
    }
    //Check Mode (body or move)
    if (AnalogInMode == 0){//MoveMode
      gait_move_y = MOVELR;
      gait_move_x = MOVEFB;
      gait_move_z_rot = MOVEYAW;
      gait_step_height = MOVEUD;
      //cam_pan = map(Ext1,-7,7,-127,127);
      //cam_tilt = map(Ext2,-7,7,-127,127);
    }
    else if (AnalogInMode == 1){//BodyMode
      gait_body_y = BODYLR;
      gait_body_x = BODYFB;
      gait_body_yaw = BODYYAW;
      gait_body_pitch = BODYPITCH;
      gait_body_z = BODYUD;
      gait_body_roll = BODYROLL;
    }
    
    if (bitRead(input_buttons, ButtonRight1)){
      move_mode = MOVE_MODE_WALK_PERIODIC;
    }
    else if (bitRead(input_buttons, ButtonRight2)){
      move_mode = MOVE_MODE_CRAWL_PERIODIC;
    }
    
    
    switch ((input_extend1 >> 4) & 0x0f) {
    case 0://Reset Exended byte values
      ExtMode0Reset();
      break;
    case 1://Do nothing
      break;
    case 2://Legacy EXT1
      break;
    case 3://Legacy EXT2
      break;
    case 4://Leg Place Mode
      break;
    case 5://Gait Mode
      ExtMode5GaitMode(input_extend1 & 0x0f);
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      break;
    case 12:
      break;
    case 13:
      break;
    }
  }
}

void ExtMode0Reset(){
  move_mode = MOVE_MODE_WALK_PERIODIC;
}

void ExtMode5GaitMode(uint8_t InputGaitMode){
  switch (InputGaitMode) {
  case 0://Default Gait
    move_mode = MOVE_MODE_WALK_PERIODIC;
    break;
  case 1://Crawl Gait
    move_mode = MOVE_MODE_CRAWL_PERIODIC;
    break;
  case 2://Swerve Mode
    move_mode = MOVE_MODE_SWERVE;
    break;
  case 3://Rule Gait
    move_mode = MOVE_MODE_WALK_RULE;
    break;
  }
}


