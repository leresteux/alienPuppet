// finir code set up image

/*  aide< utilisé<
    https://www.delftstack.com/fr/howto/arduino/arduino-2d-array/
    https://xantorohara.github.io/led-matrix-editor/
*/
//installation de la library LedControl
#include "LedControl.h"


/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 4 is connected to the DataIn
  pin 3 is connected to the CLK
  pin 2 is connected to LOAD
  We have TWO Matrix.
*/
LedControl lc = LedControl(4, 3, 2, 2);
byte ledPin[] = {8, 9};
//global
byte matrix;

//cible
boolean targetStartOk = false;
int targetCoord[] = {0, 4, 4};



//scan1 var

byte scanCol = 0;
boolean scanOk = true;
boolean scanDown; //direction scan

//scan2  vertical
byte scanRow = 0;
boolean scanVerticalOk = false;
boolean scanVerticalStarted = false;

//timerscan var
boolean bipFromTimer = true;
unsigned long oldTime;
unsigned int speedScan = 100;
unsigned long nbreBipFromTimer = 5; // utilise dans timerforscan playimage target ini
// un bip tout le speedScan soit 0,1 s
// 5 pour lancer la premiere animation

//nbre mini et max de leds buggantes
const byte  nbreBuggedLedMini = 1;
const byte nbreBuggedLedMax = 5;

// nbre de leds buggantes Sur martix 1 & 2
byte nbreBuggedLed1;
byte nbreBuggedLed2;

// etat des leds buggés
byte buggedLedState1[nbreBuggedLedMax];
byte buggedLedState2[nbreBuggedLedMax];

// possition des leds buggés
byte buggedLedMatrix1[10][2];
byte buggedLedMatrix2 [10][2];


//listes des images
//stokage des image cles sur les deux matrix
byte randomImage[2];
byte nbreFrame[2];
boolean randomImageOk[] = {false, false};
const byte onetwo[][8] = {
  {
    B00000000,
    B00011100,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00111000,
    B00000000
  }, {
    B00000000,
    B00100110,
    B00100100,
    B00100100,
    B00100100,
    B00100100,
    B01100100,
    B00000000
  }
};
const int onetwo_len = sizeof(onetwo) / 8;

boolean z = false;

void setup() {


  // Allumer les Matrix
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 1);
  lc.setIntensity(1, 1);
  // mise à zero du Display
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  // enregistrement du temps référant
  for (byte i = 0; i < 2; i++) {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], HIGH);
  }

  oldTime = millis();
  randomImageOk[0] = true;
  randomImageOk[1] = true;
  scanVerticalOk = false;
  scanOk = true;

  playImage(1, onetwo, onetwo_len);
  playImage(0, onetwo, onetwo_len);

  randomImageOk[0] = false;
  randomImageOk[1] = false;
  delay (1000);

  Serial.begin(9600);
  //ledface off
  for (byte i = 0; i < 2; i++) {
    digitalRead(ledPin[i]) == LOW;
  }
  //test
  lc.setLed(0, 0, 0, HIGH);
  delay(500);
  lc.setLed(0, 1, 0, HIGH);
  delay(500);
}

void loop() {


  timerForScan();

  lc.setIntensity(0, 0);
  lc.setIntensity(1, 0);
  bug();
  lc.setIntensity(0, 14);
  lc.setIntensity(1, 14);
  scanVertical();
  scan();
  ledFace();
  targetStart();
  playImage(1, onetwo, onetwo_len);
  playImage(0, onetwo, onetwo_len);

  //si pas d'animation image sur matrix

  if (!randomImageOk[0] && !randomImageOk[1] && !targetStartOk ) {

    // alors "selection random d'une animation"

    int randomFx = random(200);
    if (randomFx == 0 ) {

      scanVerticalOk = true;

    }
  }
}



// permet de definir la vitesse de mouvement des scans
void timerForScan() {
  if (millis() - oldTime >= speedScan) {
    oldTime = millis();
    bipFromTimer = true;
    nbreBipFromTimer++;
  } else {
    bipFromTimer = false;
  }
}

void ledFace() {
  if (bipFromTimer) {
    for (byte i = 0; i < 2; i++) {
      if (random(4) < 3) {
        if (digitalRead(ledPin[i]) == LOW) {
          digitalWrite(ledPin[i], HIGH);
        }
        else {
          digitalWrite(ledPin[i], LOW);
        }
      }
    }
  }
}

// annule toutes les animations
void animOff() {
  scanOk = false;
  scanVerticalOk = false;
  randomImageOk[0] = false;
  randomImageOk[1] = false;
  targetStartOk = false;
  lc.clearDisplay(0);
  lc.clearDisplay(1);

}

/* fonction de configuration des led Buggantes
*/
void initBug() {
  //random si (re)definition des leds buggantes pour matrix 1
  if (random(2) == 0) {

    // random du nbre de legs buggées

    nbreBuggedLed1 = random(nbreBuggedLedMini, nbreBuggedLedMax);

    //possition des leds buggées
    for ( byte i = nbreBuggedLed1; i > 0; i--) {
      buggedLedMatrix1[i][0] =  random(8);
      buggedLedMatrix1[i][1] =  random(8);
    }
  }
  // idem pour Matrix2
  if (random(2) == 0) {


    // random du nbre de legs buggées

    nbreBuggedLed2 = random(nbreBuggedLedMini, nbreBuggedLedMax);

    //possition des leds buggées
    for ( byte i = nbreBuggedLed2; i > 0; i--) {
      buggedLedMatrix2[i][0] =  random(8);
      buggedLedMatrix2[i][1] =  random(8);
    }
  }
}
// fonction de clignotement des leds Buggantes
void bug() {
  /*
    Serial.println("BUG FONCTION /////////");
    Serial.print(" nbreBuggedLed1 ");
    Serial.println(nbreBuggedLed1);
  */
  if (random(200) == 0) {
    initBug();
  };
  for (byte i = 0; i <= nbreBuggedLed1 ; i++ ) {
    //random pour changement d'etat d'une Led buggantes
    if (random(8) == 0) {
      // if the LED is off turn it on and vice-versa:
      if (buggedLedState1[i] == LOW) {
        buggedLedState1[i] = HIGH;
      }
      else {
        buggedLedState1[i] = LOW;
      };
      lc.setLed(0, buggedLedMatrix1[i][0], buggedLedMatrix1[i][1], buggedLedState1[i]);
    }

  }
  for (byte i = 0; i <= nbreBuggedLed2 ; i++ ) {
    if (random(8) == 0) {
      // if the LED is off turn it on and vice-versa:
      if (buggedLedState2[i] == LOW) {
        buggedLedState2[i] = HIGH;
      }
      else {
        buggedLedState2[i] = LOW;
      };
      lc.setLed(1, buggedLedMatrix2[i][0], buggedLedMatrix2[i][1], buggedLedState2[i]);
    }
    /*
        Serial.print ("led n° ");
        Serial.print (i);
        Serial.print(" -> etat = ");
        Serial.println(buggedLedState1[i]);
    */
  }
}

// fonction pour efet scan Horizontal
void scan () {
  if (scanOk) {
    // pour effet descendant
    if (scanDown) {
      // allume une columne et eteind la precedante pour Matrix 1 & 2
      lc.setColumn(0, scanCol, B11111111);
      lc.setColumn(0, scanCol - 1, B00000000);
      lc.setColumn(1, scanCol, B11111111);
      lc.setColumn(1, scanCol - 1, B00000000);
      // si temps désiré ecoulé changement de ligne
      if (bipFromTimer) {
        if (scanCol == 7) {
          scanDown = false;
          scanCol = 6;
        } else {
          scanCol++;
        }
      }
    }
    // idem mais pour effet montant
    else {
      lc.setColumn(0, scanCol, B11111111);
      lc.setColumn(0, scanCol + 1, B00000000);
      lc.setColumn(1, scanCol, B11111111);
      lc.setColumn(1, scanCol + 1, B00000000);
      if (bipFromTimer) {
        if (scanCol == 0) {
          scanDown = true;
          scanCol = 1;
        } else {
          scanCol--;
        }
      }
    }
  }
}

void scanVertical() {

  if (scanVerticalOk) {
    if (scanVerticalStarted == false) {
      scanVerticalStarted = true;
      scanRow = 0;
      matrix = 0;
    }
    if (bipFromTimer) {
      lc.setRow(matrix, scanRow - 1, B00000000);
      lc.setRow(matrix, scanRow, B11111111);
      if (scanRow > 7) {
        if (matrix == 0) {
          matrix = 1;
          scanRow = 0;
          // au lieu de matrix = 1;
        } else {
          scanVerticalOk = false;
          scanVerticalStarted = false;
        }

      }
      scanRow++;
    }
    // lance au hasard la fonction targetIni
    if (random(300) == 0) {
      targetIni();
    }
  }

}
void playImage(byte matrixImage, const byte image[][8], byte image_len) {
  if (randomImageOk[matrixImage] && nbreBipFromTimer % 10 == 0) {
    //lc.clearDisplay(matrix);
    if (nbreFrame[matrixImage] < image_len ) {
      for (byte j = 0; j < 8; j++) {
        lc.setColumn(matrixImage, j, image[nbreFrame[matrixImage]][j]);
      }
      nbreFrame[matrixImage]++;
      delay(50);
    } else {
      nbreFrame[matrixImage] = 0;
      randomImageOk[matrixImage] = false;
      // scanOk = true;
    }
  }
}
void targetIni() {
  targetCoord[0] = matrix;
  targetCoord[2] = scanCol;// correspond à pos scan hori
  targetCoord[1] = scanRow;// correspond à scan vertical
  animOff();
  targetStartOk = true;
  randomImageOk[0] = true;
  randomImageOk[1] = true;
  nbreBipFromTimer = 0;
  while (nbreBipFromTimer < 30) {
    timerForScan();
    targetX();
    if (nbreBipFromTimer % 4 == 0) {
      lc.clearDisplay(0);
      lc.clearDisplay(0);
      delay(100);
    }
    if (targetCoord[0] == 0) {
      playImage(1, onetwo, onetwo_len);
    }
    else {
      playImage(0, onetwo, onetwo_len);
    }
  }
}
void targetStart() {

  if (bipFromTimer && targetStartOk) {
    targetX();
    targetPos();
    float outornot = nbreBipFromTimer % 300;
Serial.println(outornot);
    if (outornot == 0) {
      nbreBipFromTimer=0;
      while (nbreBipFromTimer < 40) {
        timerForScan();
        bug();
        randomImageOk[0]=true;
        randomImageOk[1]=true;
        playImage(1, onetwo, onetwo_len);
        playImage(0, onetwo, onetwo_len);
      }
      scanOk = true;
      scanVerticalOk = true;
      targetStartOk = false;
      nbreBipFromTimer = 0;
    }
    /*
      if (random(200) == 0) {
      scanOk = true;
      scanVerticalOk = true;
      targetStartOk = false;
      }
    */
  }
}
void targetPos() {
  targetCoord[1] = targetCoord[1] + random(-1, 2);
  targetCoord[2] = targetCoord[2] + random(-1, 2);
  // coordonné en Y
  if ( targetCoord[2] < 0) {
    targetCoord[2] = 0;
  }
  else if ( targetCoord[2] > 7) {
    targetCoord[2] = 7;
  };
  // coordonné en X
  //si target sur matrix 0
  if (targetCoord[0] == 0) {
    if (targetCoord[1] < 0) {
      targetCoord[1] = 0;
    }
    if (targetCoord[1] > 7) {
      lc.clearDisplay(targetCoord[0]);
      targetCoord[0] = 1;
      targetCoord[1] = 0;
    }
  }
  // si target sur matrix 1
  else {
    if (targetCoord[1] < 0) {
      lc.clearDisplay(targetCoord[0]);
      targetCoord[0] = 0;
      targetCoord[1] = 7;
    } else if (targetCoord[1] > 7) {
      targetCoord[1] = 7;
    }
  }
}

void targetX() {
  lc.clearDisplay(targetCoord[0]);
  for (int j = -2; j <= 2; j++) {
    lc.setLed(targetCoord[0], targetCoord[1] + j, targetCoord[2], HIGH);
    lc.setLed(targetCoord[0], targetCoord[1] , targetCoord[2] + j, HIGH);
  }
  lc.setLed(targetCoord[0], targetCoord[1], targetCoord[2], LOW);
  delay(20);
}
