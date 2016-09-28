Title: Synchronisation

Description:

A simple UART program which runs on the microkernel.

--------------------------------------------------------------------------------

Building and Running Project:

This UART project runs with both Zephyr v1.4 and v1.5. The outputs would
differ slightly on both. On 1.4 you would be able to see the output from the
nanokernel side as well, however on v1.5 you would only see some scrambled
characters from the nanokernel side. In order to run the project:

V1.4:
In the Makefile for the project change the config file to point to the
1.4 config file:
CONF_FILE = prj_1.4.conf

V1.5:
Change the config file to point to the 1.5 config file:
CONF_FILE = prj_1.5.conf

make pristine

make BOARD=arduino_101_factory

if using DFU-UTIL:
dfu-util -a x86_app -D outdir/zephyr.bin -R

if using JTAG/Flyswatter:
make BOARD=arduino_101_factory flash

--------------------------------------------------------------------------------

Sample Output:

V1.4:
ipm_console0: 'Hello World!'
Kernel: 1.4
Baudrate: 9600

V1.5
(scrambled characters)
Kernel: 1.5
Baudrate: 9600
