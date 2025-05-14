#define PIN_LED_R 10
#define PIN_LED_G 11
#define PIN_LED_B 12

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    switch(Serial.read())
    {
      case 'R':
        digitalWrite(PIN_LED_R, HIGH);
        break;
      case 'r':
        digitalWrite(PIN_LED_R, LOW);
        break;
      case 'A':
        digitalWrite(PIN_LED_B, HIGH);
        break;
      case 'a':
        digitalWrite(PIN_LED_B, LOW);
        break;
      case 'G':
        digitalWrite(PIN_LED_G, !digitalRead(PIN_LED_G));
        break;
    }
  }
}
