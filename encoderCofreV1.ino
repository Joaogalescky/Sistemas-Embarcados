/* interrupt routine for Rotary Encoders
   tested with Noble RE0124PVB 17.7FINB-24 http://www.nobleusa.com/pdf/xre.pdf - available at pollin.de
   and a few others, seems pretty universal

   The average rotary encoder has three pins, seen from front: A C B
   Clockwise rotation A(on)->B(on)->A(off)->B(off)
   CounterCW rotation B(on)->A(on)->B(off)->A(off)

   and may be a push switch with another two pins, pulled low at pin 8 in this case
   raf@synapps.de 20120107

*/

// usually the rotary encoders three pins have the ground pin in the middle
enum PinAssignments {
  encoderPinA = 2,  // rigth
  encoderPinB = 3,  // left
  // clearButton = 8   // another two pins
  encoderButton = 8
};

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 0;      // change management
static boolean rotating = false;       // debounce management

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

int senha[4] = { 4, 3, 8, 6 };
const int tamanhoSenha = sizeof(senha) / sizeof(senha[0]);
int senhaResposta[tamanhoSenha];
int indiceSenha = 0;

int tentativas = 3;
bool senhaCorreta = false;

int tempo = 2;
long t;

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  // pinMode(clearButton, INPUT);
  pinMode(encoderButton, INPUT_PULLUP);

  // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  // digitalWrite(clearButton, HIGH);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);

  Serial.begin(9600);  // output
  encoderPos = 0;
  t = millis();

  cleanPassword();
}

// main loop, work is done by interrupt service routines, this one only prints stuff
void loop() {
  rotating = true;  // reset the debouncer

  if (millis() > t + 10000) {
    t = millis();
    Serial.println(encoderPos, DEC);
  }

  if (lastReportedPos != encoderPos) {
    Serial.print("Valor atual:");
    Serial.println(encoderPos, DEC);
    lastReportedPos = encoderPos;

  }

  if (digitalRead(encoderButton) == LOW) {
    delay(200);
    handleButtonPress();
  }
}

/* IDEIA
Para destrancar o cofre, deve girar o encoder até o valor correto e confirmar pelo pressionamento do botão "switch" do Encoder.
Ao armazenar todos os números referente ao tamanho da senha, pressione novamente para confirmar para verificar se acertou ou não.
*/

void handleButtonPress() {
  senhaResposta[indiceSenha] = encoderPos;
  Serial.print("Digitado: ");
  Serial.println(encoderPos);

  indiceSenha++;
  encoderPos = 0; // zera para próximo número
  lastReportedPos = -1;

  if (indiceSenha >= tamanhoSenha) {
    senhaCorreta = true;
    for (int i = 0; i < tamanhoSenha; i++) {
      if (senhaResposta[i] != senha[i]) {
        senhaCorreta = false;
        break;
      }
    }

    if (senhaCorreta) {
      Serial.println("Senha correta! Bem-vindo Sr. Stark.");
      tentativas = 3;
    } else {
      tentativas--;
      if (tentativas > 0) {
        Serial.println("Senha incorreta! Tentativas restantes: " + String(tentativas));
      } else {
        Serial.println("Todas as tentativas usadas. Aguarde " + String(tempo) + " segundos...");
        delay(tempo * 1000);
        tentativas = 3;
      }
    }

    cleanPassword();
  }
}


  void cleanPassword() {
    // Percorrer o tamanho da senha para checar se a resposta foi correta
    for (int i = 0; i < tamanhoSenha; i++) {
      senhaResposta[i] = 0;
    }
    indiceSenha = 0;
    encoderPos = 0;
    lastReportedPos = -1;
  }

  // Interrupt on A changing state
  void doEncoderA() {
    // debounce
    if (rotating) delay(1);  // wait a little until the bouncing is done
    // Test transition, did things really change?
    if (digitalRead(encoderPinA) != A_set) {  // debounce once more
      A_set = !A_set;
      // adjust counter + if A leads B
      if (A_set && !B_set) {
        encoderPos++;
      }
      rotating = false;  // no more debouncing until loop() hits again
    }
  }

  // Interrupt on B changing state, same as A above
  void doEncoderB() {
    if (rotating) delay(1);
    if (digitalRead(encoderPinB) != B_set) {
      B_set = !B_set;
      //  adjust counter - 1 if B leads A
      if (B_set && !A_set) {
        encoderPos--;
      }
    }
    rotating = false;
  }