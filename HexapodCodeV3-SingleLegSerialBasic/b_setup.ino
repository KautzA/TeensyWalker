void setup(){
  
  

  UserSerial.begin(38400);
  //COMMANDSerial.begin(38400);
  delay(2000);
  UserSerial.println("Started");
  //Setup Dynamixels
  bioloid.begin(1000000,&AXSerial,-1);
  bioloid.poseSize = NumServos;
  PWMI2C_Begin();
  GaitGenControlReset();
}
