/*
	GaitGen.h - Library for calculating diagonal pair gait for quardupod
	Arthur Kautz	
*/

#ifndef GAIT_GEN_H
#define GAIT_GEN_H

#include "Arduino.h"
#include "math.h"

class GaitGen {
	// user-accessible "public" interface
  public:
    GaitGen(void);
    void Gait1PeriodicTrot(int FootCoords[][3], int Period, int Cycle, int Xtrans, int Ytrans, int Ztrans, float ZTurn);
    void Gait1PeriodicCrawl(int FootCoords[][3], int Period, int Cycle, int Xtrans, int Ytrans, int Ztrans, float ZTurn);

  // library-accessible "private" interface
  private:
    int value;
    void doSomethingSecret(void);
};


#endif
