/*
 * Author: Henry Bruce <henry.bruce@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mraa/gpio.h"


static void
print_help()
{
    printf("list              List pins\n");
    printf("set pin level     Set pin to level (0/1)\n");
    printf("get pin           Get pin level\n");
    printf("monitor pin [off] Monitor pin level changes\n");
    printf("version           Get mraa version and board name\n");
}

static int
list_platform_pins(uint8_t platform_offset)
{
    int pin_count = mraa_get_platform_pin_count(platform_offset);
    int i;
    for (i = 0; i < pin_count; ++i) {
        int pin_id = platform_offset > 0 ? mraa_get_sub_platform_id(i) : i;
        char* pin_name = mraa_get_pin_name(pin_id);
        if (strcmp(pin_name, "INVALID") != 0) {
            printf("%02d ", pin_id);
            printf("%*s: ", (MRAA_PIN_NAME_SIZE - 1), pin_name);
            if (mraa_pin_mode_test(pin_id, MRAA_PIN_GPIO))
                printf("GPIO ");
            if (mraa_pin_mode_test(pin_id, MRAA_PIN_I2C))
                printf("I2C  ");
            if (mraa_pin_mode_test(pin_id, MRAA_PIN_SPI))
                printf("SPI  ");
            if (mraa_pin_mode_test(pin_id, MRAA_PIN_PWM))
                printf("PWM  ");
            if (mraa_pin_mode_test(pin_id, MRAA_PIN_AIO))
                printf("AIO  ");
            if (mraa_pin_mode_test(pin_id, MRAA_PIN_UART))
                printf("UART ");
            printf("\n");
        }
    }
    return pin_count;
}

static int
list_pins()
{
    int pin_count = 0;
    pin_count += list_platform_pins(MRAA_MAIN_PLATFORM_OFFSET);
    if (pin_count == 0) {
        printf("No Pins\n");
    }
    return pin_count;
}

static mraa_result_t
gpio_set(int pin, int level)
{
    mraa_gpio_context gpio = mraa_gpio_init(pin);
    if (gpio != NULL) {
        mraa_gpio_dir(gpio, MRAA_GPIO_OUT);
        mraa_gpio_write(gpio, level);
        return MRAA_SUCCESS;
    }
    return MRAA_ERROR_INVALID_RESOURCE;
}

static mraa_result_t
gpio_get(int pin, int* level)
{
    mraa_gpio_context gpio = mraa_gpio_init(pin);
    if (gpio != NULL) {
        mraa_gpio_dir(gpio, MRAA_GPIO_IN);
        *level = mraa_gpio_read(gpio);
        return MRAA_SUCCESS;
    }
    return MRAA_ERROR_INVALID_RESOURCE;
}

static void
gpio_isr_handler(void* args)
{
    mraa_gpio_context dev = (mraa_gpio_context)args;
    int level = mraa_gpio_read(dev);
    printf("Pin %d = %d\n", mraa_gpio_get_pin_raw(dev), level);
}

static mraa_result_t
gpio_isr_start(int pin)
{
    mraa_gpio_context dev = mraa_gpio_init(pin);
    if (dev != NULL) {
        mraa_result_t status = mraa_gpio_dir(dev, MRAA_GPIO_IN);
        if (status == MRAA_SUCCESS) {
            status = mraa_gpio_isr(dev, MRAA_GPIO_EDGE_BOTH, &gpio_isr_handler, dev);
        }
        return status;
    } else {
        return MRAA_ERROR_INVALID_RESOURCE;
    }
}

static mraa_result_t
gpio_isr_stop(int pin)
{
    mraa_gpio_context dev = mraa_gpio_init(pin);
    if (dev != NULL) {
        mraa_gpio_isr_exit(dev);
        mraa_gpio_close(dev);
    }
    return MRAA_SUCCESS;
}

void
gpio_cmd_help(int argc, char* argv[])
{
    print_help();
}

void
gpio_cmd_list(int argc, char* argv[])
{
    list_pins();
}

void
gpio_cmd_set(int argc, char* argv[])
{
    if (argc == 3) {
        int pin = atoi(argv[1]);
        if (gpio_set(pin, atoi(argv[2])) != MRAA_SUCCESS)
            printf("Could not set gpio %d\n", pin);
    } else {
        printf("Invalid command\n");
    }
}

void
gpio_cmd_get(int argc, char* argv[])
{
    if (argc == 2) {
        int pin = atoi(argv[1]);
        int level;
        if (gpio_get(pin, &level) == MRAA_SUCCESS) {
            printf("Pin %d = %d\n", pin, level);
        } else {
            printf("Could not read gpio %d\n", pin);
        }
    } else {
        printf("Invalid command\n");
    }
}

void
gpio_cmd_monitor(int argc, char* argv[])
{
    if (argc == 2) {
        int pin = atoi(argv[1]);
        if (gpio_isr_start(pin) == MRAA_SUCCESS) {
            printf("Monitoring level changes to pin %d\n", pin);
            // gpio_isr_handler(mraa_gpio_init(pin));
        } else {
            printf("Failed to register ISR for pin %d\n", pin);
        }
    } else if (argc == 3) {
        int pin = atoi(argv[1]);
        if (gpio_isr_stop(pin) == MRAA_SUCCESS) {
            printf("Stopped monitoring level changes to pin %d\n", pin);
        } else {
            printf("Failed to deregister ISR for pin %d\n", pin);
        }
    } else {
        printf("Invalid command\n");
    }
}

