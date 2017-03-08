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
#include <zephyr.h>

#define SLEEPTIME 1000
#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT printf
#else
#include <misc/printk.h>
#define PRINT printk
#endif

void
mraa_sleep_ms(int ms)
{
    struct k_timer timer;
    k_timer_init(&timer, NULL, NULL);
    k_timer_start(&timer, ms, 0);
    k_timer_status_sync(&timer);
}

void
main(void)
{
    // Wait 1 sec before starting
    mraa_sleep_ms(500);

    // Calling the MRAA init function which initializes the board specific details
    if (mraa_init() == MRAA_SUCCESS) {
        printf("Detected %s\n", mraa_get_platform_name());
    } else {
        printf("Unable to init MRAA successfully\n");
        return;
    }
    // Initializing the MRAA AIO layer
    mraa_aio_context dev = mraa_aio_init(0);
    if (dev == NULL) {
        printf("Unable to initialize AIO\n");
        return;
    }
    printf("Reading %d bit values\n", mraa_aio_get_bit(dev));

     while (1) {
        int val = mraa_aio_read(dev);
        printf("AIO Value: %d\n", val);
        // adding a delay of 1 sec between consecutive reads
        mraa_sleep_ms(SLEEPTIME);
    }
    if (mraa_aio_close(dev) != MRAA_SUCCESS) {
        printf("error closing analog port\n");
    }
}
