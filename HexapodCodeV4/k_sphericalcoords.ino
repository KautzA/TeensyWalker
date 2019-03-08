//Sphericalcoords
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

void LocalSpherical(void){//convert from global spherical angles to local spherical angles
  //Leg0, no change
  leg_local_spherical[0][0] = leg_global_spherical[0][0];//Azimuth
  leg_local_spherical[0][1] = leg_global_spherical[0][1];//Elevation
  leg_local_spherical[0][2] = leg_global_spherical[0][2];//Bank
  //Leg1, no change
  leg_local_spherical[0][0] = leg_global_spherical[0][0];//Azimuth
  leg_local_spherical[0][1] = leg_global_spherical[0][1];//Elevation
  leg_local_spherical[0][2] = leg_global_spherical[0][2];//Bank
  //Leg2, Azimuth +180 degrees
  leg_local_spherical[0][0] = leg_global_spherical[0][0] + PI;//Azimuth
  leg_local_spherical[0][1] = leg_global_spherical[0][1];//Elevation
  leg_local_spherical[0][2] = leg_global_spherical[0][2];//Bank
  //Leg3, Azimuth +180 degrees
  leg_local_spherical[0][0] = leg_global_spherical[0][0] + PI;//Azimuth
  leg_local_spherical[0][1] = leg_global_spherical[0][1];//Elevation
  leg_local_spherical[0][2] = leg_global_spherical[0][2];//Bank
}




void LocalSphericalCalculate(void){//Calculates new TibaRot and Tarsus based on local azimuth and elevation from leg_local_spherical
  //Call after calulating and updating leg_dynamixels
  for(int leg = 0; leg < NUM_LEGS; leg++){
    float Azimuth = leg_local_spherical[leg][0];
    float Elevation = leg_local_spherical[leg][1];
    float Coxa = leg_dynamixels[leg][0];
    float Femur = leg_dynamixels[leg][1];
    float Tibia = leg_dynamixels[leg][2];
    //float IniVect[3] = {cos(coxa)*sin(femur+tibia),sin(coxa)*cos(femur+tibia),cos(femur+tibia)};//vector of the leg(x,y,z)
    Azimuth -=Coxa;//The first rotation is faster this way, no trig compared to a cartesian rotation
    float DesVect1[3] = {
      10*cosf(Azimuth)*sinf(Elevation),10*sinf(Azimuth)*sinf(Elevation),10*cosf(Elevation)    };//output vector(x,y,z)
    float DesVect2[3];
    //desired vector


    float pitch = (HALF_PI + Femur + Tibia);
    USER_SERIAL.println(pitch);


    float XZCorr[3][3] = {//Matrix to rotate point to be in proper loaction for sphericalcoords
      {
        cosf(pitch),0,sinf(pitch)      }
      ,
      {
        0,1,0      }
      ,
      {
        -sinf(pitch),0,cosf(pitch)      }
    };


#if defined(UserSerialTransmit)
    USER_SERIAL.print("Angle x ");
    USER_SERIAL.println(DesVect1[0]);
    USER_SERIAL.print("Angle y ");
    USER_SERIAL.println(DesVect1[1]);
    USER_SERIAL.print("Angle z ");
    USER_SERIAL.println(DesVect1[2]);
#endif


    //Preform the second rotation
    DesVect2[0] = XZCorr[0][0]*DesVect1[0] + XZCorr[0][1]*DesVect1[1] + XZCorr[0][2]*DesVect1[2];
    DesVect2[1] = XZCorr[1][0]*DesVect1[0] + XZCorr[1][1]*DesVect1[1] + XZCorr[1][2]*DesVect1[2];
    DesVect2[2] = XZCorr[2][0]*DesVect1[0] + XZCorr[2][1]*DesVect1[1] + XZCorr[2][2]*DesVect1[2];

    leg_dynamixels[leg][3] = atan2f(DesVect2[1],DesVect2[0]);//Tibia Rotation in Degrees
    leg_dynamixels[leg][4] = -(acosf(DesVect2[2]/10));//Tarsus
    //leg_dynamixels[leg][5] = leg_local_spherical[leg][2];//wheel, Not currently implemented in hardware.
  }
}


void LegPlaceSixCalculate (void){//place leg in six degrees of freedom based on LegCoordsOut and leg_local_spherical leg -1 places all legs
  //Transform the coordinates for each leg
  for(int leg = 0; leg < NUM_LEGS; leg++){
    int Xplace = LegCoordsOut[leg][0];
    int Yplace = LegCoordsOut[leg][1];
    int Zplace = LegCoordsOut[leg][2];
    float Azimuth = leg_local_spherical[leg][0];
    float Elevation = leg_local_spherical[leg][1];
    float Bank = leg_local_spherical[leg][2];

    //Shift X,Y, and Z coordinates to compinsate for the tarsus
    float radius = TARSUS_LENGTH*cosf(Elevation);
    Xplace -= (radius*cosf(Azimuth));
    Yplace -=(radius*sinf(Azimuth));
    Zplace -=(TARSUS_LENGTH*sinf(Elevation));

    LegCoordsOut[leg][0] = Xplace;
    LegCoordsOut[leg][1] = Yplace;
    LegCoordsOut[leg][2] = Zplace;
  }

  //calculate the angles of the first 3 servos
  LegCalculate();
  //calculate the angles of the servos for spherical coordinates after calulating first 3 servos
  LocalSphericalCalculate();
}





