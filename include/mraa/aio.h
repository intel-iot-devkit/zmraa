/*
 * Copyright (c) 2016, Intel Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __MRAA_AIO_H__
#define __MRAA_AIO_H__

#include "common.h"
#include <stdbool.h>

/**
 * Opaque pointer definition to the internal struct _aio. This context refers
 * to one single AIO pin on the board.
 */
typedef struct _aio* mraa_aio_context;

/**
 * Initialise an Analog input device, connected to the specified pin
 *
 * @param pin Channel number to read ADC inputs
 * @returns aio context or NULL
 */
mraa_aio_context mraa_aio_init(unsigned int pin);

/**
 * Read the input voltage. By default mraa will shift the raw value up or down
 * to a 10 bit value.
 *
 * @param dev The AIO context
 * @returns The current input voltage or -1 for error
 */
int mraa_aio_read(mraa_aio_context dev);

/**
 * Read the input voltage and return it as a normalized float (0.0f-1.0f).
 *
 * @param dev The AIO context
 * @returns The current input voltage as a normalized float (0.0f-1.0f), error
 * will be signaled by -1.0f
 */
float mraa_aio_read_float(mraa_aio_context dev);

/**
 * Close the analog input context, this will free the memory for the context
 *
 * @param dev The AIO context
 * @return Result of operation
 */
mraa_result_t mraa_aio_close(mraa_aio_context dev);

/**
 * Set the bit value which mraa will shift the raw reading
 * from the ADC to. I.e. 10bits
 * @param dev the analog input context
 * @param bits the bits the return from read should be i.e 10
 *
 * @return mraa result type
 */
mraa_result_t mraa_aio_set_bit(mraa_aio_context dev, int bits);

/**
 * Gets the bit value mraa is shifting the analog read to.
 * @param dev the analog input context
 *
 * @return bit value mraa is set return from the read function
 */
int mraa_aio_get_bit(mraa_aio_context dev);

#endif
