/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Contributions: Abhishek Malik <abhishek.malik@intel.com>
 *                Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <spi.h>
#include <string.h>

#include "mraa_internal.h"
#include "mraa_internal_types.h"
#include <device.h>
#include <malloc.h>
#include <pinmux.h>
#include <sys_clock.h>
#include <zephyr.h>

#include "mraa/spi.h"

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT printf
#else
#include <misc/printk.h>
#define PRINT printk
#endif

#define SPI_SLAVE 1

#define SET_MODE_MASK 0xfffffffc

// Arduino 101 has a 32 MHz clock
// D2000 has a 32 MHz clock
#if defined(CONFIG_BOARD_ARDUINO_101)
#define SPI_DRV_NAME "SPI_1"
#define SPI_MAX_CLK_FREQ_250KHZ 128
#elif defined(CONFIG_BOARD_ARDUINO_101_SSS)
#define SPI_DRV_NAME "SPI_1"
#define SPI_MAX_CLK_FREQ_250KHZ 128
#elif defined(CONFIG_BOARD_QUARK_D2000_CRB)
#define SPI_DRV_NAME "SPI_0"
#define SPI_MAX_CLK_FREQ_250KHZ 128
#endif

typedef struct spi_config* spi_config_ptr;

mraa_spi_context
mraa_spi_init(int bus)
{
    mraa_spi_context dev = (mraa_spi_context) malloc(sizeof(struct _spi));

// SPI is available by default on the ARC core
#if defined(CONFIG_BOARD_ARDUINO_101)
    // pinmux configuring required here
    struct device* pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    if (pinmux_dev == NULL) {
        printf("Failed to get binding for pinmux\n");
        free(dev);
        return NULL;
    }
    pinmux_pin_set(pinmux_dev, 42, PINMUX_FUNC_B);
    pinmux_pin_set(pinmux_dev, 43, PINMUX_FUNC_B);
    pinmux_pin_set(pinmux_dev, 44, PINMUX_FUNC_B);
    pinmux_pin_set(pinmux_dev, 45, PINMUX_FUNC_B);

    dev->pinmux_dev = pinmux_dev;
#endif

    dev->busnum = bus;
    dev->zdev = device_get_binding(SPI_DRV_NAME);
    spi_config_ptr conf = (spi_config_ptr) malloc(sizeof(struct spi_config));
    // only default settings, can be changed by using the other functions provided
    conf->config = SPI_MODE_CPOL | SPI_MODE_CPHA | (8 << 4);
    conf->max_sys_freq = SPI_MAX_CLK_FREQ_250KHZ;
    dev->config = conf;

    if (spi_configure(dev->zdev, dev->config) != 0) {
        printf("Unable to configure the SPI Driver\n");
        free(conf);
        free(dev);
        return NULL;
    }

    spi_slave_select(dev->zdev, SPI_SLAVE);

    return dev;
}

mraa_result_t
mraa_spi_mode(mraa_spi_context dev, mraa_spi_mode_t mode)
{
    switch (mode) {
        case MRAA_SPI_MODE0:
            dev->config->config = SET_MODE_MASK & dev->config->config;
            break;
        case MRAA_SPI_MODE1:
            dev->config->config = (SET_MODE_MASK | SPI_MODE_CPHA) & dev->config->config;
            break;
        case MRAA_SPI_MODE2:
            dev->config->config = (SET_MODE_MASK | SPI_MODE_CPOL) & dev->config->config;
        case MRAA_SPI_MODE3:
            dev->config->config = (SET_MODE_MASK | SPI_MODE_CPOL | SPI_MODE_CPHA) & dev->config->config;
            break;
        default:
            dev->config->config = SET_MODE_MASK & dev->config->config;
            break;
    }
    if (spi_configure(dev->zdev, dev->config) != 0) {
        return MRAA_ERROR_UNSPECIFIED;
    }

    return MRAA_SUCCESS;
}

mraa_result_t
mraa_spi_frequency(mraa_spi_context dev, int hz)
{
    uint32_t freq = 0;
    if (hz > 0) {
        freq = sys_clock_hw_cycles_per_sec / hz;
    } else {
        return MRAA_ERROR_INVALID_PARAMETER;
    }

    dev->config->max_sys_freq = freq;
    if (spi_configure(dev->zdev, dev->config) != 0) {
        return MRAA_ERROR_UNSPECIFIED;
    }

    return MRAA_SUCCESS;
}

mraa_result_t
mraa_spi_lsbmode(mraa_spi_context dev, mraa_boolean_t lsb)
{
    if (lsb == 0) {
        dev->config->config = (dev->config->config & ~(SPI_TRANSFER_MASK)) | SPI_TRANSFER_MSB;
    } else {
        dev->config->config = (dev->config->config & ~(SPI_TRANSFER_MASK)) | SPI_TRANSFER_LSB;
    }
    if (spi_configure(dev->zdev, dev->config) != 0) {
        return MRAA_ERROR_UNSPECIFIED;
    }

    return MRAA_SUCCESS;
}

int
mraa_spi_write(mraa_spi_context dev, uint8_t data)
{
    // assuming that the data read back is in the same format
    // as the data being sent in and also of the same length
    int ret;
    uint8_t ret_data;
    // in case nothing is being read back, sending in NULL would be a
    // good idea, otherwise the SPI bus will probably hang and block
    // further execution of code.
    ret = spi_transceive(dev->zdev, &data, 1, &ret_data, 1);
    if (ret == 0) {
        return ret_data;
    }

    return -1;
}

int
mraa_spi_write_word(mraa_spi_context dev, uint16_t data)
{
    // setting the number of bits per packet to 16
    if (mraa_spi_bit_per_word(dev, 16) != MRAA_SUCCESS) {
        return -1;
    }

    uint16_t ret_data;
    int ret;
    ret = spi_transceive(dev->zdev, &data, 1, &ret_data, 1);

    // setting the number of packets back to 8
    if (mraa_spi_bit_per_word(dev, 8) != MRAA_SUCCESS) {
        return -1;
    }

    if (ret == 0) {
        return ret_data;
    }

    return -1;
}

uint8_t*
mraa_spi_write_buf(mraa_spi_context dev, uint8_t* data, int length)
{
    // Since this is a write function only the return will be NULL
    int ret;
    ret = spi_transceive(dev->zdev, data, length, NULL, 0);

    return NULL;
}

uint16_t*
mraa_spi_write_buf_word(mraa_spi_context dev, uint16_t* data, int length)
{
    // setting number of bits per packet to 16
    if (mraa_spi_bit_per_word(dev, 16) != MRAA_SUCCESS) {
        return NULL;
    }
    // might/might not use this
    // uint16_t* recv = malloc(sizeof(uint16_t) * length);
    int ret;
    ret = spi_transceive(dev->zdev, data, length, NULL, 0);

    // setting the number of packets back to 8
    if (mraa_spi_bit_per_word(dev, 8) != MRAA_SUCCESS) {
        return NULL;
    }

    return NULL;
}

mraa_result_t
mraa_spi_bit_per_word(mraa_spi_context dev, unsigned int bits)
{
    dev->config->config = (~(SPI_WORD_SIZE_MASK) &dev->config->config) | (bits << 4);
    if (spi_configure(dev->zdev, dev->config) != 0) {
        return MRAA_ERROR_UNSPECIFIED;
    }

    return MRAA_SUCCESS;
}

mraa_result_t
mraa_spi_transfer_buf(mraa_spi_context dev, uint8_t* data, uint8_t* rxbuf, int length)
{
    mraa_result_t ret;
    ret = spi_transceive(dev->zdev, data, length, rxbuf, length);

    return ret;
}

mraa_result_t
mraa_spi_transfer_buf_word(mraa_spi_context dev, uint16_t* data, uint16_t* rxbuf, int length)
{
    // setting number of bits per packet to 16
    if (mraa_spi_bit_per_word(dev, 16) != MRAA_SUCCESS) {
        return -1;
    }

    mraa_result_t ret;
    ret = spi_transceive(dev->zdev, data, length, rxbuf, length);

    // setting the number of packets back to 8
    if (mraa_spi_bit_per_word(dev, 8) != MRAA_SUCCESS) {
        return -1;
    }

    return ret;
}

mraa_result_t
mraa_spi_stop(mraa_spi_context dev)
{
    free(dev);
    return MRAA_SUCCESS;
}
