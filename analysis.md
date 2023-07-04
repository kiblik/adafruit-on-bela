This document is trying to summarise steps performed to create working implementation.

# Facts

**Used sensor:** MLX90393
**Address of sensor:** 0x0c ()


# Prechecks

## Goal: Sensor is working with the official library

### Description

We wanted to test if our sensor is not broken and it is returning data as expected.
For tests, we used [MLX90393](https://www.adafruit.com/product/4022) with the official libraries:
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

## Goal: Sensor communicate with Linux-based system over I2C

For test we choose RaspPI. 

<details>
<summary>Setup</summary>

![RaspPI connected to MLX90393 via I2C](./pics/raspPi.jpg)

</details>

<details>
<summary>List buses</summary>

```shell
$ i2cdetect -l
i2c-1	i2c       	bcm2835 (i2c@7e804000)          	I2C adapter
i2c-2	i2c       	bcm2835 (i2c@7e805000)          	I2C adapter
```

</details>

<details>
<summary>Test i2c capabilities</summary>

```shell
$ i2cdetect -F 1
Functionalities implemented by /dev/i2c-1:
I2C                              yes
SMBus Quick Command              yes
SMBus Send Byte                  yes
SMBus Receive Byte               yes
SMBus Write Byte                 yes
SMBus Read Byte                  yes
SMBus Write Word                 yes
SMBus Read Word                  yes
SMBus Process Call               yes
SMBus Block Write                yes
SMBus Block Read                 no
SMBus Block Process Call         no
SMBus PEC                        yesd
I2C Block Write                  yes
I2C Block Read                   yes
```

</details>

_Note:_ RaspPI supports "SMBus Quick Command" on the used bus. But it is not supported on Bela. It might be the reason of non-working communication.

<details>
<summary>Test regular detection - works</summary>

```shell
$ i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- 0c -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                         
```

</details>

<details>
<summary>Test "quick write" detection - works</summary>

```shell
$ i2cdetect -y -q 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- 0c -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                                               
```

</details>

<details>
<summary>Test "receive byte" detection - does not work</summary>

```shell
$ i2cdetect -y -r 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                                                                
```

</details>


<details>
<summary>Test basic functionality - works</summary>

These lines are trying to run [2 first commands performed by official library](https://github.com/adafruit/Adafruit_MLX90393_Library/blob/2ac6883873a69d45c45d4371c26a4fc7a81d7777/Adafruit_MLX90393.cpp#L72-L78
).

- Controler: `0x80` => `MLX90393_REG_EX` (enter ExitMode)
- Sensor: `0x01` => `0x01 >> 2 == MLX90393_STATUS_OK`
- Controler: `0xF0` => `MLX90393_REG_RT` (soft reset)
- Sensor: `0x05` => `0x05 >> 2 == MLX90393_STATUS_RESET` (just restarted)

```shell
$ i2ctransfer -v -y 1 w1@0x0c 0x80 r1@0x0c
msg 0: addr 0x0c, write, len 1, buf 0x80
msg 1: addr 0x0c, read, len 1, buf 0x01
$ i2ctransfer -v -y 1 w1@0x0c 0xF0 r1@0x0c
msg 0: addr 0x0c, write, len 1, buf 0xf0
msg 1: addr 0x0c, read, len 1, buf 0x05
```

</details>