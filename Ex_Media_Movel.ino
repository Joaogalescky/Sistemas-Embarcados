// João Galescky & Gabriel Albuquerque

// Exercício - Média Móvel

const int analogInPin = A0;
const int arraySize = 10;
int array[arraySize];

int sensorValue;
int sum = 0;
int movingAverage = 0;

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < arraySize; i++) { // preenche de zero
    array[i] = 0;
  }
}

void loop() {
  sensorValue = analogRead(analogInPin);
  
  for(int i = arraySize - 1; i > 0; i--) {  // Desloca todos os elementos
    array[i] = array[i-1];
  }
  array[0] = sensorValue;
  
  sum = 0;
  for(int i = 0; i < arraySize; i++) { // Média
    sum += array[i];
  }
  movingAverage = sum / arraySize;
  
  Serial.print(sensorValue);
  Serial.print(",");
  Serial.println(movingAverage);
  delay(10);
}