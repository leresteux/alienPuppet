#include <Servo.h>
//pin
byte pinDC = 6;
byte pinServo = 5;

//servo
byte maxservo = 120;
byte miniservo = 70;
byte reposservo = 90;
byte pos = reposservo;
int upOrDown;
int oldPos = pos;
Servo bras;
//DC
bool DcMove = false;
byte speed_DC;
//btn
byte BtnPush = 3;
int BtnX = A0;
//int BtnY =A1;
int ValX;
int ValY;
int ValBtnPush;

void setup()
{
  Serial.begin (9600);
  pinMode(pinDC, OUTPUT);
  pinMode(BtnPush, INPUT_PULLUP);
  bras.attach(pinServo);  // attaches the servo on pin 9 to the servo object
  delay(10);
  bras.write(pos);
  delay(1000);

  Serial.begin(9600);
}


void loop()

{
  //lecture joystick
  ValX = analogRead(BtnX);//bras
  delay(1);


  //code bras
  if (ValX < 560 && ValX > 500) {
    Arm_off();
  }
  else {
    bras.attach(pinServo);
    delay(5);
    pos = map(ValX, 0, 1023, miniservo , maxservo);
    delay(5);
    if (oldPos != pos) {
      bras.write(pos);
      oldPos = pos;
    }
    delay(5);
  };

  DC();
  delay(10);
  infoSerial();
}

void Arm_off() {
  if (pos != reposservo) {
    if (pos > reposservo) {
      upOrDown = -1;
    }
    else {
      upOrDown = 1;
    }
    pos = pos + upOrDown;
    bras.write(pos);
    delay(50);
    infoSerial();
  }
  else if (pos == reposservo) {
    bras.detach();
  }
}
void DC() {
  ValBtnPush = digitalRead(BtnPush);//DC
  delay(1);
  // code DC
  if (ValBtnPush == false) {
    DcMove =! DcMove;
    if (DcMove) {
      speed_DC = 255;
    }
    else {
      speed_DC = 0;
    }
    delay(1);
    analogWrite(pinDC, speed_DC);
    delay(500);
  }
}

void infoSerial() {
  Serial.print("pos ");
  Serial.println(pos);
  Serial.print("ValX ");
  Serial.println(ValX);
  Serial.print("btn ");
  Serial.println(DcMove);
}