Title: AIO Read

Description:

A simple Analog Read application.
--------------------------------------------------------------------------------

Building and Running Project:

This nanokernel project outputs values read in from the the A0 pin onto the
console. The project employs the MRAA AIO layer in order to read these values.
The first 10 values it reads are of 10 bits and the following 10 are 12 bits.

Running it:

make pristine

make BOARD=arduino_101_sss_factory

If using DFU-UTIL:
dfu-util -a sensor_core -D outdir/zephyr.bin -R

Or if using JTAG/Flyswatter:
make BOARD=arduino_101_sss_factory flash

--------------------------------------------------------------------------------

