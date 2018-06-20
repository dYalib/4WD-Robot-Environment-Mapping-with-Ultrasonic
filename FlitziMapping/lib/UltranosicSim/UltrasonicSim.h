#ifndef __AVR__

class UltrasonicSim{

private:
                          // 0  5   10  15  20  25  30  35  40  45  50  55  60   65  70  75 80  85  90  95  100 105 110 115 120 125 130 135 140 145 150 155 160 165 170 175 180

  const int tstVecor10[38] ={99, 99, 99, 99, 90, 90, 99, 99, 99, 99, 99, 99, 90, 99,10, 12, 5, 6, 7, 8, 9, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90};
  const int tstVecor1[38] ={20, 20, 21, 22, 21, 20, 21, 22, 22, 11, 23, 24, 152, 24, 98, 53, 122, 999, 16, 57, 60, 58, 58, 58, 57, 64, 68, 69, 82, 83, 87, 73, 76, 94, 66, 81, 71};
  const int tstVecor3[38] ={21, 20, 20, 21, 22, 21, 20, 20, 21, 22, 22, 35, 24, 107, 122, 2, 131, 5, 125, 45, 47, 59, 58, 58, 57, 57, 58, 69, 63, 68, 69, 74, 76, 68, 67, 66, 66, 66};
  const int tstVecor4[38] = {20, 20, 20, 21, 22, 21, 20, 21, 21, 22, 22, 22, 23, 87, 36, 60, 18, 14, 31, 41, 115, 11, 59, 69, 57, 57, 60, 61, 70, 68, 67, 102, 79, 68, 77, 53, 67, 95};

public:
  int distanceRead(int servoPos);
};

#endif
