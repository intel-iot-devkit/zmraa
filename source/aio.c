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

#include <stdlib.h>
#include <string.h>
#include <adc.h>
#include <misc/util.h>
#include <pinmux.h>

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

typedef struct adc_seq_entry* adc_seq_entry_ptr;
typedef struct adc_seq_table* adc_seq_table_ptr;

mraa_aio_context mraa_aio_init(unsigned int pin){
    printf("Entering the mraa aio functionality\n");
    mraa_board_t* board = plat;
    if(board == NULL){
        return NULL;
    }

    if(pin < 0 || pin >= board->phy_pin_count){
        return NULL;
    }

    if(board->pins[pin].capabilites.aio != 1){
        return NULL;
    }

    mraa_aio_context dev = (mraa_aio_context) malloc(sizeof(struct _aio));
    dev->phy_pin = board->pins[pin].aio.pinmap;
    dev->zdev = device_get_binding(ADC_DEVICE_NAME);
    if(dev->zdev == NULL)
        return NULL;

    adc_seq_entry_ptr sample = (adc_seq_entry_ptr) malloc(sizeof(struct adc_seq_entry));
    adc_seq_table_ptr table = (adc_seq_table_ptr) malloc(sizeof(struct adc_seq_table));
    sample->sampling_delay = 12;
    sample->channel_id = dev->phy_pin;
    sample->buffer_length = 4;
    uint8_t *seq_buffer;
    seq_buffer = (uint8_t *) malloc(sizeof(uint8_t)*4);
    sample->buffer = seq_buffer;
    table->entries = sample;
    table->num_entries = 1;
    dev->table = table;
    adc_enable(dev->zdev);
    dev->pin = pin;
    return dev;
}

mraa_result_t mraa_aio_close(mraa_aio_context dev){
    adc_disable(dev->zdev);
    free(dev->table->entries->buffer);
    free(dev->table->entries);
    free(dev->table);
    free(dev);
    return MRAA_SUCCESS;
}

int mraa_aio_read(mraa_aio_context dev){
    uint32_t value;
    if(adc_read(dev->zdev, dev->table))
        value = -1;
    else
        value = dev->table->entries->buffer[1]*256 + dev->table->entries->buffer[0];
    return value;
}

float mraa_aio_read_float(mraa_aio_context dev){
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

mraa_result_t mraa_aio_set_bit(mraa_aio_context dev, int bits){
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

int mraa_aio_get_bit(mraa_aio_context dev){
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

