// João Galescky

const int PIN_LED = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  Serial.println("Escolha uma quantidade de vezes para acender o LED:");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    int quantidade = Serial.parseInt();
    Serial.println("Piscando " + String(quantidade) + " de vezes...");

    int i = 0;
    while (i < quantidade) {
      digitalWrite(PIN_LED, HIGH);
      delay(500);
      digitalWrite(PIN_LED, LOW);
      delay(500);
      i++;
    }
    Serial.println("Digite outro número:");
  }
}
