#define PIN_PWM A0
#define LED 8

int potValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  int value = analogRead(PIN_PWM);
  Serial.println(value);
  potValue = map(value, 800, 50, 255, 0);
  analogWrite(LED, potValue);
  delay(30);
}
