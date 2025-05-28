#define PIN A0
#define LED 13

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  int value = analogRead(PIN);
  Serial.println(value);
  if (value >= 500) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  delay(300);
}
