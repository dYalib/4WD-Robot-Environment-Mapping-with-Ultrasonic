#ifdef __AVR__
  #include <Arduino.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <Ultrasonic.h>
  #include <Servo.h>
  #include <SPI.h>
#endif

#include <math.h>

#ifndef __AVR__
    #include <stdlib.h>
    #include <iostream>
    #include <unistd.h>
    #include <ServoSim.h>
    #include <UltrasonicSim.h>
#endif

//#define PI 3.14159265
#define MAPSIZE 20
#define RESOLUTION 2
#define MEASURINGANGLE 30
#define ROBOTLENGHT 20
#define ROBOTBREADTH 10



class Flitzi {

  #ifndef __AVR__
    typedef unsigned char byte;
    typedef bool boolean;
  #endif

public:
  Flitzi();
    void visualiseArray();
  #ifdef __AVR__
    void init();
    void showAtDisplay(String txt);
    void generateSimulationData();
  #endif


  void move (int powerLeft, int powerRight);
  void stop();
  void turn (int degree);
  void moveServo( byte servoPos);
  byte getDistance();
  //int scanEnviroment(byte ServoPos);
  byte nextServoPos(byte step);
  void enviromentMapping();



private:
  #ifndef __AVR__
    void delay(unsigned int ms);
    ServoSim servo;
  #endif

  #ifdef __AVR__
    Servo servo;
  #endif

  struct rgb {
    byte red;
    byte green;
    byte blue;
  };
  rgb getColor(char value);

  void setEnvMapVal(div_t x, div_t y, byte val);
  byte getEnvMapVal(div_t x, div_t y);
  void updateFieldProbably(unsigned char sensorAngle, byte dist,char alternationVal);
  void setFieldOfRobot();

  struct envPoint {
    char nib_00 : 4;
    char nib_10 : 4;
    char nib_01 : 4;
    char nib_11 : 4;
  };

  struct pose {
    byte x;
    byte y;
  };



  static const byte PIN_ENA PROGMEM=0;
  static const byte PIN_ENB PROGMEM =1;
  static const byte PIN_IN1 PROGMEM =3; //HIGH -> forward
  static const byte PIN_IN2 PROGMEM =5; //PWM
  static const byte PIN_IN3 PROGMEM =9; //LOW -> Back
  static const byte PIN_IN4 PROGMEM =6; //PWM
  static const byte servoForward PROGMEM = 87;
  static const byte numbertwo PROGMEM = 2;
  byte curDist;
  //"normal" scan is antiClockwise,(0° to 180°). Reverse scan is the opposite
  boolean scanReverse = false;
  envPoint envMap [MAPSIZE][MAPSIZE];
  pose curPose;
  String str="";
};
