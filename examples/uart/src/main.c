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

uint8_t buffer[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
uint8_t ret_buf[8] = {0};

void
main(void)
{
    if (mraa_init() != MRAA_SUCCESS) {
        printf("Unable to init MRAA");
    }
    mraa_uart_context dev = mraa_uart_init(1);
    mraa_uart_set_baudrate(dev, 9600);
    
    if (mraa_uart_write(dev, buffer, 8) != 8) {
        printf("Error occured while writing bytes to UART\n");
    }
    //printf("\n");

    mraa_boolean_t x = mraa_uart_data_available(dev, 100);
    int ret_len = 0;
    int i = 0;

    if(x == true) {
        ret_len = mraa_uart_read(dev, ret_buf, 8);
        printf("\nReturned length: %d and returned buffer: \n", ret_len);
        for (i = 0; i < 8; i++) {
            printf("%c", (char)ret_buf[i]);
        }
        printf("\n");
    } else {
        printf("Unable to read anything back from the fifo\n");
    }

    mraa_uart_stop(dev);
}
