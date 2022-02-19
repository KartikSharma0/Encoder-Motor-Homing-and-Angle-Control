#include <Arduino.h>
#include <Encoder.h>

void getPos();
void homing(int speed);
void goToAngle(int angle,int speed);

Encoder myEnc(2,3);
int motorpin1 = 4;
int motorpin2 = 5;
int motor1enA = 9;
long position  = 0;
long homePos = 0;
long newPos = 0;
bool homing_complete = false;

void setup() {
  Serial.begin(9600);
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
  pinMode(motor1enA, OUTPUT);
  Serial.println("Performing homing");
  homing(60);
  delay(2000);
  Serial.println("going to angle:180deg");
}

void loop() {
  goToAngle(180,60);
}

void getPos(){
  newPos = myEnc.read();
  if (newPos != position) {
  position = newPos;
  }
}

void homing(int speed){
  long laggingPos = 10000;
  long timePrev = 0;
  long tempPos = 0;
  while(homing_complete == false){
    long timeNow = millis();
    getPos();
    analogWrite(motor1enA, speed);
    digitalWrite(motorpin1, HIGH);
    digitalWrite(motorpin2, LOW);
    if(timeNow - timePrev >1500){
      timePrev = timeNow;
      laggingPos = tempPos;
      tempPos = position;
    }
    if(laggingPos==position){
      analogWrite(motorpin1,LOW);
      analogWrite(motorpin2,LOW);
      Serial.println("stalled");
      homePos = position;
      Serial.println("The home position is:");
      Serial.println(homePos);
      homing_complete = true;
    }
  }
}

void goToAngle(int angle, int speed){
  long target = 1.89 * angle;
  Serial.println("the target encoder count is:");
  Serial.println(target);
  Serial.println("the current encoder count is:");
  Serial.println(position - homePos);
  while(position - homePos > target){
    getPos();
    analogWrite(motor1enA, speed);
    digitalWrite(motorpin1, LOW);
    digitalWrite(motorpin2, HIGH);
  }
  while(position - homePos < target){
    getPos();
    analogWrite(motor1enA, speed);
    digitalWrite(motorpin1, HIGH);
    digitalWrite(motorpin2, LOW);
  }
  while(position - homePos == target){
    getPos();
    digitalWrite(motorpin1, LOW);
    digitalWrite(motorpin2, LOW);
  }
}