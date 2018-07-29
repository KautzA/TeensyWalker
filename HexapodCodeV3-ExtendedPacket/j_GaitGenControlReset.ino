//Code to reset Gait gen control variables
//Stops robot and sets cycle to 5 seconds and sep height to 10 cm
void GaitGenControlReset(){
  GaitPeriod = 2500;
  GaitMoveX = 0;
  GaitMoveY = 0;
  GaitStepHeight = 10;
  GaitMoveZrot = 0;
  GaitBodyX = 0;
  GaitBodyY = 0;
  GaitBodyZ = 0;
  GaitBodyPitch = 0;
  GaitBodyRoll = 0;
  GaitBodyYaw = 0;
}
