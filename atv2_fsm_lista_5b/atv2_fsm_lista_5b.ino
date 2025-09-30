/* Descrição
  2. Controle de Portão Eletrônico com Reversão de Movimento Portas usando FSM e
  Arduino

  Desenvolver um sistema de controle para um portão eletrônico, implementando
  uma Máquina de Estados Finitos (FSM) no Arduino que gerencia a lógica de operação
  cíclica e a reversão de movimento.
*/

/* Itens
   - Botão único de controle (PIN_BTN)
   - Dois sensores de fim de curso (PIN_SENSOR_OPEN, PIN_SENSOR_CLOSE)
   - Motor controlado por PIN_MOTOR_EN (liga/desliga) e PIN_MOTOR_DIR (direção)
   - LED_OPEN, LED_CLOSE (indicadores de direção)
   - LED_ALERT (piscando enquanto há movimento)
   - Timeout de segurança: 30s (TIMEOUT_MOVEMENT)
   - Debounce para botões/sensores (50 ms)
*/

// Pinos
#define PIN_BTN 2
#define PIN_SENSOR_ABERTO 3   // Sensor
#define PIN_SENSOR_FECHADO 4  // Sensor
#define PIN_MOTOR_ABERTO 5    // Motor
#define PIN_MOTOR_FECHADO 6   // Motor
#define PIN_LED_ABERTO 8      // Led
#define PIN_LED_FECHADO 9     // Led
#define PIN_LED_ALERTA 10     // Led

const long DEBOUNCE_MS = 50;
const long TIMEOUT_MS = 30000UL  // ? Oque é UL
  const long PISCAR_INTERVALO_MS = 500;

enum Estado {
  FECHADO,
  ABERTO,
  FECHADO_MOVENDO,
  ABERTO_MOVENDO,
  PARADO
};

Estado estado = PARADO;

enum Direcao {
  DIR_NENHUM,
  DIR_ABRINDO,
  DIR_FECHANDO
};

Direcao ultimaDirecao = DIR_NENHUM;

struct DebouncedEntrada {
  int pin;
  bool estadoAtual;
  int 
}

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
