/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
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

#include <malloc.h>
#include <misc/util.h>
#include <mraa/aio.h>
#include <pinmux.h>
#include <sys_clock.h>
#include <zephyr.h>

#define SLEEPTICKS SECONDS(1)
#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT printf
#else
#include <misc/printk.h>
#define PRINT printk
#endif

void
main(void)
{
    struct nano_timer timer;
    void* timer_data[1];
    nano_timer_init(&timer, timer_data);
    // Calling the MRAA init function which initializes the board specific details
    if (mraa_init() != MRAA_SUCCESS) {
        printf("Unable to init MRAA successfully\n");
    }
    // Initializing the MRAA AIO layer
    mraa_aio_context dev = mraa_aio_init(0);
    int val;
    for (int i = 0; i < 10; i++) {
        val = mraa_aio_read(dev);
        printf("AIO Value: %d\n", val);
        // adding a delay of 1 sec between consecutive reads
        nano_timer_start(&timer, SLEEPTICKS);
        nano_timer_test(&timer, TICKS_UNLIMITED);
    }
    // increasing to 12 bits, it is 10 bits by default
    if (mraa_aio_set_bit(dev, 12) != MRAA_SUCCESS) {
        printf("Unable to set bits to 12\n");
    }
    for (int i = 0; i < 10; i++) {
        val = mraa_aio_read(dev);
        printf("AIO Value: %d\n", val);
        // adding a delay of 1 sec between consecutive reads
        nano_timer_start(&timer, SLEEPTICKS);
        nano_timer_test(&timer, TICKS_UNLIMITED);
    }
    if (mraa_aio_close(dev) != MRAA_SUCCESS) {
        printf("error closing analog port\n");
    }
}
