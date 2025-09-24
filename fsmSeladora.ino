unsigned long tempoAnterior = 0;

const long TEMPO_VACUO = 5000;   // 30 segundos
const long TEMPO_AQUECIMENTO = 5000; // 5 segundos

// Definição dos pinos 
const int PIN_VACUO = 4;
const int PIN_AQUECIMENTO = 3;
const int PIN_LED_PRONTO = 2;
const int PIN_TAMPA = 8;

// Estados 
enum Estados{ 
  ST_INICIAL = 0, 
  ST_VACUO = 1, 
  ST_AQUECIMENTO = 2,
  ST_PRONTO =3
};

Estados estadoAtual;
Estados proximoEstado;

void setup() {
  
  pinMode(PIN_VACUO, OUTPUT);
  pinMode(PIN_AQUECIMENTO, OUTPUT);
  pinMode(PIN_LED_PRONTO, OUTPUT);
  pinMode(PIN_TAMPA, INPUT_PULLUP);

  // Inicinado no estado Verde
  estadoAtual = ST_INICIAL;
  digitalWrite(PIN_VACUO, LOW);
  digitalWrite(PIN_AQUECIMENTO, LOW);
  digitalWrite(PIN_LED_PRONTO, LOW);

  Serial.begin(9600);

}

bool isTampaFechada(){
	bool tampa = digitalRead(PIN_TAMPA);
	return tampa;
	
}

// Máquina de estados finitos (FSM) para controle da seladora
void fsm(){

  // Obtém o tempo atual
  unsigned long tempoAtual = millis();  
  
  switch (estadoAtual) {
    case ST_INICIAL:
      
      digitalWrite(PIN_VACUO, LOW);
	  digitalWrite(PIN_AQUECIMENTO, LOW);
      digitalWrite(PIN_LED_PRONTO, LOW);
            
      if (isTampaFechada()) {        
        proximoEstado = ST_VACUO;
		tempoAnterior = millis(); 		
      }
      break;
      
    case ST_VACUO:
      digitalWrite(PIN_VACUO, HIGH);
	  digitalWrite(PIN_AQUECIMENTO, LOW);
      digitalWrite(PIN_LED_PRONTO, LOW);
	  
	  if (!isTampaFechada()) {        
        proximoEstado = ST_INICIAL; 
      } else if (tempoAtual - tempoAnterior >= TEMPO_VACUO) {
        tempoAnterior = millis(); 
        proximoEstado = ST_AQUECIMENTO;
      }
      break;
      
    case ST_AQUECIMENTO:
      digitalWrite(PIN_VACUO, HIGH);
	  digitalWrite(PIN_AQUECIMENTO, HIGH);
      digitalWrite(PIN_LED_PRONTO, LOW);      
      
      if (!isTampaFechada()) {        
        proximoEstado = ST_INICIAL; 
      } else if (tempoAtual - tempoAnterior >= TEMPO_AQUECIMENTO) {
        tempoAnterior = millis(); 
        proximoEstado = ST_PRONTO; 
      }
      break;
	
	case ST_PRONTO:
      digitalWrite(PIN_VACUO, LOW);
	  digitalWrite(PIN_AQUECIMENTO, LOW);
      digitalWrite(PIN_LED_PRONTO, HIGH);      
      
      if (!isTampaFechada()) {        
        proximoEstado = ST_INICIAL; 
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
