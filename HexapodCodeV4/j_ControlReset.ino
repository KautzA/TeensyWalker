//controlreset
//Code to reset Gait gen control variables
//Stops robot and sets cycle to 5 seconds and sep height to 10 cm
void GaitGenControlReset(){
  gait_period = 2000;
  gait_move_x = 0;
  gait_move_y = 0;
  gait_step_height = 10;
  gait_move_z_rot = 0;
  gait_body_x = 0;
  gait_body_y = 0;
  gait_body_z = 0;
  gait_body_pitch = 0;
  gait_body_roll = 0;
  gait_body_yaw = 0;
}
