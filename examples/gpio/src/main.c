#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mraa.h"
#include "mraa/gpio.h"
#include "device.h"
#include "misc/util.h"

int main() {
    if(mraa_init() != MRAA_SUCCESS) {
        printf("Unable to init mraa\n");
    }

    mraa_gpio_context dev = mraa_gpio_init(88);
    if(dev == NULL) {
        printf("Unable to init gpio\n");
    }

    if(mraa_gpio_dir(dev, MRAA_GPIO_IN) != MRAA_SUCCESS) {
        printf("Unable to set direction of the GPIO pin\n");
    }

    int value = mraa_gpio_read(dev);
    printf("Value read: %d\n", value);

    return 0;
}
