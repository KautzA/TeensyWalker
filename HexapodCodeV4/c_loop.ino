//loop
void loop() {
  time_1 = millis();
  if ((time_1 - last_voltage_read) > 500){ //read the voltage of servo two every half second
    current_voltage = ax12GetRegister(2,AX_PRESENT_VOLTAGE,1);
    last_voltage_read = time_1;
  }
  

  //Display the time on the display
  MainDisplay();
#if defined(OLED_ENABLE)
  display.display();
#endif



  GetInputs();
  switch (move_mode){
  case MOVE_MODE_WALK_PERIODIC:
    GaitGen1(gait_period, time_1, gait_move_x, gait_move_y, gait_step_height, gait_move_z_rot);
    BodyMod(gait_body_x, gait_body_y, gait_body_z, gait_body_pitch, gait_body_roll, gait_body_yaw);
    LegCoords();
    LegCalculate();
    break;
  case MOVE_MODE_CRAWL_PERIODIC:
    GaitGen2(gait_period, time_1, gait_move_x, gait_move_y, gait_step_height, gait_move_z_rot);
    BodyMod(gait_body_x, gait_body_y, gait_body_z, gait_body_pitch, gait_body_roll, gait_body_yaw);
    LegCoords();
    LegCalculate();
    break;
  case MOVE_MODE_SWERVE:
    SwerveSteer(gait_move_x, gait_move_y, gait_move_z_rot, gait_step_height, gait_step_height);
    BodyMod(gait_body_x, gait_body_y, gait_body_z, gait_body_pitch, gait_body_roll, gait_body_yaw);
    LegCoords();
    LocalSpherical();
    LegPlaceSixCalculate();
    break;
  }
  

  //sphericalCoords goes here
#if defined(GIMBAL_ENABLE)
  GimbalControl(cam_pan, cam_tilt, gimbal_mode);
#endif

  DXLServoMap();//Output motor values
  //PWMServoMap();


  //Debug Print Statements
#if defined(USER_SERIAL_TRANSMIT)
  Serial.println(time_1);
  for (int i = 0; i < 4; i++){
    USER_SERIAL.println("------");
    USER_SERIAL.print("Coxa = ");
    USER_SERIAL.println(leg_dynamixels[i][0]);
    USER_SERIAL.print("Femur = ");
    USER_SERIAL.println(leg_dynamixels[i][1]);
    USER_SERIAL.print("Tibia = ");
    USER_SERIAL.println(leg_dynamixels[i][2]);
    USER_SERIAL.print("Tibia2 = ");
    USER_SERIAL.println(leg_dynamixels[i][3]);
    USER_SERIAL.print("Tharsus = ");
    USER_SERIAL.println(leg_dynamixels[i][4]);
    USER_SERIAL.print("\n");
  }
#endif

  //Blink the running LED
  if(time_1%500<150){
    digitalWrite(LED_RUNNING,HIGH);
  }
  else{
    digitalWrite(LED_RUNNING,LOW);
  }
  //Handle error LED patterns
  if(error_state & B00000001){//DXL Error
    if(time_1%500<150){
      digitalWrite(LED_ERROR,HIGH);
    }
    else{
      digitalWrite(LED_ERROR,LOW);
    }
  }
  if(error_state & B00000010){//CMD error
    if(time_1%500<250){
      digitalWrite(LED_ERROR,HIGH);
    }
    else{
      digitalWrite(LED_ERROR,LOW);
    }
  }

  delay(50);

}




