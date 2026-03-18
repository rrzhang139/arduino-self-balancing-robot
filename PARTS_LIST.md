# Parts List and Sourcing

## Required Components

### Microcontroller & Sensors
| Component | Quantity | Notes |
|-----------|----------|-------|
| Arduino Pro Mini | 1 | 5V version recommended |
| GY-521 module (MPU-6050) | 1 | Accelerometer + Gyroscope |
| US-020 ultrasonic sensor | 1 | 4-pin version (Vcc, Trig, Echo, Gnd) |

### Motor Control
| Component | Quantity | Notes |
|-----------|----------|-------|
| DRV8833 Pololu motor driver | 1 | Dual H-bridge motor driver |
| Micro metal gear motors (N20) | 2 | 6V, 200 RPM, with brackets |
| Wheels (42x19mm) | 2 | Matching diameter for motors |

### Power System
| Component | Quantity | Notes |
|-----------|----------|-------|
| 5V boost converter | 2 | Separate for motors and controller |
| NCR18650 battery | 1 | Lithium-ion, 3.7V nominal |
| Battery holder | 1 | For 18650 battery |
| On/off switch | 1 | Power switch |

### Mechanical Parts
| Component | Quantity | Notes |
|-----------|----------|-------|
| Double-sided prototype PCB | 3 | 4cm x 6cm each |
| Nylon spacers (25mm) | 8 | For layer spacing |
| Nylon nuts | 4 | For securing spacers |

### Electronic Components
| Component | Quantity | Notes |
|-----------|----------|-------|
| 10uF capacitor | 1 | For motor power supply filtering |
| Jumper wires | Various | For connections |
| Berg connectors | Various | For secure connections |

## Sourcing Information

### Where to Buy
- **AliExpress** - Budget-friendly, longer shipping
- **Amazon** - Faster shipping, higher prices
- **Pololu** - Official motor driver source
- **SparkFun/Adafruit** - Quality components, good documentation
- **Local electronics stores** - Immediate availability

### Component Specifications

#### Arduino Pro Mini
- Operating voltage: 5V
- Input voltage: 5-12V (via regulator)
- Digital I/O pins: 14
- Analog input pins: 6

#### MPU6050 (GY-521)
- 3-axis accelerometer (±2g, ±4g, ±8g, ±16g)
- 3-axis gyroscope (±250, ±500, ±1000, ±2000°/s)
- I2C interface
- Operating voltage: 3.3V or 5V

#### DRV8833 Motor Driver
- Dual H-bridge
- Voltage: 2.7V to 10.8V
- Current: 1.5A per channel
- PWM control

#### N20 Motors
- Voltage: 6V
- Speed: 200 RPM (no load)
- Gear ratio: 30:1 (typical)
- Shaft diameter: 3mm

## Estimated Cost

- **Budget build**: $30-50 (AliExpress)
- **Mid-range**: $50-80 (Mix of sources)
- **Premium**: $80-120 (Quality components)

## Alternative Components

### If Arduino Pro Mini unavailable:
- Arduino Nano (larger, but compatible)
- ESP32 (more powerful, WiFi/Bluetooth)

### If DRV8833 unavailable:
- L298N (older, larger, but works)
- TB6612FNG (similar to DRV8833)

### If MPU6050 unavailable:
- MPU9250 (includes magnetometer)
- BNO055 (9-DOF sensor with sensor fusion)

## Tools Required

- Soldering iron and solder
- Wire strippers
- Multimeter
- Hot glue gun (optional, for securing components)
- Small screwdrivers
- Breadboard (for initial testing)

