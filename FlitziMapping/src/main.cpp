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
  byte d = 0;

#ifndef __AVR__
  void delay(int ms) {
    usleep(ms);
  }
#endif

void setup() {
  #ifdef __AVR__

    robi.init();
    robi.waitForButtonPress();
  #endif

  #ifndef __AVR__
    robi.enviromentMapping();
    robi.visualiseArray();
    robi.move(10);
    robi.setFieldOfRobot();
    robi.enviromentMapping();
    robi.visualiseArray();
  #endif
  //robi.generateSimulationData();
  //robi.generateSimulationData();
  //robi.generateSimulationData();
}

void loop() {
#ifdef __AVR__
  robi.moveServo(0);
  robi.enviromentMapping();
  robi.paintOnDisplay();
  robi.visualiseArray();
  robi.waitForButtonPress();
  delay(1000);
  robi.move(10);
#endif
}

#ifndef __AVR__
int main() {
  setup();
return 0;
}
#endif
