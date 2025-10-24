// 1 - Medir velocidade do cooler com interrupção externa

const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9;  // Analog output pin that the LED is attached to

#define PIN_LED 13
#define PIN_INTERRUPCAO 2

unsigned long previousMillis = 0;
const long interval = 1000;

int sensorValue = 0;  // valor lido do cooler girando
int outputValue = 0;  // valor do PWM (analog out)
int rpmValue = 0;     // valor final do RPM
volatile int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_INTERRUPCAO, INPUT_PULLUP);
  // FALLING: transition from HIGH to LOW
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPCAO), contar, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(analogInPin);            // leitura do potenciometro
  outputValue = map(sensorValue, 0, 1023, 0, 255);  // mapeamento para PWM
  analogWrite(analogOutPin, outputValue);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    noInterrupts();

    rpmValue = (count / 2.0) * 60;

    Serial.print("RPM: ");
    Serial.println(rpmValue);
    Serial.print("PWM: ");
    Serial.println(outputValue);
    Serial.println();

    count = 0;

    interrupts();
  }
}

void contar() {
  count++;
}