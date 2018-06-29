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
    #define PI 3.14159265
    #include <string>
#endif


#define MAPSIZE 20
#define RESOLUTION 2
#define MEASURINGANGLE 16
#define ROBOTLENGHT 20
#define ROBOTBREADTH 10
#define ROBOT_US_GAP 2



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
    void paintOnDisplay();
    void enableLED();
    void disableLED();
    void waitForButtonPress();
  #endif


  void move (int powerLeft, int powerRight);
  void move (int cm);
  void stop();
  void turn (int degree);
  void moveServo( byte servoPos);
  byte getDistance();
  byte nextServoPos(byte step);
  void enviromentMapping();
  void setFieldOfRobot();


private:
  #ifdef __AVR__
    Servo servo;
  #endif

  #ifndef __AVR__
    void delay(unsigned int ms);
    ServoSim servo;
    bool haseMoved=false;
  #endif

  struct rgb {
    byte red;
    byte green;
    byte blue;
  };

  struct arrayPos {
    byte x;
    byte y;
    byte nib;

    bool operator!=(arrayPos& other) {
      return x != other.x || y != other.y || nib != other.nib;
    }
  };

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

  byte curDist;
  //"normal" scan is antiClockwise,(0° to 180°). Reverse scan is the opposite
  boolean scanReverse = false;
  envPoint envMap [MAPSIZE][MAPSIZE];
  //envPoint envMap [1][1];
  pose curPose;

  #ifdef __AVR__
    static const byte PIN_ENA PROGMEM=0;
    static const byte PIN_ENB PROGMEM =1;
    static const byte PIN_IN1 PROGMEM =3; //HIGH -> forward
    static const byte PIN_IN2 PROGMEM =5; //PWM
    static const byte PIN_IN3 PROGMEM =9; //LOW -> Back
    static const byte PIN_IN4 PROGMEM =6; //PWM
    static const byte PIN_SWITCH PROGMEM = 11;
    static const byte PIN_LED PROGMEM =12;
    static const byte servoForward PROGMEM = 87;
    static const byte numbertwo PROGMEM = 2;
  #endif

  #ifndef __AVR__
    static const byte PIN_ENA=0;
    static const byte PIN_ENB=1;
    static const byte PIN_IN1=3; //HIGH -> forward
    static const byte PIN_IN2=5; //PWM
    static const byte PIN_IN3=9; //LOW -> Back
    static const byte PIN_IN4=6; //PWM
    static const byte servoForward= 87;
    static const byte numbertwo= 2;
  #endif


  rgb getColor(char value);
  arrayPos getArrayPos(div_t x, div_t y);
  void setEnvMapVal(arrayPos curArrayPos, byte val);
  byte getEnvMapVal(arrayPos curArrayPos);
  void updateFieldProbably(arrayPos curArrayPos, char alternationVal);
  arrayPos trigonom(int sensorAngle, byte dist);

};
