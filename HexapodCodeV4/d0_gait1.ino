//gait1
//This code covers the basic Gait Generator it moves diagonal pairs of legs on a quad
void GaitGen1(int period, int cycle, int trans_x, int trans_y, int trans_z, float turn_z){
  //Sale the inputs
  trans_x =map(trans_x,-127,127,-40,40);
  trans_y =map(trans_y,-127,127,-40,40);
  trans_z =map(trans_z,-127,127,0,20);
  turn_z  =-map(turn_z,-127,127,-50,50)/100;

  float generator1 = ((4.0 *(abs((cycle%period)-(period/2))-(period/4)))/period);
  int generator_z = (-1*((abs((cycle%period)-(period/2)))/((cycle%period)-(period/2))));
  float theta1 = (turn_z * PI / 4 *generator1);
  
  //Type1 and Type two are added to legs to produce movement
  float offsets1[3] = {(trans_x*generator1),(trans_y*generator1),(trans_z*generator_z)};
  float offsets2[3] = {(trans_x*generator1),(trans_y*generator1),(-trans_z*generator_z)};


  //First pass does gait generation for translation
  int output1[NUM_LEGS][3] = {
    { //Leg 0
      (kInitialPositions[0][0] + offsets1[0]),//X (output[0][0])
      (kInitialPositions[0][1] + offsets1[1]),//Y (output[0][1])
      (kInitialPositions[0][2] + offsets1[2]) //Z (output[0][2])
    },//end Leg 0
    { //Leg 1
      (kInitialPositions[1][0] + offsets2[0]),//X
      (kInitialPositions[1][1] + offsets2[1]),//Y
      (kInitialPositions[1][2] + offsets2[2]) //Z
    },//end Leg 1
    { //Leg 2
      (kInitialPositions[2][0] + offsets1[0]),//X
      (kInitialPositions[2][1] + offsets1[1]),//Y
      (kInitialPositions[2][2] + offsets1[2]) //Z
    },//end Leg 2
    { //Leg 3
      (kInitialPositions[3][0] + offsets2[0]),//X
      (kInitialPositions[3][1] + offsets2[1]),//Y
      (kInitialPositions[3][2] + offsets2[2]) //Z
    },//end Leg 3
    };


    //Second pass does gait rotate in place
    int output2[NUM_LEGS][3];
  output2[0][0] = (output1[0][0]*cosf(theta1)-output1[0][1]*sinf(theta1));
  output2[0][1] = (output1[0][0]*sinf(theta1)+output1[0][1]*cosf(theta1));
  output2[0][2] = output1[0][2];

  output2[1][0] = (output1[1][0]*cosf(-theta1)-output1[1][1]*sinf(-theta1));
  output2[1][1] = (output1[1][0]*sinf(-theta1)+output1[1][1]*cosf(-theta1));
  output2[1][2] = output1[1][2];

  output2[2][0] = (output1[2][0]*cosf(theta1)-output1[2][1]*sinf(theta1));
  output2[2][1] = (output1[2][0]*sinf(theta1)+output1[2][1]*cosf(theta1));
  output2[2][2] = output1[2][2];

  output2[3][0] = (output1[3][0]*cosf(-theta1)-output1[3][1]*sinf(-theta1));
  output2[3][1] = (output1[3][0]*sinf(-theta1)+output1[3][1]*cosf(-theta1));
  output2[3][2] = output1[3][2];

  //Update GaitGenOut
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      gait_gen_out[i][j] = output2[i][j];
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
