// João Vitor Campõe Galescky & Gabriel Albuquerque

// Faltou inserir o tempo com millis para quando não estiver ativado o botão e quando estiver

#define PIN_LED 13
const int analogInPin = A0;

int estadoLed = LOW;
int sensorValue = 0;
int outputValue = 0;

unsigned long tempoAnterior = 0;
unsigned long intervalo = 0;

void setup() {
  // to run once
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, estadoLed);
}

void loop() {
  // to run repeatedly
  unsigned long tempoAtual = millis(); // milisegundos
  
  sensorValue = analogRead(analogInPin);
  intervalo = map(sensorValue, 0, 1023, 100, 1000);

  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    // operario ternário
    estadoLed = (estadoLed == LOW) ? HIGH : LOW; // se sim, HIGH, se não LOW
    digitalWrite(PIN_LED, estadoLed);
  }
}
