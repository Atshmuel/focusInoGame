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

const int timeBuffer = 150;
const int maxDiff = 1000;

unsigned long startTime;
unsigned long endTime;

int currState;
int currectPress = 0;
int pressCounter = 0;

bool initProgram = true;
bool isWon;
bool restart=false;

const int btnsArr[ARR_LEN] = { greenBtn, redBtn, yellowBtn, blueBtn };
const int ledsArr[ARR_LEN] = { greenLed, redLed, yellowLed, blueLed };
const int tonesArr[ARR_LEN] = { NOTE_C4, NOTE_G3, NOTE_A3, NOTE_F2 };

int currValArr[ARR_LEN];
int lastValArr[ARR_LEN];
unsigned long lastPressTime[ARR_LEN];
int chosenIndexsArr[MAX_TIMES];
int pressedIndexs[MAX_TIMES];

//helpers
void btnsSetup() {
  for (int i = 0; i < ARR_LEN; i++) {
    if (initProgram) {
      pinMode(btnsArr[i], INPUT_PULLUP);
    }
    lastValArr[i] = digitalRead(btnsArr[i]);
    lastPressTime[i] = millis();
  }
}
void ledsSetup() {
  for (int i = 0; i < ARR_LEN; i++) {
    if (initProgram) {
      pinMode(ledsArr[i], OUTPUT);
    }
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
int btnPressed() {
  int btnNum = -1;
  for (int i = 0; i < ARR_LEN; i++) {
    currValArr[i] = digitalRead(btnsArr[i]);
    if (!currValArr[i] && lastValArr[i] && (millis() - lastPressTime[i] > timeBuffer)) {
      lastPressTime[i] = millis();
      btnNum = i;
      pressedIndexs[pressCounter++] = btnNum;
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
//end of helpers

//game functions
void startGame() {
  btnsSetup();
  ledsSetup();
  chosenIndexsSetup();
  chooseRandomLeds();
  if (restart) { delay(maxDiff/3); }
  showLights();
  currState = GAME_IS_ON;
  isWon = true;
  currectPress = 0;
  pressCounter = 0;
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
      winGame();
      break;
    case LOSE_GAME:
      loseGame();
      break;
  }
}



