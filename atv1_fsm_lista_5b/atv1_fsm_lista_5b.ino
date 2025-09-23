/* Descrição
  1. Controle de Acesso de Portaria com Duas Portas usando FSM e Arduino

  Desenvolver um sistema de controle de acesso para uma portaria que possui duas
  portas sequenciais usando uma Máquina de Estados Finitos (FSM) implementada no
  Arduino.
*/

// ? Ver sobre a questão do Debounce dos botões/sensores
// ? E se os 2 botões das portas forem pressionados ao mesmo tempo. O'que fazer ?
// ? E se cair a energia? Deve travar todas as portas!

// Pinos
// Botões
#define PIN_BTN_P1 2
#define PIN_BTN_P2 3
#define PIN_SENSOR_P1 4
#define PIN_SENSOR_P2 5
// Eletroimãs | LEDS
#define PIN_TRAVA_P1 6
#define PIN_TRAVA_P2 7

// Estados
enum Estado
{
  S_PORTAS_TRANCADAS,
  S_P1_DESTRANCADA,
  S_P1_ABERTO,
  S_P2_DESTRANCADA,
  S_P2_ABERTO
};

Estado estadoAtual = S_PORTAS_TRANCADAS;
Estado estadoAnterior = S_PORTAS_TRANCADAS; // para detectar mudanças

// Tempo
unsigned long tempoDestravado = 0;
const unsigned long TIMEOUT = 10000; // 10 segundos

void setup()
{
  // put your setup code here, to run once:
  pinMode(PIN_BTN_P1, INPUT_PULLUP);
  pinMode(PIN_BTN_P2, INPUT_PULLUP);
  pinMode(PIN_SENSOR_P1, INPUT_PULLUP);
  pinMode(PIN_SENSOR_P2, INPUT_PULLUP);
  pinMode(PIN_TRAVA_P1, OUTPUT); // HIGH = travado / LOW = destravado
  pinMode(PIN_TRAVA_P2, OUTPUT);
  Serial.begin(9600);
  atualizarSaidas();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // Inverte a ordem do INPUT_PULLUP: FALSE quando solto, TRUE quando ativo
  bool btn_porta1 = !digitalRead(PIN_BTN_P1);
  bool btn_porta2 = !digitalRead(PIN_BTN_P2);
  bool porta1_aberta = !digitalRead(PIN_SENSOR_P1);
  bool porta2_aberta = !digitalRead(PIN_SENSOR_P2);

  switch (estadoAtual)
  {
  // Ambas as portas trancadas
  case S_PORTAS_TRANCADAS:
    if (btn_porta1)
    {
      estadoAtual = S_P1_DESTRANCADA;
      tempoDestravado = millis();
    }
    else if (btn_porta2)
    {
      estadoAtual = S_P2_DESTRANCADA;
      tempoDestravado = millis();
    }
    break;
  // Se 1º porta destrancada
  case S_P1_DESTRANCADA:
    if (porta1_aberta)
    {
      estadoAtual = S_P1_ABERTO;
    }
    // Se tempo atual - tempo de quando foi destrava for >= ao limite de tempo
    else if (millis() - tempoDestravado >= TIMEOUT)
    {
      estadoAtual = S_PORTAS_TRANCADAS;
    }
    break;
  // Se 1º porta aberta
  case S_P1_ABERTO:
    if (!porta1_aberta)
    {
      estadoAtual = S_PORTAS_TRANCADAS;
    }
    break;
  // Se 2º porta destrancada
  case S_P2_DESTRANCADA:
    if (porta2_aberta)
    {
      estadoAtual = S_P2_ABERTO;
    }
    else if (millis() - tempoDestravado >= TIMEOUT)
    {
      estadoAtual = S_PORTAS_TRANCADAS;
    }
    break;
  // Se 2º porta aberta
  case S_P2_ABERTO:
    if (!porta2_aberta)
    {
      estadoAtual = S_PORTAS_TRANCADAS;
    }
    break;
  }
  atualizarSaidas();
}

void printEstado(Estado estado)
{
  switch (estado)
  {
  case S_PORTAS_TRANCADAS:
    Serial.print("PORTAS_TRANCADAS");
    break;
  case S_P1_DESTRANCADA:
    Serial.print("P1_DESTRANCADA");
    break;
  case S_P1_ABERTO:
    Serial.print("P1_ABERTO");
    break;
  case S_P2_DESTRANCADA:
    Serial.print("P2_DESTRANCADA");
    break;
  case S_P2_ABERTO:
    Serial.print("P2_ABERTO");
    break;
  }
}

void atualizarSaidas()
{
  // Imprime no terminal apenas quando muda de estado
  if (estadoAtual != estadoAnterior)
  {
    Serial.print("Estado anterior: ");
    printEstado(estadoAnterior);
    Serial.print(" -> ");
    printEstado(estadoAtual);
    Serial.println();
    estadoAnterior = estadoAtual; // Atualiza o estado anterior
  }

  switch (estadoAtual)
  {
  case S_PORTAS_TRANCADAS:
    digitalWrite(PIN_TRAVA_P1, HIGH);
    digitalWrite(PIN_TRAVA_P2, HIGH);
    break;
  case S_P1_DESTRANCADA:
    digitalWrite(PIN_TRAVA_P1, LOW);
    digitalWrite(PIN_TRAVA_P2, HIGH);
    break;
  case S_P1_ABERTO:
    digitalWrite(PIN_TRAVA_P1, LOW);
    digitalWrite(PIN_TRAVA_P2, HIGH);
    break;
  case S_P2_DESTRANCADA:
    digitalWrite(PIN_TRAVA_P1, HIGH);
    digitalWrite(PIN_TRAVA_P2, LOW);
    break;
  case S_P2_ABERTO:
    digitalWrite(PIN_TRAVA_P1, HIGH);
    digitalWrite(PIN_TRAVA_P2, LOW);
    break;
  }
}

// Para passar o nome do estado
const char *nomeEstado(Estado estado)
{
  switch (estado)
  {
  case S_PORTAS_TRANCADAS:
    return "S_PORTAS_TRANCADAS";
  case S_P1_DESTRANCADA:
    return "S_P1_DESTRANCADA";
  case S_P1_ABERTO:
    return "S_P1_ABERTO";
  case S_P2_DESTRANCADA:
    return "S_P2_DESTRANCADA";
  case S_P2_ABERTO:
    return "S_P2_ABERTO";
  default:
    return "Estado desconhecido";
  }
}