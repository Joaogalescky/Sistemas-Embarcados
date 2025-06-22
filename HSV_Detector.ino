/*
2) Identificar cores com o Arduino utilizando um LDR e um led RGB. O sistema deve
ser calibrado para no mínimo 7 cores. O programa deve verificar o acionamento de um
botão, caso acionado, deve identificar a cor do objeto e imprimir na saída serial os
componentes RGB, também de configurar o led para que a cor seja a mais próxima da
cor do material amostrado.
*/

// Autores: João Vitor Campõe Galescky & Gabriel Albuquerque

// pinos
const int PIN_LED_R = 11;
const int PIN_LED_G = 10;
const int PIN_LED_B = 9;
const int PIN_LDR = A0;
const int PIN_BUTTON = 2;

struct Cor {
  String nome;
  byte rgb[3];                   // 0 = red; 1 = green; 2 = blue
  float hue, saturation, value;  // 0-360
};

Cor cores[7] = {
  { "BRANCO", { 255, 255, 255 }, 0, 0, 1 },
  { "AMARELO", { 255, 255, 0 }, 60, 1, 1 },
  { "VERDE", { 0, 255, 0 }, 120, 1, 1 },
  { "AZUL", { 0, 0, 255 }, 240, 1, 1 },
  { "VERMELHO", { 255, 0, 0 }, 0, 1, 1 },
  { "CINZA", { 128, 128, 128 }, 0, 0, 0.5 },
  { "PRETO", { 0, 0, 0 }, 0, 0, 0 }
};

// Mapeamento LDR -> RGB estimado
int ldrRanges[7][2] = {
  { 800, 1023 },  // Branco
  { 300, 499 },   // Azul
  { 400, 599 },   // Verde
  { 600, 799 },   // Amarelo
  { 200, 399 },   // Vermelho
  { 100, 299 },   // Cinza
  { 0, 199 }      // Preto
};

bool calibrado = false;

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
    case 0:
      r = v, g = t, b = p;
      break;
    case 1:
      r = q, g = v, b = p;
      break;
    case 2:
      r = p, g = v, b = t;
      break;
    case 3:
      r = p, g = q, b = v;
      break;
    case 4:
      r = t, g = p, b = v;
      break;
    case 5:
      r = v, g = p, b = q;
      break;
  }

  rgb[0] = r * 255;
  rgb[1] = g * 255;
  rgb[2] = b * 255;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("HSV-RGB Detector de Cores");
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);
  demonstrarHsv();
  calibrar();
  calibrado = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(PIN_BUTTON) == LOW) {
    delay(200);
    identificarCor();
  }
  delay(50);
}

void identificarCor() {
  Serial.println("Identificando Cor");

  int ldr = lerLDR();
  Serial.println("LDR: " + String(ldr));

  // Estimar RGB pelo LDR
  byte rgbEstimado[3];
  estimarRGB(ldr, rgbEstimado);

  Serial.print("RGB estimado: ");
  printRGB(rgbEstimado);

  // Encontrar cor mais proxima
  int melhorCor = encontrarCorMaisProxima(rgbEstimado);

  // Exibe resultado
  Serial.println("Cor identificada: " + cores[melhorCor].nome);
  Serial.print("RGB referência: ");
  printRGB(cores[melhorCor].rgb);

  // Reproduzir cor
  reproduzirCor(cores[melhorCor]);
}

int encontrarCorMaisProxima(byte rgbEstimado[3]) {
  float menorDistancia = 999999;  // Para busca do menor valor
  int melhorIndex = 0;

  for (int i = 0; i < 7; i++) {
    float distancia = distanciaEuclidianaRGB(
      rgbEstimado[0], rgbEstimado[1], rgbEstimado[2],
      cores[i].rgb[0], cores[i].rgb[1], cores[i].rgb[2]);

    if (distancia < menorDistancia) {
      menorDistancia = distancia;
      melhorIndex = i;
    }
  }
  return melhorIndex;
}

void estimarRGB(int ldr, byte rgbEstimado[3]) {
  // Busca na tabela de mapeamento LDR
  int corIndex = -1;
  for (int i = 0; i < 7; i++) {
    if (ldr >= ldrRanges[i][0] && ldr <= ldrRanges[i][1]) {
      corIndex = i;
      break;
    }
  }

  if (corIndex >= 0) {
    // Usa cor de referência
    rgbEstimado[0] = cores[corIndex].rgb[0];
    rgbEstimado[1] = cores[corIndex].rgb[1];
    rgbEstimado[2] = cores[corIndex].rgb[2];
  } else {
    // interpolação linear (intensidade proporcional) baseada no valor LDR
    // contigência para caso não encontre na tabela
    float intensidade = (float)ldr / 1023.0;
    rgbEstimado[0] = intensidade * 255;
    rgbEstimado[1] = intensidade * 255;
    rgbEstimado[2] = intensidade * 255;
  }
}

void reproduzirCor(Cor cor) {
  byte rgb[3];
  hsvToRgb(cor.hue / 360.0, cor.saturation, cor.value, rgb);

  analogWrite(PIN_LED_R, rgb[0]);
  analogWrite(PIN_LED_G, rgb[1]);
  analogWrite(PIN_LED_B, rgb[2]);

  Serial.print("HSV configurado: ");
  Serial.print("H=" + String(cor.hue) + "°,");
  Serial.print("S=" + String(cor.saturation) + ", ");
  Serial.println("V=" + String(cor.value));
  Serial.print("HSV -> RGB: ");
  printRGB(rgb);
}

void calibrar() {
  Serial.println("Posicione cada cor sobre o LDR quando solicitado:");

  for (int i = 0; i < 7; i++) {
    Serial.println("\nPosicione a cor: " + cores[i].nome);
    Serial.println("Pressione o botao quando estiver pronto...");

    while (digitalRead(PIN_BUTTON) == HIGH) {
      int ldr = lerLDR();
      Serial.println("LDR atual: " + String(ldr));
      delay(500);
    }

    delay(200);  // Debounce

    long soma = 0;
    int leituras = 10;
    Serial.println("Fazendo leituras...");

    for (int j = 0; j < leituras; j++) {
      soma += lerLDR();
      delay(50);
    }

    int media = soma / leituras;

    // Atualizar o range para a cor (com margem de erro)
    int margem = 30;
    ldrRanges[i][0] = max(0, media - margem);
    ldrRanges[i][1] = min(1023, media + margem);

    Serial.println("Cor " + cores[i].nome + " calibrada!");
    Serial.println("Valor médio: " + String(media));
    Serial.println("Range: " + String(ldrRanges[i][0]) + " - " + String(ldrRanges[i][1]));

    // Aguardar soltar o botão
    while (digitalRead(PIN_BUTTON) == LOW) {
      delay(50);
    }
  }
  ajustarRanges();

  Serial.println("\nCalibragem concluída");
  Serial.println("Ranges finais:");
  for (int i = 0; i < 7; i++) {
    Serial.println(cores[i].nome + ": " + String(ldrRanges[i][0]) + " - " + String(ldrRanges[i][1]));
  }
  Serial.println("Sistema pronto para identificação!\n");
}

void ajustarRanges() {
  // Ordenar as cores por valor mínimo do LDR
  for (int i = 0; i < 6; i++) {
    for (int j = i + 1; j < 7; j++) {
      if (ldrRanges[i][0] > ldrRanges[j][0]) {
        // Trocar ranges
        int temp[2] = { ldrRanges[i][0], ldrRanges[i][1] };
        ldrRanges[i][0] = ldrRanges[j][0];
        ldrRanges[i][1] = ldrRanges[j][1];
        ldrRanges[j][0] = temp[0];
        ldrRanges[j][1] = temp[1];

        // Trocar cores correspondentes
        Cor tempCor = cores[i];
        cores[i] = cores[j];
        cores[j] = tempCor;
      }
    }
  }

  // Ajustar sobreposições
  for (int i = 0; i < 6; i++) {
    if (ldrRanges[i][1] >= ldrRanges[i + 1][0]) {
      int meio = (ldrRanges[i][1] + ldrRanges[i + 1][0]) / 2;
      ldrRanges[i][1] = meio - 1;
      ldrRanges[i + 1][0] = meio;
    }
  }
}

void demonstrarHsv() {
  Serial.println("\nDemonstracao modelo HSV - Percorrendo espectro de cores");
  byte rgb[3];
  for (double h = 0; h < 360; h += 15) {
    hsvToRgb(h / 360, 1, 1, rgb);
    Serial.print("H=" + String(h) + "° -> ");
    Serial.print("R=" + String(rgb[0]) + " ");
    Serial.print("G=" + String(rgb[1]) + " ");
    Serial.print("B=" + String(rgb[2]));

    analogWrite(PIN_LED_R, rgb[0]);
    analogWrite(PIN_LED_G, rgb[1]);
    analogWrite(PIN_LED_B, rgb[2]);
    delay(200);
  }
  analogWrite(PIN_LED_R, LOW);
  analogWrite(PIN_LED_G, LOW);
  analogWrite(PIN_LED_B, LOW);
  Serial.println("Demonstracao concluida! Pronto para identificacao");
}

// Medir entre dois pontos em um espaço
double distanciaEuclidianaRGB(byte r1, byte g1, byte b1, byte r2, byte g2, byte b2) {
  double difR = r1 - r2;
  double difG = g1 - g2;
  double difB = b1 - b2;
  return sqrt(difR * difR + difG * difG + difB * difB);
}

int lerLDR() {
  long soma = 0;
  for (int i = 0; i < 10; i++) {
    soma += analogRead(PIN_LDR);
    delay(10);
  }
  return soma / 10;
}

void printRGB(byte rgb[3]) {
  Serial.print("R=" + String(rgb[0]) + ", ");
  Serial.print("G=" + String(rgb[1]) + ", ");
  Serial.println("B=" + String(rgb[2]));
}