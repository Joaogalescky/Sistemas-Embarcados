/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

const int PIN_SERV = 3;
const int PIN_B1 = 13;
const int PIN_B2 = 12;
const int pos45 = 45;
const int pos135 = 135;

void setup() {
  Serial.begin(9600);
  myservo.attach(PIN_SERV);
  pinMode(PIN_B1, INPUT_PULLUP);
  pinMode(PIN_B2, INPUT_PULLUP);
  myservo.write(pos45);
}

void loop() {
  if (digitalRead(PIN_B1) == false) {
    myservo.write(pos45);
    delay(500);
    myservo.write(pos135);
    delay(500);
  }
  if (digitalRead(PIN_B2) == false) {
    myservo.write(pos135);
    delay(1000);
    myservo.write(pos45);
    delay(1000);
  }
}