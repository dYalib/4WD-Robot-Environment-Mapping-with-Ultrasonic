#ifdef __AVR__
  #include <Arduino.h>
#endif

#include <Flitzi.h>

#ifndef __AVR__
  #include <stdlib.h>
  #include <unistd.h>
  typedef unsigned char byte;
  typedef bool boolean;
#endif


Flitzi robi;
  byte i = 0;
  int d = 0;

#ifndef __AVR__
  void delay(int ms) {
    usleep(ms);
  }
#endif

void setup() {
  #ifdef __AVR__
    robi.init();
    robi.moveServo(0);
    delay(3000);
  #endif

  #ifndef __AVR__
    robi.visualiseArray();
  #endif
  //robi.generateSimulationData();
  //robi.generateSimulationData();
  //robi.generateSimulationData();
}

void loop() {
  i = robi.nextServoPos(5);
  robi.moveServo(i);
  d = robi.getDistance();
  #ifdef __AVR__
    robi.showAtDisplay(String(d));
  #endif

  #ifndef __AVR__
    std::cout << "Dist: " << d << "\n";
  #endif

}

#ifndef __AVR__
int main() {
  setup();
/*
  while(1==1) {
    loop();
  }
  */
return 0;
}
#endif
