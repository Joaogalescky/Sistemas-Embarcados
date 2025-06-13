/*
2) Identificar cores com o Arduino utilizando um LDR e um led RGB. O sistema deve
ser calibrado para no mínimo 7 cores. O programa deve verificar o acionamento de um
botão, caso acionado, deve identificar a cor do objeto e imprimir na saída serial os
componentes RGB, também de configurar o led para que a cor seja a mais próxima da
cor do material amostrado.
*/

//Autores: João Vitor Campõe Galescky & Gabriel Albuquerque

//pinos
const int PIN_LED_R = 11;
const int PIN_LED_G = 10;
const int PIN_LED_B = 9;
const int PIN_LDR = A0;
const int PIN_BUTTON = 2;

//variavel
byte rgb[3];

struct CoresReferencia {
  String nome;
  byte rgb[3];  //0 = red; 1 = green; 2 = blue
  double hue;   // 0-360
  double saturation;
  double value;
};

CoresReferencia referenciandoCores[7] = {
  { "BRANCO", { 255, 255, 255 }, 0, 0, 1 },
  { "AMARELO", { 255, 255, 0 }, 60, 1, 1 },
  { "VERDE", { 0, 255, 0 }, 120, 1, 1 },
  { "AZUL", { 0, 0, 255 }, 240, 1, 1 },
  { "VERMELHO", { 255, 0, 0 }, 0, 1, 1 },
  { "CINZA", { 128, 128, 128 }, 0, 0, 0.5 },
  { "PRETO", { 0, 0, 0 }, 0, 0, 0 }
};

/**
* Converts an HSV cor value to RGB.
* Assumes h, s, and v are contained in the set [0, 1] and
* returns r, g, and b in the set [0, 255].
* @param Number h The hue
* @param Number s The saturation
* @param Number v The value
* @return Array The RGB representation
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
};

void loop() {
    // put your main code here, to run repeatedly:
};