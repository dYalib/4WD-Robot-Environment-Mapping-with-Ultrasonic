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
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(5,0);
    //servo range is 180° to 20°. Thats a mechanical issue.
    //servo.attach(7,750,2400);
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
  std::cout << "Move PowerL: " << powerLeft << " PowerR: " << powerRight << '\n';
#endif

}

void Flitzi::move(int cm) {
  move(125,125);
  delay(1000);
  curPose.y = curPose.y + cm;
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
  //Serial.println(txt);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,0);

  //display.print(c);
  display.println(txt);
  display.display();
}
#endif

byte Flitzi::getDistance() {
  unsigned int normDist = 0;

  for (byte i=0; i < 10; i++) {
    #ifdef __AVR__
      normDist += ultrasonic.distanceRead();
      //Serial.println(" normDist: " + String(normDist));
    #endif

    #ifndef __AVR__
      normDist+= ultrasonic.distanceRead(servo.read());
    #endif
  }

  normDist = normDist / 10;
  if (normDist > 255 || normDist==0) normDist=255;

  #ifdef __AVR__
    //Serial.println("Dist:" + String(normDist / 10));
    showAtDisplay(String(normDist));
  #endif

  return (normDist);
}

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
    //if (curAngle > 15 && curAngle < 165) {
      for (byte a=0; a <= MEASURINGANGLE; a++ ) {

           if (trigonom(curAngle - MEASURINGANGLE/2 + a, dist) != oldPos) {
              updateFieldProbably(trigonom(curAngle - MEASURINGANGLE/2 +a, dist),1);
              oldPos = trigonom(curAngle - MEASURINGANGLE/2 +a, dist);
          }
      //  }
      };

      /*
      visualiseArray();
      delay(1000);
      */
    //freefieldS

    for (int k=dist - RESOLUTION; k > 0; k = k - RESOLUTION ) {
    //   consider ultrasonic measuring angle
      //if (curAngle > 15 && curAngle < 165) {
        for (byte a=0; a <= MEASURINGANGLE; a++ ) {
          if (trigonom(curAngle - MEASURINGANGLE/2 + a, k) != oldPos) {
             updateFieldProbably(trigonom(curAngle - MEASURINGANGLE/2 +a, k),-1);
             oldPos = trigonom(curAngle - MEASURINGANGLE/2 +a, k);
           }
        // }
      }
    };
    curAngle = nextServoPos(5);
  } while (scanReverse == false);
};

#ifdef __AVR__
  void Flitzi::generateSimulationData() {

    moveServo(0);
    Serial.print("{");
    do {
      Serial.print(String(getDistance()) + ", ");
      delay(200);
      moveServo(nextServoPos(5));

    }while (scanReverse ==false);

    Serial.println(F("1000}"));
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
    //name = std::string("out") + std::string(rand() % 100) + std::string(".ppm");
    FILE *f = fopen(name.c_str(), "wb");
    fprintf(f, "P6\n%i %i 255\n", MAPSIZE * 2, MAPSIZE *2);
  #endif

  #ifdef __AVR__
    showAtDisplay(F("Send data..."));
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
          Serial.print(" ");
          Serial.print(curColor.green); // 0 .. 255 GREEN
          Serial.print(" ");
          Serial.print(curColor.blue);
          Serial.print("   ");  // 0 .. 255 BLUE
          //delay(20);
          //  line = line + String(curColor.red) + " " + String(curColor.green) + " " + String(curColor.blue) + "   ";
          #endif
     }

    #ifdef __AVR__
      Serial.println(F(""));
    #endif

      }
      #ifndef __AVR__
        fclose(f);
      #endif

      #ifdef __AVR__
        showAtDisplay(F("done!"));
      #endif

  }

Flitzi::arrayPos Flitzi::getArrayPos(div_t x, div_t y) {
  arrayPos curArrayPos;

  if (x.quot < 0 || x.quot >= MAPSIZE  || y.quot < 0 || y.quot >= MAPSIZE) {
    curArrayPos.x = 255;
    curArrayPos.y = 255;
    curArrayPos.nib = 255;
    //std::cout << "x: " << (int) curArrayPos.x  << " y: " << (int) curArrayPos.y << " nib: " << (int)curArrayPos.nib << "\n";
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
 //std::cout << "x index: " << x.quot << " y index: " << y.quot << "\n";
  if (!(curArrayPos.x == 255 or curArrayPos.y == 255 or curArrayPos.nib==255)){
    //std::cout << "---- x: " << (int) curArrayPos.x << " y: " << (int) curArrayPos.y << "-----" "\n";
    switch (curArrayPos.nib) {
      case 0 : envMap[curArrayPos.x][curArrayPos.y].nib_00 = val; break;
      case 1 : envMap[curArrayPos.x][curArrayPos.y].nib_01 = val; break;
      case 2 : envMap[curArrayPos.x][curArrayPos.y].nib_10 = val; break;
      case 3 : envMap[curArrayPos.x][curArrayPos.y].nib_11 = val; break;
    }
  }
}

byte Flitzi::getEnvMapVal(arrayPos curArrayPos) {
  //std::cout << "x index: " << x.quot << " y index: " << y.quot << "\n";
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
  //std::cout << "sensorAngle: " << (int) sensorAngle << "\n";
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
//TODO: Check Valid Robot Pos!
  for (byte x=0;x < ROBOTBREADTH / 2; x++){
    for (byte y=0; y < ROBOTLENGHT; y++) {
      setEnvMapVal(getArrayPos(div (curPose.x + x,4), div (curPose.y - y, 4)), -7);
      setEnvMapVal(getArrayPos(div (curPose.x -x ,4), div (curPose.y - y, 4)), -7);
    }
  }
}
