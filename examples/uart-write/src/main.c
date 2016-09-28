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

#include "version.h"
#include <device.h>
#include <mraa/uart.h>
#include <string.h>
#include <sys_clock.h>
#include <zephyr.h>

#define UART_DEVICE "UART_1"
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
    if (mraa_init() != MRAA_SUCCESS) {
        printf("Unable to init MRAA");
    }
    mraa_uart_context dev = mraa_uart_init(1);
#if KERNELVERSION == 0x1040000
    if (mraa_uart_write(dev, "Kernel: 1.4\n", 12) != 12) {
        printf("Error occured while writing bytes to UART\n");
    }
    if (mraa_uart_write(dev, "Baudrate: 9600\n", 15) != 15) {
        printf("Error occured while writing bytes to UART\n");
    }
    // adding a delay of 100 ms
    nano_timer_start(&timer, MSEC(100));
    nano_timer_test(&timer, TICKS_UNLIMITED);
#elif KERNELVERSION >= 0x1050000
    // function only available with Zephyr 1.5 and above
    mraa_uart_set_baudrate(dev, 9600);
    if (mraa_uart_write(dev, "Kernel: 1.5\n", 12) != 12) {
        printf("Error occured while writing bytes to UART\n");
    }
    if (mraa_uart_write(dev, "Baudrate: 9600\n", 15) != 15) {
        printf("Error occured while writing bytes to UART\n");
    }
    // adding a delay of 100 ms
    nano_timer_start(&timer, MSEC(100));
    nano_timer_test(&timer, TICKS_UNLIMITED);
#endif
    mraa_uart_stop(dev);
}
