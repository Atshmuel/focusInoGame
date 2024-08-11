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

const int timeBuffer = 50;
const int maxDiff = 1000;

unsigned long startTime;
unsigned long endTime;

int currState;
int currectPress = 0;
int pressCounter = 0;

bool initProgram = true;
bool isWon;
bool restart = false;

const int btnsArr[ARR_LEN] = { greenBtn, redBtn, yellowBtn, blueBtn };
const int ledsArr[ARR_LEN] = { greenLed, redLed, yellowLed, blueLed };
const int tonesArr[ARR_LEN] = { NOTE_C4, NOTE_G3, NOTE_A3, NOTE_F2 };

int currValArr[ARR_LEN];
int lastValArr[ARR_LEN];
unsigned long lastPressTime[ARR_LEN];
int chosenIndexsArr[MAX_TIMES];
int pressedIndexs[MAX_TIMES];

//helpers
void btnAndLedSetup() {
  for (int i = 0; i < ARR_LEN; i++) {
    if (initProgram) {
      pinMode(btnsArr[i], INPUT_PULLUP);
      pinMode(ledsArr[i], OUTPUT);
    }
    lastValArr[i] = digitalRead(btnsArr[i]);
    lastPressTime[i] = millis();
    digitalWrite(ledsArr[i], LOW);  //making sure that all the leds are off
  }
}
void chosenIndexsSetup() {
  for (int i = 0; i < MAX_TIMES; i++) {
    chosenIndexsArr[i] = -1;
    pressedIndexs[i] = -1;
  }
}
void chooseRandomLeds() {
  int chosenNum;
  for (int i = 0; i < MAX_TIMES; i++) {
    chosenNum = random(0, ARR_LEN);
    if (!includes(chosenNum)) {
      chosenIndexsArr[i] = chosenNum;
    } else {
      i--;
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
  for (int i = 0; i < MAX_TIMES; i++) {
    ledOn(chosenIndexsArr[i]);
  }
  delay(maxDiff);
  for (int i = 0; i < MAX_TIMES; i++) {
    ledOff(chosenIndexsArr[i]);
  }
  delay(maxDiff / 2);
  playTone(0, maxDiff / 3);
}
int btnPressed(bool count) {
  int btnNum = -1;
  for (int i = 0; i < ARR_LEN; i++) {
    currValArr[i] = digitalRead(btnsArr[i]);
    if (!currValArr[i] && lastValArr[i] && (millis() - lastPressTime[i] > timeBuffer)) {
      lastPressTime[i] = millis();
      btnNum = i;
      if (count) {
        pressedIndexs[pressCounter++] = btnNum;
      }
    }
    lastValArr[i] = currValArr[i];
  }
  return btnNum;
}
bool includes(int num) {
  for (int i = 0; i < MAX_TIMES; i++) {
    if (chosenIndexsArr[i] == num) {
      return true;
    }
  }
  return false;
}
bool repeats() {
  for (int i = 0; i < MAX_TIMES; i++) {
    for (int k = i + 1; k < MAX_TIMES; k++) {
      if (pressedIndexs[i] == pressedIndexs[k])
        return true;
    }
  }
  return false;
}
//end of helpers

//game functions
void startGame() {
  btnAndLedSetup();
  chosenIndexsSetup();
  chooseRandomLeds();
  if (restart) { delay(maxDiff / 5); }
  showLights();
  currState = GAME_IS_ON;
  isWon = true;
  currectPress = 0;
  pressCounter = 0;
}
void gameIsOn() {
  int currBtnNum = btnPressed(true);
  if (currBtnNum != -1 && pressCounter == 1) {
    startTime = millis();
  }
  if (pressCounter == MAX_TIMES) {
    endTime = millis();
    if (!repeats()) {
      for (int i = 0; i < MAX_TIMES; i++) {
        currectPress += includes(pressedIndexs[i]);
      }
      if (endTime - startTime < maxDiff && currectPress == MAX_TIMES && isWon) {
        currState = WIN_GAME;
      }
      if (currectPress != MAX_TIMES || endTime - startTime > maxDiff) {
        isWon = false;
        currState = LOSE_GAME;
      }
    } else {
      isWon = false;
      currState = LOSE_GAME;
    }
  }
}
void winOrLose(bool win) {
  win ? ledOn(0) : ledOn(1);
  win ? playTone(1, 0) : playTone(2, 0);
  delay(timeBuffer);
  int currBtnNum = btnPressed(false);
  if (currBtnNum != -1) {
    win ? playTone(1, -1) : playTone(2, -1);
    restart = true;

    currState = START_GAME;
  }
}
//end of game functions

void setup() {
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(A1));
  currState = START_GAME;
}
void loop() {
  switch (currState) {
    case START_GAME:
      startGame();
      initProgram = false;
      break;
    case GAME_IS_ON:
      gameIsOn();
      break;
    case WIN_GAME:
      winOrLose(isWon);
      break;
    case LOSE_GAME:
      winOrLose(isWon);
      break;
  }
}
