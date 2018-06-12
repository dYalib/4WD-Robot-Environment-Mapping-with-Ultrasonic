#include <Flitzi.h>

#define OLED_RESET 4
#define NUMFLAKES 10
#define XPOS 2
#define YPOS 1
#define DELTAY 2

static Ultrasonic ultrasonic(A2,A3);
static Adafruit_SSD1306 display(OLED_RESET);


Flitzi::Flitzi() {
  curDist = 0;
}

void Flitzi::init() {
  //Adafruit_SSD1306 display(OLED_RESET);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,0);
  servo.attach(7,600,2400);
  moveServo(servoForward);

}
void Flitzi::move (int powerLeft, int powerRight) {
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


void Flitzi::stop(){
  move(0,0);
}

void Flitzi::turn (int degree) {
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

void Flitzi::moveServo( int servoPos){
  if (servoPos < 5) servoPos= 5;
  if (servoPos > 180) servoPos= 180;
  int delayTime=1000;
  delayTime = abs(servo.read() - servoPos) * 3;
  servo.write(servoPos);
  //Serial.println("Time:" + String(delayTime));
  //Serial.println("Pos");
  //Serial.println(String(servo.read()));
  delay(delayTime);
}

void Flitzi::showAtDisplay(String txt) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,0);
  display.println(txt);
  display.display();
}
int Flitzi::getDistance() {
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

int Flitzi::scanEnviroment(int ServoPos) {
  moveServo(ServoPos);
  int val = getDistance();
  showAtDisplay(String(val));
  return val;
}

int Flitzi::nextServoPos(int curPos, int step) {
  int pos = curPos + step;
  if (pos > 180) pos = 5;
  return pos;
}

void Flitzi::enviromentMapping(){

};
