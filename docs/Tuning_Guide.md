# Self-Balancing Robot — Tuning Guide

## Prerequisites

### 1. Install Libraries

Copy these two folders from [jrowberg/i2cdevlib](https://github.com/jrowberg/i2cdevlib) into your Arduino libraries folder (`~/Documents/Arduino/libraries/`):

- `Arduino/I2Cdev`
- `Arduino/MPU6050`

The `i2cdevlib` directory in this repo already contains these — you can copy from there:

```
cp -r i2cdevlib/Arduino/I2Cdev ~/Documents/Arduino/libraries/
cp -r i2cdevlib/Arduino/MPU6050 ~/Documents/Arduino/libraries/
```

### 2. Calibrate MPU-6050 Offsets

Upload the `IMU_Zero` sketch from the MPU6050 library examples:

`File → Examples → MPU6050 → IMU_Zero`

1. Place the robot **flat and still** on a level surface
2. Open Serial Monitor at 9600 baud
3. Wait for it to converge (takes 5-10 minutes)
4. It outputs six offsets — you need three of them:
   - `AccY offset` → replace `ACC_Y_OFFSET` in the code
   - `AccZ offset` → replace `ACC_Z_OFFSET` in the code
   - `GyroX offset` → replace `GYRO_X_OFFSET` in the code

### 3. Find Your Target Angle

1. Set `Kp = 0`, `Ki = 0`, `Kd = 0` (all gains off)
2. Upload the sketch and open Serial Monitor
3. Hold the robot at its natural balance point — the angle where it would stay upright if you let go
4. Read the `Angle:` value — this is your `targetAngle`
5. It won't be exactly 0. Typical values: -2 to 5 degrees depending on weight distribution

## Tuning Procedure

### Step 1: Kp (Proportional)

This is the main correction force. Start here.

| Action | What happens |
|--------|-------------|
| Set `Kp = 20`, `Ki = 0`, `Kd = 0` | Robot should try to correct but oscillate wildly |
| Increase Kp (try 30, 40, 50...) | Stronger response, faster oscillation |
| Decrease Kp | Weaker response, slower to react |

**Goal:** Find the Kp where the robot tries hard to stay up but oscillates back and forth. Don't try to eliminate oscillation yet — that's what Kd is for.

**Typical range:** 20–80

### Step 2: Kd (Derivative)

This dampens oscillation. Add it once Kp is roughly set.

| Action | What happens |
|--------|-------------|
| Set `Kd = 0.5` | Oscillation should decrease |
| Increase Kd (try 1.0, 1.5, 2.0) | More dampening, smoother motion |
| Too much Kd | Robot feels sluggish, may vibrate/jitter |

**Goal:** Robot stays up but may slowly drift in one direction.

**Typical range:** 0.1–2.0

### Step 3: Ki (Integral)

This eliminates steady-state drift. Add it last and keep it small.

| Action | What happens |
|--------|-------------|
| Set `Ki = 0.5` | Should correct slow drift |
| Increase Ki (try 1.0, 2.0) | Faster drift correction |
| Too much Ki | Overshoots, oscillates slowly |

**Goal:** Robot balances and self-corrects when nudged.

**Typical range:** 0.5–5.0

## Troubleshooting

### Robot falls and wheels spin the wrong way
The motors are driving in the wrong direction relative to the tilt. Fix:
```cpp
// Change this line in loop():
setMotors(motorPower, motorPower);
// To:
setMotors(-motorPower, -motorPower);
```

### Robot oscillates violently
- Kp is too high — lower it
- Make sure your target angle is correct

### Robot balances briefly then slowly falls
- Ki is too low or zero — increase it slightly
- Double-check target angle — even 1° off matters

### Robot vibrates/jitters in place
- Kd is too high — lower it
- Serial.print can slow the loop — comment out debug prints once tuning is close

### Robot doesn't respond at all
- Check motor wiring and driver connections
- Verify MPU offsets are set (not still `YOUR_VALUE`)
- Make sure batteries are charged

### One motor is weaker than the other
You can add a trim factor:
```cpp
setMotors(motorPower * 1.0, motorPower * 0.95);  // reduce left by 5%
```

## Quick Reference

| Parameter | Start | Typical Range | Effect |
|-----------|-------|---------------|--------|
| Kp | 20 | 20–80 | Correction strength |
| Ki | 0 | 0.5–5.0 | Drift elimination |
| Kd | 0 | 0.1–2.0 | Oscillation dampening |
| targetAngle | 0 | -5 to 5 | Balance point |

## Testing Sequence

1. Power on, hold the robot upright
2. It should try to correct when you tilt it slightly
3. If motors go the wrong way → flip motor sign
4. If it oscillates → tune Kp/Kd
5. If it drifts → tune Ki and targetAngle
6. Once stable, comment out `Serial.print` lines for a tighter control loop
