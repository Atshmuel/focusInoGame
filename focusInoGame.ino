#include "pitches.h"

#define ARR_LEN 4
#define MAX_TIMES 3

#define greenLed 10
#define yellowLed 9
#define redLed 11
#define blueLed 6

#define greenBtn 2
#define yellowBtn 3
#define redBtn 4
#define blueBtn 12

#define buzzerPin 5

#define START_GAME 42
#define GAME_IS_ON 43
#define WIN_GAME 44
#define LOSE_GAME 45

const int timeBuffer = 150;
const int maxDiff = 1000;

unsigned long startTime;
unsigned long endTime;

int i = 0;
int currState;
int correctPress = 0;
int pressCounter = 0;

const int btnsArr[ARR_LEN] = { greenBtn, redBtn, yellowBtn, blueBtn };
const int ledsArr[ARR_LEN] = { greenLed, redLed, yellowLed, blueLed };
const int tonesArr[ARR_LEN] = { NOTE_C4, NOTE_G3, NOTE_A3, NOTE_F2 };

int currValArr[ARR_LEN];
int lastValArr[ARR_LEN];
unsigned long lastPressTime[ARR_LEN];

bool chosenIndexsArr[ARR_LEN];
bool pressedIndexs[ARR_LEN];


//helpers
void btnAndLedSetup() {
  for (i = 0; i < ARR_LEN; i++) {
    if (millis() == 0) {
      pinMode(btnsArr[i], INPUT_PULLUP);
      pinMode(ledsArr[i], OUTPUT);
    }
    lastValArr[i] = digitalRead(btnsArr[i]);
    lastPressTime[i] = millis();
    digitalWrite(ledsArr[i], LOW);  //making sure that all the leds are off
  }
}
void chosenIndexsSetup() {
  for (i = 0; i < ARR_LEN; i++) {
    chosenIndexsArr[i] = false;
    pressedIndexs[i] = false;
  }
}
void chooseRandomLeds() {
  int chosenNum;
  i = 0;
  while (i < MAX_TIMES) {
    chosenNum = random(0, ARR_LEN);
    if (!chosenIndexsArr[chosenNum]) {
      chosenIndexsArr[chosenNum] = true;
      i++;
    }
  }
}
void playTone(int num, int time) {
  if (time == -1) {
    noTone(buzzerPin);
  }
  if (time == 0) {
    tone(buzzerPin, tonesArr[num]);
  }

  if (time > 0) {
    tone(buzzerPin, tonesArr[num], time);
  }
}
void ledOn(int num) {
  digitalWrite(ledsArr[num], HIGH);
}
void ledOff(int num) {
  digitalWrite(ledsArr[num], LOW);
}
void showLights() {
  for (i = 0; i < ARR_LEN; i++) {
    if (chosenIndexsArr[i]) ledOn(i);
  }
  delay(maxDiff);
  for (i = 0; i < ARR_LEN; i++) {
    if (chosenIndexsArr[i]) ledOff(i);
  }
  delay(maxDiff / 2);
  playTone(0, maxDiff / 3);
}

int btnPressed() {
  int btnNum = -1;
  for (i = 0; i < ARR_LEN; i++) {
    currValArr[i] = digitalRead(btnsArr[i]);
    if (!currValArr[i] && lastValArr[i] && (millis() - lastPressTime[i] > timeBuffer)) {
      lastPressTime[i] = millis();
      btnNum = i;
      if (currState == GAME_IS_ON && pressCounter < 3) {
      if (pressCounter == 1) {
        startTime = millis();
      }
        pressedIndexs[btnNum] = true;
        pressCounter++;
      }
    }
    lastValArr[i] = currValArr[i];
  }
  return btnNum;
}
//end of helpers

//game functions
void startGame() {
  correctPress = 0;
  pressCounter = 0;
  btnAndLedSetup();
  chosenIndexsSetup();
  chooseRandomLeds();
  if (millis() != 0) { delay(maxDiff / 5); }
  showLights();
  currState = GAME_IS_ON;

}
void gameIsOn() {
  int currBtnNum = btnPressed();
  if (pressCounter == MAX_TIMES) {
    endTime = millis();
    for (i = 0; i < ARR_LEN; i++) {
      correctPress += pressedIndexs[i] && chosenIndexsArr[i] ? 1 : 0;
    }
    if (endTime - startTime < maxDiff && correctPress == MAX_TIMES) {
      currState = WIN_GAME;
    }
    if (correctPress != MAX_TIMES || endTime - startTime > maxDiff) {
      currState = LOSE_GAME;
    }
  }
}
void winOrLose(bool win) {
  win ? ledOn(0) : ledOn(1);
  win ? playTone(1, 0) : playTone(2, 0);
  delay(timeBuffer);
  int currBtnNum = btnPressed();
  if (currBtnNum != -1) {
    win ? playTone(1, -1) : playTone(2, -1);
    currState = START_GAME;
  }
}
//end of game functions

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(A1));
  currState = START_GAME;
}
void loop() {
  switch (currState) {
    case START_GAME:
      startGame();
      break;
    case GAME_IS_ON:
      gameIsOn();
      break;
    case WIN_GAME:
      winOrLose(true);
      break;
    case LOSE_GAME:
      winOrLose(false);
      break;
  }
}
