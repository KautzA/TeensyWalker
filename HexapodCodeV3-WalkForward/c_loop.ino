void loop() {
  Tim1 = millis();
  //Debug print Statements
  //UserSerial.print("Time =");
  //UserSerial.println(Tim1);
  //Actual Calculations
  //GetCommander();
  
  GaitPeriod = 5000;
  GaitMoveX = 65;
  GaitMoveY = 0;
  GaitStepHeight = 65;
  GaitMoveZrot = 0;
  GaitBodyX = 0;
  GaitBodyY = 0;
  GaitBodyPitch = 0;
  GaitBodyRoll = 0;
  GaitBodyYaw = 0;
  
  GaitGen(GaitPeriod,Tim1,GaitMoveX,GaitMoveY,10+GaitStepHeight,GaitMoveZrot);
  BodyMod(GaitBodyX,GaitBodyY,GaitBodyZ,GaitBodyPitch,GaitBodyRoll,GaitBodyYaw);
  LegCoords();
  LegCalculate();
  
  for(int i = 0; i < NUM_LEGS; i++){//Update LegDynamixels
    for(int j = 0; j < NUM_SERVOS_PER_LEG; j++){
      LegDynamixels[i][j] = LegCalculateOut[i][j];
    }
  }
  //sphericalCoords goes here
  
  
  DXLServoMap();//Output motor values
  //PWMServoMap();
  
  
  
  //Debug Print Statements
  #if defined(UserSerialTransmit)
    Serial.print(Tim1);
    for (int i = 0; i < 4; i++){
      UserSerial.println("------");
      UserSerial.print("Coxa = ");
      UserSerial.println(LegDynamixels[i][0]);
      UserSerial.print("Femur = ");
      UserSerial.println(LegDynamixels[i][1]);
      UserSerial.print("Tibia = ");
      UserSerial.println(LegDynamixels[i][2]);
      UserSerial.print("Tibia2 = ");
      UserSerial.println(LegDynamixels[i][3]);
      UserSerial.print("Tharsus = ");
      UserSerial.println(LegDynamixels[i][4]);
      UserSerial.println("\n");
    }
  #endif
  delay(100);

}
