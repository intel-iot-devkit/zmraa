/*
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

#include <gpio.h>
#include <mraa/gpio.h>
#include "mraa_internal_types.h"

#if defined(CONFIG_SOC_QUARK_D2000)
#define GPIO_OUT_PIN    8
#define GPIO_INT_PIN    24
#define GPIO_NAME       "GPIO_"
#endif

#if defined(CONFIG_GPIO_DW_0)
#define GPIO_DRV_NAME CONFIG_GPIO_DW_0_NAME
#elif defined(CONFIG_GPIO_QMSI_0)
#define GPIO_DRV_NAME CONFIG_GPIO_QMSI_0_NAME
#else
#define GPIO_DRV_NAME CONFIG_GPIO_DW_0_NAME
#endif

struct _gpio _internalgpios[4];

mraa_gpio_context
mraa_gpio_init(int pin)
{
    return mraa_gpio_init_raw(pin);
}

mraa_gpio_context
mraa_gpio_init_raw(int gpiopin)
{
    //mraa_gpio_context dev = malloc(sizeof(struct _gpio));
    mraa_gpio_context dev = &_internalgpios[0];
    dev->phy_pin = gpiopin;
    dev->pin = gpiopin;
    dev->gpio_dev = device_get_binding(GPIO_DRV_NAME);
    gpio_pin_configure(dev->gpio_dev, dev->phy_pin, GPIO_DIR_OUT);
    return dev;
}

mraa_result_t
mraa_gpio_dir(mraa_gpio_context dev, mraa_gpio_dir_t dir)
{
    gpio_pin_configure(dev->gpio_dev, dev->phy_pin, GPIO_DIR_OUT);
    return MRAA_SUCCESS;
}

int mraa_gpio_read(mraa_gpio_context dev)
{
    return -1;
}

mraa_result_t
mraa_gpio_write(mraa_gpio_context dev, int value)
{
     gpio_pin_write(dev->gpio_dev, dev->phy_pin, 1);
     return MRAA_SUCCESS;
}

mraa_result_t
mraa_gpio_close(mraa_gpio_context dev)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}
