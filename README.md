ZMRAA is an implementation of the MRAA API for Zephyr project.

Releases
-----------------
* v1.1.0 - Support for Zephyr 1.7.  Added Quark SE C1000 Development board
  support.
* v1.0.0 - Initial release of ZMRAA targeting Zephyr 1.6
  Includes preliminary support for AIO, GPIO, I2C, PWM, SPI, and UART

Supported Devices
-----------------
* Arduino 101
* Quark D2000 CRB
* Quark SE C1000 Development Board

Supported APIs
-----------------
* GPIO
* I2C
* AIO
* PWM
* I2C
* SPI

Protocols per Core: Arduino 101
-------------------------------
In general almost every protocol mentioned above is present on each core.
However, that stops being the case when you look at it from the POV of what's
present on the Arduino headers. The following table lists the protocols that
are present per core and exposed via the headers:

| Protocol |           ARC                  |           QUARK                |
|----------|--------------------------------|--------------------------------|
|   AIO    | Present                        | Not present                    |
|   GPIO   | Present                        | Present                        |
|   PWM    | Present                        | Present                        |
|   I2C    | Present                        | Not present                    |
|   UART   | Not present                    | Present                        |
|   SPI    | Present (Only internally)      | Present                        |

Pin Availability on Arduino 101 via ZMRAA
-----------------------------------------
On 101 there are at least 69 pins that can be configured. ZMRAA tries to provide
access to as many as possible. Most of the pins exposed on the headers of the
Arduino 101 can be accessed via both cores - Quark and ARC, but there are a
few pins and protocols that can be accessed from only one of the cores. To see
which protocol is available on which core please check the corresponding section.

A look at the pins accessible via the **Quark core:**
**GPIO** - D2-D5, D7, D8, D10-D13
**PWM** - D3, D5, D6, D9
**UART** - D0(RX), D1(TX)
**SPI** - D11(MOSI), D12(MISO), D13(SCK) | CS via GPIO

Pins accessible via the **ARC core:**
**GPIO** - D14-D19 (These are analog pins A0-A5 numbered 14-19)
*ex: mraa_gpio_init(14); in this case pin A0 would be used as a digital pin*
*    mraa_gpio_init(15); in this case it's pin A1 and so on and so forth*

**AIO** - A0-A5, D10-D13
*you can number the digital pins as they are: mraa_aio_init(10)*
*This initializes pin D10*

**PWM** - D3, D5, D6, D9
**I2C** - I2C pins on the board
**SPI** - Not exposed externally, but has access to the internal BMI160

*If a protocol and its corresponding pins have not been mentioned under a*
*particular core then that protocol is not available on that core.*

Adding ZMRAA to Zephyr
----------------------

```
cd zephyr/ext/lib
git clone https://github.com/intel-iot-devkit/zmraa.git mraa
```

Then you need to apply [zmraa.patch](zmraa.patch) to add the configuration entries
```
cd $ZEPHYR_BASE
git am ext/lib/mraa/zmraa.patch
```

ZMRAA Examples Note: Please change pin numbers accordingly based on platform.

ZMRAA AIO Example
-----------------
The ZMRAA AIO example needs special care when running on D2000. Please see following for more info

prj.conf AIO setting for D2000:
CONFIG_ADC_QMSI=y
CONFIG_ADC_QMSI_SS=n

prj.conf AIO setting for C1000_SS and A101_SS:
CONFIG_ADC_QMSI=n
CONFIG_ADC_QMSI_SS=y

ZMRAA Shell Example
-------------------
The ZMRAA shell example creates a simple Zephyr shell which provides a command set for interacting with ZMRAA GPIOs and I2C busses.  The ZMRAA shell is available on all supported board targets.

**Board targets:**
* `BOARD=arduino_101`
* `BOARD=arduino_101_sss`
* `BOARD=quark_d2000_crb`
* `BOARD=quark_se_c1000_devboard`
* `BOARD=quark_se_c1000_ss_devboard`

### Arduino 101
Build the image
```
source $ZEPHYR_BASE/zephyr-env.sh
cd ext/lib/mraa/examples/shell
make BOARD=arduino_101
```

[Flash the target](https://www.zephyrproject.org/doc/boards/x86/arduino_101/doc/board.html#flashing).  After resetting the target, a mraa shell will be available:
```
shell> ZMRAA v0.0.3 on Arduino 101 x86

mraa> gpio list
00     IO0: GPIO UART
01     IO1: GPIO UART
02     IO2: GPIO
03     IO3: GPIO PWM
...
```

### Quark D2000 CRB
Build the image
```
source $ZEPHYR_BASE/zephyr-env.sh
cd ext/lib/mraa/examples/shell
make BOARD=quark_d2000_crb
```

[Flash the target](https://www.zephyrproject.org/doc/boards/x86/quark_d2000_crb/doc/quark_d2000_crb.html#flashing).  After resetting the target, a mraa shell will be available:
```
shell> ZMRAA v0.0.3 on Intel D2000 CRB
mraa> gpio list
00     IO0: GPIO UART
01     IO1: GPIO UART
02     IO2: GPIO
03     IO3: GPIO
...
```

### Quark SE C1000 Development Board
Build the image
```
source $ZEPHYR_BASE/zephyr-env.sh
cd ext/lib/mraa/examples/shell
make BOARD=quark_se_c1000_devboard
```

Flash the target
```
make BOARD=quark_se_c1000_devboard flash
```

```
shell> ZMRAA v0.0.3 on Quark SE Devboard/C1000 x86
mraa> gpio list
00        :
01 VDD_HDR:
02 VDD_HDR:
03 AP_UART: GPIO UART
...
```
