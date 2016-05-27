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
#include "mraa/i2c.h"
#include <i2c.h>
#include <sys_clock.h>
#include <misc/util.h>

#define SLEEPTICKS  SECONDS(1)

#define LM75B_ADDRESS                   (0x48)
#define LM75B_REGISTER_TEMPERATURE      (0x00)
#define LM75B_REGISTER_CONFIGURATION    (0x01)
#define LM75B_CONFIG_SHUTDOWN_MASK      (0x01)
#define LM75B_CONFIG_SHUTDOWN_POWERON   (0x00)
#define LM75B_CONFIG_SHUTDOWN_SHUTDOWN  (0x01)

static void
get_temperature(mraa_i2c_context i2c)
{
    mraa_i2c_address(i2c, LM75B_ADDRESS);
    mraa_result_t status = mraa_i2c_write_byte_data(i2c, LM75B_CONFIG_SHUTDOWN_POWERON, LM75B_REGISTER_CONFIGURATION);
    if (status != MRAA_SUCCESS) {
        printf("mraa_i2c_write_byte %x failed\n", LM75B_REGISTER_CONFIGURATION);
        return;
    }
    int regValue = mraa_i2c_read_word_data(i2c, LM75B_REGISTER_TEMPERATURE);
    if (regValue == -1) {
        printf("Failed to read temperature\n");
        return;
    }
    int temperature = regValue & 0xFF;
    printf("Temperature = %d\n", temperature);
}

void
main(void)
{
    struct nano_timer timer;
    void *timer_data[1];
    nano_timer_init(&timer, timer_data);

    printf("mbed application shield test\n");
    mraa_result_t status = mraa_init();
    if (status != MRAA_SUCCESS) {
        printf("mraa_i2c_init failed\n");
        return;
    }
    printf("mraa version %s on %s\n", mraa_get_version(), mraa_get_platform_name());
    mraa_i2c_context i2c = mraa_i2c_init(0);
    if (i2c == NULL) {
        printf("mraa_i2c_init failed\n");
        return;
    }

    while (1) {
        get_temperature(i2c);
        nano_timer_start(&timer, SLEEPTICKS);
        nano_timer_test(&timer, TICKS_UNLIMITED);
    }
}
