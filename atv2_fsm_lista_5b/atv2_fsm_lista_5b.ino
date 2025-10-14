/* Descrição
  2. Controle de Portão Eletrônico com Reversão de Movimento Portas usando FSM e
  Arduino

  Desenvolver um sistema de controle para um portão eletrônico, implementando
  uma Máquina de Estados Finitos (FSM) no Arduino que gerencia a lógica de operação
  cíclica e a reversão de movimento.
*/

/* Pinos
  - PIN_BTN: botão de controle (entrada com pullup)
  - PIN_BTN_SENSOR_ABERTO: sensor "totalmente aberto" (entrada com pullup)
  - PIN_BTN_SENSOR_FECHADO: sensor "totalmente fechado" (entrada com pullup)
  - PIN_LED_ABERTO: led indicando que está abrindo
  - PIN_LED_FECHADO: led indicando que está fechando
  - PIN_LED_ALERTA: led de alerta que pisca quando em movimento
*/

// Pinos
#define PIN_BTN 2
#define PIN_BTN_SENSOR_ABERTO 3
#define PIN_BTN_SENSOR_FECHADO 4
#define PIN_LED_ABERTO 10
#define PIN_LED_FECHADO 9
#define PIN_LED_ALERTA 8

// Variáveis
const unsigned long DEBOUNCE = 50;
const unsigned long TIMEOUT = 30000;
const unsigned long TEMPO_ALERTA = 500;

unsigned long ultimoDebounce = 0;
int ultimaLeituraBtn = HIGH;
int ultimaEstadoBtn = HIGH;
bool eventoBtn = false;

unsigned long inicioMovimento = 0;
unsigned long ultimoAlerta = 0;
bool alertaLed = false;

enum Estado
{
  EST_PARADO,
  EST_ABRINDO,
  EST_FECHANDO,
  EST_INTERROMPIDO
};

Estado estado = EST_PARADO;
Estado estadoAnterior = estado;

enum Direcao
{
  DIR_NENHUM,
  DIR_ABRINDO,
  DIR_FECHANDO
};

Direcao direcao = DIR_NENHUM;
Direcao direcaoAnterior = direcao;

inline bool ehSensorAberto()
{
  return digitalRead(PIN_BTN_SENSOR_ABERTO) == LOW;
}
inline bool ehSensorFechado()
{
  return digitalRead(PIN_BTN_SENSOR_FECHADO) == LOW;
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Iniciando Sistema de Controle de Portao Eletronico...");
  // pinos de entrada
  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_BTN_SENSOR_ABERTO, INPUT_PULLUP);
  pinMode(PIN_BTN_SENSOR_FECHADO, INPUT_PULLUP);
  // pinos de saída
  pinMode(PIN_LED_ABERTO, OUTPUT);
  pinMode(PIN_LED_FECHADO, OUTPUT);
  pinMode(PIN_LED_ALERTA, OUTPUT);
  digitalWrite(PIN_LED_ABERTO, LOW);
  digitalWrite(PIN_LED_FECHADO, LOW);
  digitalWrite(PIN_LED_ALERTA, LOW);
  ultimaLeituraBtn = digitalRead(PIN_BTN);
  ultimaEstadoBtn = ultimaLeituraBtn;
  ultimoDebounce = millis();
}

void AbrirPortao()
{
  digitalWrite(PIN_LED_ABERTO, HIGH);
  digitalWrite(PIN_LED_FECHADO, LOW);
  inicioMovimento = millis();
  Serial.println("Abrindo portao...");
}

void FecharPortao()
{
  digitalWrite(PIN_LED_ABERTO, LOW);
  digitalWrite(PIN_LED_FECHADO, HIGH);
  inicioMovimento = millis();
  Serial.println("Fechando portao...");
}

void PararMotor()
{
  digitalWrite(PIN_LED_ABERTO, LOW);
  digitalWrite(PIN_LED_FECHADO, LOW);
  inicioMovimento = 0;
  Serial.println("Parando portao...");
}

void Alerta()
{
  long tempoAtual = millis();
  if (estado == EST_ABRINDO || estado == EST_FECHANDO)
  {
    if (tempoAtual - ultimoAlerta >= TEMPO_ALERTA)
    {
      ultimoAlerta = tempoAtual;
      alertaLed = !alertaLed;
      digitalWrite(PIN_LED_ALERTA, alertaLed ? HIGH : LOW);
    }
  }
  else
  {
    alertaLed = false;
    digitalWrite(PIN_LED_ALERTA, LOW);
  }
}

void Debounce()
{
  int leitura = digitalRead(PIN_BTN);
  long tempoAtual = millis();

  if (leitura != ultimaLeituraBtn)
  {
    ultimoDebounce = tempoAtual;
  }

  if ((tempoAtual - ultimaLeituraBtn) >= DEBOUNCE)
  {
    if (leitura != ultimaEstadoBtn)
    {
      ultimaEstadoBtn = leitura;

      if (leitura == LOW)
      {
        eventoBtn = true;
      }
    }
  }
  ultimaLeituraBtn = leitura;
}

void loop()
{
  Debounce();

  Alerta();

  // Parado
  if (ehSensorAberto() && ehSensorFechado())
  {
    PararMotor();
    estado = EST_PARADO;
    direcao = DIR_NENHUM;
  }
  // Totalmente aberto
  else if (ehSensorAberto())
  {
    PararMotor();
    estado = EST_PARADO;
    direcao = DIR_ABRINDO;
  }
  // Totalmente fechado
  else if (ehSensorFechado())
  {
    PararMotor();
    estado = EST_PARADO;
    direcao = DIR_FECHANDO;
  }
  // TIMEOUT
  if ((estado == EST_ABRINDO || estado == EST_FECHANDO) && inicioMovimento != 0)
  {
    if (millis() - inicioMovimento >= TIMEOUT)
    {
      Serial.println("Tempo excedido!");
      if (estado == EST_ABRINDO)
      {
        direcao = DIR_ABRINDO;
      }
      else if (estado == EST_FECHANDO)
      {
        direcao = DIR_FECHANDO;
      }
      PararMotor();
      estado = EST_INTERROMPIDO;
    }
  }

  if (eventoBtn)
  {
    eventoBtn = false;

    switch (estado)
    {
    case EST_PARADO:
      if (direcao == DIR_ABRINDO)
      {
        estado = EST_FECHANDO;
        FecharPortao();
      }
      else if (direcao == DIR_FECHANDO)
      {
        estado = EST_ABRINDO;
        AbrirPortao();
      }
      else
      {
        estado = EST_ABRINDO;
        AbrirPortao();
      }
      break;

    case EST_ABRINDO:
      PararMotor();
      direcao = DIR_ABRINDO;
      estado = EST_INTERROMPIDO;
      break;

    case EST_FECHANDO:
      PararMotor();
      direcao = DIR_FECHANDO;
      estado = EST_INTERROMPIDO;
      break;

    case EST_INTERROMPIDO:
      if (direcao == DIR_ABRINDO)
      {
        estado = EST_FECHANDO;
        FecharPortao();
      }
      else if (direcao == DIR_FECHANDO)
      {
        estado = EST_ABRINDO;
        AbrirPortao();
      }
      else
      {
        estado = EST_ABRINDO;
        AbrirPortao();
      }
      break;
    }
  }
  delay(5);
}