/*
	GaitGen.h - Library for calculating diagonal pair gait for quardupod
	Arthur Kautz	
*/


#include "Arduino.h"

#include "Gait_Gen.h"


// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

GaitGen::GaitGen(void)
{
  value = 0;
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void GaitGen::Gait1PeriodicTrot(int FootCoords[][3], int Period, int Cycle, int Xtrans, int Ytrans, int Ztrans, float ZTurn)
{
 
}

void GaitGen::Gait1PeriodicCrawl(int FootCoords[][3], int Period, int Cycle, int Xtrans, int Ytrans, int Ztrans, float ZTurn)
{
  
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void Test::doSomethingSecret(void)
{

}

