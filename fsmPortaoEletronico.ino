#include <ezButton.h>

#define debugL6

// Definição dos pinos 
// Saídas
const int PIN_DIRECAO_MOTOR = 2;
const int PIN_ACIONAMENTO_MOTOR = 3;
// Entradas
const int PIN_FIM_CURSO_ABERTO = 4;
const int PIN_FIM_CURSO_FECHADO = 5;
const int PIN_BTN_ACIONAMENTO = 6;


ezButton btnFCAberto(PIN_FIM_CURSO_ABERTO);
ezButton btnFCFechado(PIN_FIM_CURSO_FECHADO);
ezButton btnAcionamento(PIN_BTN_ACIONAMENTO);

// Estados 
enum EstadoPortaoEletronico { 
  ST_FECHADO = 0, 
  ST_= 1,   
};

EstadoPortaoEletronico estadoAtual;
EstadoPortaoEletronico proximoEstado;


void abrirPortao(){
  digitalWrite(PIN_ACIONAMENTO_MOTOR, LOW);
  digitalWrite(PIN_DIRECAO_MOTOR, HIGH);
}


void fecharPortao(){
  digitalWrite(PIN_ACIONAMENTO_MOTOR, HIGH);
  digitalWrite(PIN_DIRECAO_MOTOR, HIGH);
}


void pararPortao(){
  digitalWrite(PIN_ACIONAMENTO_MOTOR, LOW);  
}


void setup() {  
  pinMode(PIN_DIRECAO_MOTOR, OUTPUT);
  pinMode(PIN_ACIONAMENTO_MOTOR, OUTPUT);

  // A biblioteca ezbutton já configura as entradas (pullup)
  // Veja no src no github: https://github.com/ArduinoGetStarted/button
  
  // Inicinado no estado ...
  estadoAtual = ST_FECHADO;  
  digitalWrite(PIN_ACIONAMENTO_MOTOR, LOW);
  digitalWrite(PIN_DIRECAO_MOTOR, LOW);
  
  Serial.begin(9600);
}


// Máquina de estados finitos (FSM) para controle do portão
void fsm(){  
  switch (estadoAtual) {
    case ST_FECHADO:
      pararPortao();

      if ( btnAcionamento.isPressed()) {
       
        //proximoEstado = ; 
      }
      break;
      
    case ST_:
      
      abrirPortao();
      /*      
      if () {
        
        proximoEstado = ST_;
      }
      */
      break;    
  }

  // debug
  #if defined debugL6
  if(estadoAtual != proximoEstado){// trocou de estado
     Serial.print("Estado: ");Serial.print(estadoAtual);
     Serial.print(" -> ");Serial.println(proximoEstado);
  }
  #endif

  // atualiza o próximo estado
  estadoAtual = proximoEstado;
}

void loop() {  
  // Loop para debounce dos botões
  btnFCAberto.loop();
  btnFCFechado.loop();
  btnAcionamento.loop();

  fsm();     
}
