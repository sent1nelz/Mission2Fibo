#include <Servo.h>

#define ENA 3 // left Side Motor
#define IN1 1
#define IN2 2

#define ENB 6  // Right Side Motor
#define IN3 4
#define IN4 5

#define CENA 9 // motor crane
#define CIN1 8
#define CIN2 7

#define dPIN 10 // servo

Servo Servo;

uint16_t ibusChannels[14];
uint8_t ibusBuffer[32];
uint8_t ibusIndex = 0;

void decodeIBUS() {
  for (int ch = 0; ch < 14; ch++) {
    uint16_t low = ibusBuffer[2 + ch * 2];
    uint16_t high = ibusBuffer[3 + ch * 2];
    ibusChannels[ch] = low | (high << 8);
  }
}

uint16_t readChannel(int ch) {
  return ibusChannels[ch];
}

void motor(int channel, int power) {
  int pwmPin, inPinA, inPinB;

  if (channel == 1) {
    pwmPin = ENA;
    inPinA = IN1;
    inPinB = IN2;
  } else if (channel == 2) {
    pwmPin = ENB;
    inPinA = IN4;
    inPinB = IN3;
  }

  if (power > 100) power = 100;
  if (power < -100) power = -100;

  int speedPWM = map(abs(power), 0, 100, 0, 255);

    if (power > 0) {
    digitalWrite(inPinA, HIGH);
    digitalWrite(inPinB, LOW);
    analogWrite(pwmPin, speedPWM);
  } 
  else if (power < 0) {
    digitalWrite(inPinA, LOW);
    digitalWrite(inPinB, HIGH);
    analogWrite(pwmPin, speedPWM);
  } 
  else {
    digitalWrite(inPinA, LOW);
    digitalWrite(inPinB, LOW);
    analogWrite(pwmPin, 0);
  }
}

void moveForward(int speed) {
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(1, speed);
  motor(2, speed);
}
void moveBackward(int speed) {
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(1, -speed);
  motor(2, -speed);
}

void moveLeft(int speed) {
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(1, -speed);
  motor(2, speed);
}

void moveRight(int speed) {
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(1, speed);
  motor(2, -speed);
}

void stopmotors() {
  motor(1, 0);
  motor(2, 0);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(CENA, OUTPUT);
  pinMode(CIN1, OUTPUT);
  pinMode(CIN2, OUTPUT);
  
  Servo.attach(dPIN);
}

void loop() {
  while (Serial1.available()) {
    uint8_t b = Serial1.read();

    if (ibusIndex == 0 && b != 0x20) continue; // frame size check
    ibusBuffer[ibusIndex++] = b;

    if (ibusIndex == 32) {
      decodeIBUS();
      ibusIndex = 0;
    }
  }

  int ch1 = readChannel(1);
  // int ch2 = readChannel(2);
  int ch3 = readChannel(3);
  int ch4 = readChannel(4);
  int ch5 = readChannel(5);
  // int ch6 = readChannel(6);

  int FB = map(ch1, 1000, 2000, -100, 100);
  int crane = map(ch4, 1000, 2000, -100, 100);
  int LR = map(ch3, 1000, 2000, -100, 100);
  int grip = map(ch5, 1000, 2000, -100, 100);

  String craneStatus = "OFF";

  if (crane > 90){
    craneStatus = "UP";
    analogWrite(CENA, 255);
    digitalWrite(CIN1, HIGH);
    digitalWrite(CIN2, LOW);
  }
  else if (crane < -90){
    craneStatus = "OFF";
    analogWrite(CENA, 0);
    digitalWrite(CIN1, LOW);
    digitalWrite(CIN2, HIGH);
  }
  else if (crane == 0){
    craneStatus = "Down";
    analogWrite(CENA, 255);
    digitalWrite(CIN1, LOW);
    digitalWrite(CIN2, HIGH);
  }

  String gripStatus = "OPEN";

  if (grip <= 0){
    gripStatus = "OPEN";
    Servo.write(0);
  }
  else {
    gripStatus = "ClOSE";
    Servo.write(56);
  }

  String direction = "Stop";

  if (FB > 20){direction = "Forward"; moveForward(FB);}
  else if (FB < -20){direction = "Backward"; moveBackward(abs(FB));}
  else if (LR > 20){direction = "Right"; moveRight(LR);}
  else if (LR < -20){direction = "Left"; moveLeft(abs(LR));}
  else {direction = "Stop"; stopmotors();}

  Serial.print("X: "); Serial.print(FB);
  Serial.print(" | Y: "); Serial.print(LR);
  Serial.print(" ---> "); Serial.print(direction);
  Serial.print(" | GripStaus: "); Serial.print(gripStatus);
  Serial.print(" | GripVal: "); Serial.print(grip);
  Serial.print(" | CraneStatus : "); Serial.print(craneStatus);
  Serial.print(" | CraneVal : "); Serial.println(crane);

    // Serial.print("ch1 : "); Serial.print(ch1);
    // Serial.print("  ch2 : "); Serial.print(ch2);
    // Serial.print("  ch3 : "); Serial.print(ch3);
    // Serial.print("  ch4 : "); Serial.print(ch4);
    // Serial.print("  ch5 : "); Serial.print(ch5);
    // Serial.print("  ch6 : "); Serial.print(ch6);
    // Serial.print("\n ");
}
