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
  curPose.x = 20;
  curPose.y = 30;

  setFieldOfRobot();


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

  for (byte i=0; i < 10; i++) {
    #ifdef __AVR__
      normDist += ultrasonic.distanceRead();
      //Serial.println(" normDist: " + String(normDist));
    #endif

    #ifndef __AVR__
      normDist+= ultrasonic.distanceRead(servo.read());
    #endif
  }

  if (normDist == 0) normDist=255;
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
Flitzi::rgb Flitzi::getColor(int value){
  rgb rgbVal;
  switch (value) {
    case -7 : {rgbVal.red = 0; rgbVal.green=165; rgbVal.blue=0; break;}
    case -6 : {rgbVal.red = 0; rgbVal.green=180; rgbVal.blue=0; break;}
    case -5 : {rgbVal.red = 0; rgbVal.green=119; rgbVal.blue=0; break;}
    case -4 : {rgbVal.red = 0; rgbVal.green=210; rgbVal.blue=0; break;}
    case -3 : {rgbVal.red = 0; rgbVal.green=225; rgbVal.blue=0; break;}
    case -2 : {rgbVal.red = 0; rgbVal.green=240; rgbVal.blue=0; break;}
    case -1 : {rgbVal.red = 0; rgbVal.green=255; rgbVal.blue=0; break;}
    case 0 : {rgbVal.red = 125; rgbVal.green=125; rgbVal.blue=125; break;}
    case 1 : {rgbVal.red = 255; rgbVal.green=0; rgbVal.blue=0; break;}
    case 2 : {rgbVal.red = 240; rgbVal.green=0; rgbVal.blue=0; break;}
    case 3 : {rgbVal.red = 225; rgbVal.green=0; rgbVal.blue=0; break;}
    case 4 : {rgbVal.red = 210; rgbVal.green=0; rgbVal.blue=0; break;}
    case 5 : {rgbVal.red = 119; rgbVal.green=0; rgbVal.blue=0; break;}
    case 6 : {rgbVal.red = 180; rgbVal.green=0; rgbVal.blue=0; break;}
    case 7 : {rgbVal.red = 165; rgbVal.green=0; rgbVal.blue=0; break;}
    default : {rgbVal.red = 0; rgbVal.green=0; rgbVal.blue=0; break;}
  }


  return rgbVal;
}


void Flitzi::visualiseArray() {
  //envMap[0][0].nib_00= -7;
  //envMap[2][3].nib_11 =-7;
  updateFieldProbably(90, 17, 7);
  FILE *f = fopen("out.ppm", "wb");
  fprintf(f, "P6\n%i %i 255\n", MAPSIZE * 2, MAPSIZE *2);
  for (int y= (MAPSIZE *2) -1; y >= 0 ; y--) {
      for (int x=0 ;x <= (MAPSIZE * 2) -1; x++) {
        rgb curColor = getColor(0);
          //y and x even
          if ( y % 2 == 0 &&  x % 2 == 0)  {
            curColor = getColor(envMap[x/2][y/2].nib_00);
        }
          //y even and x odd
          if ( y % 2 == 0 &&  x % 2 != 0)  {
            curColor = getColor(envMap[x/2][y/2].nib_10);
          }
          //y odd and x even
          if ( y % 2 != 0 &&  x % 2 == 0)  {
            curColor = getColor(envMap[x/2][y/2].nib_01);
          }
          //y and x odd
          if ( y % 2 != 0 &&  x % 2 != 0)  {
            curColor = getColor(envMap[x/2][y/2].nib_11);
          }

          fputc(curColor.red, f);   // 0 .. 255 RED
          fputc(curColor.green, f); // 0 .. 255 GREEN
          fputc(curColor.blue, f);  // 0 .. 255 BLUE
     }

      }
      fclose(f);
  }
#endif

void Flitzi::setEnvMapVal(div_t x, div_t y, byte val) {
  if (x.rem < 2) {
    if (y.rem < 2) {
      envMap[x.quot][y.quot].nib_00 = val;
    }
    else {
      envMap[x.quot][y.quot].nib_01 = val;
    }
  }
  else {
    if (y.rem < 2) {
      envMap[x.quot][y.quot].nib_10 = val;
    }
    else {
      envMap[x.quot][y.quot].nib_11 = val;
    }
  }
}


byte Flitzi::getEnvMapVal(div_t x, div_t y) {
  if (x.rem < 2) {
    if (y.rem < 2) {
      return envMap[x.quot][y.quot].nib_00;
    }
    else {
      return envMap[x.quot][y.quot].nib_01;
    }
  }
  else {
    if (y.rem < 2) {
      return envMap[x.quot][y.quot].nib_10;
    }
    else {
      return envMap[x.quot][y.quot].nib_11;
    }
  }
}

void Flitzi::updateFieldProbably(byte sensorAngle, byte dist, char alternationVal ){
  switch (sensorAngle) {
    case 0: {
      if (dist + curPose.x >= MAPSIZE * RESOLUTION * 2) {
        dist = MAPSIZE * RESOLUTION * 2 - curPose.x -1;
      }
      byte oldVal = getEnvMapVal(div(dist, 4), div(curPose.y, 4));
      setEnvMapVal(div(dist + curPose.x, 4), div(curPose.y, 4), oldVal + alternationVal);
      break;
      }

    case 90: {
      if (dist + curPose.y >= MAPSIZE * RESOLUTION * 2) {
        dist = MAPSIZE * RESOLUTION * 2 - curPose.y -1;
              }
      byte oldVal = getEnvMapVal(div(curPose.x, 4), div(curPose.y + dist, 4));
      setEnvMapVal(div(curPose.x, 4), div(curPose.y + dist, 4), oldVal + alternationVal);
      break;
      }


    case 180: {
      if (curPose.x - dist < 0) {
        dist = curPose.x;
              }
      byte oldVal = getEnvMapVal(div(dist, 4), div(curPose.y, 4));
      setEnvMapVal(div(curPose.x - dist, 4), div(curPose.y, 4), oldVal + alternationVal);
      break;
      }
    }
}


void Flitzi::setFieldOfRobot(){
//TODO: Check Valid Robot Pos!
  for (byte x=0;x < ROBOTBREADTH / 2; x++){
    for (byte y=0; y < ROBOTLENGHT; y++) {
      setEnvMapVal(div (curPose.x + x,4), div (curPose.y - y, 4), -7);
      setEnvMapVal(div (curPose.x -x ,4), div (curPose.y - y, 4), -7);
    }
  }
}
