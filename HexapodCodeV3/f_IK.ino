//ik
//Translate global coordinates to local coordinates
void LegCoords(){
  int Output3[NUM_LEGS][3];
  //Leg 0
  Output3[0][0] = (BodyModOut[0][0] + CogX);
  Output3[0][1] = (BodyModOut[0][1] - CogY);
  Output3[0][2] = BodyModOut[0][2];

  Output3[1][0] = (BodyModOut[1][0] - CogX);
  Output3[1][1] = (BodyModOut[1][1] - CogY);
  Output3[1][2] = BodyModOut[1][2];

  Output3[2][0] = -(BodyModOut[2][0] - CogX);
  Output3[2][1] = -(BodyModOut[2][1] + CogY);
  Output3[2][2] = BodyModOut[2][2];

  Output3[3][0] = -(BodyModOut[3][0] + CogX);
  Output3[3][1] = -(BodyModOut[3][1] + CogY);
  Output3[3][2] = BodyModOut[3][2];


  //Return Output2 to LegCoordsOut
  for (int i = 0; i < NUM_LEGS; i++){
    for (int j = 0; j < 3; j++){
      LegCoordsOut[i][j] = Output3[i][j];
    }
  }
}

//-----------------------------------------------------------------
//Translate local coordinates into servo angles in radians
void LegCalculate(){
  float Output4[NUM_LEGS][NUM_SERVOS_PER_LEG];
  for (int i = 0; i < NUM_LEGS; i++){
    float Xpos = LegCoordsOut[i][0];
    float Ypos = LegCoordsOut[i][1];
    float Zpos = LegCoordsOut[i][2];
    float LegRadius = sqrt((Xpos*Xpos)+(Ypos*Ypos));
    //UserSerial.print("Leg Calculate ");
    //UserSerial.print(Xpos);
    //UserSerial.print(",");
    //UserSerial.print(Xpos);
    //UserSerial.print(",");
    //UserSerial.print(Ypos);
    //UserSerial.print(",");
    //UserSerial.print(Zpos);
    //UserSerial.print(",");
    //UserSerial.print(((FemurLength*FemurLength)-((LegRadius*LegRadius)+(Zpos*Zpos))-(TibiaLength*TibiaLength))/(-2*sqrt((LegRadius*LegRadius)+(Zpos*Zpos))*TibiaLength));
    //UserSerial.println(",");
    
    Output4[i][0] = -(atanf(Ypos/Xpos));
    //Output4[i][0] = -(atan2(-Ypos,Xpos));
    ;//Coxa
    Output4[i][1] = (atan2f(Zpos,LegRadius) + acos(((FEMUR_LENGTH*FEMUR_LENGTH)-((LegRadius*LegRadius)+(Zpos*Zpos))-(TIBIA_LENGTH*TIBIA_LENGTH))/(-2*sqrt((LegRadius*LegRadius)+(Zpos*Zpos))*TIBIA_LENGTH)));//Femur
    Output4[i][2] = -(acosf(((TIBIA_LENGTH*TIBIA_LENGTH)+(FEMUR_LENGTH*FEMUR_LENGTH)-((LegRadius*LegRadius)+(Zpos*Zpos)))/(-2*FEMUR_LENGTH*TIBIA_LENGTH)));//Tibia
    Output4[i][3] = (0);
    Output4[i][4] = (HALF_PI + Output4[i][2] + Output4[i][1]);//Tharsus
  }
  for (int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < NUM_SERVOS_PER_LEG; j++){
      LegDynamixels[i][j] = Output4[i][j];
    }
  }
}
