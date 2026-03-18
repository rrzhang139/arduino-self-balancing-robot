// Motor A (Right motor) — original pins, unchanged
#define AIN1 4    // Direction
#define AIN2 5    // PWM

// Motor B (Left motor) — remapped from D6/D7 (defective)
#define BIN1 8    // Direction
#define BIN2 9    // PWM

void setup() {
  Serial.begin(9600);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void loop() {
  Serial.println("Both Forward");
  analogWrite(AIN2, 150);
  digitalWrite(AIN1, LOW);
  analogWrite(BIN2, 150);
  digitalWrite(BIN1, LOW);
  delay(2000);

  Serial.println("Stop");
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  delay(2000);

  Serial.println("Both Reverse");
  digitalWrite(AIN2, LOW);
  analogWrite(AIN1, 150);
  digitalWrite(BIN2, LOW);
  analogWrite(BIN1, 150);
  delay(2000);

  Serial.println("Stop");
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  delay(2000);
}