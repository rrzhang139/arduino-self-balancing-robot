#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

MPU6050 mpu;

// ----- parameters you can tune -----
const int WINDOW_SIZE = 20;        // averaging window
const float SPIKE_THRESH = 0.8;    // degrees deviation
const int SAMPLE_DELAY = 10;       // ms between samples
// ----------------------------------

float window[WINDOW_SIZE];
int idx = 0;
bool filled = false;

int16_t ay, az;

float computeAverage() {
  float sum = 0;
  int count = filled ? WINDOW_SIZE : idx;
  for (int i = 0; i < count; i++) sum += window[i];
  return sum / count;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  Serial.println("MPU average + spike detector running");
}

void loop() {
  // read accelerometer
  ay = mpu.getAccelerationY();
  az = mpu.getAccelerationZ();

  float angle = atan2(ay, az) * 180 / PI;

  // update sliding window
  window[idx] = angle;
  idx++;
  if (idx >= WINDOW_SIZE) {
    idx = 0;
    filled = true;
  }

  float avg = computeAverage();
  float diff = angle - avg;

  // interpret
  if (abs(diff) > SPIKE_THRESH) {
    if (diff > 0)
      Serial.println("SPIKE UP  (sudden forward tilt)");
    else
      Serial.println("SPIKE DOWN (sudden backward tilt)");
  } else {
    Serial.println("STABLE");
  }

  delay(SAMPLE_DELAY);
}
