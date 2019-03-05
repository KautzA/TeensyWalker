//ik
//Translate global coordinates to local coordinates
void LegCoords(){
  int output_3[NUM_LEGS][3];
  //Leg 0
  output_3[0][0] = (body_mod_out[0][0] + COG_X);
  output_3[0][1] = (body_mod_out[0][1] - COG_Y);
  output_3[0][2] = body_mod_out[0][2];

  output_3[1][0] = (body_mod_out[1][0] - COG_X);
  output_3[1][1] = (body_mod_out[1][1] - COG_Y);
  output_3[1][2] = body_mod_out[1][2];

  output_3[2][0] = -(body_mod_out[2][0] - COG_X);
  output_3[2][1] = -(body_mod_out[2][1] + COG_Y);
  output_3[2][2] = body_mod_out[2][2];

  output_3[3][0] = -(body_mod_out[3][0] + COG_X);
  output_3[3][1] = -(body_mod_out[3][1] + COG_Y);
  output_3[3][2] = body_mod_out[3][2];


  //Return Output3 to LegCoordsOut
  for (int i = 0; i < NUM_LEGS; i++){
    for (int j = 0; j < 3; j++){
      leg_coords_out[i][j] = output_3[i][j];
    }
  }
}

//-----------------------------------------------------------------
//Translate local coordinates into servo angles in radians
void LegCalculate(){
  float output_4[NUM_LEGS][NUM_SERVOS_PER_LEG];
  for (int i = 0; i < NUM_LEGS; i++){
    float pos_x = leg_coords_out[i][0];
    float pos_y = leg_coords_out[i][1];
    float pos_z = leg_coords_out[i][2];
    float leg_radius = sqrt((pos_x*pos_x)+(pos_y*pos_y));
    //UserSerial.print("Leg Calculate ");
    //UserSerial.print(pos_x);
    //UserSerial.print(",");
    //UserSerial.print(pos_x);
    //UserSerial.print(",");
    //UserSerial.print(pos_y);
    //UserSerial.print(",");
    //UserSerial.print(pos_z);
    //UserSerial.print(",");
    //UserSerial.print(((FemurLength*FemurLength)-((leg_radius*leg_radius)+(pos_z*pos_z))-(TibiaLength*TibiaLength))/(-2*sqrt((leg_radius*leg_radius)+(pos_z*pos_z))*TibiaLength));
    //UserSerial.println(",");
    
    output_4[i][0] = -(atanf(pos_y/pos_x));
    //Output4[i][0] = -(atan2(-pos_y,pos_x));
    ;//Coxa
    output_4[i][1] = (atan2f(pos_z,leg_radius) + acos(((FEMUR_LENGTH*FEMUR_LENGTH)-((leg_radius*leg_radius)+(pos_z*pos_z))-(TIBIA_LENGTH*TIBIA_LENGTH))/(-2*sqrt((leg_radius*leg_radius)+(pos_z*pos_z))*TIBIA_LENGTH)));//Femur
    output_4[i][2] = -(acosf(((TIBIA_LENGTH*TIBIA_LENGTH)+(FEMUR_LENGTH*FEMUR_LENGTH)-((leg_radius*leg_radius)+(pos_z*pos_z)))/(-2*FEMUR_LENGTH*TIBIA_LENGTH)));//Tibia
    output_4[i][3] = (0);
    output_4[i][4] = (HALF_PI + output_4[i][2] + output_4[i][1]);//Tharsus
  }
  for (int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < NUM_SERVOS_PER_LEG; j++){
      leg_dynamixels[i][j] = output_4[i][j];
    }
  }
}
