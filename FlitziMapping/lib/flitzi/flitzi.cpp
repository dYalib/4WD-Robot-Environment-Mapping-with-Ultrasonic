#include <Arduino.h>
#include <Ultrasonic.h>
#include <Servo.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int  PIN_ENA=0;
const int  PIN_ENB=1;
const int  PIN_IN1=3; //HIGH -> forward
const int  PIN_IN2=5; //PWM
const int  PIN_IN3=9; //LOW -> Back
const int  PIN_IN4=6; //PWM
const int  servoForward= 85;


Servo servo;

Ultrasonic ultrasonic(A2,A3);
int curDist=0;

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 2
#define YPOS 1
#define DELTAY 2

boolean mapping[30][100];


void move (int powerLeft, int powerRight) {
  if (powerLeft < 0) {
    digitalWrite(PIN_IN3, HIGH);
  } else {
    digitalWrite(PIN_IN3, LOW);
  }

  if (powerRight < 0) {
    digitalWrite(PIN_IN1, HIGH);
  }
  else {
    digitalWrite(PIN_IN1, LOW);
  }
  analogWrite(PIN_IN2, powerRight);
  analogWrite(PIN_IN4, powerLeft);
}


void stop(){
  move(0,0);
}

void turn (int degree) {
switch (degree) {
  case 90 : {
    move(180,-180);
    delay(500);
    stop();
    break;
  }
  case -90 : {
    move(-180,180);
    delay(500);
    stop();
    break;
  }

  case 180: {
    move(180,-180);
    delay(1000);
    stop();
    break;
  }

  case -180: {
    move(-180,180);
    delay(1000);
    stop();
    break;
  }
 }
}

void moveServo( int servoPos){
  if (servoPos < 5) servoPos= 5;
  if (servoPos > 180) servoPos= 180;
  int delayTime=0;
  delayTime = abs(servo.read() - servoPos) * 3;
  servo.write(servoPos);
  Serial.println(String(servo.read()));
  delay(delayTime);
}


void showAtDisplay(String txt) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,0);
  display.println(txt);
  display.display();
}

int getDistance() {
  int normDist = 0;
  for (int i=0; i < 10; i++) {
    normDist += ultrasonic.distanceRead();
    //Serial.println(" normDist: " + String(normDist));
  }
  if (normDist == 0) normDist=9999;
  normDist = normDist / 10;
  //Serial.println("Dist:" + String(normDist / 10));
  showAtDisplay(String(normDist));
  return (normDist);

}

int scanEnviroment(int ServoPos) {
  moveServo(ServoPos);
  int val = getDistance();
  showAtDisplay(String(val));
  return val;
}

int nextServoPos(int curPos, int step) {
  int pos = curPos + step;
  if (pos > 180) pos = 5;
  return pos;
}

void enviromentMapping(){


}
