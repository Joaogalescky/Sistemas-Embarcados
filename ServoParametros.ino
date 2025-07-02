#include <Servo.h>

Servo myservo;

const int PIN_SERV = 3;
int tempoExecucao = 0;
int diferenca;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(PIN_SERV);
  myservo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  moveServo(10, 120, 5000)
}

void moveServo(int posInicial, int posFinal, long tempoMillis) {
  unsigned long tempo_atual = millis();

  if (posFinal < posInicial) {
    maior = posFinal;
    posFinal = posInicial;
    posInicial = maior;
  }
  diferenca = posFinal

  tempoExecucao /= ;

  for (int i = posInicial; i <= posFinal; i++){
    myservo.write(i);
    delay(tempoMillis);
  }
}