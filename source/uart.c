/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
 *                Brendan le Foll <brendan.le.foll@intel.com>
 *                Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014 - 2015 Intel Corporation.
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

#include "mraa_internal.h"
#include "mraa_internal_types.h"
#include "version.h"
#include <device.h>
#include <pinmux.h>
#include <string.h>
#include <sys_clock.h>
#include <uart.h>
#include <zephyr.h>

#include "mraa/uart.h"

#if defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_ARDUINO_101)
#define UART_DEVICE "UART_1"
#elif defined(CONFIG_BOARD_QUARK_D2000_CRB)
#define UART_DEVICE "UART_0"
#endif

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT printf
#else
#include <misc/printk.h>
#define PRINT printk
#endif

mraa_uart_context
mraa_uart_init(int uart)
{

    mraa_board_t* board = plat;

    if (board == NULL) {
        printf("uart: platform not initialised\n");
        return NULL;
    }

    struct device* pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    if (pinmux_dev == NULL) {
        printf("Failed to get binding for pinmux\n");
        return NULL;
    }

#if defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_ARDUINO_101)
    if (uart == 0) {
        pinmux_pin_set(pinmux_dev, 17, PINMUX_FUNC_C);
        pinmux_pin_set(pinmux_dev, 16, PINMUX_FUNC_C);
        mraa_set_pininfo(board, 0, 17, "IO0", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 1 });
        mraa_set_pininfo(board, 1, 16, "IO1", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 1 });
    }
#endif

    mraa_uart_context dev = (mraa_uart_context) malloc(sizeof(struct _uart));
    dev->zdev = device_get_binding(UART_DEVICE);
    dev->block = 1;

    return dev;
}

mraa_uart_context
mraa_uart_init_raw(const char* path)
{
    return NULL;
}

int
mraa_uart_write(mraa_uart_context dev, const char* buf, size_t length)
{
    // this isn't the expected implementation, it should be length
    // instead of length+1. However, the output gets completely
    // screwed up with the actual length. Need to look into this
    // to find RC and fix. Current implementation works though.
    unsigned char ret[length + 1];
    unsigned char temp;
    int i = 0;
    for (i = 0; i < length + 1; i++) {
        temp = buf[i];
        ret[i] = uart_poll_out(dev->zdev, temp);
        if (ret[i] != temp) {
            return i;
        }
    }

    return length;
}

mraa_result_t
mraa_uart_get_dev_path(mraa_uart_context dev)
{
    return MRAA_ERROR_FEATURE_NOT_SUPPORTED;
}

// you have to set the uart line control flag in the config file
// to be able to use this function
mraa_result_t
mraa_uart_set_baudrate(mraa_uart_context dev, unsigned int baud)
{
#if KERNELVERSION >= 0x1050000
    int ret = uart_line_ctrl_set(dev->zdev, LINE_CTRL_BAUD_RATE, baud);
    if (ret == 0) {
        return MRAA_SUCCESS;
    } else {
        return MRAA_ERROR_UNSPECIFIED;
    }
#else
    return MRAA_ERROR_FEATURE_NOT_SUPPORTED;
#endif
}

// to be set from config file for now, maybe in the future if the line control
// implements this functionality, then this can be implemented
mraa_result_t
mraa_uart_set_flowcontrol(mraa_uart_context dev, mraa_boolean_t xonxoff, mraa_boolean_t rtscts)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

int
mraa_uart_read(mraa_uart_context dev, char* buf, size_t length)
{
    unsigned char store;
    int ret, i = 0;
    for (i = 0; i < length; i++) {
        ret = uart_poll_in(dev->zdev, &store);
        if (ret == -1) {
            return i;
        } else if (ret == 0) {
            buf[i] = store;
        } else {
            return i;
        }
    }

    return length;
}

// currently not implemented/ to be implemented once there is more clarity about zephyr API
mraa_result_t
mraa_uart_flush(mraa_uart_context dev)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

// this can possibly be set from the uart_qmsi_init fn, however
// there is no direct access to the fn, hence this feature might have to wait
mraa_result_t
mraa_uart_set_mode(mraa_uart_context dev, int bytesize, mraa_uart_parity_t parity, int stopbits)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

// need to check the irq part
mraa_result_t
mraa_uart_set_timeout(mraa_uart_context dev, int read, int write, int interchar)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

// couldn't find any implementation in zephyr
mraa_result_t
mraa_uart_set_non_blocking(mraa_uart_context dev, mraa_boolean_t nonblock)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

mraa_result_t
mraa_uart_stop(mraa_uart_context dev)
{
    free(dev);
    return MRAA_SUCCESS;
}

// need to check the irq functions
mraa_boolean_t
mraa_uart_data_available(mraa_uart_context dev, unsigned int millis)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}
