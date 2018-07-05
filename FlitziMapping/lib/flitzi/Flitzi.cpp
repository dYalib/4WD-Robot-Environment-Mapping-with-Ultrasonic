#include <Flitzi.h>
#ifdef __AVR__
  #define OLED_RESET 4
  #define NUMFLAKES 10
  #define XPOS 2
  #define YPOS 1
  #define DELTAY 2
  static  Ultrasonic ultrasonic(A2,A3);
  static  Adafruit_SSD1306 display(OLED_RESET);
#endif

#ifndef __AVR__
  static UltrasonicSim ultrasonic;
  typedef unsigned char byte;
#endif

Flitzi::Flitzi() {
  curDist = 0;
  curPose.x = 40;
  curPose.y = 20;
  setFieldOfRobot();
}

#ifndef __AVR__
  void Flitzi::delay(unsigned int ms) {
    usleep(ms * 1000);
  }
#endif


#ifdef __AVR__
  void Flitzi::init() {

    pinMode(PIN_LED, OUTPUT);


    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(5,0);
    servo.attach(7);
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
  haseMoved=true;
  std::cout << "Move PowerL: " << powerLeft << " PowerR: " << powerRight << '\n';
#endif

}

void Flitzi::move(int cm) {
  move(90,90);
  delay(450);
  stop();
  curPose.y = curPose.y + cm;
  setFieldOfRobot();
}

void Flitzi::stop(){
  move(0,0);
}

#ifdef __AVR__
  void Flitzi::enableLED(){
    digitalWrite(12,HIGH);
  }

  void Flitzi::disableLED(){
    digitalWrite(PIN_LED,LOW);
  }

  void Flitzi::waitForButtonPress() {
    enableLED();
      while(digitalRead(13) == LOW) {}
    disableLED();
  }
#endif

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
    delay(delayTime);

  #ifndef __AVR__
    std::cout << "Move Servo to " << (int) servoPos << '\n';
  #endif
}

byte Flitzi::getDistance() {
  unsigned int normDist = 0;

  for (byte i=0; i < 10; i++) {
    #ifdef __AVR__
      normDist += ultrasonic.distanceRead();
    #endif

    #ifndef __AVR__
      if (haseMoved) {
        normDist+= ultrasonic.distanceRead(servo.read(),5);
      } else {
        normDist+= ultrasonic.distanceRead(servo.read(),0);
      }
    #endif
  }
  normDist = normDist / 10;
  if (normDist > 255 || normDist==0) normDist=255;

  #ifdef __AVR__
    showAtDisplay(String(normDist));
  #endif

  return (normDist);
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
  byte curAngle = 0;
  scanReverse=false;
  byte dist=0;
  do {
    moveServo(curAngle);
    dist=getDistance();
    arrayPos oldPos;

  //obstacles
  //  consider ultrasonic measuring angle
      for (byte a=0; a <= MEASURINGANGLE; a++ ) {

           if (trigonom(curAngle - MEASURINGANGLE/2 + a, dist) != oldPos) {
              updateFieldProbably(trigonom(curAngle - MEASURINGANGLE/2 +a, dist),1);
              oldPos = trigonom(curAngle - MEASURINGANGLE/2 +a, dist);
          }
      };
    //freefieldS

    for (int k=dist - RESOLUTION; k > 0; k = k - RESOLUTION ) {
    //   consider ultrasonic measuring angle
        for (byte a=0; a <= MEASURINGANGLE; a++ ) {
          if (trigonom(curAngle - MEASURINGANGLE/2 + a, k) != oldPos) {
             updateFieldProbably(trigonom(curAngle - MEASURINGANGLE/2 +a, k),-1);
             oldPos = trigonom(curAngle - MEASURINGANGLE/2 +a, k);
           }
      }
    };
    curAngle = nextServoPos(5);
  } while (scanReverse == false);
};

#ifdef __AVR__
  void Flitzi::generateSimulationData() {
    byte nextPos = 0;
    moveServo(0);
    Serial.print("{");
    do {
      Serial.print(String(getDistance()) + F(", "));
      delay(200);
      moveServo(nextPos);
      nextPos = nextServoPos(5);

    }while (nextPos < 180);
    moveServo(nextPos);
    Serial.print(String(getDistance()) + F("}"));
}
#endif

Flitzi::rgb Flitzi::getColor(char value){
  rgb rgbVal;

  if (value > 0) {
    rgbVal.red = (255 - ((value-1) * 15));
    rgbVal.green = 0;
    rgbVal.blue = 0;
  }

  if (value < 0) {
    rgbVal.red = 0;
    rgbVal.green = (255 + ((value+1) * 15));
    rgbVal.blue = 0;
  }

  if (value == 0) {
    rgbVal.red = 125;
    rgbVal.green = 125;
    rgbVal.blue = 125;
  }

  return rgbVal;
}

void Flitzi::visualiseArray() {
  #ifndef __AVR__
    std::string name("out");
    name.append(std::to_string(curPose.x));
    name.append(std::to_string(curPose.y));
    name.append(".ppm");
    FILE *f = fopen(name.c_str(), "wb");
    fprintf(f, "P6\n%i %i 255\n", MAPSIZE * 2, MAPSIZE *2);
  #endif

  #ifdef __AVR__
    Serial.println(F("P3 "));
    Serial.println(F("40 40"));
    Serial.println(F("255"));
  #endif

  for (char y= (MAPSIZE *2) -1; y >= 0 ; y--) {
      for (char x=0 ;x <= (MAPSIZE * 2) -1; x++) {
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

          #ifndef __AVR__
            fputc(curColor.red, f);   // 0 .. 255 RED
            fputc(curColor.green, f); // 0 .. 255 GREEN
            fputc(curColor.blue, f);  // 0 .. 255 BLUE
          #endif

          #ifdef __AVR__
          Serial.print(curColor.red);   // 0 .. 255 RED
          Serial.print(F(" "));
          Serial.print(curColor.green); // 0 .. 255 GREEN
          Serial.print(F(" "));
          Serial.print(curColor.blue);
          Serial.print(F("   "));  // 0 .. 255 BLUE
          #endif
     }

    #ifdef __AVR__
      Serial.println(F(""));
    #endif

      }
      #ifndef __AVR__
        fclose(f);
      #endif
  }

Flitzi::arrayPos Flitzi::getArrayPos(div_t x, div_t y) {
  arrayPos curArrayPos;

  if (x.quot < 0 || x.quot >= MAPSIZE  || y.quot < 0 || y.quot >= MAPSIZE) {
    curArrayPos.x = 255;
    curArrayPos.y = 255;
    curArrayPos.nib = 255;
    return curArrayPos;
  }
  curArrayPos.x =x.quot;
  curArrayPos.y = y.quot;
  if (x.rem < 2) {
    if (y.rem < 2) {
      curArrayPos.nib=0;
    }
    else {
      curArrayPos.nib=1;
    }
  }
  else {
    if (y.rem < 2) {
      curArrayPos.nib=2;
    }
    else {
      curArrayPos.nib=3;
    }
  }
  return curArrayPos;
}

void Flitzi::setEnvMapVal(arrayPos curArrayPos, byte val) {
  if (!(curArrayPos.x == 255 or curArrayPos.y == 255 or curArrayPos.nib==255)){
    switch (curArrayPos.nib) {
      case 0 : envMap[curArrayPos.x][curArrayPos.y].nib_00 = val; break;
      case 1 : envMap[curArrayPos.x][curArrayPos.y].nib_01 = val; break;
      case 2 : envMap[curArrayPos.x][curArrayPos.y].nib_10 = val; break;
      case 3 : envMap[curArrayPos.x][curArrayPos.y].nib_11 = val; break;
    }
  }
}

byte Flitzi::getEnvMapVal(arrayPos curArrayPos) {
  if (!(curArrayPos.x == 255 or curArrayPos.y == 255 or curArrayPos.nib==255)){
    switch (curArrayPos.nib) {
      case 0 : return envMap[curArrayPos.x][curArrayPos.y].nib_00; break;
      case 1 : return envMap[curArrayPos.x][curArrayPos.y].nib_01; break;
      case 2 : return envMap[curArrayPos.x][curArrayPos.y].nib_10; break;
      case 3 : return envMap[curArrayPos.x][curArrayPos.y].nib_11; break;
    }
  }
  return 0;
}


void Flitzi::updateFieldProbably(arrayPos curArrayPos, char alternationVal) {
  char oldVal = getEnvMapVal(curArrayPos);
  if (oldVal + alternationVal >=-7 and oldVal + alternationVal <= 7) oldVal = oldVal + alternationVal;
  setEnvMapVal(curArrayPos, oldVal);
}

Flitzi::arrayPos Flitzi::trigonom(int sensorAngle, byte dist){
  switch (sensorAngle) {
    case 0: {
      return getArrayPos(div(dist + curPose.x + ROBOT_US_GAP , 4), div(curPose.y -ROBOT_US_GAP, 4));
      break;
      }

    case 90: {
      return getArrayPos(div(curPose.x, 4), div(curPose.y + dist, 4));
      break;
      }


    case 180: {
      return getArrayPos(div(curPose.x - ROBOT_US_GAP - dist, 4), div(curPose.y - ROBOT_US_GAP , 4));
      break;
      }

    default: {
        unsigned int x = round (cos(sensorAngle * PI / 180) * dist +  curPose.x  + cos(sensorAngle * PI / 180) * ROBOT_US_GAP);
         unsigned int y = round (sin(sensorAngle * PI / 180) * dist + curPose.y + (-1 *ROBOT_US_GAP) + sin(sensorAngle * PI / 180) * ROBOT_US_GAP);
        return getArrayPos(div(x, 4), div(y, 4));
      }
    }
}


void Flitzi::setFieldOfRobot(){
  for (byte x=0;x < ROBOTWIDTH / 2; x++){
    for (byte y=0; y < ROBOTLENGHT; y++) {
      setEnvMapVal(getArrayPos(div (curPose.x + x,4), div (curPose.y - y, 4)), -7);
      setEnvMapVal(getArrayPos(div (curPose.x -x ,4), div (curPose.y - y, 4)), -7);
    }
  }
}

#ifdef __AVR__
void Flitzi::paintOnDisplay() {
  display.clearDisplay();

  display.drawPixel(1,1,1);

  for (char y= (MAPSIZE *2) -8; y >= 0 ; y--) {
      for (char x=0 ;x <= (MAPSIZE * 2) -1; x++) {
        if ( y % 2 == 0 &&  x % 2 == 0)  {
          if (envMap[x/2][y/2].nib_00 > 0) display.drawPixel(24 + 2*x ,32-y ,1);
        }
        //y even and x odd
        if ( y % 2 == 0 &&  x % 2 != 0)  {
          if (envMap[x/2][y/2].nib_10 > 0 ) display.drawPixel(24 + 2*x ,32-y ,1);
        }
        //y odd and x even
        if ( y % 2 != 0 &&  x % 2 == 0)  {
          if (envMap[x/2][y/2].nib_01 > 0 ) display.drawPixel(24 + 2*x ,32-y ,1);
        }
        //y and x odd
        if ( y % 2 != 0 &&  x % 2 != 0)  {
          if (envMap[x/2][y/2].nib_11 > 0 ) display.drawPixel(24 + 2*x ,32-y ,1) ;
        }

    }
    }
  display.display();
}
#endif
