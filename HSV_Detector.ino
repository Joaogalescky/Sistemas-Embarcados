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

struct mapaLdr {
  int minValue;
  int maxValue;
  byte redEstimado;
  byte greenEstimado;
  byte blueEstimado;
};

//faixa de valores ldr para cada cor
mapaLdr ldrMapeando[7] = {
  { 800, 1023, 255, 255, 255 },  // BRANCO
  { 600, 799, 255, 255, 100 },   // AMARELO
  { 400, 599, 100, 255, 100 },   // VERDE
  { 300, 499, 100, 100, 255 },   // AZUL
  { 200, 399, 255, 100, 100 },   // VERMELHO
  { 100, 299, 150, 150, 150 },   // CINZA
  { 0, 199, 50, 50, 50 }         // PRETO
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

void identificarCor() {
  Serial.println("Identificando Cor");

  int valorLdr = leituraMediaLdr();
  byte rgbEstimado[3];
  rgbEstimadoDeLdr(valorLdr, rgbEstimado);

  Serial.println("Valor LDR: " + String(valorLdr));
  Serial.print("RGB estimado do LDR: ");
  Serial.print("R=" + String(rgbEstimado[0]) + ", ");
  Serial.print("G=" + String(rgbEstimado[1]) + ", ");
  Serial.print("B=" + String(rgbEstimado[2]));

  //encontrar cor com menor diferenca
  int melhorIndex = encontrarMelhorCor(rgbEstimado);
  CoresReferencia melhorCaso = referenciandoCores[melhorIndex];

  //calc diferença quadrática
  double diferencaFinal = diferencaQuadratica(
    rgbEstimado[0], rgbEstimado[1].rgbEstimado[2],
    melhorCaso.rgb[0], melhorCaso.rgb[1], melhorCaso.rgb[2]);

  coresResultantes(melhorCaso, diferencaFinal);

  //reproduzir a cor usando HSV para RGB
  reproduzirCorHsv(melhorCaso);

  Serial.println("Fim identificacao!");
}

//encontrar melhor correspondencia
int encontrarMelhorCor(byte rgbEstimado[3]) {
  double minDiferenca = 999999;  //artificial - para busca do menor valor
  int melhorIndex = 0;

  Serial.println("\nComparando com cores de referencia:");

  for (int i = 0; i < 7; i++) {
    double diferenca = diferencaQuadratica(
      rgbEstimado[0], rgbEstimado[1], rgbEstimado[2],
      referenciandoCores[i].rgb[0], referenciandoCores[i].rgb[1], referenciandoCores[i].rgb[2]);

    Serial.print(referenciandoCores[i].nome);
    Serial.println(" - Diferenca: " + String(diferenca, 2));

    if (diferenca < minDiferenca) {
      minDiferenca = diferenca;
      melhorIndex = i;
    }
  }
  return melhorIndex;
}

void rgbEstimadoDeLdr(int valorLdr, byte rgbEstimado[3]) {
  //busca na tabela de mapeamento LDR
  for (int i = 0; i < 7; i++) {
    if (valorLdr >= ldrMapeando[i].minValue && valorLdr <= ldrMapeando[i].maxValue) {
      rgbEstimado[0] = ldrMapeando[i].redEstimado;
      rgbEstimado[1] = ldrMapeando[i].greenEstimado;
      rgbEstimado[2] = ldrMapeando[i].blueEstimado;
      return;
    }
  }

  //interpolação linear baseada no valor LDR
  //contigência para caso não encontre na tabela
  float intensidade = (float)valorLdr / 1023.0;
  rgbEstimado[0] = intensidade * 255;
  rgbEstimado[1] = intensidade * 255;
  rgbEstimado[2] = intensidade * 255;
}

void reproduzirCorHsv(CoresReferencia cor) {
  hsvToRgb(cor.hue / 360.0, cor.saturation, cor.value, rgb);

  analogWrite(PIN_LED_R, rgb[0]);
  analogWrite(PIN_LED_G, rgb[1]);
  analogWrite(PIN_LED_B, rgb[2]);

  Serial.print("Cor reproduzida via HSV: ");
  Serial.print("H=" + String(cor.hue) + "°,");
  Serial.print("S=" + String(cor.saturation) + ", ");
  Serial.println("V=" + String(cor.value));
  Serial.print("RGB resultante: ");
  Serial.print("R=" + String(rgb[0]) + ", ");
  Serial.print("G=" + String(rgb[1]) + ", ");
  Serial.println("B=" + String(rgb[2]));
}

//exibir resultados
void coresResultantes(CoresReferencia cor, double diferenca) {
  Serial.println("\nResultado da Identificacao:");
  Serial.println("Cor identificada: " + String(cor.nome));
  Serial.print("RGB de referencia: ");
  Serial.print("R=" + String(cor.rgb[0]) + ", ");
  Serial.print("G=" + String(cor.rgb[1]) + ", ");
  Serial.println("B=" + String(cor.rgb[2]));
  Serial.print("HSV de referencia: ");
  Serial.print("H=" + String(cor.hue) + "°, ");
  Serial.print("S=" + String(cor.saturation) + ", ");
  Serial.println("V=" + String(cor.value));
  Serial.print("Diferenca quadratica: " + String(diferenca, 2));
}

void setup() {
    // put your setup code here, to run once:
};

void loop() {
    // put your main code here, to run repeatedly:
};