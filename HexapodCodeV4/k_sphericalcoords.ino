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
    float spherical_azimuth = leg_local_spherical[leg][0];
    float spherical_elevation = leg_local_spherical[leg][1];
    float angle_coxa = leg_dynamixels[leg][0];
    float angle_femur = leg_dynamixels[leg][1];
    float angle_tibia = leg_dynamixels[leg][2];
    //float IniVect[3] = {cos(coxa)*sin(femur+tibia),sin(coxa)*cos(femur+tibia),cos(femur+tibia)};//vector of the leg(x,y,z)
    spherical_azimuth -=angle_coxa;//The first rotation is faster this way, no trig compared to a cartesian rotation
    float des_vect1[3] = {
      10*cosf(spherical_azimuth)*sinf(spherical_elevation),10*sinf(spherical_azimuth)*sinf(spherical_elevation),10*cosf(spherical_elevation)    };//output vector(x,y,z)
    float des_vect2[3];
    //desired vector


    float pitch = (HALF_PI + angle_femur + angle_tibia);
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
    USER_SERIAL.println(des_vect1[0]);
    USER_SERIAL.print("Angle y ");
    USER_SERIAL.println(des_vect1[1]);
    USER_SERIAL.print("Angle z ");
    USER_SERIAL.println(des_vect1[2]);
#endif


    //Preform the second rotation
    des_vect2[0] = XZCorr[0][0]*des_vect1[0] + XZCorr[0][1]*des_vect1[1] + XZCorr[0][2]*des_vect1[2];
    des_vect2[1] = XZCorr[1][0]*des_vect1[0] + XZCorr[1][1]*des_vect1[1] + XZCorr[1][2]*des_vect1[2];
    des_vect2[2] = XZCorr[2][0]*des_vect1[0] + XZCorr[2][1]*des_vect1[1] + XZCorr[2][2]*des_vect1[2];

    leg_dynamixels[leg][3] = atan2f(des_vect2[1],des_vect2[0]);//Tibia Rotation in Degrees
    leg_dynamixels[leg][4] = -(acosf(des_vect2[2]/10));//Tarsus
    //leg_dynamixels[leg][5] = leg_local_spherical[leg][2];//wheel, Not currently implemented in hardware.
  }
}


void LegPlaceSixCalculate (void){//place leg in six degrees of freedom based on leg_coords_out and leg_local_spherical leg -1 places all legs
  //Transform the coordinates for each leg
  for(int leg = 0; leg < NUM_LEGS; leg++){
    int place_x = leg_coords_out[leg][0];
    int place_y = leg_coords_out[leg][1];
    int place_z = leg_coords_out[leg][2];
    float foot_azimuth = leg_local_spherical[leg][0];
    float foot_elevation = leg_local_spherical[leg][1];
    float Bank = leg_local_spherical[leg][2];

    //Shift X,Y, and Z coordinates to compinsate for the tarsus
    float radius = TARSUS_LENGTH*cosf(foot_elevation);
    place_x -= (radius*cosf(foot_azimuth));
    place_y -=(radius*sinf(foot_azimuth));
    place_z -=(TARSUS_LENGTH*sinf(foot_elevation));

    leg_coords_out[leg][0] = place_x;
    leg_coords_out[leg][1] = place_y;
    leg_coords_out[leg][2] = place_z;
  }

  //calculate the angles of the first 3 servos
  LegCalculate();
  //calculate the angles of the servos for spherical coordinates after calulating first 3 servos
  LocalSphericalCalculate();
}





