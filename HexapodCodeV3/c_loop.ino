void loop() {
  Tim1 = millis();

  //Display the time on the display
  MainDisplay();
#if defined(OLED_ENABLE)
  display.display();
#endif



  GetInputs();
  switch (MoveMode){
  case MOVE_MODE_WALK_PERIODIC:
    GaitGen1(GaitPeriod,Tim1,GaitMoveX,GaitMoveY,GaitStepHeight,GaitMoveZrot);
    break;
  case MOVE_MODE_CRAWL_PERIODIC:
    GaitGen2(GaitPeriod,Tim1,GaitMoveX,GaitMoveY,GaitStepHeight,GaitMoveZrot);
    break;
  }
  BodyMod(GaitBodyX,GaitBodyY,GaitBodyZ,GaitBodyPitch,GaitBodyRoll,GaitBodyYaw);
  LegCoords();
  LegCalculate();

  for(int i = 0; i < NUM_LEGS; i++){//Update LegDynamixels
    for(int j = 0; j < NUM_SERVOS_PER_LEG; j++){
      LegDynamixels[i][j] = LegCalculateOut[i][j];
    }
  }

  //sphericalCoords goes here

  GimbalControl(CamPan, CamTilt, GimbalMode);
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

  //Blink the running LED
  if(Tim1%500<150){
    digitalWrite(LED_YEL,HIGH);
  }
  else{
    digitalWrite(LED_YEL,LOW);
  }
  //Handle error LED patterns
  if(ErrorState & b00000001){//DXL Error
    if(Tim1%500<150){
      digitalWrite(LED_RED,HIGH);
    }
    else{
      digitalWrite(LED_RED,LOW);
    }
  }
  if(ErrorState & b00000010){//CMD error
    if(Tim1%500<250){
      digitalWrite(LED_RED,HIGH);
    }
    else{
      digitalWrite(LED_RED,LOW);
    }
  }

  delay(50);

}




