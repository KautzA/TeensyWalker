//setup
void setup(){


  //Open Serial Ports
  USER_SERIAL.begin(38400);
  COMMAND_SERIAL.begin(38400);

#if defined(GIMBAL_ENABLE)//Gimbal Enable
  GimbalOut.begin(GIMBAL_PPM_PIN);
#endif//end gimbal enable

#if defined(OLED_ENABLE)//OLED Enable
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
#endif//End OLED Enable
  delay(2000);

#if defined(USER_SERIAL_TRANSMIT)
  USER_SERIAL.println("Started");
#endif

  //Set pinmode for LED pins
  pinMode(BUTTON_RGB_PIN,BUTTON_RGB_MODE);
  pinMode(LED_RUNNING,OUTPUT);
  pinMode(LED_ERROR,OUTPUT);
  pinMode(LED_RGB_RED,OUTPUT);
  pinMode(LED_RGB_GRE,OUTPUT);
  pinMode(LED_RGB_BLU,OUTPUT);


  //Setup Dynamixels
  bioloid.begin(1000000,&DXL_SERIAL,-1);
  bioloid.poseSize = NUM_SERVOS;
  //PWMI2C_Begin();
  GaitGenControlReset();

  //Verify All servos are in place
  for (int i = 0; i < NUM_LEGS; i++){
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++){
      if (ServoPosQuerry(kDXLServoLimits[i][j][2])== -1){
        servo_check_val[i][j] = 1;
#if defined(USER_SERIAL_TRANSMIT)
        USER_SERIAL.print("Servo ");
        USER_SERIAL.print(kDXLServoLimits[i][j][2]);
        USER_SERIAL.println(" is missing");
#endif
      }
      else{
#if defined(USER_SERIAL_TRANSMIT)
        USER_SERIAL.print("Servo ");
        USER_SERIAL.print(kDXLServoLimits[i][j][2]);
        USER_SERIAL.print(" is at ");
        USER_SERIAL.println(ServoPosQuerry(kDXLServoLimits[i][j][2]));
#endif
      }
    }
  }


  //Test if servo has reset to ID 1
  if (ServoPosQuerry(1)!= -1){
    USER_SERIAL.println("Servo 1 Detected, Error?");
  }
}

