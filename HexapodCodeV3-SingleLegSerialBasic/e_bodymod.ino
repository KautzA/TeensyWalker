//This code covers the basic Gait Generator
void BodyMod(int Xlean, int Ylean, float Zheight, float pitch, float roll, float yaw){//input is in degrees
  //Scale the inputs
  Xlean    =map(Xlean,-127,127,-40,40);
  Ylean    =map(Ylean,-127,127,-40,40);
  Zheight  =map(Zheight,-127,127,-40,40);
  pitch    =map(pitch,-127,127,-30,30);
  roll     =map(roll,-127,127,-30,30);
  yaw      =map(yaw,-127,127,-30,30);



  int BodyOutput1[NUM_LEGS][3];
  int BodyOutput2[NUM_LEGS][3];
  pitch = pitch*0.0174533;
  roll = roll*0.0174533;
  yaw = yaw*0.0174533;
  for (int i = 0; i < NUM_LEGS; i++){
    for (int j = 0; j <3; j++){
      BodyOutput1[i][j] = GaitGenOut[i][j];
    }
  }
  
  //First pass does body translate relative to legs

  BodyOutput1[0][0] += Xlean;
  BodyOutput1[0][1] += Ylean;
  BodyOutput1[0][2] -= Zheight;

  BodyOutput1[1][0] -= Xlean;
  BodyOutput1[1][1] -= Ylean;
  BodyOutput1[1][2] -= Zheight;

  BodyOutput1[2][0] += Xlean;
  BodyOutput1[2][1] += Ylean;
  BodyOutput1[2][2] -= Zheight;

  BodyOutput1[3][0] -= Xlean;
  BodyOutput1[3][1] -= Ylean;
  BodyOutput1[3][2] -= Zheight;
  
  
  float PitchMatrix[3][3] ={
   {1,         0,          0},
   {0,cos(pitch),-sin(pitch)},
   {0,sin(pitch), cos(pitch)}
  };
  float RollMatrix[3][3] ={
   { cos(roll),0,sin(roll)},
   {         0,1,        0},
   {-sin(roll),0,cos(roll)}
  };
  float YawMatrix[3][3] ={
   {cos(yaw),-sin(yaw),0},
   {sin(yaw), cos(yaw),0},
   {       0,        0,1}
  };
  
  //Pitch
  for(int i = 0; i < NUM_LEGS; i++){
    BodyOutput2[i][0] = PitchMatrix[0][0]*BodyOutput1[i][0] + PitchMatrix[0][1]*BodyOutput1[i][1] + PitchMatrix[0][2]*BodyOutput1[i][2];
    BodyOutput2[i][1] = PitchMatrix[1][0]*BodyOutput1[i][0] + PitchMatrix[1][1]*BodyOutput1[i][1] + PitchMatrix[1][2]*BodyOutput1[i][2];
    BodyOutput2[i][2] = PitchMatrix[2][0]*BodyOutput1[i][0] + PitchMatrix[2][1]*BodyOutput1[i][1] + PitchMatrix[2][2]*BodyOutput1[i][2];
  }
  //Roll
  for(int i = 0; i < NUM_LEGS; i++){
    BodyOutput1[i][0] = RollMatrix[0][0]*BodyOutput2[i][0] + RollMatrix[0][1]*BodyOutput2[i][1] + RollMatrix[0][2]*BodyOutput2[i][2];
    BodyOutput1[i][1] = RollMatrix[1][0]*BodyOutput2[i][0] + RollMatrix[1][1]*BodyOutput2[i][1] + RollMatrix[1][2]*BodyOutput2[i][2];
    BodyOutput1[i][2] = RollMatrix[2][0]*BodyOutput2[i][0] + RollMatrix[2][1]*BodyOutput2[i][1] + RollMatrix[2][2]*BodyOutput2[i][2];
  }
  //Yaw
  for(int i = 0; i < NUM_LEGS; i++){
    BodyOutput2[i][0] = YawMatrix[0][0]*BodyOutput1[i][0] + YawMatrix[0][1]*BodyOutput1[i][1] + YawMatrix[0][2]*BodyOutput1[i][2];
    BodyOutput2[i][1] = YawMatrix[1][0]*BodyOutput1[i][0] + YawMatrix[1][1]*BodyOutput1[i][1] + YawMatrix[1][2]*BodyOutput1[i][2];
    BodyOutput2[i][2] = YawMatrix[2][0]*BodyOutput1[i][0] + YawMatrix[2][1]*BodyOutput1[i][1] + YawMatrix[2][2]*BodyOutput1[i][2];
  }
  //Second pass does pitch (ZY)
  /*for(int i = 0; i < 4; i++){
    BodyOutput2[i][0] = BodyOutput1[i][0];
    BodyOutput2[i][1] = (BodyOutput1[i][2]*cos(pitch)-BodyOutput1[i][1]*sin(pitch));
    BodyOutput2[i][2] = (BodyOutput1[i][2]*cos(pitch)-BodyOutput1[i][1]*sin(pitch));
  }
  
  //Third pass does roll (ZX)
  for(int i = 0; i < 4; i++){
    BodyOutput1[i][0] = (BodyOutput2[i][0]*cos(roll)-BodyOutput2[i][2]*sin(roll));
    BodyOutput1[i][1] = BodyOutput2[i][1];
    BodyOutput1[i][2] = (BodyOutput2[i][0]*cos(roll)-BodyOutput2[i][2]*sin(roll));
  }

  //Fourth pass does yaw (XY)
  for(int i = 0; i < 4; i++){
    BodyOutput2[i][0] = (BodyOutput1[i][0]*cos(yaw)-BodyOutput1[i][1]*sin(yaw));
    BodyOutput2[i][1] = (BodyOutput1[i][0]*cos(yaw)-BodyOutput1[i][1]*sin(yaw));
    BodyOutput2[i][2] = BodyOutput1[i][2];
  }*/


  //Update GaitGenOut
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      BodyModOut[i][j] = BodyOutput2[i][j];
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
