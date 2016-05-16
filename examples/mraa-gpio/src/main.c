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
#include <misc/shell.h>
#include "mraa/gpio.h"

struct gpio_source {
    int pin;
    mraa_gpio_context context;
};

void
print_version()
{
    printf("Version %s on %s\n", mraa_get_version(), mraa_get_platform_name());
}

void
print_help()
{
    printf("list              List pins\n");
    printf("set pin level     Set pin to level (0/1)\n");
    printf("get pin           Get pin level\n");
    printf("monitor pin       Monitor pin level changes\n");
    printf("version           Get mraa version and board name\n");
}

void
print_command_error()
{
    printf("Invalid command, options are:\n");
    print_help();
}

int
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

int
list_pins()
{
    int pin_count = 0;
    pin_count += list_platform_pins(MRAA_MAIN_PLATFORM_OFFSET);
    if (pin_count == 0) {
        printf("No Pins\n");
    }
    return pin_count;
}

mraa_result_t
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

mraa_result_t
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

void
gpio_isr_handler(void* args)
{
    struct gpio_source* gpio_info = (struct gpio_source*) args;
    int level = mraa_gpio_read(gpio_info->context);
    printf("Pin %d = %d\n", gpio_info->pin, level);
}

mraa_result_t
gpio_isr_start(struct gpio_source* gpio_info)
{
    // printf("gpio_isr_start\n");
    gpio_info->context = mraa_gpio_init(gpio_info->pin);
    if (gpio_info->context != NULL) {
        mraa_result_t status = mraa_gpio_dir(gpio_info->context, MRAA_GPIO_IN);
        if (status == MRAA_SUCCESS) {
            status = mraa_gpio_isr(gpio_info->context, MRAA_GPIO_EDGE_BOTH, &gpio_isr_handler, gpio_info);
        }
        return status;
    } else {
        return MRAA_ERROR_INVALID_RESOURCE;
    }
}

mraa_result_t
gpio_isr_stop(struct gpio_source* gpio_info)
{
    mraa_gpio_isr_exit(gpio_info->context);
    mraa_gpio_close(gpio_info->context);
    return MRAA_SUCCESS;
}

static void
shell_cmd_help(int argc, char* argv[])
{
    print_help();
}

static void
shell_cmd_version(int argc, char* argv[])
{
    print_version();
}

static void
shell_cmd_list(int argc, char* argv[])
{
    list_pins();
}

static void
shell_cmd_set(int argc, char* argv[])
{
    if (argc == 3) {
        int pin = atoi(argv[1]);
        if (gpio_set(pin, atoi(argv[2])) != MRAA_SUCCESS)
            printf("Could not set gpio %d\n", pin);
    } else {
        printf("Invalid command\n");
    }
}

static void
shell_cmd_get(int argc, char* argv[])
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

static void
shell_cmd_monitor(int argc, char* argv[])
{
    if (argc == 2) {
        // printf("shell_cmd_monitor\n");
        int pin = atoi(argv[1]);
        struct gpio_source gpio_info;
        gpio_info.pin = pin;
        if (gpio_isr_start(&gpio_info) == MRAA_SUCCESS) {
            printf("Monitoring level changes to pin %d\n", pin);
            gpio_isr_handler(&gpio_info);
            // gpio_isr_stop(&gpio_info);
        } else {
            printf("Failed to register ISR for pin %d\n", pin);
        }
    } else {
        printf("Invalid command\n");
    }
}


void
main(void)
{
	const struct shell_cmd commands[] = {
        { "syntax", shell_cmd_help },
		{ "version", shell_cmd_version },
		{ "list", shell_cmd_list },
		{ "set", shell_cmd_set },
        { "get", shell_cmd_get },
        { "monitor", shell_cmd_monitor },
		{ NULL, NULL }
	};

    mraa_result_t status = mraa_init();
    if (status == MRAA_SUCCESS) {
        print_version();
        shell_init("mraa> ", commands);
    } else {
        printf("mraa_init() failed with error code %d\n", status);
    }

#if 0
    if (argc == 1) {
        print_command_error();
    }

    if (argc > 1) {
        if (strcmp(argv[1], "list") == 0) {
            list_pins();
        } else if (strcmp(argv[1], "help") == 0) {
            print_help();
        } else if (strcmp(argv[1], "version") == 0) {
            print_version();
        } else if ((strcmp(argv[1], "set") == 0)) {
            if (argc == 4) {
                int pin = atoi(argv[2]);
                if (gpio_set(pin, atoi(argv[3])) != MRAA_SUCCESS)
                    printf("Could not initialize gpio %d\n", pin);
            } else {
                print_command_error();
            }
        } else if ((strcmp(argv[1], "get") == 0)) {
            if (argc == 3) {
                int pin = atoi(argv[2]);
                int level;
                if (gpio_get(pin, &level) == MRAA_SUCCESS) {
                    printf("Pin %d = %d\n", pin, level);
                } else {
                    printf("Could not initialize gpio %d\n", pin);
                }
            } else {
                print_command_error();
            }
        } else if (strcmp(argv[1], "monitor") == 0) {
            if (argc == 3) {
                int pin = atoi(argv[2]);
                struct gpio_source gpio_info;
                gpio_info.pin = pin;
                if (gpio_isr_start(&gpio_info) == MRAA_SUCCESS) {
                    printf("Monitoring level changes to pin %d. Press RETURN to exit.\n", pin);
                    gpio_isr_handler(&gpio_info);
                    // gpio_isr_stop(&gpio_info);
                } else {
                    printf("Failed to register ISR for pin %d\n", pin);
                }
            } else {
                print_command_error();
            }
        } else {
            print_command_error();
        }
    }
#endif
}
