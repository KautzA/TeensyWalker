void loop() {
  Tim1 = millis();
  //Debug print Statements
  //UserSerial.print("Time =");
  //UserSerial.println(Tim1);
  //Actual Calculations
  while(UserSerial.available() > 0){
    UserSerial.read();
  }
  
  UserSerial.println("Select leg number");
  while(UserSerial.available() <=0){
    delay(1);
  }
  int ActLeg = UserSerial.parseInt();

  /*
  UserSerial.println("Enter space Coordinates, x,y,z");
  while(UserSerial.available() <=0){
    delay(1);
  }
  BodyModOut[0][0] = UserSerial.parseInt();
  while(UserSerial.available() <=0){
    delay(1);
  }
  BodyModOut[0][1] = UserSerial.parseInt();
  while(UserSerial.available() <=0){
    delay(1);
  }
  BodyModOut[0][2] = UserSerial.parseInt();
  //*/
  
  BodyModOut[0][0] = Leg0InitX;
  BodyModOut[0][1] = Leg0InitY;
  BodyModOut[0][2] = Leg0InitZ;
  
  BodyModOut[1][0] = Leg1InitX;
  BodyModOut[1][1] = Leg1InitY;
  BodyModOut[1][2] = Leg1InitZ;
  
  BodyModOut[2][0] = Leg2InitX;
  BodyModOut[2][1] = Leg2InitY;
  BodyModOut[2][2] = Leg2InitZ;
  
  BodyModOut[3][0] = Leg3InitX;
  BodyModOut[3][1] = Leg3InitY;
  BodyModOut[3][2] = Leg3InitZ;
  //*/
  
  UserSerial.println("Enter Rotation Coordinates, Azimuth,Elevation");
  while(UserSerial.available() <=0){
    delay(1);
  }
  int LegAzimuth = UserSerial.parseInt();
  while(UserSerial.available() <=0){
    delay(1);
  }
  int LegElevation = UserSerial.parseInt();
   
  LegCoords();
  LegCalculate();
  
  for(int i = 0; i < NUM_LEGS; i++){//Update LegDynamixels
    for(int j = 0; j < NUM_SERVOS_PER_LEG; j++){
      LegDynamixels[i][j] = LegCalculateOut[i][j];
    }
  }
  float Angles[2] = {((LegAzimuth)*(3.14/180)),((LegElevation)*(3.14/180))};
  LocalServoGen(ActLeg,Angles[0],Angles[1],LegDynamixels[ActLeg][0],LegDynamixels[ActLeg][1],LegDynamixels[ActLeg][2]);
  
  //sphericalCoords goes here
  
  
  DXLServoMap();//Output motor values
  //PWMServoMap();
  
  
  UserSerial.println("");
  UserSerial.print("X = ");
  UserSerial.println(BodyModOut[0][0]);
  UserSerial.print("Y = ");
  UserSerial.println(BodyModOut[0][1]);
  UserSerial.print("Z = ");
  UserSerial.println(BodyModOut[0][2]);
  UserSerial.print("A = ");
  UserSerial.println(Angles[0]);
  UserSerial.print("E = ");
  UserSerial.println(Angles[1]);
  
  
  
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
