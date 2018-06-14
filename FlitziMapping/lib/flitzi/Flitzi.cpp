#include <Flitzi.h>
#ifdef __AVR__
  #define OLED_RESET 4
  #define NUMFLAKES 10
  #define XPOS 2
  #define YPOS 1
  #define DELTAY 2

  static Ultrasonic ultrasonic(A2,A3);
  static Adafruit_SSD1306 display(OLED_RESET);
#endif

#ifndef __AVR__
  static UltrasonicSim ultrasonic;
  typedef unsigned char byte;
#endif

Flitzi::Flitzi() {
  curDist = 0;
}

#ifndef __AVR__
  void Flitzi::delay(int ms) {
    usleep(ms);
  }
#endif


#ifdef __AVR__
  void Flitzi::init() {
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(5,0);
    //servo range is 180° to 20°. Thats a mechanical issue.
    servo.attach(7,750,2400);
    moveServo(servoForward);
}
#endif

void Flitzi::move (int powerLeft, int powerRight) {
#ifdef __AVR__
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
#endif

#ifndef __AVR__
  std::cout << "Move PowerL: " << powerLeft << " PowerR: " << powerRight << '\n';
#endif

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

void Flitzi::moveServo( byte servoPos){
    int delayTime = 0;
    delayTime = abs(servo.read() - servoPos) * 3;
    servo.write(servoPos);
    //Serial.println("Time:" + String(delayTime));
    //Serial.println("Pos");
    //Serial.println(String(servo.read()));
    delay(delayTime);

  #ifndef __AVR__
    std::cout << "Move Servo to " << (int) servoPos << '\n';
  #endif
}

#ifdef __AVR__
void Flitzi::showAtDisplay(String txt) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,0);
  display.println(txt);
  display.display();
}
#endif

int Flitzi::getDistance() {
  int normDist = 0;

  for (int i=0; i < 10; i++) {
    #ifdef __AVR__
      normDist += ultrasonic.distanceRead();
      //Serial.println(" normDist: " + String(normDist));
    #endif

    #ifndef __AVR__
      normDist+= ultrasonic.distanceRead(servo.read());
    #endif
  }




  if (normDist == 0) normDist=9999;
  normDist = normDist / 10;
  #ifdef __AVR__
    //Serial.println("Dist:" + String(normDist / 10));
    showAtDisplay(String(normDist));
  #endif

  return (normDist);
}

/*
int Flitzi::scanEnviroment(byte ServoPos) {
  moveServo(ServoPos);
  int val = getDistance();
  showAtDisplay(String(val));
  return val;
}
*/

byte Flitzi::nextServoPos(byte step) {
int newPos = 0;
  if (scanReverse == true) {
      newPos = servo.read() - step;
  }
    else {
        newPos = servo.read() + step;
    };
  if (newPos > 180 || newPos < 0 ) {
    scanReverse = not scanReverse;
    newPos = nextServoPos(step);
  }
  return newPos;
}

void Flitzi::enviromentMapping(){

};

#ifdef __AVR__
  void Flitzi::generateSimulationData() {

    moveServo(0);
    Serial.print("{");
    do {
      Serial.print(String(getDistance()) + ", ");
      //delay(200);
      moveServo(nextServoPos(5));

    }while (scanReverse ==false);

    Serial.println("1000}");
}
#endif

#ifndef __AVR__
void Flitzi::visualiseArray() {
  envMap[2][3].nib1 = 1;
  envMap[2][3].nib2 = 1;
  FILE *f = fopen("out.ppm", "wb");
  fprintf(f, "P6\n%i %i 255\n", MAPSIZE * 2, MAPSIZE *2);
  for (int y=0; y<MAPSIZE * 2; y++) {
      for (int x=0; x<MAPSIZE * 2; x++) {
         if (envMap[x][y].nib1 == 0) {;
          fputc(255, f);   // 0 .. 255 RED
          fputc(0, f); // 0 .. 255 GREEN
          fputc(0, f);  // 0 .. 255 BLUE
        }
        if (envMap[x][y].nib2 == 0) {;
         fputc(255, f);   // 0 .. 255 RED
         fputc(0, f); // 0 .. 255 GREEN
         fputc(0, f);  // 0 .. 255 BLUE
       }

       if (envMap[x][y].nib1 == 1) {;
        fputc(0, f);   // 0 .. 255 RED
        fputc(255, f); // 0 .. 255 GREEN
        fputc(0, f);  // 0 .. 255 BLUE
      }
      if (envMap[x][y].nib2 == 1) {;
       fputc(0, f);   // 0 .. 255 RED
       fputc(255, f); // 0 .. 255 GREEN
       fputc(0, f);  // 0 .. 255 BLUE
     }

      }
  }
  fclose(f);
}
#endif
