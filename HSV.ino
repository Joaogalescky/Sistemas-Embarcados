/*
1)Desenvolva um protótipo com Arduino que demonstre o modelo de cores HSV
usando um LED RGB. Para teste e apresentação, monte um circuito na protoboard com
os componentes necessários para o funcionamento (led RGB, resistores,
potênciometros).

  a) Desenvolva um algoritmo que percorra constantemente os 360o de Hue do modelo e
  converta em valores RGB para acender o LED. Para a atividade considere para a
  Saturation e o Value o valor 1.
*/

#define PIN_LED_R 11
#define PIN_LED_G 10
#define PIN_LED_B 9
byte rgb[3];

/**
* Converts an HSV color value to
* RGB.
* Assumes h, s, and v are
* contained in the set [0, 1] and
* returns r, g, and b in the
* set [0, 255].
*
* @param Number h The hue
* @param Number s The saturation
* @param Number v The value
* @return Array The RGB
* representation
*/
void hsvToRgb(double h, double s, double v, byte rgb[]) {
  double r, g, b;
  int i = int(h * 6);
  double f = h * 6 - i;
  double p = v * (1 - s);
  double q = v * (1 - f * s);
  double t = v * (1 - (1 - f) * s);
  switch (i % 6) {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    case 5: r = v, g = p, b = q; break;
  }
  rgb[0] = r * 255;
  rgb[1] = g * 255;
  rgb[2] = b * 255;
}

void setup() {
  // put your setup code here, to run once:
  Serial.println("Iniciado...");
  Serial.begin(9600);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  // for (int i = 0; i < 346; i += 15) {
  for (double h = 0; h < 360; h++) {
    hsvToRgb(h / 360, 1, 1, rgb);
    Serial.print("R = ");
    Serial.println(rgb[0]);
    Serial.print("G = ");
    Serial.println(rgb[1]);
    Serial.print("B = ");
    Serial.println(rgb[2]);

    analogWrite(PIN_LED_R, rgb[0]);
    analogWrite(PIN_LED_G, rgb[1]);
    analogWrite(PIN_LED_B, rgb[2]);
    delay(100);
  };
};