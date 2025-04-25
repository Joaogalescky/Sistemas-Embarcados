// João Vitor Campõe Galescky & Gabriel Albuquerque

// Faltou inserir o tempo com millis para quando não estiver ativado o botão e quando estiver

#define PIN_LED 11
#define TEMPO_BOTAO_DESATIVADO 1000
#define TEMPO_BOTAO_ATIVADO 500
#define BOTAO 7

int estadoAtual = LOW;
int botaoEstado = 0;
unsigned long tempoAnterior = 0;
unsigned long intervalo = TEMPO_BOTAO_DESATIVADO;

void setup() {
  // to run once
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP)
  digitalWrite(PIN_LED, LOW);
  int estadoAtual = LOW;
}

void loop() {
  // to run repeatedly
  botaoEstado = digitalRead(BOTAO);
  unsigned long tempoAtual = millis(); // milisegundos
  
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    if (botaoEstado == LOW) {
      if (TEMPO_BOTAO_ATIVADO) {
        Serial.println("Botao Desativado");
        digitalWrite(PIN_LED, HIGH)
      }
    } else {
      if (TEMPO_BOTAO_DESATIVADO) {
          Serial.println("Botao Ativado");
          digitalWrite(PIN_LED, LOW) 
      }
    }
    digitalWrite(PIN_LED, estadoAtual);
  }
}
