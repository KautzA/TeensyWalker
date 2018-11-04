//This code covers the crawl Gait Generator
//WIP
const uint8_t GaitLegOffset[NUM_LEGS] = {
  0,2,1,3};
/*
    Order (number)
 0(0) 2(1)
 \\ //
 5(5)-- --4(6)
 // \\
 3(3) 1(2)
 */

int GaitGen2Cycle(int InputArray[NUM_LEGS][3],int LegNumber, int Period, int Cycle,int Xtrans, int Ytrans,int Ztrans, float ZTurn, int NumLegsPeriod = NUM_LEGS){//translation for gait2
  float Generator0 = (Cycle-((GaitLegOffset[LegNumber]*Period)/NumLegsPeriod))%Period; //generates cyclic values offeset for leg number.
  float Modifiers[4] = {
    0,0,0,0  };

  if (Generator0<(Period/NumLegsPeriod)){//lifted leg segment
    float Generator1 = ((2.0*(float)NumLegsPeriod/Period)*(Generator0-(Period/(2.0*(float)NumLegsPeriod))));//Periodic function that goes from -1 to 1 in 1/NumLegs of period
    if(!(LegNumber%2)){//Invert legs 1 and 3
      Generator1 = -Generator1;
    }
    Modifiers[0] = Xtrans*Generator1;
    Modifiers[1] = Ytrans*Generator1;
    Modifiers[2] = Ztrans;
    Modifiers[3] = ((float)ZTurn * 3.14 / 4.0 *Generator1);
  }
  else{//Ground Leg Segment
    float Generator1 = ((2.0*(float)NumLegsPeriod/Period)*((-Generator0/(NumLegsPeriod-1))+(Period/(2.0*((float)NumLegsPeriod-1)))));//Periodic function that goes from 1 to -1 in NumLegs-1/NumLegs of period
    if(!(LegNumber%2)){//Invert legs 1 and 3
      Generator1 = -Generator1;
    }
    Modifiers[0] = Xtrans*Generator1;
    Modifiers[1] = Ytrans*Generator1;
    Modifiers[2] = -Ztrans;
    Modifiers[3] = (ZTurn * 3.14 / 4 *Generator1);
  }

  int Output1[3];//Translate

  Output1[0] = InputArray[LegNumber][0] + Modifiers[0];//X
  Output1[1] = InputArray[LegNumber][1] + Modifiers[1];//Y
  Output1[2] = InputArray[LegNumber][2] + Modifiers[2];//Z

  int Output2[3];//Rotate

  Output2[0] = (Output1[0]*cosf(Modifiers[3])-Output1[1]*sinf(Modifiers[3]));
  Output2[1] = (Output1[0]*sinf(Modifiers[3])+Output1[1]*cosf(Modifiers[3]));
  Output2[2] = Output1[2];

  //Update input

  InputArray[LegNumber][0] = Output2[0];
  InputArray[LegNumber][1] = Output2[1];
  InputArray[LegNumber][2] = Output2[2];
}

void GaitGen2(int Period, int Cycle, int Xtrans, int Ytrans, int Ztrans, float ZTurn){
  //Sale the inputs
  Xtrans =map(Xtrans,-127,127,-40,40);
  Ytrans =map(Ytrans,-127,127,-40,40);
  Ztrans =map(Ztrans,-127,127,0,20);
  ZTurn  =-map(ZTurn,-127,127,-50,50)/100;

  //First pass, sets base position
  int Output0[NUM_LEGS][3];
  for (int i = 0; i<NUM_LEGS; i++){
    for (int j = 0; j<3; j++){
      Output0[i][j] = InitialPositions[i][j];
    }
  }

  for(int i = 0; i<NUM_LEGS; i++){
    GaitGen2Cycle(Output0,i,Period,Cycle,Xtrans,Ytrans,Ztrans,ZTurn);
  }




  //Update GaitGenOut
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      GaitGenOut[i][j] = Output0[i][j];
    }
  }
}

