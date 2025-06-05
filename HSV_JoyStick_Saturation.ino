/*
1)Desenvolva um protótipo com Arduino que demonstre o modelo de cores HSV
usando um LED RGB. Para teste e apresentação, monte um circuito na protoboard com
os componentes necessários para o funcionamento (led RGB, resistores,
potênciometros).

c) Desenvolva um algoritmo que através de dois potenciômetros controle os valores de
Hue e Value, considere o valor de Saturation 1.
*/

#define PIN_LED_R 11
#define PIN_LED_G 10
#define PIN_LED_B 9

byte rgb[3];

double JoyStick_X = A0
double JoyStick_Y = A1

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
  void
  hsvToRgb(double h, double s, double v, byte rgb[]) {
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

}

void loop() {

}