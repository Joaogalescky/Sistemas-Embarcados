#define PIN_LED_VERDE 13
#define PIN_LED_AMARELO 12
#define PIN_LED_VERMELHO 11

#define TEMPO_VERDE 2000
#define TEMPO_AMARELO 1000
#define TEMPO_VERMELHO 3000

int estadoAtual = 0; // 0 = verde, 1 = amarelo, 2 = vermelho
unsigned long tempoAnterior = 0;
unsigned long intervalo = TEMPO_VERDE;

void setup() {
  // to run once
  Serial.begin(9600);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_AMARELO, OUTPUT);
  pinMode(PIN_LED_VERMELHO, OUTPUT);

  digitalWrite(PIN_LED_VERDE, LOW);
  digitalWrite(PIN_LED_AMARELO, LOW);
  digitalWrite(PIN_LED_VERMELHO, LOW);
}

void loop() {
  // to run repeatedly
  unsigned long tempoAtual = millis(); // milisegundos
  
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    // Controla o LED
    estadoAtual = (estadoAtual + 1) % 3; // Em 0, ele volta
    
    switch (estadoAtual) {
      case 0:
        Serial.println("Sinal Verde");
        digitalWrite(PIN_LED_VERDE, HIGH);
        digitalWrite(PIN_LED_AMARELO, LOW);
        digitalWrite(PIN_LED_VERMELHO, LOW);
        delay(TEMPO_VERDE);
        break;
      case 1:
        Serial.println("Sinal Amarelo");
        digitalWrite(PIN_LED_VERDE, LOW);
        digitalWrite(PIN_LED_AMARELO, HIGH);
        delay(TEMPO_AMARELO);
        break;
      case 2:
        Serial.println("Sinal Vermelho");
        digitalWrite(PIN_LED_AMARELO, LOW);
        digitalWrite(PIN_LED_VERMELHO, HIGH)
          delay(TEMPO_VERMELHO);
        break;
    }
  }
}
