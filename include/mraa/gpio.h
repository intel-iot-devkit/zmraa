/*
 * Copyright (c) 2015, Intel Corporation. All rights reserved.
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

#ifndef __MRAA_GPIO_H__
#define __MRAA_GPIO_H__

#include "common.h"
#include <stdbool.h>

typedef struct _gpio* mraa_gpio_context;

/**
 * Gpio Output modes
 */
typedef enum {
    MRAA_GPIO_STRONG = 0,   /**< Default. Strong high and low */
    MRAA_GPIO_PULLUP = 1,   /**< Resistive High */
    MRAA_GPIO_PULLDOWN = 2, /**< Resistive Low */
    MRAA_GPIO_HIZ = 3       /**< High Z State */
} mraa_gpio_mode_t;

/**
 * Gpio Direction options
 */
typedef enum {
    MRAA_GPIO_OUT = 0,      /**< Output. A Mode can also be set */
    MRAA_GPIO_IN = 1,       /**< Input */
    MRAA_GPIO_OUT_HIGH = 2, /**< Output. Init High */
    MRAA_GPIO_OUT_LOW = 3   /**< Output. Init Low */
} mraa_gpio_dir_t;

/**
 * Gpio Edge types for interupts
 */
typedef enum {
    MRAA_GPIO_EDGE_NONE = 0,   /**< No interrupt on Gpio */
    MRAA_GPIO_EDGE_BOTH = 1,   /**< Interupt on rising & falling */
    MRAA_GPIO_EDGE_RISING = 2, /**< Interupt on rising only */
    MRAA_GPIO_EDGE_FALLING = 3 /**< Interupt on falling only */
} mraa_gpio_edge_t;

/**
 * Initialise a gpio
 *
 * @param pin the pin number
 * @param ss whether gpio is on ARC/SS or not
 */
mraa_gpio_context mraa_gpio_init_raw(int pin);

/**
 * Initialise a gpio
 *
 * @param pin the pin number
 */
mraa_gpio_context mraa_gpio_init(int pin);

/**
 * Set an interupt on pin
 *
 * @param dev The Gpio context
 * @param edge The edge mode to set the gpio into
 * @param fptr Function pointer to function to be called when interupt is
 * triggered
 * @param args Arguments passed to the interrupt handler (fptr)
 * @return Result of operation
 */
mraa_result_t mraa_gpio_isr(mraa_gpio_context dev, mraa_gpio_edge_t edge, void (*fptr)(void*), void* args);

/**
 * Stop the current interupt watcher on this Gpio, and set the Gpio edge mode
 * to MRAA_GPIO_EDGE_NONE
 *
 * @param dev The Gpio context
 * @return Result of operation
 */
mraa_result_t mraa_gpio_isr_exit(mraa_gpio_context dev);

/**
 * Set Gpio Output Mode,
 *
 * @param dev The Gpio context
 * @param mode The Gpio Output Mode
 * @return Result of operation
 */
mraa_result_t mraa_gpio_mode(mraa_gpio_context dev, mraa_gpio_mode_t mode);

/**
 * Set the edge mode on the gpio
 *
#include "mraa/common.h"
 * @param dev The Gpio context
 * @param mode The edge mode to set the gpio into
 * @return Result of operation
 */
mraa_result_t mraa_gpio_edge_mode(mraa_gpio_context dev, mraa_gpio_edge_t mode);

/**
 * Close the Gpio context
 * - Will free the memory for the context and unexport the Gpio
 *
 * @param dev The Gpio context
 * @return Result of operation
 */
mraa_result_t mraa_gpio_close(mraa_gpio_context dev);

/**
 * Set Gpio direction
 *
 * @param dev The Gpio context
 * @param dir The direction of the Gpio
 * @return Result of operation
 */
mraa_result_t mraa_gpio_dir(mraa_gpio_context dev, mraa_gpio_dir_t dir);

/**
 * Write to the Gpio Value.
 *
 * @param dev The Gpio context
 * @param value Integer value to write
 * @return Result of operation
 */
mraa_result_t mraa_gpio_write(mraa_gpio_context dev, int val);

/**
 * Read the Gpio value. This can be 0 or 1. A resonse of -1 means that there
 * was a fatal error.
 *
 * @param dev The Gpio context
 * @return Result of operation
 */
int mraa_gpio_read(mraa_gpio_context dev);

/**
 * Get a pin number of the gpio, invalid will return -1
 *
 * @param dev The Gpio context
 * @return Pin number
 */
int mraa_gpio_get_pin(mraa_gpio_context dev);

/**
 * Change ownership of the context.
 *
 * @param dev The Gpio context
 * @param owner Does this context own the pin
 * @return Result of operation
 */
mraa_result_t mraa_gpio_owner(mraa_gpio_context dev, mraa_boolean_t owner);

/**
 * Get a pin number of the gpio, invalid will return -1
 *
 * @param dev The Gpio context
 * @return Pin number
 */
int mraa_gpio_get_pin(mraa_gpio_context dev);

/**
 * Get a gpio number as used within sysfs, invalid will return -1
 *
 * @param dev The Gpio context
 * @return gpio number
 */
int mraa_gpio_get_pin_raw(mraa_gpio_context dev);


#endif
