//Untested and not added to the code, here for memory and future use

/*

 +Z
 ^       +X
 |       /
 |     /
 |   /
 | /
 o------------> +Y
 
 X is to the front of the servo
 
 
 */

void LocalSpherical(float Angles[NUM_LEGS][2]){
  /*float LocalLeg1[2]={Angles[0][0],Angles[0][1]};
   float LocalLeg2[2]={-Angles[1][0],-Angles[1][1]};
   float LocalLeg3[2]={-Angles[2][0],-Angles[2][1]};
   float LocalLeg3[2]={Angles[3][0],Angles[3][1]};*/
  Angles[1][0] *= -1;
  Angles[1][1] *= -1;
  Angles[2][0] *= -1;
  Angles[2][1] *= -1;

  for(int i = 0; i < NUM_LEGS; i++){
    LocalServoGen(i,Angles[i][0],Angles[i][1],LegDynamixels[i][0],LegDynamixels[i][1],LegDynamixels[i][2]);
  }
}




void LocalServoGen(int leg,float azimuth, float elevation, float coxa, float femur, float tibia){//Calculates new TibaRot and Tarsus based on local azimuth and elevation
  //float IniVect[3] = {cos(coxa)*sin(femur+tibia),sin(coxa)*cos(femur+tibia),cos(femur+tibia)};//vector of the leg(x,y,z)
  azimuth -=coxa;//The first rotation is faster this way, no trig compared to a cartesian rotation
  float DesVect1[3] = {
    10*cosf(azimuth)*sinf(elevation),10*sinf(azimuth)*sinf(elevation),10*cosf(elevation)      };//output vector(x,y,z)
  float DesVect2[3];
  //desired vector


  float pitch = (1.5708 + femur + tibia);
  UserSerial.println(pitch);


  float XZCorr[3][3] = {//Matrix to rotate point to be in proper loaction for sphericalcoords
    {cosf(pitch),0,sinf(pitch)},
    {0,1,0},
    {-sinf(pitch),0,cosf(pitch)}};
    
    
#if defined(UserSerialTransmit)
  UserSerial.print("Angle x ");
  UserSerial.println(DesVect1[0]);
  UserSerial.print("Angle y ");
  UserSerial.println(DesVect1[1]);
  UserSerial.print("Angle z ");
  UserSerial.println(DesVect1[2]);
#endif


  //Preform the second rotation
  DesVect2[0] = XZCorr[0][0]*DesVect1[0] + XZCorr[0][1]*DesVect1[1] + XZCorr[0][2]*DesVect1[2];
  DesVect2[1] = XZCorr[1][0]*DesVect1[0] + XZCorr[1][1]*DesVect1[1] + XZCorr[1][2]*DesVect1[2];
  DesVect2[2] = XZCorr[2][0]*DesVect1[0] + XZCorr[2][1]*DesVect1[1] + XZCorr[2][2]*DesVect1[2];

  LegDynamixels[leg][3] = atan2f(DesVect2[1],DesVect2[0]);//Tibia Rotation in Degrees
  LegDynamixels[leg][4] = -(acosf(DesVect2[2]/10));//Tarsus

#if defined(UserSerialTransmit)
  UserSerial.print("FirstShift x ");
  UserSerial.println(DesVect2[0]);
  UserSerial.print("FirstShift y ");
  UserSerial.println(DesVect2[1]);
  UserSerial.print("FirstShift z ");
  UserSerial.println(DesVect2[2]);

  UserSerial.print("SecondShift x ");
  UserSerial.println(DesVect1[0]);
  UserSerial.print("SecondShift y ");
  UserSerial.println(DesVect1[1]);
  UserSerial.print("SecondShift z ");
  UserSerial.println(DesVect1[2]);

  UserSerial.println("");
#endif

  //other stuff
}


void LegPlace (uint8_t leg, int Xplace, int Yplace, int Zplace, int Xrot, int Yrot, int Zrot){
  int Output0[NUM_LEGS][3];
  for (int i = 0; i<NUM_LEGS; i++){
    for (int j = 0; j<3; j++){
      Output0[i][j] = InitialPositions[i][j];
    }
  }
  
  Output0[leg][0] = Xplace;
  Output0[leg][1] = Yplace;
  Output0[leg][2] = Zplace;
}



