#include <Arduino.h>
#include <Flitzi.h>

Flitzi robi;
  int i = 0;

void setup() {
  robi.init();
}

void loop() {

  robi.showAtDisplay(String(i));
  i++;
  delay(500);

}
