#ifdef __AVR__
  #include <Arduino.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
  #include <Ultrasonic.h>
  #include <Servo.h>
  #include <SPI.h>
#endif

#ifndef __AVR__
    #include <stdlib.h>
    #include <iostream>
    #include <unistd.h>
    #include <ServoSim.h>
    #include <UltrasonicSim.h>
#endif

#define MAPSIZE 20


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
  #endif

  #ifdef __AVR__
    Servo servo;
  #endif

  struct envPoint {
    unsigned char nib1 : 4;
    unsigned char nib2 : 4;
    unsigned char nib3 : 4;
    unsigned char nib4 : 4;
  };

  const byte  PIN_ENA=0;
  const byte  PIN_ENB=1;
  const byte  PIN_IN1=3; //HIGH -> forward
  const byte  PIN_IN2=5; //PWM
  const byte  PIN_IN3=9; //LOW -> Back
  const byte  PIN_IN4=6; //PWM
  const byte  servoForward= 87;
  int curDist;
  //"normal" scan is antiClockwise,(0° to 180°). Reverse scan is the opposite
  boolean scanReverse = false;
  envPoint envMap [MAPSIZE][MAPSIZE];
};
