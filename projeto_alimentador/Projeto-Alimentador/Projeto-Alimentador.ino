// https://randomnerdtutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/

#include "Adafruit_LEDBackpack.h"

#define PIN_LED_R 13
#define PIN_LED_G 11
#define PIN_LED_B 12
#define PIN_POT A0
#define PIN_SDA A4
#define PIN_SCL A5

int sensorValue = 0;
int outputValue = 0;

unsigned long previousTime = 0;
unsigned long interval = 0;

Adafruit_7segment dis = Adafruit_7segment();

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);

  dis.begin(0x70);
}

void loop() {
  unsigned long currentTime = millis();

  sensorValue = analogRead(PIN_POT);
  outputValue = map(sensorValue, 0, 1023, 0, 2000);
  analogWrite(sensorValue, outputValue);

  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;

    if (outputValue > 1000) {
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
    } else if (outputValue > 500) {
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
    } else {
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, LOW);
    }

    dis.println(outputValue);
    dis.writeDisplay();
  }
}
