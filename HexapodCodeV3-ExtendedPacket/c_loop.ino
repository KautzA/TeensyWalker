void loop() {
  Tim1 = millis();
  
  //Blink the running LED
  if(Tim1%500<150){
    digitalWrite(LED_YEL,HIGH);
  }
  else{
    digitalWrite(LED_YEL,LOW);
    digitalWrite(LED_RED,LOW);
  }
  
  GetInputs();
  GaitGen(GaitPeriod,Tim1,GaitMoveX,GaitMoveY,GaitStepHeight,GaitMoveZrot);
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
    Serial.println(Tim1);
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
      UserSerial.print("\n");
    }
  #endif
  delay(50);

}
