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


#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#else
#include <misc/printk.h>
#define printf printk
#endif
#include <stdlib.h>
#include <string.h>
#include <gpio.h>
#include <misc/util.h>
#include <pinmux.h>
#include "mraa/gpio.h"
#include "mraa_internal.h"
#include "mraa_internal_types.h"


#if defined(CONFIG_GPIO_DW_0)
#define GPIO_DRV_NAME CONFIG_GPIO_DW_0_NAME
#elif defined(CONFIG_GPIO_QMSI_0)
#define GPIO_DRV_NAME CONFIG_GPIO_QMSI_0_NAME
#else
#error GPIO driver not defined
#endif

static struct _gpio _internalgpios[CONFIG_MRAA_PIN_COUNT];
static struct _gpio tmp_context;
static int edge_flags = 0;


/*
Use container_of macro to get gpio context as per advice from ivan.briano@intel.com on 5/13/16.
henry.bruce: Callback function signatures usually include a "user data" parameter so callback can be given some context.
             This is missing from gpio_callback_handler_t. Is this by design or an omission?
ivan.briano: By design, use the callback_handler as a field in your struct and get it in the callback with container_of()
*/
static void gpio_internal_callback(struct device *port, struct gpio_callback *cb, uint32_t pins)
{
    mraa_gpio_context dev = CONTAINER_OF(cb, struct _gpio, zcallback);
    if (dev->isr != NULL)
        dev->isr(dev->isr_args);
}

mraa_gpio_context
mraa_gpio_init(int pin)
{
    mraa_board_t* board = plat;
    if (board == NULL) {
        // syslog(LOG_ERR, "gpio: platform not initialised");
        return NULL;
    }

    if (pin < 0 || pin >= board->phy_pin_count) {
        // syslog(LOG_ERR, "gpio: pin %i beyond platform definition", pin);
        return NULL;
    }
    if (board->pins[pin].capabilites.gpio != 1) {
        // syslog(LOG_ERR, "gpio: pin %i not capable of gpio", pin);
        return NULL;
    }
#if 0
    if (board->pins[pin].gpio.pinmux != -1) {
	struct device* pinmux_dev = device_get_binding("pinmux");
        printf("phew\n");
	if (!pinmux_dev) {
            printf("error\n");
            return NULL;
        }
	if (pinmux_pin_set(pinmux_dev, board->pins[pin].gpio.pinmap, board->pins[pin].gpio.pinmux) != 0) {
            printf("error2\n");
            return NULL;
	}
    }
#endif
    if (board->pins[pin].gpio.mux_total > 0) {
        if (mraa_setup_mux_mapped(board->pins[pin].gpio) != MRAA_SUCCESS) {
            return NULL;
        }
    }
    mraa_gpio_context dev = &_internalgpios[pin];
    mraa_gpio_context tmp = mraa_gpio_init_raw(board->pins[pin].gpio.pinmap);
    memcpy(dev, tmp, sizeof(struct _gpio));
    dev->pin = pin;
    return dev;
}

mraa_gpio_context
mraa_gpio_init_raw(int gpiopin)
{
    mraa_gpio_context dev = &tmp_context;
    dev->phy_pin = gpiopin;
    dev->zdev = device_get_binding(GPIO_DRV_NAME);
    if (dev->zdev == NULL)
        return NULL;
    int ret = gpio_pin_configure(dev->zdev, dev->phy_pin, GPIO_DIR_OUT);
    if (ret) {
        printf("Error %d configuring %s pin %d\n", ret, GPIO_DRV_NAME, dev->phy_pin);
        return NULL;
    }
    return dev;
}

mraa_result_t
mraa_gpio_dir(mraa_gpio_context dev, mraa_gpio_dir_t dir)
{
    int flags = 0;
    uint32_t value = 0;
    switch (dir) {
        case MRAA_GPIO_OUT:
            flags = GPIO_DIR_OUT;
            break;
        case MRAA_GPIO_IN:
            flags = GPIO_DIR_IN;
            break;
        case MRAA_GPIO_OUT_HIGH:
            flags = GPIO_DIR_OUT;
            value = 1;
            break;
        case MRAA_GPIO_OUT_LOW:
            flags = GPIO_DIR_OUT;
            value = 0;
            break;
        default:
            return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
    }
    if (gpio_pin_configure(dev->zdev, dev->phy_pin, flags))
        return MRAA_ERROR_UNSPECIFIED;
    if (dir == MRAA_GPIO_OUT_HIGH || dir == MRAA_GPIO_OUT_LOW) {
        if (gpio_pin_write(dev->zdev, dev->phy_pin, value))
            return MRAA_ERROR_UNSPECIFIED;
    }
    return MRAA_SUCCESS;
}


mraa_result_t
mraa_gpio_edge_mode(mraa_gpio_context dev, mraa_gpio_edge_t mode)
{
    switch (mode) {
        case MRAA_GPIO_EDGE_NONE:
            edge_flags = 0;
            break;
        case MRAA_GPIO_EDGE_BOTH:
            edge_flags = GPIO_INT_EDGE | GPIO_INT_DOUBLE_EDGE;
            break;
        case MRAA_GPIO_EDGE_RISING:
            edge_flags = GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH;
            break;
        case MRAA_GPIO_EDGE_FALLING:
            edge_flags = GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW;
            break;
        default:
            return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
    }

    return MRAA_SUCCESS;
}


int
mraa_gpio_read(mraa_gpio_context dev)
{
    uint32_t value;
    if (gpio_pin_read(dev->zdev, dev->phy_pin, &value))
        return -1;
    else
        return (int)value;
}

mraa_result_t
mraa_gpio_write(mraa_gpio_context dev, int value)
{
     if (gpio_pin_write(dev->zdev, dev->phy_pin, (uint32_t)value))
        return MRAA_ERROR_UNSPECIFIED;
     return MRAA_SUCCESS;
}


mraa_result_t
mraa_gpio_isr(mraa_gpio_context dev, mraa_gpio_edge_t edge_mode, void (*fptr)(void*), void* args)
{
    if (MRAA_SUCCESS != mraa_gpio_edge_mode(dev, edge_mode)) {
        return MRAA_ERROR_UNSPECIFIED;
    }
    int flags = GPIO_DIR_IN | GPIO_INT | GPIO_INT_DEBOUNCE | edge_flags;
    // printf("isr addr = %x, args = %x\n", fptr, args);
    int ret = gpio_pin_configure(dev->zdev, dev->phy_pin, flags);
    if (ret) {
        return MRAA_ERROR_UNSPECIFIED;
    }
    dev->isr = fptr;
    dev->isr_args = args;
    gpio_init_callback(&(dev->zcallback), gpio_internal_callback, BIT(dev->phy_pin));
    ret = gpio_add_callback(dev->zdev, &(dev->zcallback));
    if (ret) {
        return MRAA_ERROR_UNSPECIFIED;
    }
    ret = gpio_pin_enable_callback(dev->zdev, dev->phy_pin);
    if (ret) {
        return MRAA_ERROR_UNSPECIFIED;
    }
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_gpio_isr_exit(mraa_gpio_context dev)
{
    return gpio_pin_disable_callback(dev->zdev, dev->phy_pin) ? MRAA_ERROR_UNSPECIFIED : MRAA_SUCCESS;
}


mraa_result_t
mraa_gpio_owner(mraa_gpio_context dev, mraa_boolean_t own)
{
    if (dev == NULL) {
        return MRAA_ERROR_INVALID_RESOURCE;
    }
    dev->owner = own;
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_gpio_mode(mraa_gpio_context dev, mraa_gpio_mode_t mode)
{
    return MRAA_SUCCESS;
}

int
mraa_gpio_get_pin(mraa_gpio_context dev)
{
    if (dev == NULL) {
        return -1;
    }
    return dev->phy_pin;
}

int
mraa_gpio_get_pin_raw(mraa_gpio_context dev)
{
    if (dev == NULL) {
        return -1;
    }
    return dev->pin;
}

mraa_result_t
mraa_gpio_close(mraa_gpio_context dev)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}
