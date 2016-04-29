ccflags-y +=-I$(srctree)/lib/mraa/include
lib-$(CONFIG_MRAA) := source/mraa.o
lib-$(CONFIG_MRAA_GPIO) += source/gpio.o
