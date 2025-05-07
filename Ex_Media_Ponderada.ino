// João Galescky

// Exercício - Média Ponderada

const int analogInPin = A0;
const int arraySize = 5;

int array[arraySize];

int sensorValue;
int sum = 0;
int pesoSum = 0;

float peso;
float pesoMedia;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < arraySize; i++) { // preenche de '0'
    array[i] = 0;
  }
}

void loop() {
  int sensorValue = analogRead(analogInPin);

  for (int i = 0; i < arraySize - 1; i++) { // Desloca todos os elementos
    array[i] = array[i + 1];
  }
  array[arraySize - 1] = sensorValue; // Novo valor na última posição

  sum = 0;
  pesoSum = 0;

  for (int i = 0; i < arraySize; i++) { // Média ponderada - pesos progressivos 
    peso = 1.0 + (i * 0.5); 
    sum += array[i] * peso;
    pesoSum += peso;
  }

  pesoMedia = (float)sum / pesoSum;

  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(pesoMedia);
  delay(100);
}