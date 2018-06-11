//map values to the servos, test for boundries and write to servos
void DXLServoMap(){
  for (int i = 0; i < NUM_LEGS; i++){//Iterate through the legs
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++){//Iterate through the servos on each leg
      int DegreeAngle = LegDynamixels[i][j] * (180/3.14159);
      int ServoPos = map(DegreeAngle, -150, 150, 0, 1024);
      if ((ServoPos > DXLServoLimits[i][j][0])&&(ServoPos < DXLServoLimits[i][j][1])){// Servo is within limits
        ServoWrite(DXLServoLimits[i][j][2],ServoPos);
        #if defined(UserSerialTransmit)
          UserSerial.print(DXLServoLimits[i][j][2]);
          UserSerial.print("G");
        #endif
      }
      else{//Servo not in limits
        //Do nothing
        #if defined(UserSerialTransmit)
          UserSerial.print(DXLServoLimits[i][j][2]);
          UserSerial.print("B");
          UserSerial.print(ServoPos);
          UserSerial.print("--");
        #endif
      }
      //UserSerial.print(ServoLimits[i][j][2]);
      //UserSerial.print(" ");
      //UserSerial.println(ServoPos);
    }
    #if defined(UserSerialTransmit)
      UserSerial.println();
    #endif
  }
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//Write to Servos
void ServoWrite(int ServoID, int Position){
  SetPosition(ServoID,Position);
  /*UserSerial.print(ServoID);
   UserSerial.print(",");
   UserSerial.println(Position);
   */
  //Code Here
}


