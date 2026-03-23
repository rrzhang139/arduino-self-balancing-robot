// Self-Balancing Robot — PID control with millis()-based loop
// Motor A (Right): D4 direction, D5 PWM
// Motor B (Left):  D8 direction, D9 PWM
// MPU-6050 on default I2C (A4/A5)

#include <Wire.h>

// ===================== PIN DEFINITIONS =====================
#define AIN1 4  // Right motor direction
#define AIN2 5  // Right motor PWM
#define BIN1 8  // Left motor direction
#define BIN2 9  // Left motor PWM

// ===================== MPU-6050 ============================
#define MPU_ADDR 0x68

// Accelerometer & gyro offsets — from IMU_Zero calibration
#define ACC_Y_OFFSET -77
#define ACC_Z_OFFSET 1556
#define GYRO_X_OFFSET -2

// ===================== PID PARAMETERS ======================
// Tune in this order: Kp -> Kd -> Ki
float Kp = 25;
float Ki = 10;
float Kd = 0.3;

float targetAngle = 0;

// Minimum PWM to overcome motor dead zone
#define MOTOR_MIN_PWM 30

// ===================== STATE ===============================
int16_t accY, accZ, gyroX;
float accAngle, currentAngle;
float prevAngle = 0;
float errorSum = 0;
float motorPower;

unsigned long prevTime;
int loopCount = 0;
#define LOOP_INTERVAL 5  // ms between PID updates

// ===================== SETUP ===============================
void setup() {
  Serial.begin(115200);

  // Motor pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // MPU-6050 init
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1
  Wire.write(0x00);  // Wake up
  Wire.endTransmission();

  // Set accelerometer to ±2g
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission();

  // Set gyroscope to ±250 deg/s
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission();

  prevTime = millis();

  Serial.println("Self-Balancing Robot");
  delay(1000);
}

// ===================== SENSOR READ =========================
void readMPU() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  int16_t rawAccX = Wire.read() << 8 | Wire.read();  // skip
  accY = (Wire.read() << 8 | Wire.read()) - ACC_Y_OFFSET;
  accZ = (Wire.read() << 8 | Wire.read()) - ACC_Z_OFFSET;
  int16_t rawTemp = Wire.read() << 8 | Wire.read();  // skip
  gyroX = (Wire.read() << 8 | Wire.read()) - GYRO_X_OFFSET;
}

// ===================== MOTOR CONTROL =======================
void setMotors(int pwmA, int pwmB) {
  // Apply dead zone compensation — if PID wants to move, ensure
  // we send at least enough PWM to actually turn the motors
  if (pwmA > 0 && pwmA < MOTOR_MIN_PWM) pwmA = MOTOR_MIN_PWM;
  if (pwmA < 0 && pwmA > -MOTOR_MIN_PWM) pwmA = -MOTOR_MIN_PWM;
  if (pwmB > 0 && pwmB < MOTOR_MIN_PWM) pwmB = MOTOR_MIN_PWM;
  if (pwmB < 0 && pwmB > -MOTOR_MIN_PWM) pwmB = -MOTOR_MIN_PWM;

  // Right motor (A)
  if (pwmA >= 0) {
    digitalWrite(AIN1, LOW);
    analogWrite(AIN2, constrain(pwmA, 0, 255));
  } else {
    digitalWrite(AIN2, LOW);
    analogWrite(AIN1, constrain(-pwmA, 0, 255));
  }

  // Left motor (B) — direction flipped to match right motor
  if (pwmB >= 0) {
    digitalWrite(BIN2, LOW);
    analogWrite(BIN1, constrain(pwmB, 0, 255));
  } else {
    digitalWrite(BIN1, LOW);
    analogWrite(BIN2, constrain(-pwmB, 0, 255));
  }
}

void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

// ===================== MAIN LOOP ===========================
void loop() {
  unsigned long currTime = millis();
  if (currTime - prevTime < LOOP_INTERVAL) return;

  float dt = (currTime - prevTime) / 1000.0;
  prevTime = currTime;

  readMPU();

  // Complementary filter
  accAngle = atan2(accY, accZ) * 180.0 / PI;
  float gyroRate = gyroX / 131.0;
  currentAngle = 0.9934 * (currentAngle + gyroRate * dt) + 0.0066 * accAngle;

  // PID
  float error = currentAngle - targetAngle;
  errorSum += error * dt;
  errorSum = constrain(errorSum, -100, 100);  // anti-windup

  motorPower = Kp * error + Ki * errorSum + Kd * (currentAngle - prevAngle) / dt;
  prevAngle = currentAngle;

  // Safety cutoff — if tipped too far, stop and reset
  if (abs(currentAngle) > 30) {
    stopMotors();
    errorSum = 0;
    currentAngle = accAngle;
    prevAngle = accAngle;
    return;
  }

  // Drive motors
  setMotors(motorPower, motorPower);

  // Debug output — only every 20th loop (~10Hz instead of 200Hz)
  loopCount++;
  if (loopCount >= 20) {
    loopCount = 0;
    Serial.print(currentAngle, 1);
    Serial.print('\t');
    Serial.print(error, 1);
    Serial.print('\t');
    Serial.println((int)motorPower);
  }
}
