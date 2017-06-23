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

    mraa_gpio_context dev = mraa_gpio_init(62);
    if(dev == NULL) {
        printf("Unable to init gpio\n");
    }

    if(mraa_gpio_dir(dev, MRAA_GPIO_OUT) != MRAA_SUCCESS) {
        printf("Unable to set direction of the GPIO pin\n");
    }

//    int value = mraa_gpio_read(dev);
//    printf("Value read: %d\n", value);

    int counter = 0;
    while(1) {
        if(counter%2 == 0)
            mraa_gpio_write(dev, 1);
        else
            mraa_gpio_write(dev, 0);

        counter++;
        printf("counter: %d\n", counter);
        k_busy_wait(500000);
    }

    return 0;
}
