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

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
