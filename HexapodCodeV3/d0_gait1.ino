//gait1
//This code covers the basic Gait Generator
void GaitGen1(int Period, int Cycle, int Xtrans, int Ytrans, int Ztrans, float ZTurn){
    //Sale the inputs
    Xtrans =map(Xtrans,-127,127,-40,40);
    Ytrans =map(Ytrans,-127,127,-40,40);
    Ztrans =map(Ztrans,-127,127,0,20);
    ZTurn  =-map(ZTurn,-127,127,-50,50)/100;

  float Generator1 = ((4.0 *(abs((Cycle%Period)-(Period/2))-(Period/4)))/Period);
  float Generator2 = Generator1;
  int GeneratorZ = (-1*((abs((Cycle%Period)-(Period/2)))/((Cycle%Period)-(Period/2))));
  float Theta1 = (ZTurn * 3.14 / 4 *Generator1);
  
  //Type1 and Type two are added to legs to produce movement
  float Type1[3] = {
    (Xtrans*Generator1),(Ytrans*Generator1),(Ztrans*GeneratorZ)  };
  float Type2[3] = {
    (Xtrans*Generator2),(Ytrans*Generator2),(-Ztrans*GeneratorZ)  };


  //First pass does gait generation for translation
  int Output1[NUM_LEGS][3] = {
    { //Leg 0
      (InitialPositions[0][0] + Type1[0]),//X (Output[0][0])
      (InitialPositions[0][1] + Type1[1]),//Y (Output[0][1])
      (InitialPositions[0][2] + Type1[2]) //Z (Output[0][2])

      }
      ,
    { //Leg 1
      (InitialPositions[1][0] + Type2[0]),//X
      (InitialPositions[1][1] + Type2[1]),//Y
      (InitialPositions[1][2] + Type2[2]) //Z
      }
      ,
    { //Leg 2
      (InitialPositions[2][0] + Type1[0]),//X
      (InitialPositions[2][1] + Type1[1]),//Y
      (InitialPositions[2][2] + Type1[2]) //Z
      }
      ,
    { //Leg 3
      (InitialPositions[3][0] + Type2[0]),//X
      (InitialPositions[3][1] + Type2[1]),//Y
      (InitialPositions[3][2] + Type2[2]) //Z
      }
      ,
    };


    //Second pass does gait rotate in place
    int Output2[NUM_LEGS][3];
  Output2[0][0] = (Output1[0][0]*cosf(Theta1)-Output1[0][1]*sinf(Theta1));
  Output2[0][1] = (Output1[0][0]*sinf(Theta1)+Output1[0][1]*cosf(Theta1));
  Output2[0][2] = Output1[0][2];

  Output2[1][0] = (Output1[1][0]*cosf(-Theta1)-Output1[1][1]*sinf(-Theta1));
  Output2[1][1] = (Output1[1][0]*sinf(-Theta1)+Output1[1][1]*cosf(-Theta1));
  Output2[1][2] = Output1[1][2];

  Output2[2][0] = (Output1[2][0]*cosf(Theta1)-Output1[2][1]*sinf(Theta1));
  Output2[2][1] = (Output1[2][0]*sinf(Theta1)+Output1[2][1]*cosf(Theta1));
  Output2[2][2] = Output1[2][2];

  Output2[3][0] = (Output1[3][0]*cosf(-Theta1)-Output1[3][1]*sinf(-Theta1));
  Output2[3][1] = (Output1[3][0]*sinf(-Theta1)+Output1[3][1]*cosf(-Theta1));
  Output2[3][2] = Output1[3][2];

  //Update GaitGenOut
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      GaitGenOut[i][j] = Output2[i][j];
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
