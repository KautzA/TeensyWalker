void setup(){


  //Open Serial Ports
  UserSerial.begin(38400);
  CommandSerial.begin(38400);

#if defined(GIMBAL_ENABLE)//Gimbal Enable
  GimbalOut.begin(GIMBAL_PPM_PIN);
#endif//end gimbal enable

#if defined(OLED_ENABLE)//OLED Enable
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
#endif//End OLED Enable
  delay(2000);

#if defined(UserSerialTransmit)
  UserSerial.println("Started");
#endif

  //Set pinmode for LED pins
  pinMode(LED_YEL,OUTPUT);
  pinMode(LED_RED,OUTPUT);


  //Setup Dynamixels
  bioloid.begin(1000000,&AXSerial,-1);
  bioloid.poseSize = NumServos;
  //PWMI2C_Begin();
  GaitGenControlReset();

  //Verify All servos are in place
  for (int i = 0; i < NUM_LEGS; i++){
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++){
      if (ServoPosQuerry(DXLServoLimits[i][j][2])== -1){
        ServoCheckVal[i][j] = 1;
#if defined(UserSerialTransmit)
        UserSerial.print("Servo ");
        UserSerial.print(DXLServoLimits[i][j][2]);
        UserSerial.println(" is missing");
#endif
      }
      else{
#if defined(UserSerialTransmit)
        UserSerial.print("Servo ");
        UserSerial.print(DXLServoLimits[i][j][2]);
        UserSerial.print(" is at ");
        UserSerial.println(ServoPosQuerry(DXLServoLimits[i][j][2]));
#endif
      }
    }
  }


  //Test if servo has reset to ID 1
  if (ServoPosQuerry(1)!= -1){
    UserSerial.println("Servo 1 Detected, Error?");
  }
}

