// João Vitor Campõe Galescky & Gabriel Albuquerque

#define PIN_LED_R 11
#define PIN_LED_G 10
#define PIN_LED_B 9

char qLed = 0;
int potValue = 0;
char intensidade = 0;

void setup() {
  // to run once
  Serial.begin(9600);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);
}

void loop() {
  if (Serial.available()) {
    qLed = Serial.read();
    if (qLed == 'R' || qLed == 'G' || qLed == 'B') {
      while (!Serial.available());
      intensidade = Serial.read();
      if (intensidade >= '0' && intensidade <= '9') {
        potValue = intensidade - '0';
        potValue = map(potValue, 0, 9, 0, 255);
        switch (qLed) {
          case 'R':
            analogWrite(PIN_LED_R, potValue);
            break;
          case 'G':
            analogWrite(PIN_LED_G, potValue);
            break;
          case 'B':
            analogWrite(PIN_LED_B, potValue);
            break;
        }
      }
    }
  }
}