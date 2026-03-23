#include <Wire.h>
#include <math.h>

#define MPU_ADDR 0x68

int16_t accY, accZ, gyroX;
float accAngle, gyroAngle = 0, currentAngle = 0;
unsigned long prevTime;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Wake up MPU-6050 (it starts in sleep mode)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0x00);  // Wake up
  Wire.endTransmission();

  Serial.println("MPU-6050 Angle Test");
  Serial.println("Tilt the robot forward/backward and watch the angle change.");
  Serial.println("Hold it still at startup to set the zero point.");
  Serial.println();

  prevTime = millis();
  delay(100);
}

void readMPU() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // Starting register for accel data
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  int16_t accX = Wire.read() << 8 | Wire.read();  // Not used but must read
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
  int16_t temp = Wire.read() << 8 | Wire.read();   // Not used but must read
  gyroX = Wire.read() << 8 | Wire.read();
  // Skip gyroY and gyroZ (we don't need them)
}

void loop() {
  unsigned long currTime = millis();
  float dt = (currTime - prevTime) / 1000.0;
  prevTime = currTime;

  readMPU();

  // Angle from accelerometer
  accAngle = atan2(accY, accZ) * 180.0 / PI;

  // Angle from gyroscope (integrate angular velocity)
  float gyroRate = gyroX / 131.0;  // 131 LSB per deg/s at default sensitivity
  gyroAngle = gyroAngle + gyroRate * dt;

  // Complementary filter — combines both for stable reading
  currentAngle = 0.9934 * (currentAngle + gyroRate * dt) + 0.0066 * accAngle;

  Serial.print("Acc: ");
  Serial.print(accAngle, 1);
  Serial.print("  Gyro: ");
  Serial.print(gyroAngle, 1);
  Serial.print("  Filtered: ");
  Serial.println(currentAngle, 1);

  delay(10);
}