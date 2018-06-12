#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Ultrasonic.h>
#include <Servo.h>
#include <SPI.h>



class Flitzi {

public:
  Flitzi();
  void init();

  void move (int powerLeft, int powerRight);
  void stop();
  void turn (int degree);
  void moveServo( byte servoPos);
  void showAtDisplay(String txt);
  int getDistance();
  int scanEnviroment(byte ServoPos);
  byte nextServoPos(byte step);
  void generateSimulationData();

  void enviromentMapping();


private:
  const byte  PIN_ENA=0;
  const byte  PIN_ENB=1;
  const byte  PIN_IN1=3; //HIGH -> forward
  const byte  PIN_IN2=5; //PWM
  const byte  PIN_IN3=9; //LOW -> Back
  const byte  PIN_IN4=6; //PWM
  const byte  servoForward= 87;
  Servo servo;
  int curDist;
  //"normal" scan is clockwise,(180° to 0°). Reverse scan is the opposite
  boolean scanReverse = false;
  //boolean mapping[30][100];

//Test Data
#ifndef __AVR__
  const int tstVecor1[38] ={20, 20, 21, 22, 21, 20, 21, 22, 22, 11, 23, 24, 152, 24, 98, 53, 122, 999, 16, 57, 60, 58, 58, 58, 57, 64, 68, 69, 82, 83, 87, 73, 76, 94, 66, 81, 71};
  const int tstVecor2[38] ={21, 20, 20, 21, 22, 21, 20, 20, 21, 22, 22, 35, 24, 107, 122, 2, 131, 5, 125, 45, 47, 59, 58, 58, 57, 57, 58, 69, 63, 68, 69, 74, 76, 68, 67, 66, 66, 66};
  const int tstVecor3[38] = {20, 20, 20, 21, 22, 21, 20, 21, 21, 22, 22, 22, 23, 87, 36, 60, 18, 14, 31, 41, 115, 11, 59, 69, 57, 57, 60, 61, 70, 68, 67, 102, 79, 68, 77, 53, 67, 95};
#endif

};
