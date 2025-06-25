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

  int value;  // variable to read the value from the analog pin

void setup() {
  Serial.begin(9600);
  pinMode(PIN_B1, INPUT_PULLUP);
  pinMode(PIN_B2, INPUT_PULLUP);
  myservo.attach(PIN_SERV);
}

void loop() {
  if (digitalRead(PIN_B1) == false) {
    myservo.write(45);
    delay(200);
    myservo.write(90);
    delay(200);
  } else {
    myservo.write(90);
  }
  if (digitalRead(PIN_B2) == false) {
    myservo.write(135);
    delay(100);
    myservo.write(90);
    delay(100);
  }
  else {
    myservo.write(90);
  }
  value = map(value, 0, 1023, 0, 180);  // scale it for use with the servo (value between 0 and 180)
  myservo.write(value);                 // sets the servo position according to the scaled value
  delay(20);                            // waits for the servo to get there
}