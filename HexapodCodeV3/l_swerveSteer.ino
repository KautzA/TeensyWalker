


void SwerveSteer(float Xmove, float Ymove, float Zrot, int WheelWidth = 300, int WheelLength = 300){
  
  Xmove = map(Xmove,-127,127,-100,100)/100;
  Ymove = map(Ymove,-127,127,-100,100)/100;
  Zrot  = map(Zrot,-127,127,314,-314)/100;//zrot is in radians and counterclockwise is positive
  
  //calcualte the leg positions
  int LegPos[NUM_LEGS][3] = {
                {-WheelWidth/2,WheelLength/2,Leg0InitZ},
                {WheelWidth/2,WheelLength/2,Leg1InitZ},
                {WheelWidth/2,-WheelLength/2,Leg2InitZ},
                {-WheelWidth/2,-WheelLength/2,Leg3InitZ}};
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      GaitGenOut[i][j] = LegPos[i][j];
    }
  }
  
  
  
  
  //Code that does the swerve steering--------------------------------------------------------
  float ModeA = Xmove - (Zrot*WheelLength/2);
  float ModeB = Xmove + (Zrot*WheelLength/2);
  float ModeC = Ymove - (Zrot*WheelWidth/2);
  float ModeD = Ymove + (Zrot*WheelWidth/2);
  
  float LegWheelSpherical1[NUM_LEGS][3]{//Azimuth, Elevation, RotationSpeed
    {atan2(ModeB,ModeC),0,sqrt(ModeB*ModeB+ModeC*ModeC)},//FR
    {atan2(ModeA,ModeC),0,sqrt(ModeA*ModeA+ModeC*ModeC)},//BR
    {atan2(ModeA,ModeD),0,sqrt(ModeA*ModeA+ModeD*ModeD)},//BL
    {atan2(ModeB,ModeD),0,sqrt(ModeB*ModeB+ModeD*ModeD)}};//FL
  
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
  
  //Set the legs and motors
}
