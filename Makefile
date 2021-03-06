ccflags-y +=-I$(srctree)/ext/lib/mraa/include
ccflags-y +=-I$(ZEPHYR_BASE)/drivers

obj-$(CONFIG_MRAA) := source/mraa.o
obj-$(CONFIG_MRAA_GPIO) += source/gpio.o
obj-$(CONFIG_MRAA_I2C) += source/i2c.o
obj-$(CONFIG_MRAA_PWM) += source/pwm.o
obj-$(CONFIG_MRAA_AIO) += source/aio.o
obj-$(CONFIG_MRAA_UART) += source/uart.o
obj-$(CONFIG_MRAA_SPI) += source/spi.o
obj-$(CONFIG_BOARD_ARDUINO_101) += source/arduino_101.o
obj-$(CONFIG_BOARD_ARDUINO_101_SSS) += source/arduino_101_sss.o
obj-$(CONFIG_BOARD_QUARK_D2000_CRB) += source/intel_d2k_crb.o
obj-$(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD) += source/quark_se_devboard.o
obj-$(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD_SS) += source/quark_se_sss_devboard.o
obj-$(CONFIG_BOARD_NUCLEO_L476RG) += source/stm32_nucleo_l476rg.o
