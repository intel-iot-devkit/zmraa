ZMRAA is an implementation of the MRAA API for Zephyr project.

Supported Devices
-----------------
* Arduino 101
* Quark D2000 CRB

Supported APIs
-----------------
* GPIO
* I2C

Adding ZMRAA to Zephyr
----------------------

```
cd zephyr/ext/lib
git clone git@github.com:intel-iot-devkit/zmraa.git mraa
```

Then you need to apply [zmraa.patch](zmraa.patch) to add the configuration entries
```
cd $ZEPHYR_BASE
git am ext/lib/mraa/zmraa.patch
```

Build GPIO Example
------------------
Add mraa and gpio driver to your zephyr configuration and build sample.
Device Drivers -> GPIO Drivers -> QMSI GPIO driver
MRAA -> Mraa GPIO function support
If you use a Quark D2k board then switch BOARD=arduino_101 for
BOARD=quark_d2000_crb

```
source $ZEPHYR_BASE/zephyr-env.sh
cd ext/lib/mraa/examples/shell
make BOARD=arduino_101 menuconfig
```

Now build the sample.

```
make BOARD=arduino_101 
```
