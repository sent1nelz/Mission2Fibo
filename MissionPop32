  #include <IBusBM.h>
  #include <POP32.h>
  #include <Servo.h>

  #define ENA PB3
  #define IN1 PC14
  #define IN2 PC15

  #define M1 1
  #define M2 2
  #define M3 3
  #define M4 4

  Servo Servo;

  IBusBM ibus;

  void moveForward(int speed) { // เดินหน้า 
    speed = abs(constrain(speed, 0, 100));
    speed = map(speed, 20, 100, 0, 100);
    Serial.print(speed);
    motor(M1, speed);
    motor(M2, speed);
    motor(M3, speed);
    motor(M4, speed);
  }
  void moveBackward(int speed) { // ถอยหลัง
    speed = abs(constrain(speed, 0, 100));
    speed = map(speed, 20, 100, 0, 100);
    motor(M1, -speed);
    motor(M2, -speed);
    motor(M3, -speed);
    motor(M4, -speed);
  }

  void moveLeft(int speed) { // สไลด์ซ้าย
    speed = abs(constrain(speed, 0, 100));
    speed = map(speed, 20, 100, 0, 100);
    motor(M1, -speed);
    motor(M2, speed);
    motor(M3, -speed);
    motor(M4, speed);
  }

  void moveRight(int speed) { // สไลด์ขวา
    speed = abs(constrain(speed, 0, 100));
    speed = map(speed, 20, 100, 0, 100);
    motor(M1, speed);
    motor(M2, -speed);
    motor(M3, speed);
    motor(M4, -speed);
  }

  void stopmotors() {
    motor(M1, 0);
    motor(M2, 0);
    motor(M3, 0);
    motor(M4, 0);
  }


  void setup() {
    Serial.begin(115200);

    Serial1.begin(115200);
    ibus.begin(Serial1);
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    
    Servo.attach(PB11);
  }

  void loop() {
    
    ibus.loop(); 
    int ch1 = ibus.readChannel(1);
    // int ch2 = ibus.readChannel(2); 
    int ch3 = ibus.readChannel(3); 
    int ch4 = ibus.readChannel(4);
    int ch5 = ibus.readChannel(5);  
    // int ch6 = ibus.readChannel(6);

    int FB = map(ch1, 1000, 2000, -100, 100);
    int crane = map(ch4, 1000, 2000, -100, 100);
    int LR = map(ch3, 1000, 2000, -100, 100);
    int grip = map(ch5, 1000, 2000, -100, 100);

      if (crane > 90){ //  
      analogWrite(ENA, 255);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    }
    else if (crane < -90){  
      analogWrite(ENA, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
    else if (crane == 0){
      analogWrite(ENA, 255);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }

    if (grip <= 0){
      Servo.write(0); // ปล่อย
    }
    else {
      Servo.write(56); // คีบ
    }

    String direction = "Stop";

    if (FB > 20){direction = "Forward"; moveForward(FB);}
    else if (FB < -20){direction = "Backward"; moveBackward(abs(FB));}

    else if (LR > 20){direction = "Strafe Right"; moveRight(LR);}
    else if (LR < -20){direction = "Strafe Left"; moveLeft(abs(LR));}

    else {direction = "Stop"; stopmotors();}

    Serial.print("X: "); Serial.print(FB);
    Serial.print(" | Y: "); Serial.print(LR);
    Serial.print(" | switch: "); Serial.print(grip);
    Serial.print(" | crane : "); Serial.print(crane);
    Serial.print(" ---> "); Serial.println(direction);

    // Serial.print("ch1 : "); Serial.print(ch1);
    // Serial.print("  ch2 : "); Serial.print(ch2);
    // Serial.print("  ch3 : "); Serial.print(ch3);
    // Serial.print("  ch4 : "); Serial.print(ch4);
    // Serial.print("  ch5 : "); Serial.print(ch5);
    // Serial.print("  ch6 : "); Serial.print(ch6);
    // Serial.print("\n ");

  }
