#include <Arduino.h>
#include <Flitzi.h>

Flitzi robi;
  int i = 0;

void setup() {

  robi.init();
  delay(3000);
  robi.generateSimulationData();
  robi.generateSimulationData();
  robi.generateSimulationData();
/*
  robi.moveServo(0);
  delay(1000);
/*
  robi.moveServo(175);
  delay(1000);
  robi.moveServo(180);
  delay(1000);
  robi.moveServo(185);
  delay(1000);
  robi.moveServo(190);
  delay(1000);
  robi.moveServo(200);
  delay(1000);
  */
}

void loop() {
  /*
  i = robi.nextServoPos(5);
  robi.moveServo(i);
  robi.getDistance();
  robi.showAtDisplay(String(robi.getDistance()));
*/
}
