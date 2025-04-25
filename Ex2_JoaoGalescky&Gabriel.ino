// João Vitor Campõe Galescky & Gabriel Albuquerque

// Faltou inserir o tempo com millis para quando não estiver ativado o botão e quando estiver

#define PIN_LED 11
#define TEMPO_BOTAO_DESATIVADO 1000
#define TEMPO_BOTAO_ATIVADO 500
#define BOTAO 7

int estadoLed = LOW;
int botaoEstado;
unsigned long tempoAnterior = 0;
unsigned long intervalo = TEMPO_BOTAO_DESATIVADO;

void setup() {
  // to run once
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  digitalWrite(PIN_LED, estadoLed);
}

void loop() {
  // to run repeatedly
  botaoEstado = digitalRead(BOTAO);
  unsigned long tempoAtual = millis(); // milisegundos
  
  if (botaoEstado == LOW) { // pullup
    intervalo = TEMPO_BOTAO_ATIVADO;
  } else {
    intervalo = TEMPO_BOTAO_DESATIVADO;
  }

  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    // operario ternário
    estadoLed = (estadoLed == LOW) ? HIGH : LOW; // se sim, HIGH, se não LOW
    digitalWrite(PIN_LED, estadoLed)

    // estado
    if (botaoEstado == LOW) {
      Serial.println("Botao pressionado - 500ms")
    } else {
      Serial.println("Botao solto - 1000ms")
    }
  }
}
