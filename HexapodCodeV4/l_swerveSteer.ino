//Swervesteer
const int SWERVEBASEWIDTH = 300;
const int SWERVEBASELENGTH = 300;

void SwerveSteer(float Xmove, float Ymove, float Zrot, int WheelWidth, int WheelLength){
  //map inputs
  Xmove = float(map(Xmove,-127,127,-100,100))/100.0;
  Ymove = float(map(Ymove,-127,127,-100,100))/100.0;
  Zrot  = float(map(Zrot,-127,127,314,-314))/100.0;//zrot is in radians from +x and counterclockwise is positive
  WheelWidth = map(WheelWidth,-127,127,-100,100)+SWERVEBASEWIDTH;
  WheelLength = map(WheelWidth,-127,127,-100,100)+SWERVEBASELENGTH;
  
  float WheelVelocity = 0.001; //value unknown
  
  //calcualte the leg positions and output them
  int LegPos[NUM_LEGS][3] = {
                {-WheelWidth/2,WheelLength/2,kInitialPositions[0][2]},
                {WheelWidth/2,WheelLength/2,kInitialPositions[1][2]},
                {WheelWidth/2,-WheelLength/2,kInitialPositions[2][2]},
                {-WheelWidth/2,-WheelLength/2,kInitialPositions[3][2]}};
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      gait_gen_out[i][j] = LegPos[i][j];
    }
  }
  
  
  
  
  //Code that does the swerve steering--------------------------------------------------------
  //idea taken from http://www.simbotics.org/resources/mobility/omnidirectional-drive
  float ModeA = Xmove - (Zrot*WheelLength/2);
  float ModeB = Xmove + (Zrot*WheelLength/2);
  float ModeC = Ymove - (Zrot*WheelWidth/2);
  float ModeD = Ymove + (Zrot*WheelWidth/2);
  
  float LegWheelSpherical1[NUM_LEGS][3]{//Azimuth, Elevation, RotationSpeed
    {atan2(ModeB,ModeC),0,sqrt(ModeB*ModeB+ModeC*ModeC)},//FR
    {atan2(ModeA,ModeC),0,sqrt(ModeA*ModeA+ModeC*ModeC)},//BR
    {atan2(ModeA,ModeD),0,sqrt(ModeA*ModeA+ModeD*ModeD)},//BL
    {atan2(ModeB,ModeD),0,sqrt(ModeB*ModeB+ModeD*ModeD)}};//FL
  
  //duplicate values for scaling
  float LegWheelSpherical2[4][3];
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 3; j++){
      LegWheelSpherical2[i][j] = LegWheelSpherical1[i][j];
    }
  }
  
  
  //Scale values so largest has a max of 1
  if (LegWheelSpherical1[0][2] > 1){
    for(int i = 0; i < 4; i++){
      LegWheelSpherical2[i][2] /= LegWheelSpherical1[0][2];
    }
  }
  if (LegWheelSpherical1[1][2] > 1){
    for(int i = 0; i < 4; i++){
      LegWheelSpherical2[i][2] /= LegWheelSpherical1[1][2];
    }
  }
  if (LegWheelSpherical1[2][2] > 1){
    for(int i = 0; i < 4; i++){
      LegWheelSpherical2[i][2] /= LegWheelSpherical1[2][2];
    }
  }
  if (LegWheelSpherical1[3][2] > 1){
    for(int i = 0; i < 4; i++){
      LegWheelSpherical2[i][2] /= LegWheelSpherical1[3][2];
    }
  }
  
  //Set the spherical part
  for(int i = 0; i < 3; i++){
    leg_global_spherical[i][0] = LegWheelSpherical1[1][0];
    leg_global_spherical[i][1] = LegWheelSpherical1[1][1];
    leg_global_spherical[i][2] += LegWheelSpherical1[1][2]*WheelVelocity;
  }
}
