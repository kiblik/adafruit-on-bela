This document is trying to summarise steps performed to create working implementation.

# Prechecks

## Goal: Sensor is working with oficial library

### Description

We wanted to test if our sensor is not broken and it is returning data as expected.
For tests, we used [MLX90393](https://www.adafruit.com/product/4022) with oficial libraries:
- [Adafruit MLX90393](https://github.com/adafruit/Adafruit_MLX90393_Library) version `2.0.4`
  - [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO) version `1.14.2`
  - [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor) version `1.1.10`
  - Lib [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) was download by Arduino IDE as well but is was not used during testing (lib is necessary only if you use display for testing; we used only serial port for debug messages)

Provided example [basicdemo.ino](https://github.com/adafruit/Adafruit_MLX90393_Library/blob/master/examples/basicdemo/basicdemo.ino) was used for testing.

<details>
<summary>Setup</summary>

![Arduino connected to MLX90393 via I2C](./pics/arduino.jpg)

</details>

<details>
<summary>Example output</summary>

```
Starting Adafruit MLX90393 Demo
Found a MLX90393 sensor
Gain set to: 2.5 x
X: -9.0120 uT
Y: -12.0160 uT
Z: 37.5100 uT
X: -9.01 	Y: -12.02 	Z: 37.51 uTesla 
```

</details>

**Result:** Sensor works
