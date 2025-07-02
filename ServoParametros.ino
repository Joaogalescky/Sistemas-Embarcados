// João Galescky & Gabriel Albuquerque
/* Questão
Faça uma função para movimentar o servo, passando 3 parâmetros: posição inicial, posição
final, tempo de deslocamento (considere a variação mínima de 1 grau).
void moveServo(int posInicial, int posFinal, long tempoMillis){}
*/

#include <Servo.h>

Servo myservo;

const int PIN_SERV = 3;

void setup() {
  Serial.begin(9600);
  myservo.attach(PIN_SERV);
  myservo.write(0);
}

void loop() {
  moveServo(10, 120, 5000);
  delay(2000);
  moveServo(120, 10, 5000);
  delay(2000);
}

void moveServo(int posInicial, int posFinal, long tempoMillis) {
  int diferenca;

  // dá para usar => int diferenca = abs(posFinal - posInicial);
  if (posInicial > posFinal) {
    diferenca = posInicial - posFinal;
  } else {
    diferenca = posFinal - posInicial;
  }

  int grauPorDelay = tempoMillis / diferenca;

  // avançar, caso a = 10 e b = 50
  if (posInicial < posFinal) {
    for (int i = posInicial; i <= posFinal; i++){
      myservo.write(i);
      delay(grauPorDelay);
    }
  // retroceder, caso a = 50 e b = 10
  } else {
    for (int i = posInicial; i >= posFinal; i--){
      myservo.write(i);
      delay(grauPorDelay);
    }
  }
}