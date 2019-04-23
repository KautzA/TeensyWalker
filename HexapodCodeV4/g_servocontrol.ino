//servocontrol
//map values to the servos, test for boundries and write to servos
void DXLServoMap(){
  for (int i = 0; i < NUM_LEGS; i++){//Iterate through the legs
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++){//Iterate through the servos on each leg
      int degree_angle = leg_dynamixels[i][j] * (180.0/PI);
      int servo_pos = map(degree_angle, -150, 150, 0, 1024);
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
void ServoWrite(int servo_id, int target_position){
  SetPosition(servo_id,target_position);
  /*USER_SERIAL.print(ServoID);
   USER_SERIAL.print(",");
   USER_SERIAL.println(Position);
   */
  //Code Here
}


