int sensorInterrupt = 0;       // Interrupt 0 is on Digital Pin 2
int sensorPin = 2;             // Flow sensor connected to digital pin 2
int solenoidValve = 5;         // Solenoid valve connected to digital pin 5
unsigned int SetPoint = 400;   // Target volume in millilitres (400 mL)

float calibrationFactor = 90.0; // Calibration factor (depends on your sensor)

volatile byte pulseCount = 0;
float flowRate = 0.0;
unsigned int flowMilliLitres = 0;
unsigned long totalMilliLitres = 0;
unsigned long oldTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(solenoidValve, OUTPUT);
  digitalWrite(solenoidValve, HIGH);  // Start with valve closed (HIGH = off)
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);      // Enable pull-up resistor
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  Serial.println("Flow Sensor Test Started...");
}

void loop() {
  if ((millis() - oldTime) > 1000) {
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    Serial.print("Flow rate: ");
    Serial.print(flowMilliLitres);
    Serial.print(" mL/s\t");

    Serial.print("Total: ");
    Serial.print(totalMilliLitres);
    Serial.println(" mL");

    if (totalMilliLitres >= SetPoint) {
      SetSolenoidValve();
    }

    pulseCount = 0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

void pulseCounter() {
  pulseCount++;
}

void SetSolenoidValve() {
  digitalWrite(solenoidValve, LOW);
  Serial.println("Target reached — valve closed!");
}
