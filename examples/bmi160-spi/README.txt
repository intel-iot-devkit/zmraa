Title: Synchronisation

Description:

A simple SPI example using the SPI API from ZMRAA.

--------------------------------------------------------------------------------

Building and Running Project:

This nanokernel project outputs to the console the chip ID that it reads from
the onboard BMI160 on the Arduino 101.  It can be built and executed
on as follows:

    make BOARD=arduino_101_sss_factory
    dfu-util -a sensor_core -D outdir/zephyr.bin -R

