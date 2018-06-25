#ifndef __AVR__
#include <UltrasonicSim.h>
#include <stdlib.h>
#include <time.h>

int UltrasonicSim::distanceRead(int ServoPos,int offset){
srand (time(NULL));
  try{
    switch (rand() % 5 + offset){
      case 0: return tstVecor1[ServoPos / 5];
      case 1: return tstVecor2[ServoPos / 5];
      case 2: return tstVecor3[ServoPos / 5];
      case 3: return tstVecor4[ServoPos / 5];
      case 4: return tstVecor5[ServoPos / 5];
      case 5: return tstVecor6[ServoPos / 5];
      case 6: return tstVecor7[ServoPos / 5];
      case 7: return tstVecor8[ServoPos / 5];
      case 8: return tstVecor9[ServoPos / 5];
      case 9: return tstVecor10[ServoPos / 5];
      default: return 255;
    }
  } catch(int ex) {
    return 255;
  }
}
#endif
