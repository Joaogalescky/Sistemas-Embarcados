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
const int PIN_POT = A0;  // analog pin used to connect the potentiometer
int value;    // variable to read the value from the analog pin

void setup() {
  Serial.begin(9600);
  myservo.attach(PIN_SERV);  // attaches the servo on pin 9 to the servo objec
}

void loop() {
  value = analogRead(PIN_POT);            // reads the value of the potentiometer (value between 0 and 1023)
  value = map(value, 0, 1023, 0, 180);     // scale it for use with the servo (value between 0 and 180)
  myservo.write(value);                  // sets the servo position according to the scaled value
  delay(20);                           // waits for the servo to get there
}
