const int echoPin = 4;
const int triggerPin = 3;
long duration, cm, mm;
// long mmAnterior = -1;
long cmAnterior = -1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
}

void loop() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(5);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  cm = ((double)duration/2) / 29.387; // Aqui ele está convertendo para cm
  // mm = ((double)duration/2) / 293.87; // Aqui ele está convertendo para mm

  if (cm != cmAnterior) {
    Serial.print("ALARME!");
  }

  cmAnterior = cm;

  // Serial.print(cm);
  // Serial.print("cm");
  // Serial.println();

  delay(250);
}