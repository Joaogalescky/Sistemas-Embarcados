// João Vitor Campõe Galescky & Gabriel Albuquerque

#define PIN_LED 11
#define TEMPO 250

int estadoAtual = LOW;
unsigned long tempoAnterior = 0;
unsigned long intervalo = TEMPO;

void setup() {
  // to run once
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  int estadoAtual = LOW;
}

void loop() {
  // to run repeatedly
  unsigned long tempoAtual = millis(); // milisegundos
  
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    if (estadoAtual == LOW) {
      Serial.println("Sinal Forte");
      estadoAtual = HIGH;
    } else {
      Serial.println("Sinal Fraco");
      estadoAtual = LOW;
    }
    digitalWrite(PIN_LED, estadoAtual);
  }
}
