//Functions to control the Gimbal using PPM


void GimbalControl(float pos_x, float pos_y, float gimbal_command){//pos_x and _y go from -127 to 127, gimbal_command is 0,1,2 
  pos_x = map(pos_x, -127, 127, 1000, 2000);
  pos_x = map(pos_y, -127, 127, 1000, 2000);
  gimbal_command = map(gimbal_command, 0, 2, 1000, 2000);
  
  GimbalOut.write(1, pos_x);
  GimbalOut.write(1, pos_y);
  GimbalOut.write(1, gimbal_command);
}
