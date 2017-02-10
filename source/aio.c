/*
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

#include <adc.h>
#include <misc/util.h>
#include <pinmux.h>
#include <stdlib.h>
#include <string.h>

#include "mraa/aio.h"
#include "mraa_internal.h"
#include "mraa_internal_types.h"

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#else
#include <misc/printk.h>
#define printf printk
#endif

#if defined(CONFIG_ADC)
#define ADC_DEVICE_NAME "ADC_0"
#endif

#define DEFAULT_BITS 10

static int raw_bits;

typedef struct adc_seq_entry* adc_seq_entry_ptr;
typedef struct adc_seq_table* adc_seq_table_ptr;

mraa_aio_context
mraa_aio_init(unsigned int pin)
{
    mraa_board_t* board = plat;
    if (board == NULL) {
        return NULL;
    }

    if (pin < 0 || pin >= board->phy_pin_count) {
        return NULL;
    }

    struct device* pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    if (pinmux_dev == NULL) {
        printf("Failed to get binding for pinmux\n");
        return NULL;
    }

#if defined(CONFIG_BOARD_QUARK_D2000_CRB)
    d2k_pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    // confused about the numbering of digital pins lower than 6
    // This needs to be resolved.
    if (pin == 7) {
        pinmux_pin_set(d2k_pinmux_dev, 8, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 7, 8, "IO7", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    } else if (pin == 8) {
        pinmux_pin_set(d2k_pinmux_dev, 9, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 8, 9, "IO8", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    } else if (pin == 10) {
        pinmux_pin_set(d2k_pinmux_dev, 0, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 10, 0, "IO10", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    } else if (pin == 11) {
        pinmux_pin_set(d2k_pinmux_dev, 17, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 11, 17, "IO11", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    } else if (pin == 12) {
        pinmux_pin_set(d2k_pinmux_dev, 18, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 12, 18, "IO12", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    } else if (pin == 13) {
        pinmux_pin_set(d2k_pinmux_dev, 16, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 13, 16, "IO13", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    }
#endif
#if defined(CONFIG_BOARD_ARDUINO_101_SSS)
    if (pin == 0) {
        pinmux_pin_set(pinmux_dev, 10, PINMUX_FUNC_B);
    } else if (pin == 1) {
        pinmux_pin_set(pinmux_dev, 11, PINMUX_FUNC_B);
    } else if (pin == 2) {
        pinmux_pin_set(pinmux_dev, 12, PINMUX_FUNC_B);
    } else if (pin == 3) {
        pinmux_pin_set(pinmux_dev, 13, PINMUX_FUNC_B);
    } else if (pin == 4) {
        pinmux_pin_set(pinmux_dev, 14, PINMUX_FUNC_B);
    } else if (pin == 5) {
        pinmux_pin_set(pinmux_dev, 9, PINMUX_FUNC_B);
    } else if (pin == 10) {
        pinmux_pin_set(pinmux_dev, 0, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 10, 0, "A8", (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 1, 0 });
    } else if (pin == 11) {
        pinmux_pin_set(pinmux_dev, 3, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 11, 3, "A9", (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 1, 0 });
    } else if (pin == 12) {
        pinmux_pin_set(pinmux_dev, 1, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 12, 1, "A10", (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 1, 0 });
    } else if (pin == 13) {
        pinmux_pin_set(pinmux_dev, 2, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 13, 2, "A11", (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 1, 0 });
    } else {
        printf("Pin %d not enabled/Can't be enabled\n", pin);
        return NULL;
    }
#endif

    if (board->pins[pin].capabilites.aio != 1) {
        return NULL;
    }

    mraa_aio_context dev = (mraa_aio_context) malloc(sizeof(struct _aio));
    dev->phy_pin = board->pins[pin].aio.pinmap;
    dev->zdev = device_get_binding(ADC_DEVICE_NAME);
    if (dev->zdev == NULL)
        return NULL;

    dev->value_bit = DEFAULT_BITS;

    adc_seq_entry_ptr sample = (adc_seq_entry_ptr) malloc(sizeof(struct adc_seq_entry));
    adc_seq_table_ptr table = (adc_seq_table_ptr) malloc(sizeof(struct adc_seq_table));
    sample->sampling_delay = 12;
    sample->channel_id = dev->phy_pin;
    sample->buffer_length = 4;
    uint8_t* seq_buffer;
    seq_buffer = (uint8_t*) malloc(sizeof(uint8_t) * 4);
    sample->buffer = seq_buffer;
    table->entries = sample;
    table->num_entries = 1;
    dev->table = table;
    adc_enable(dev->zdev);
    dev->pin = pin;

    raw_bits = mraa_adc_raw_bits();

    return dev;
}

mraa_result_t
mraa_aio_close(mraa_aio_context dev)
{
    adc_disable(dev->zdev);
    free(dev->table->entries->buffer);
    free(dev->table->entries);
    free(dev->table);
    free(dev);
    return MRAA_SUCCESS;
}

int
mraa_aio_read(mraa_aio_context dev)
{
    uint32_t value;
    uint32_t shifter_value;

    if (adc_read(dev->zdev, dev->table))
        return -1;
    value = dev->table->entries->buffer[1] * 256 + dev->table->entries->buffer[0];

    if (dev->value_bit != raw_bits) {
        if (raw_bits > dev->value_bit) {
            shifter_value = raw_bits - dev->value_bit;
            value = value >> shifter_value;
        } else {
            shifter_value = dev->value_bit - raw_bits;
            value = value << shifter_value;
        }
    }

    return value;
}

float
mraa_aio_read_float(mraa_aio_context dev)
{
    if (dev == NULL)
        return -1.0;

    int32_t analog_value_int = mraa_aio_read(dev);
    if (analog_value_int < 0)
        return -1.0;

    float max_analog_value = (1 << dev->value_bit) - 1;
    return analog_value_int / max_analog_value;
}

mraa_result_t
mraa_aio_set_bit(mraa_aio_context dev, int bits)
{
    if (dev == NULL || bits < 1)
        return MRAA_ERROR_INVALID_RESOURCE;

    dev->value_bit = bits;
    return MRAA_SUCCESS;
}

int
mraa_aio_get_bit(mraa_aio_context dev)
{
    if (dev == NULL)
        return 0;

    return dev->value_bit;
}
