unsigned long tempoAnterior = 0;
const long TEMPO_VERDE = 5000;   // 5 segundos
const long TEMPO_AMARELO = 2000; // 2 segundos
const long TEMPO_VERMELHO = 5000; // 5 segundos

// Definição dos pinos dos LEDs
const int PIN_LED_VERDE = 2;
const int PIN_LED_AMARELO = 3;
const int PIN_LED_VERMELHO = 4;

// Estados 
enum EstadoSemaforo { 
  ST_VERDE = 0, 
  ST_AMARELO = 1, 
  ST_VERMELHO = 2 
};

EstadoSemaforo estadoAtual;
EstadoSemaforo proximoEstado;

void setup() {
  
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_AMARELO, OUTPUT);
  pinMode(PIN_LED_VERMELHO, OUTPUT);

  // Inicinado no estado Verde
  estadoAtual = ST_VERDE;
  digitalWrite(PIN_LED_VERDE, HIGH);
  digitalWrite(PIN_LED_AMARELO, LOW);
  digitalWrite(PIN_LED_VERMELHO, LOW);

  Serial.begin(9600);

}

// Máquina de estados finitos (FSM) para controle do semáforo
void fsm(){

  // Obtém o tempo atual
  unsigned long tempoAtual = millis();  
  
  switch (estadoAtual) {
    case ST_VERDE:
      // Acende o LED verde e apaga os outros
      digitalWrite(PIN_LED_VERDE, HIGH);
      digitalWrite(PIN_LED_AMARELO, LOW);
      digitalWrite(PIN_LED_VERMELHO, LOW);
            
      if (tempoAtual - tempoAnterior >= TEMPO_VERDE) {
        tempoAnterior = tempoAtual; // Atualiza o tempo anterior
        proximoEstado = ST_AMARELO; // Muda para o estado amarelo
      }
      break;
      
    case ST_AMARELO:
      // Acende o LED amarelo e apaga os outros
      digitalWrite(PIN_LED_VERDE, LOW);
      digitalWrite(PIN_LED_AMARELO, HIGH);
      digitalWrite(PIN_LED_VERMELHO, LOW);
            
      if (tempoAtual - tempoAnterior >= TEMPO_AMARELO) {
        tempoAnterior = tempoAtual; 
        proximoEstado = ST_VERMELHO;
      }
      break;
      
    case ST_VERMELHO:
      // Acende o LED vermelho e apaga os outros
      digitalWrite(PIN_LED_VERDE, LOW);
      digitalWrite(PIN_LED_AMARELO, LOW);
      digitalWrite(PIN_LED_VERMELHO, HIGH);
      
      
      if (tempoAtual - tempoAnterior >= TEMPO_VERMELHO) {
        tempoAnterior = tempoAtual; 
        proximoEstado = ST_VERDE; 
      }
      break;
  }

  // debug
  if(estadoAtual != proximoEstado){// trocou de estado
     Serial.print("Estado: ");Serial.print(estadoAtual);
     Serial.print(" -> ");Serial.println(proximoEstado);
  }

  // atualiza o próximo estado
  estadoAtual = proximoEstado;

}

void loop() {

    fsm();   
  
}
