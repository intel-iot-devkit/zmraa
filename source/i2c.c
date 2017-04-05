/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
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

#include <i2c.h>
#include <string.h>
#include <stdlib.h>
#include <pinmux.h>
#include "mraa_internal.h"
#include "mraa_internal_types.h"
#include "mraa/i2c.h"


mraa_i2c_context
mraa_i2c_init(int bus)
{
    mraa_board_t* board = plat;
    if (board == NULL) {
        // syslog(LOG_ERR, "i2c: Platform Not Initialised");
        return NULL;
    }

    if (bus >= board->i2c_bus_count) {
        // syslog(LOG_ERR, "Above i2c bus count");
        return NULL;
    }

    struct device* pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    if (pinmux_dev == NULL) {
        printf("Failed to get binding for pinmux\n");
        return NULL;
    }

#if defined(CONFIG_BOARD_ARDUINO_101_SSS)
    if (bus == 0) {
        pinmux_pin_set(pinmux_dev, 14, PINMUX_FUNC_B);
        pinmux_pin_set(pinmux_dev, 9, PINMUX_FUNC_B);
        pinmux_pin_pullup(pinmux_dev, 14, PINMUX_FUNC_B);
        pinmux_pin_pullup(pinmux_dev, 9, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 4, 14, "A4  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 1, 0 });
        mraa_set_pininfo(board, 5, 9, "A5  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 1, 0 });
    }
#endif

    if (board->i2c_bus[bus].bus_id == -1) {
        // syslog(LOG_ERR, "Invalid i2c bus, moving to default i2c bus");
        bus = board->def_i2c_bus;
    }
    if (!board->no_bus_mux) {
        int pos = board->i2c_bus[bus].sda;
        if (board->pins[pos].i2c.mux_total > 0) {
            if (mraa_setup_mux_mapped(board->pins[pos].i2c) != MRAA_SUCCESS) {
                // syslog(LOG_ERR, "i2c: Failed to set-up i2c sda multiplexer");
                return NULL;
            }
        }

        pos = board->i2c_bus[bus].scl;
        if (board->pins[pos].i2c.mux_total > 0) {
            if (mraa_setup_mux_mapped(board->pins[pos].i2c) != MRAA_SUCCESS) {
                // syslog(LOG_ERR, "i2c: Failed to set-up i2c scl multiplexer");
                return NULL;
            }
        }
    }

    return mraa_i2c_init_raw(plat->i2c_bus[bus].bus_id);
}


mraa_i2c_context
mraa_i2c_init_raw(unsigned int bus)
{
    char device_name[8];
    mraa_i2c_context dev = (mraa_i2c_context) malloc(sizeof(struct _i2c));
    if (!dev)
    {
        printf("%s: context allocation failed\n", __FUNCTION__);
        return NULL;
    }

    sprintf(device_name, "I2C_%d", bus);
    dev->zdev = device_get_binding(device_name);
    if (dev->zdev == NULL) {
        printf("Failed to get binding for %s\n", device_name);
        free(dev);
        return NULL;
    }
    dev->busnum = bus;
    dev->zcfg.raw = 0;
    dev->zcfg.bits.use_10_bit_addr = 0;
    dev->zcfg.bits.speed = I2C_SPEED_STANDARD;
    dev->zcfg.bits.is_master_device = 1;
    if (i2c_configure(dev->zdev, dev->zcfg.raw) != 0)
    {
        free(dev);
        return NULL;
    }
    return dev;
}


mraa_result_t
mraa_i2c_frequency(mraa_i2c_context dev, mraa_i2c_mode_t mode)
{
    switch (mode) {
        case MRAA_I2C_STD: /**< up to 100Khz */
            dev->zcfg.bits.speed = I2C_SPEED_STANDARD;
            break;
        case MRAA_I2C_FAST: /**< up to 400Khz */
            dev->zcfg.bits.speed = I2C_SPEED_FAST;
            break;
        case MRAA_I2C_HIGH: /**< up to 3.4Mhz */
            dev->zcfg.bits.speed = I2C_SPEED_HIGH;
            break;
    }
    return i2c_configure(dev->zdev, dev->zcfg.raw) == 0 ? MRAA_SUCCESS : MRAA_ERROR_UNSPECIFIED;
}


mraa_result_t
mraa_i2c_address(mraa_i2c_context dev, uint8_t addr)
{
    dev->addr = addr;
    return MRAA_SUCCESS;
}


int
mraa_i2c_read(mraa_i2c_context dev, uint8_t* data, int length)
{
    int status = i2c_read(dev->zdev, data, length, dev->addr);
    return status ? 0 : length;
}

mraa_result_t
mraa_i2c_write(mraa_i2c_context dev, const uint8_t* data, int bytesToWrite)
{
    int status = i2c_write(dev->zdev, (uint8_t *)data, bytesToWrite, dev->addr);
    return status == 0 ? MRAA_SUCCESS : MRAA_ERROR_INVALID_HANDLE;
}


int
mraa_i2c_read_byte(mraa_i2c_context dev)
{
    uint8_t data;
    int bytes_read = mraa_i2c_read(dev, &data, 1);
    return bytes_read == 1 ? data : -1;
}

int
mraa_i2c_read_byte_data(mraa_i2c_context dev, uint8_t command)
{
    uint8_t data;
    return i2c_reg_read_byte(dev->zdev, dev->addr, command, &data) ? -1 : data;
}

int
mraa_i2c_read_word_data(mraa_i2c_context dev, uint8_t command)
{
    uint16_t data;
    if (mraa_i2c_read_bytes_data(dev, command, (uint8_t*)&data, 2) == 2)
        return data;
    else
        return -1;
}

int
mraa_i2c_read_bytes_data(mraa_i2c_context dev, uint8_t command, uint8_t* data, int length)
{
    return i2c_burst_read(dev->zdev, dev->addr, command, data, length) == 0 ? length : 0;
}


mraa_result_t
mraa_i2c_write_byte(mraa_i2c_context dev, uint8_t data)
{
    mraa_result_t status = mraa_i2c_write(dev, &data, 1);
    return status;
}


mraa_result_t
mraa_i2c_write_byte_data(mraa_i2c_context dev, const uint8_t data, const uint8_t command)
{
    uint8_t buf[2];
    buf[0] = command;
    buf[1] = data;
    mraa_result_t status = mraa_i2c_write(dev, buf, 2);
    return status;
}

mraa_result_t
mraa_i2c_write_word_data(mraa_i2c_context dev, const uint16_t data, const uint8_t command)
{
    uint8_t buf[3];
    buf[0] = command;
    buf[1] = (uint8_t) data;
    buf[2] = (uint8_t)(data >> 8);
    mraa_result_t status = mraa_i2c_write(dev, buf, 3);
    return status;
}

mraa_result_t
mraa_i2c_update_byte_data(mraa_i2c_context dev, const uint16_t mask, const uint8_t data, const uint8_t command)
{
    int status = i2c_reg_update_byte(dev->zdev, dev->addr, command, mask, data);
    return status == 0 ? MRAA_SUCCESS : MRAA_ERROR_UNSPECIFIED;
}


mraa_result_t
mraa_i2c_stop(mraa_i2c_context dev)
{
    free(dev);
    return MRAA_SUCCESS;
}

