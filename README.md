# Arduino Self-Balancing Robot

![Self-Balancing Robot](gallery/final.gif)

A two-wheeled self-balancing robot built from scratch with Arduino Pro Mini, MPU6050 IMU, and PID control. Based on the [Instructables tutorial](https://www.instructables.com/Arduino-Self-Balancing-Robot-1/).

## Project Overview

This project involves building a 4" x 4" self-balancing robot that can move around while avoiding obstacles. The robot uses an Arduino Pro Mini, MPU6050 IMU sensor, and ultrasonic distance sensor to maintain balance and navigate.

## Parts List

### Core Components
1. **Arduino Pro Mini** - Main microcontroller
2. **GY-521 module with MPU-6050** - Accelerometer and gyroscope sensor
3. **DRV8833 Pololu motor driver** - Dual motor driver
4. **2x 5V boost converter modules** - Voltage regulators (separate for motors and controller)
5. **US-020 ultrasonic distance sensor** - Obstacle detection
6. **NCR18650 battery and holder** - Power supply
7. **Pair of micro metal gear motors (N20, 6V, 200 rpm) and brackets**
8. **Pair of 42x19mm wheels**
9. **3x Double-sided prototype PCB (4cm x 6cm)** - For building the three-layer structure
10. **8x 25cm Nylon spacers and 4x nylon nuts** - For spacing layers

### Additional Components
- Cables and wires
- Berg connectors
- On/off switch
- 10uF capacitor (for motor power supply)

## Theory: How Self-Balancing Works

The self-balancing robot is essentially an **inverted pendulum**. Unlike a normal pendulum, it cannot stay balanced on its own and will fall over. To balance it:

1. **Detect the fall direction** - Use sensors to determine which way the robot is tilting
2. **Drive wheels in the fall direction** - Move the wheels to catch up with the center of gravity
3. **Maintain equilibrium** - Keep the center of gravity above the pivot point (wheel contact)

The robot uses:
- **MPU6050** to measure tilt angle and angular velocity
- **PID controller** to generate motor control signals
- **Complementary filter** to combine accelerometer and gyroscope readings

## Hardware Assembly

### Three-Layer Structure

1. **Bottom Layer**: Motors and motor driver
2. **Middle Layer**: Arduino Pro Mini, MPU6050, and 5V boost regulator for controller
3. **Top Layer**: Battery, on/off switch, and ultrasonic sensor

### Important Design Notes

- **Separate voltage regulators**: Use two separate 5V boost converters - one for motors and one for controller/IMU. This prevents motor noise from interfering with the controller.
- **Capacitor**: Add a 10uF capacitor at the motor power supply terminals to filter noise.
- **Spacing**: Use 25mm nylon spacers between layers.

### Wiring Connections

#### Motor Driver (DRV8833)
- Left motor PWM: Pin 6
- Left motor direction: Pin 7
- Right motor PWM: Pin 5
- Right motor direction: Pin 4

#### MPU6050 (I2C)
- SDA: A4 (Arduino Pro Mini)
- SCL: A5 (Arduino Pro Mini)

#### Ultrasonic Sensor (US-020)
- Trigger: Pin 9
- Echo: Pin 8
- Vcc: 5V
- Gnd: GND

## Software Implementation

### Key Concepts

#### 1. Angle Measurement Using Accelerometer

The accelerometer measures acceleration along Y and Z axes. The angle is calculated using:

```cpp
accAngle = atan2(accY, accZ) * RAD_TO_DEG;
```

**Issue**: Horizontal acceleration interferes with readings during movement.

#### 2. Angle Measurement Using Gyroscope

The gyroscope measures angular rate. The angle is calculated by integrating:

```cpp
gyroRate = map(gyroX, -32768, 32767, -250, 250);
gyroAngle = gyroAngle + (float)gyroRate * loopTime / 1000;
```

**Issue**: Gyroscope drifts over time.

#### 3. Complementary Filter

Combines accelerometer and gyroscope readings to get stable, accurate angle:

```cpp
currentAngle = 0.9934 * (previousAngle + gyroAngle) + 0.0066 * (accAngle);
```

- **0.9934**: High-pass filter coefficient for gyroscope
- **0.0066**: Low-pass filter coefficient for accelerometer
- **Time constant**: 0.75 seconds

#### 4. PID Control

The PID controller generates motor output:

- **P (Proportional)**: Response proportional to angle error
- **I (Integral)**: Response based on accumulated error (handles steady-state error)
- **D (Derivative)**: Response based on rate of change (damping)

```cpp
motorPower = Kp * (error) + Ki * (errorSum) * sampleTime - Kd * (currentAngle - prevAngle) / sampleTime;
```

### PID Tuning Steps

1. Set Ki and Kd to zero, gradually increase Kp until robot oscillates
2. Increase Ki to improve response speed and reduce steady-state error
3. Increase Kd to reduce oscillations and overshoot
4. Fine-tune all parameters iteratively

### Default PID Constants

- **Kp**: 40
- **Ki**: 40
- **Kd**: 0.05
- **Sample Time**: 5ms (0.005 seconds)
- **Target Angle**: -2.5 degrees

### MPU6050 Calibration

Calibrate offsets to eliminate sensor errors:

```cpp
mpu.setYAccelOffset(1593);
mpu.setZAccelOffset(963);
mpu.setXGyroOffset(40);
```

**Note**: These values are specific to each sensor. Use calibration code to find your sensor's offsets.

### Timer Interrupt

The code uses Timer1 interrupt to create precise 5ms sampling intervals for PID control. This ensures consistent timing regardless of other code execution.

### Obstacle Avoidance

- Read ultrasonic sensor every 100ms
- If distance < 20cm, rotate the robot to avoid obstacle
- Rotation is achieved by reversing one motor

## Required Libraries

1. **I2Cdev** - For I2C communication with MPU6050
2. **MPU6050** - MPU6050 sensor library by Jeff Rowberg
3. **NewPing** - Ultrasonic sensor library

### Installation

Install via Arduino Library Manager or download from:
- I2Cdev: https://github.com/jrowberg/i2cdevlib
- MPU6050: Part of i2cdevlib
- NewPing: https://github.com/PaulStoffregen/NewPing

## Code Structure

The main code includes:

1. **Setup**: Initialize pins, MPU6050, and timer interrupt
2. **Main Loop**: Read sensors, set motor power, check for obstacles
3. **ISR (Interrupt Service Routine)**: Calculate angle, run PID control every 5ms

## Building Tips

1. **Physical Layout**: Draw component placement on paper before soldering
2. **Power Separation**: Critical to use separate regulators for motors and controller
3. **Calibration**: Always calibrate MPU6050 offsets for your specific sensor
4. **PID Tuning**: Be patient - tuning takes time and testing
5. **Size Matters**: Larger robots are easier to balance than small ones

## Troubleshooting

### Robot Freezes
- **Cause**: Motor noise interfering with controller
- **Solution**: Use separate voltage regulators and add capacitor

### Angle Readings Unstable
- **Cause**: Horizontal acceleration or gyro drift
- **Solution**: Adjust complementary filter coefficients

### Robot Oscillates Too Much
- **Cause**: Kp too high or Kd too low
- **Solution**: Reduce Kp, increase Kd

### Robot Doesn't Respond Fast Enough
- **Cause**: Ki too low
- **Solution**: Increase Ki gradually

## Project Files

- `README.md` - This guide
- `src/` - Arduino source code (to be added)
- `docs/` - Additional documentation (to be added)
- `parts/` - Parts list and sourcing information (to be added)

## Next Steps

1. Gather all required components
2. Assemble the three-layer structure
3. Wire all components according to the pin assignments
4. Install required Arduino libraries
5. Upload the code and calibrate MPU6050
6. Tune PID constants for your specific robot
7. Test obstacle avoidance functionality

## References

- [Original Instructables Tutorial](https://www.instructables.com/Arduino-Self-Balancing-Robot-1/)
- [MPU6050 Library Documentation](https://github.com/jrowberg/i2cdevlib)
- [NewPing Library](https://github.com/PaulStoffregen/NewPing)

## License

This project is based on the Instructables tutorial by midhun_s. Please refer to the original source for licensing information.

