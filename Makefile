ccflags-y +=-I$(srctree)/lib/mraa/include
lib-$(CONFIG_MRAA) := source/mraa.o
lib-$(CONFIG_MRAA_GPIO) += source/gpio.o
lib-$(CONFIG_MRAA_GPIO) += source/i2c.o
lib-$(CONFIG_BOARD_ARDUINO_101) += source/arduino_101.o
lib-$(CONFIG_BOARD_ARDUINO_101_SSS) += source/arduino_101_sss.o
lib-$(CONFIG_BOARD_QUARK_D2000_CRB) += source/intel_d2k_crb.o
