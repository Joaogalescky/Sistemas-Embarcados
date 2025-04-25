// João Vitor Campõe Galescky

#define PIN_LED_1 11
#define PIN_LED_2 10
#define PIN_LED_3 9

#define INTERVALO_LED_1 350
#define INTERVALO_LED_2 2500
#define TEMPO_LED_3 5000

#define BOTAO 7

int estadoLed1 = LOW;
int estadoLed2 = LOW;
unsigned long tempoAnteriorLed1 = 0;
unsigned long tempoAnteriorLed2 = 0;
unsigned long tempoAtivacaoLed3 = 0;
bool led3Ativado = false;

void setup() {
  // to run once
  Serial.begin(9600);

  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  pinMode(PIN_LED_3, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);

  digitalWrite(PIN_LED_1, estadoLed1);
  digitalWrite(PIN_LED_2, estadoLed2);
  digitalWrite(PIN_LED_3, LOW);
}

void loop() {
  // to run repeatedly
  unsigned long tempoAtual = millis(); // milisegundos

  if (tempoAtual - tempoAnteriorLed1 >= INTERVALO_LED_1) {
    tempoAnteriorLed1 = tempoAtual;
    if (estadoLed1 == LOW) {
        estadoLed1 = HIGH;
    }  else {
        estadoLed1 = LOW;
    }
    digitalWrite(PIN_LED_1, estadoLed1);
  }

  if (tempoAtual - tempoAnteriorLed2 >= INTERVALO_LED_2) {
    tempoAnteriorLed2 = tempoAtual;
    if (estadoLed2 == LOW) {
        estadoLed2 = HIGH;
    }  else {
        estadoLed2 = LOW;
    }
    digitalWrite(PIN_LED_2, estadoLed2);
  }

  if (digitalRead(BOTAO) == LOW) {
    digitalWrite(PIN_LED_3, HIGH);
    led3Ativado = true;
    tempoAtivacaoLed3 = tempoAtual;
  }
  if (led3Ativado && (tempoAtual - tempoAtivacaoLed3 >= TEMPO_LED_3)) {
    digitalWrite(PIN_LED_3, LOW);
    led3Ativado = false;
  }
}
