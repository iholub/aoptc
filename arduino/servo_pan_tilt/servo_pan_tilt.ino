//#include <SoftwareSerial.h>
#include <Servo.h>
String command = ""; // Stores response of the HC-06 Bluetooth device
String cmdStr = "";
boolean parseSuccess = false;

const int hServoPin = 9; // Servo library disables analogWrite() (PWM) functionality on pins 9 and 10 
const int vServoPin = 10; // Servo library disables analogWrite() (PWM) functionality on pins 9 and 10

const int bufLength = 20;
char inData[bufLength]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
boolean readSuccess = false;
boolean readBufferOverflow = false;
boolean clearBuf = false;
boolean cmdUpdateServoH = false;
boolean cmdUpdateServoV = false;

Servo servoH;          // horizontal servo
Servo servoV;         // vertical servo

const int servoCenter = 90;
const int servoMin = 10;
const int servoMax = 160;

int hPos = servoCenter;
int vPos = servoCenter;

char hServoDir;
char vServoDir;

char actionCommand;

static char outstr[15];

void setup() {
  Serial.begin(115200);
  //Serial.begin(9600);
  servoH.attach(hServoPin);  // set horizontal to digital pin 10
  servoV.attach(vServoPin);  // set vertical servo to digital pin 11

}

void loop() {
  readSuccess = false;
  parseSuccess = false;
  cmdUpdateServoH = false;
  cmdUpdateServoV = false;

  readCommand();

  if (!readSuccess) {
    return;
  }
  boolean statOk = false;
  if (!readBufferOverflow) {
    parseCommand();
    if (parseSuccess) {
      statOk = true;
      if (cmdUpdateServoH) {
        updateServo(servoH, hServoDir, &hPos);
      }
      if (cmdUpdateServoV) {
        updateServo(servoV, vServoDir, &vPos);
      }
    }
  }

}

void readCommand() {
  while (Serial.available() > 0) {
    inChar = Serial.read(); // Read a character
    if (inChar == 'z') {
      inData[index] = '\0'; // Null terminate the string
      readSuccess = true;
      readBufferOverflow = false;
      cmdStr = String(inData);
      doClearBuf();
      break;
    }

    inData[index] = inChar; // Store it
    index++; // Increment where to write next
    if (index >= bufLength - 1) {
      inData[index] = '\0'; // Null terminate the string
      readSuccess = true;
      readBufferOverflow = true;
      cmdStr = String(inData);
      doClearBuf();
      break;
    }
  }
}

void doClearBuf() {
  for (int i = 0;i < bufLength - 1; i++) {
    inData[i] = 0;
  }
  index=0;
  clearBuf = false;
}

void parseCommand() {
  int strLen = cmdStr.length();
  int pos = 0;
  boolean err = false;
  while (pos <= strLen - 1) {
    actionCommand = cmdStr.charAt(pos);
    int cmdLength = 0;
    if (actionCommand == 'h') {
      cmdLength = 1;
      if (pos + cmdLength <= strLen - 1) {
        cmdUpdateServoH = true;
        parseServoCommandH(pos);
      } 
      else {
        err = true;
        break;
      }
    } 
    else
      if (actionCommand == 'v') {
        cmdLength = 1;
        if (pos + cmdLength <= strLen - 1) {
          cmdUpdateServoV = true;
          parseServoCommandV(pos);
        } 
        else {
          err = true;
          break;
        }
      }
      else {
        err = true;
        break;
      }
    pos = pos + 1 + cmdLength;
  }

  parseSuccess = !err;
}

void parseServoCommandH(int pos) {
  hServoDir = cmdStr.charAt(pos + 1);
}

void parseServoCommandV(int pos) {
  vServoDir = cmdStr.charAt(pos + 1);
}

void updateServo(Servo servo, char dir, int * pos) {
  int inc;
  switch (dir) {
  case '0':
    inc = -10;
    break;
  case '1':
    inc = 10;
    break;
  }
  int newVal = *pos + inc;
  if (newVal >= servoMin && newVal <= servoMax) {
    *pos = *pos + inc;
  }
  servo.write(*pos);
}


