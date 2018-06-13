#ifndef __AVR__

#include <ServoSim.h>
#include <iostream>

void ServoSim::write(int pos){
  curpos=pos;
}

int ServoSim::read(){
  return curpos;
}

#endif
