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
  void moveServo( int servoPos);
  void showAtDisplay(String txt);
  int getDistance();
  int scanEnviroment(int ServoPos);
  int nextServoPos(int curPos, int step);
  void enviromentMapping();


private:
  const int  PIN_ENA=0;
  const int  PIN_ENB=1;
  const int  PIN_IN1=3; //HIGH -> forward
  const int  PIN_IN2=5; //PWM
  const int  PIN_IN3=9; //LOW -> Back
  const int  PIN_IN4=6; //PWM
  const int  servoForward= 87;
  Servo servo;
  int curDist;
  //boolean mapping[30][100];
};
