const int led = 13;
const int pinoInterrupcao = 2;
volatile int est = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(pinoInterrupcao, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinoInterrupcao), atualizaLed, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(".");
  delay(1000);
}

void atualizaLed() {
  est = digitalRead(pinoInterrupcao);
  digitalWrite(led, est);
}
