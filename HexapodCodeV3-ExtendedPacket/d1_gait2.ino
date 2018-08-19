//This code covers the crawl Gait Generator
//WIP

int GaitGen1Path(int InputArray[][3],int LegNumber, int Cycle, int Period,int Xtrans, int Ytrans,int Ztrans, int NumLegsPeriod = 4){// Leg Offset should be from 0 to NumLegsPeriod-1
  float Generator0 = (Cycle-((LegNumber/NumLegsPeriod)*Period))%Period;
  float Modifiers[3] = {0,0,0};
  
  if (Generator0<(Period/NumLegsPeriod)){//lifted leg segment
    float Generator1 = ((2.0*(float)NumLegsPeriod/Period)*(Generator0-(Period/(2.0*(float)NumLegsPeriod))));//Periodic function that goes from -1 to 1 in 1/NumLegs of period
    Modifiers[0] = Xtrans*Generator1;
    Modifiers[1] = Ytrans*Generator1;
    Modifiers[2] = Ztrans;
  }
  else{//Ground Leg Segment
    float Generator1 = ((2.0*(float)NumLegsPeriod/Period)*((-Generator0/(NumLegsPeriod-1))+(Period/(2.0*((float)NumLegsPeriod-1)))));//Periodic function that goes from 1 to -1 in NumLegs-1/NumLegs of period
    Modifiers[0] = Xtrans*Generator1;
    Modifiers[1] = Ytrans*Generator1;
    Modifiers[2] = -Ztrans;
  }
  
  InputArray[LegNumber][0] += Modifiers[0];//X
  InputArray[LegNumber][1] += Modifiers[1];//Y
  InputArray[LegNumber][2] += Modifiers[2];//Z
}

void GaitGen2(int Period, int Cycle, int Xtrans, int Ytrans, int Ztrans, float ZTurn){
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
      (Leg0InitX),//X (Output[0][0])
      (Leg0InitY),//Y (Output[0][1])
      (Leg0InitZ) //Z (Output[0][2])

      }
      ,
    { //Leg 1
      (Leg1InitX),//X
      (Leg1InitY),//Y
      (Leg1InitZ) //Z
      }
      ,
    { //Leg 2
      (Leg2InitX),//X
      (Leg2InitY),//Y
      (Leg2InitZ) //Z
      }
      ,
    { //Leg 3
      (Leg3InitX),//X
      (Leg3InitY),//Y
      (Leg3InitZ) //Z
      }
      ,
    };
    


    //Second pass does gait rotate in place
    int Output2[NUM_LEGS][3];
  Output2[0][0] = (Output1[0][0]*cos(Theta1)-Output1[0][1]*sin(Theta1));
  Output2[0][1] = (Output1[0][0]*sin(Theta1)+Output1[0][1]*cos(Theta1));
  Output2[0][2] = Output1[0][2];

  Output2[1][0] = (Output1[1][0]*cos(-Theta1)-Output1[1][1]*sin(-Theta1));
  Output2[1][1] = (Output1[1][0]*sin(-Theta1)+Output1[1][1]*cos(-Theta1));
  Output2[1][2] = Output1[1][2];

  Output2[2][0] = (Output1[2][0]*cos(Theta1)-Output1[2][1]*sin(Theta1));
  Output2[2][1] = (Output1[2][0]*sin(Theta1)+Output1[2][1]*cos(Theta1));
  Output2[2][2] = Output1[2][2];

  Output2[3][0] = (Output1[3][0]*cos(-Theta1)-Output1[3][1]*sin(-Theta1));
  Output2[3][1] = (Output1[3][0]*sin(-Theta1)+Output1[3][1]*cos(-Theta1));
  Output2[3][2] = Output1[3][2];

  //Update GaitGenOut
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      GaitGenOut[i][j] = Output2[i][j];
    }
  }
}
