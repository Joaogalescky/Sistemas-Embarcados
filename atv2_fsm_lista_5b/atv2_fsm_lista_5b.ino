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
const unsigned long INTERVALO_LOOP = 5;

unsigned long ultimoDebounce = 0;
int ultimaLeituraBtn = HIGH;
int ultimaEstadoBtn = HIGH;
bool eventoBtn = false;

unsigned long inicioMovimento = 0;
unsigned long ultimoAlerta = 0;
bool alertaLed = false;

unsigned long ultimoLoop = 0;

int ultimoEstadoSensorAberto = HIGH;
int ultimoEstadoSensorFechado = HIGH;

enum Estado {
  EST_PARADO,
  EST_ABRINDO,
  EST_FECHANDO,
  EST_INTERROMPIDO
};

Estado estado = EST_PARADO;
Estado estadoAnterior = estado;

enum Direcao {
  DIR_NENHUM,
  DIR_ABRINDO,
  DIR_FECHANDO
};

Direcao direcao = DIR_NENHUM;
Direcao direcaoAnterior = direcao;

inline bool ehSensorAberto() {
  return digitalRead(PIN_BTN_SENSOR_ABERTO) == LOW;
}

inline bool ehSensorFechado() {
  return digitalRead(PIN_BTN_SENSOR_FECHADO) == LOW;
}

void setup() {
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
  ultimoEstadoSensorAberto = digitalRead(PIN_BTN_SENSOR_ABERTO);
  ultimoEstadoSensorFechado = digitalRead(PIN_BTN_SENSOR_FECHADO);
}

void AbrirPortao() {
  digitalWrite(PIN_LED_ABERTO, HIGH);
  digitalWrite(PIN_LED_FECHADO, LOW);
  inicioMovimento = millis();
  direcao = DIR_ABRINDO;
  Serial.println("Abrindo portao...");
  Serial.println("Alerta, portao em movimento!");
}

void FecharPortao() {
  digitalWrite(PIN_LED_ABERTO, LOW);
  digitalWrite(PIN_LED_FECHADO, HIGH);
  inicioMovimento = millis();
  direcao = DIR_FECHANDO;
  Serial.println("Fechando portao...");
  Serial.println("Alerta, portao em movimento!");
}

void PararMotor() {
  digitalWrite(PIN_LED_ABERTO, LOW);
  digitalWrite(PIN_LED_FECHADO, LOW);
  if (estado == EST_ABRINDO || estado == EST_FECHANDO) {
    Serial.println("Parando portao...");
  }
  if (estado == EST_ABRINDO) {
    direcaoAnterior = DIR_ABRINDO;
  } else if (estado == EST_FECHANDO) {
    direcaoAnterior = DIR_FECHANDO;
  }
  inicioMovimento = 0;
  direcao = DIR_NENHUM;
}

void Alerta() {
  unsigned long tempoAtual = millis();
  if (estado == EST_ABRINDO || estado == EST_FECHANDO) {
    if (tempoAtual - ultimoAlerta >= TEMPO_ALERTA) {
      ultimoAlerta = tempoAtual;
      alertaLed = !alertaLed;
      digitalWrite(PIN_LED_ALERTA, alertaLed ? HIGH : LOW);
    }
  } else {
    alertaLed = false;
    digitalWrite(PIN_LED_ALERTA, LOW);
  }
}

void printEstadoFSM() {
  Serial.print("Estado atual: ");
  switch (estado) {
    case EST_PARADO: Serial.println("PARADO"); break;
    case EST_ABRINDO: Serial.println("ABRINDO"); break;
    case EST_FECHANDO: Serial.println("FECHANDO"); break;
    case EST_INTERROMPIDO: Serial.println("INTERROMPIDO"); break;
  }
}

void Debounce() {
  int leitura = digitalRead(PIN_BTN);
  unsigned long tempoAtual = millis();

  if (leitura != ultimaLeituraBtn) {
    ultimoDebounce = tempoAtual;
  }

  if ((tempoAtual - ultimoDebounce) >= DEBOUNCE) {
    if (leitura != ultimaEstadoBtn) {
      ultimaEstadoBtn = leitura;
      if (leitura == LOW) {
        eventoBtn = true;
      }
    }
  }
  ultimaLeituraBtn = leitura;
}

void loop() {
  int leituraAberto = digitalRead(PIN_BTN_SENSOR_ABERTO);
  int leituraFechado = digitalRead(PIN_BTN_SENSOR_FECHADO);

  if (leituraAberto != ultimoEstadoSensorAberto) {
    ultimoEstadoSensorAberto = leituraAberto;
    if (leituraAberto == LOW) {
      Serial.println("Sensor ABERTO acionado");
    } else {
      Serial.println("Sensor ABERTO liberado");
    }
  }

  if (leituraFechado != ultimoEstadoSensorFechado) {
    ultimoEstadoSensorFechado = leituraFechado;
    if (leituraFechado == LOW) {
      Serial.println("Sensor FECHADO acionado");
    } else {
      Serial.println("Sensor FECHADO liberado");
    }
  }

  unsigned long tempoAtual = millis();
  if (tempoAtual - ultimoLoop >= INTERVALO_LOOP) {
    ultimoLoop = tempoAtual;

    Debounce();
    Alerta();

    bool sensorAbertoAtivo = (leituraAberto == LOW);
    bool sensorFechadoAtivo = (leituraFechado == LOW);

    // Parado
    if (sensorAbertoAtivo && sensorFechadoAtivo) {
      PararMotor();
      estado = EST_PARADO;
      direcao = DIR_NENHUM;
    }
    // Totalmente aberto
    else if (sensorAbertoAtivo) {
      PararMotor();
      estado = EST_PARADO;
    }
    // Totalmente fechado
    else if (sensorFechadoAtivo) {
      PararMotor();
      estado = EST_PARADO;
    }
    // TIMEOUT
    if ((estado == EST_ABRINDO || estado == EST_FECHANDO) && inicioMovimento != 0) {
      if (tempoAtual - inicioMovimento >= TIMEOUT) {
        Serial.println("Tempo excedido!");
        if (estado == EST_ABRINDO) {
          direcaoAnterior = DIR_ABRINDO;
        } else if (estado == EST_FECHANDO) {
          direcaoAnterior = DIR_FECHANDO;
        }
        PararMotor();
        estado = EST_INTERROMPIDO;
        alertaLed = false;
        digitalWrite(PIN_LED_ALERTA, LOW);
      }
    }

    if (eventoBtn) {
      eventoBtn = false;

      switch (estado) {
        case EST_PARADO:
          if (direcaoAnterior == DIR_ABRINDO) {
            estado = EST_FECHANDO;
            FecharPortao();
          } else if (direcaoAnterior == DIR_FECHANDO) {
            estado = EST_ABRINDO;
            AbrirPortao();
          } else {
            estado = EST_ABRINDO;
            AbrirPortao();
          }
          break;

        case EST_ABRINDO:
          PararMotor();
          direcaoAnterior = DIR_ABRINDO;
          estado = EST_INTERROMPIDO;
          break;

        case EST_FECHANDO:
          PararMotor();
          direcaoAnterior = DIR_FECHANDO;
          estado = EST_INTERROMPIDO;
          break;

        case EST_INTERROMPIDO:
          if (direcaoAnterior == DIR_ABRINDO) {
            estado = EST_FECHANDO;
            FecharPortao();
          } else if (direcaoAnterior == DIR_FECHANDO) {
            estado = EST_ABRINDO;
            AbrirPortao();
          } else {
            estado = EST_ABRINDO;
            AbrirPortao();
          }
          break;
      }
    }
    if (estado != estadoAnterior) {
      printEstadoFSM();
      estadoAnterior = estado;
    }
  }
}