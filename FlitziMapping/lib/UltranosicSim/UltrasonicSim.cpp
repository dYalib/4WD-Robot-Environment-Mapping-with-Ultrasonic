#ifndef __AVR__
#include <UltrasonicSim.h>

int UltrasonicSim::distanceRead(int ServoPos){
  try{
    return tstVecor1[ServoPos / 5];
  } catch(int ex) {
    return 9999;
  }
}
#endif
