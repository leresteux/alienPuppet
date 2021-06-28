//We always have to include the library
#include "LedControl.h"

/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 12 is connected to the DataIn
  pin 11 is connected to the CLK
  pin 10 is connected to LOAD
  We have only a single MAX72XX.
*/
#define NBR_MTX 1
LedControl lc = LedControl(4, 3, 2, NBR_MTX);

byte ledMaster[] = {3, 3};
/* we always wait a bit between updates of the display */
unsigned long delaytime = 300;

int R;
void setup() {
  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  Serial.begin(9600);
  for (int i = 0; i < NBR_MTX; i++) {
    lc.shutdown(i, false);
    /* Set the brightness to a medium values */
    lc.setIntensity(i, 1);
    /* and clear the display */
    lc.clearDisplay(i);
  }

}
void loop() {
  for (int i = 0; i < 40; i++) {
    crazyEye();
    bigEye();
    delay(100);
    eyeClean();
    delay(10);
  }
target();
  lc.clearDisplay(0);
}
void eye() {

  lc.setLed(0, ledMaster[0], ledMaster[1], true);
  lc.setLed(0, ledMaster[0], ledMaster[1] + 1, true);
  lc.setLed(0, ledMaster[0], ledMaster[1] - 1, true);
  lc.setLed(0, ledMaster[0] + 1, ledMaster[1], true);
  lc.setLed(0, ledMaster[0] - 1, ledMaster[1], true);

}
void target() {

  lc.setLed(0, ledMaster[0], ledMaster[1], true);
  for (int i = 0; i <= 6; i++) {
    lc.setLed(0, ledMaster[0], ledMaster[1] + i, true);
    lc.setLed(0, ledMaster[0], ledMaster[1] - i, true);
    lc.setLed(0, ledMaster[0] + i, ledMaster[1], true);
    lc.setLed(0, ledMaster[0] - i, ledMaster[1], true);
    delay(300);
  }
  delay(2000);
}
void bigEye() {

  //lc.setLed(0, ledMaster[0], ledMaster[1], true);

  lc.setLed(0, ledMaster[0], ledMaster[1] + 1, true);

  lc.setLed(0, ledMaster[0], ledMaster[1] - 1, true);

  lc.setLed(0, ledMaster[0] + 1, ledMaster[1], true);

  lc.setLed(0, ledMaster[0] - 1, ledMaster[1], true);

  lc.setLed(0, ledMaster[0], ledMaster[1] + 2, true);

  lc.setLed(0, ledMaster[0], ledMaster[1] - 2, true);

  lc.setLed(0, ledMaster[0] + 2, ledMaster[1], true);

  lc.setLed(0, ledMaster[0] - 2, ledMaster[1], true);

}
void eyeClean() {

  lc.setLed(0, ledMaster[0], ledMaster[1], false);
  lc.setLed(0, ledMaster[0], ledMaster[1] + 1, false);
  lc.setLed(0, ledMaster[0], ledMaster[1] - 1, false);
  lc.setLed(0, ledMaster[0] + 1, ledMaster[1], false);
  lc.setLed(0, ledMaster[0] - 1, ledMaster[1], false);


  lc.setLed(0, ledMaster[0], ledMaster[1] + 2, false);

  lc.setLed(0, ledMaster[0], ledMaster[1] - 2, false);

  lc.setLed(0, ledMaster[0] + 2, ledMaster[1], false);

  lc.setLed(0, ledMaster[0] - 2, ledMaster[1], false);


}

void crazyEye() {
  R = random(-1, 2);
  ledMaster[0] = ledMaster[0] + R;
  delay(10);
  if (ledMaster[0] < 1) {
    ledMaster[0] = 1;
  }
  else if (ledMaster[0] > 6) {
    ledMaster[0] = 6;
  }
  delay(10);
  R = random(-1, 2);
  ledMaster[1] = ledMaster[1] + R;
  delay(10);
  if (ledMaster[1] < 1) {
    ledMaster[1] = 1;
  }
  else if (ledMaster[1] > 6) {
    ledMaster[1] = 6;
  }
  delay(10);
  Serial.println(ledMaster[0]);
}
