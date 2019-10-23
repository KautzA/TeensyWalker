//servocontrol
//map values to the servos, test for boundries and write to servos
void DXLServoMap(){
  for (int i = 0; i < NUM_LEGS; i++){//Iterate through the legs
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++){//Iterate through the servos on each leg
      int degree_angle = leg_dynamixels[i][j] * (180.0/PI);
      int servo_pos = map(degree_angle, -90, 90, 1000, 2000);
      if ((servo_pos > kDXLServoLimits[i][j][0])&&(servo_pos < kDXLServoLimits[i][j][1])){// Servo is within limits
        ServoWrite(kDXLServoLimits[i][j][2],servo_pos);
        #if defined(USER_SERIAL_TRANSMIT)
          USER_SERIAL.print(kDXLServoLimits[i][j][2]);
          USER_SERIAL.print("G");
        #endif
      }
      else{//Servo not in limits
        //Do nothing
        #if defined(USER_SERIAL_TRANSMIT)
          USER_SERIAL.print(kDXLServoLimits[i][j][2]);
          USER_SERIAL.print("B");
          USER_SERIAL.print(servo_pos);
          USER_SERIAL.print("--");
        #endif
      }
      //USER_SERIAL.print(ServoLimits[i][j][2]);
      //USER_SERIAL.print(" ");
      //USER_SERIAL.println(ServoPos);
    }
    #if defined(USER_SERIAL_TRANSMIT)
      USER_SERIAL.println();
    #endif
  }
}
//-----------------------------------------------------------------

//Querry Servo
//Querry's position of a servo, if no response or response times out returns -1
//#define AX_PRESENT_POSITION_L
int ServoPosQuerry(int servo_id){
  return (ax12GetRegister(servo_id,AX_PRESENT_POSITION_L,2));
}


//-----------------------------------------------------------------
//Write to Servos
void ServoWrite(byte ServoID, unsigned int Pos){
  byte Header = 170; //Header that opens the command and allows autobaudrate to be set
  byte Device = 12; //Identifier for the maestro in case of chaining Default 12
  byte TruncatedCommand = 04;//0x04 represents the command to set servo position
  byte Servoplace = constrain(ServoID,0,999);//127);//the location of the servo on the controller
  byte Data1 = 0;//This holds the lower 7 bits of the position
  byte Data2 = 0;//This holds the upper 7 bits of the position
  unsigned long ScalePos = Pos*4;
  USER_SERIAL.println(ScalePos);
  for (int i = 0; i < 7; i++){
    bitWrite(Data1,i,bitRead(ScalePos,i));
  }
  for (int i = 0; i < 7; i++){
    bitWrite(Data2,i,bitRead(ScalePos,(i+7)));
  }
  //Write the packet
  USER_SERIAL.println(Header);
  USER_SERIAL.println(Device);
  USER_SERIAL.println(TruncatedCommand);
  USER_SERIAL.println(Servoplace);
  USER_SERIAL.println(Data1);
  USER_SERIAL.println(Data2);
  
  
  MAESTRO_SERIAL.write(Header);
  MAESTRO_SERIAL.write(Device);
  MAESTRO_SERIAL.write(TruncatedCommand);
  MAESTRO_SERIAL.write(Servoplace);
  MAESTRO_SERIAL.write(Data1);
  MAESTRO_SERIAL.write(Data2);
}


