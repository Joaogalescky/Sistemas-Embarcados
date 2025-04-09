#define PIN_LED_VERDE 13
#define PIN_LED_AMARELO 12
#define PIN_LED_VERMELHO 11
#define TEMPO_VERDE 2000
#define TEMPO_AMARELO 1000
#define TEMPO_VERMELHO 3000

void setup() {
  // to run once
  Serial.begin(9600);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_AMARELO, OUTPUT);
  pinMode(PIN_LED_VERMELHO, OUTPUT);
}

void loop() {
  Serial.println("Sinal Verde");
  digitalWrite(PIN_LED_VERDE, HIGH);
  digitalWrite(PIN_LED_AMARELO, LOW);
  digitalWrite(PIN_LED_VERMELHO, LOW);
  delay(TEMPO_VERDE);
  Serial.println("Sinal Amarelo");
  digitalWrite(PIN_LED_VERDE, LOW);
  digitalWrite(PIN_LED_AMARELO, HIGH);
  delay(TEMPO_AMARELO);
  Serial.println("Sinal Vermelho");
  digitalWrite(PIN_LED_AMARELO, LOW);
  digitalWrite(PIN_LED_VERMELHO, HIGH)
  delay(TEMPO_VERMELHO);
  // to run repeatedly
}
