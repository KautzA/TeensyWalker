//gait2
//This code covers the crawl Gait Generator
//WIP
const uint8_t kGaitLegOffset[NUM_LEGS] = {0,2,1,3};
/*
    Order (number)
   0(0) 2(1)
     \\ //
 5(4)-- --4(5)
     // \\
   3(3) 1(2)
 */

int GaitGen2Cycle(int input_array[NUM_LEGS][3],int leg_number, int period, int cycle,int translate_x, int translate_y,int translate_z, float turn_z){//translation for gait2
  int num_legs_period = NUM_LEGS;
  float generator0 = (cycle-((kGaitLegOffset[leg_number]*period)/num_legs_period))%period; //generates cyclic values offeset for leg number.
  float modifiers[4] = {
    0,0,0,0  };

  if (generator0<(period/num_legs_period)){//lifted leg segment
    float generator1 = ((2.0*(float)num_legs_period/period)*(generator0-(period/(2.0*(float)num_legs_period))));//periodic function that goes from -1 to 1 in 1/NumLegs of period
    if(!(leg_number%2)){//Invert legs 1 and 3
      generator1 = -generator1;
    }
    modifiers[0] = translate_x*generator1;
    modifiers[1] = translate_y*generator1;
    modifiers[2] = translate_z;
    modifiers[3] = -((float)turn_z * PI / 4.0 *generator1);
  }
  else{//Ground Leg Segment
    float generator1 = ((2.0*(float)num_legs_period/period)*((-generator0/(num_legs_period-1))+(period/(2.0*((float)num_legs_period-1)))));//periodic function that goes from 1 to -1 in NumLegs-1/NumLegs of period
    if(!(leg_number%2)){//Invert legs 1 and 3
      generator1 = -generator1;
    }
    modifiers[0] = translate_x*generator1;
    modifiers[1] = translate_y*generator1;
    modifiers[2] = -translate_z;
    modifiers[3] = -(turn_z * PI / 4 *generator1);
  }

  int Output1[3];//Translate

  Output1[0] = input_array[leg_number][0] + modifiers[0];//X
  Output1[1] = input_array[leg_number][1] + modifiers[1];//Y
  Output1[2] = input_array[leg_number][2] + modifiers[2];//Z

  int Output2[3];//Rotate

  Output2[0] = (Output1[0]*cosf(modifiers[3])-Output1[1]*sinf(modifiers[3]));
  Output2[1] = (Output1[0]*sinf(modifiers[3])+Output1[1]*cosf(modifiers[3]));
  Output2[2] = Output1[2];

  //Update input

  input_array[leg_number][0] = Output2[0];
  input_array[leg_number][1] = Output2[1];
  input_array[leg_number][2] = Output2[2];
}

void GaitGen2(int period, int cycle, int translate_x, int translate_y, int translate_z, float turn_z){
  //Sale the inputs
  translate_x =map(translate_x,-127,127,-40,40);
  translate_y =map(translate_y,-127,127,-40,40);
  translate_z =map(translate_z,-127,127,0,20);
  turn_z  =-map(turn_z,-127,127,-50,50)/100;

  //First pass, sets base position
  int output0[NUM_LEGS][3];
  for (int i = 0; i<NUM_LEGS; i++){
    for (int j = 0; j<3; j++){
      output0[i][j] = kInitialPositions[i][j];
    }
  }

  for(int i = 0; i<NUM_LEGS; i++){
    GaitGen2Cycle(output0,i,period,cycle,translate_x,translate_y,translate_z,turn_z);
  }




  //Update GaitGenOut
  for(int i = 0; i < NUM_LEGS; i++){
    for(int j = 0; j < 3; j++){
      gait_gen_out[i][j] = output0[i][j];
    }
  }
}

