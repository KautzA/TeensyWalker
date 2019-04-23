//bodymod
//This code covers the basic Gait Generator
void BodyMod(int translate_x, int translate_y, float translate_z, float pitch, float roll, float yaw){//input is in degrees
  //Scale the inputs
  translate_x    =map(translate_x,-127,127,-40,40);
  translate_y    =map(translate_y,-127,127,-40,40);
  translate_z  =map(translate_z,-127,127,-40,40);
  pitch    =map(pitch,-127,127,-30,30);
  roll     =map(roll,-127,127,-30,30);
  yaw      =map(yaw,-127,127,-30,30);



  int body_output_1[NUM_LEGS][3];
  int body_output_2[NUM_LEGS][3];
  pitch = pitch*0.0174533;
  roll = roll*0.0174533;
  yaw = yaw*0.0174533;
  for (int i = 0; i < NUM_LEGS; i++){
    for (int j = 0; j <3; j++){
      body_output_1[i][j] = gait_gen_out[i][j];
    }
  }
  
  //First pass does body translate relative to legs

  body_output_1[0][0] += translate_x;
  body_output_1[0][1] += translate_y;
  body_output_1[0][2] -= translate_z;

  body_output_1[1][0] -= translate_x;
  body_output_1[1][1] -= translate_y;
  body_output_1[1][2] -= translate_z;

  body_output_1[2][0] += translate_x;
  body_output_1[2][1] += translate_y;
  body_output_1[2][2] -= translate_z;

  body_output_1[3][0] -= translate_x;
  body_output_1[3][1] -= translate_y;
  body_output_1[3][2] -= translate_z;
  
  
  float pitch_matrix[3][3] ={
   {1,         0,          0},
   {0,cosf(pitch),-sinf(pitch)},
   {0,sinf(pitch), cosf(pitch)}
  };
  float roll_matrix[3][3] ={
   { cosf(roll),0,sinf(roll)},
   {         0,1,        0},
   {-sinf(roll),0,cosf(roll)}
  };
  float yaw_matrix[3][3] ={
   {cosf(yaw),-sinf(yaw),0},
   {sinf(yaw), cosf(yaw),0},
   {       0,        0,1}
  };
  
  //Pitch
  for(int i = 0; i < NUM_LEGS; i++){
    body_output_2[i][0] = pitch_matrix[0][0]*body_output_1[i][0] + pitch_matrix[0][1]*body_output_1[i][1] + pitch_matrix[0][2]*body_output_1[i][2];
    body_output_2[i][1] = pitch_matrix[1][0]*body_output_1[i][0] + pitch_matrix[1][1]*body_output_1[i][1] + pitch_matrix[1][2]*body_output_1[i][2];
    body_output_2[i][2] = pitch_matrix[2][0]*body_output_1[i][0] + pitch_matrix[2][1]*body_output_1[i][1] + pitch_matrix[2][2]*body_output_1[i][2];
  }
  //Roll
  for(int i = 0; i < NUM_LEGS; i++){
    body_output_1[i][0] = roll_matrix[0][0]*body_output_2[i][0] + roll_matrix[0][1]*body_output_2[i][1] + roll_matrix[0][2]*body_output_2[i][2];
    body_output_1[i][1] = roll_matrix[1][0]*body_output_2[i][0] + roll_matrix[1][1]*body_output_2[i][1] + roll_matrix[1][2]*body_output_2[i][2];
    body_output_1[i][2] = roll_matrix[2][0]*body_output_2[i][0] + roll_matrix[2][1]*body_output_2[i][1] + roll_matrix[2][2]*body_output_2[i][2];
  }
  //Yaw
  for(int i = 0; i < NUM_LEGS; i++){
    body_output_2[i][0] = yaw_matrix[0][0]*body_output_1[i][0] + yaw_matrix[0][1]*body_output_1[i][1] + yaw_matrix[0][2]*body_output_1[i][2];
    body_output_2[i][1] = yaw_matrix[1][0]*body_output_1[i][0] + yaw_matrix[1][1]*body_output_1[i][1] + yaw_matrix[1][2]*body_output_1[i][2];
    body_output_2[i][2] = yaw_matrix[2][0]*body_output_1[i][0] + yaw_matrix[2][1]*body_output_1[i][1] + yaw_matrix[2][2]*body_output_1[i][2];
  }
  //Second pass does pitch (ZY)
  /*for(int i = 0; i < 4; i++){
    body_output_2[i][0] = body_output_1[i][0];
    body_output_2[i][1] = (body_output_1[i][2]*cos(pitch)-body_output_1[i][1]*sin(pitch));
    body_output_2[i][2] = (body_output_1[i][2]*cos(pitch)-body_output_1[i][1]*sin(pitch));
  }
  
  //Third pass does roll (ZX)
  for(int i = 0; i < 4; i++){
    body_output_1[i][0] = (body_output_2[i][0]*cos(roll)-body_output_2[i][2]*sin(roll));
    body_output_1[i][1] = body_output_2[i][1];
    body_output_1[i][2] = (body_output_2[i][0]*cos(roll)-body_output_2[i][2]*sin(roll));
  }

  //Fourth pass does yaw (XY)
  for(int i = 0; i < 4; i++){
    body_output_2[i][0] = (body_output_1[i][0]*cos(yaw)-body_output_1[i][1]*sin(yaw));
    body_output_2[i][1] = (body_output_1[i][0]*cos(yaw)-body_output_1[i][1]*sin(yaw));
    body_output_2[i][2] = body_output_1[i][2];
  }*/


  //Update GaitGenOut
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      body_mod_out[i][j] = body_output_2[i][j];
    }
  }

  //Debug Print 
  //UserSerial.print("Generator 1 = ");
  //UserSerial.println(Generator1);
  //UserSerial.print("Generator 2 = ");
  //UserSerial.println(Generator2);
  //UserSerial.print("Generator Z = ");
  //UserSerial.println(GeneratorZ);
  //UserSerial.print("Type1 = ");
  //UserSerial.print(Type1[0]);
  //UserSerial.print(" ");
  //UserSerial.print(Type1[1]);
  //UserSerial.print(" ");
  //UserSerial.println(Type1[2]);
  //UserSerial.print("Type2 = ");
  //UserSerial.print(Type2[0]);
  //UserSerial.print(" ");
  //UserSerial.print(Type2[1]);
  //UserSerial.print(" ");
  //UserSerial.println(Type2[2]);



}
