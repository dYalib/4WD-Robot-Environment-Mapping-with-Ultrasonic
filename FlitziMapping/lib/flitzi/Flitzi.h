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

#define PI 3.14159265
#define MAPSIZE 20
#define RESOLUTION 2
#define MEASURINGANGLE 30
#define ROBOTLENGHT 22
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
  int getDistance();
  //int scanEnviroment(byte ServoPos);
  byte nextServoPos(byte step);
  void enviromentMapping();



private:
  #ifndef __AVR__
    void delay(int ms);
    ServoSim servo;

    struct rgb {
      byte red;
      byte green;
      byte blue;
    };
    rgb getColor(int value);
  #endif

  #ifdef __AVR__
    Servo servo;
  #endif

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



  const byte  PIN_ENA=0;
  const byte  PIN_ENB=1;
  const byte  PIN_IN1=3; //HIGH -> forward
  const byte  PIN_IN2=5; //PWM
  const byte  PIN_IN3=9; //LOW -> Back
  const byte  PIN_IN4=6; //PWM
  const byte  servoForward= 87;
  byte curDist;
  //"normal" scan is antiClockwise,(0° to 180°). Reverse scan is the opposite
  boolean scanReverse = false;
  envPoint envMap [MAPSIZE][MAPSIZE];
  pose curPose;
};
