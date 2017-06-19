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


#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#else
#include <misc/printk.h>
#define printf printk
#endif
#include "mraa/gpio.h"
#include "mraa_internal.h"
#include "mraa_internal_types.h"
#include <gpio.h>
#include <misc/util.h>

#if defined(CONFIG_BOARD_QUARK_D2000_CRB) || defined(CONFIG_BOARD_ARDUINO_101) || \
    defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD) || \
    defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD_SS)
#include <pinmux.h>
#elif defined(CONFIG_BOARD_NUCLEO_L476RG)
#include <pinmux.h>
#include <pinmux/stm32/pinmux_stm32.h>
#endif

#include <stdlib.h>
#include <string.h>


#if defined(CONFIG_GPIO_QMSI)
#define GPIO_DRV_NAME CONFIG_GPIO_QMSI_0_NAME
#elif defined(CONFIG_GPIO_QMSI_SS_0)
#define GPIO_DRV_NAME CONFIG_GPIO_QMSI_SS_0_NAME
#elif defined(CONFIG_GPIO_QMSI_SS_1)
#define GPIO_DRV_NAME CONFIG_GPIO_QMSI_SS_1_NAME
#else
#define GPIO_DRV_NAME "GPIO_0"
#endif
static int edge_flags = 0;
char stm32_drv[6];

/*
 * Use container_of macro to get gpio context as per advice from
 * ivan.briano@intel.com on 5/13/16.
 *
 * henry.bruce: Callback function signatures usually include a "user
 *  data" parameter so callback can be given some context.  This is
 *  missing from gpio_callback_handler_t. Is this by design or an
 *  omission?
 *
 * ivan.briano: By design, use the callback_handler as a field in your
 *  struct and get it in the callback with container_of()
 */
static void
gpio_internal_callback(struct device* port, struct gpio_callback* cb, uint32_t pins)
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
        printf("gpio: platform not initialised\n");
        return NULL;
    }

    if (pin < 0 || pin >= board->phy_pin_count) {
        printf("gpio: pin %i beyond platform definition\n", pin);
        return NULL;
    }

#if defined(CONFIG_BOARD_QUARK_D2000_CRB) || defined(CONFIG_BOARD_ARDUINO_101) || \
    defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD) || \
    defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD_SS)
    struct device* pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    if (pinmux_dev == NULL) {
        printf("Failed to get binding for pinmux\n");
        return NULL;
    }
#elif defined(CONFIG_BOARD_NUCLEO_L476RG)
    // pinmux doesn't seem to work as expected
    /*
    struct device* pinmux_dev = device_get_binding(STM32_PINMUX_NAME);
    if (pinmux_dev == NULL) {
        printf("Failed to get binding for pinmux\n");
        return NULL;
    }
    */
#endif

#if defined(CONFIG_BOARD_QUARK_D2000_CRB)
    d2k_pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    switch(pin) {
    case 14:
        pinmux_pin_set(d2k_pinmux_dev, 3, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 14, 3, "A0", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
        break;
    case 15:
        pinmux_pin_set(d2k_pinmux_dev, 4, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 15, 4, "A1", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
        break;
    case 16:
        pinmux_pin_set(d2k_pinmux_dev, 14, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 16, 14, "A2", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
        break;
    case 17:
        pinmux_pin_set(d2k_pinmux_dev, 15, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 17, 15, "A3", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
        break;
    case 18:
        pinmux_pin_set(d2k_pinmux_dev, 7, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 18, 7, "A4", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 0, 0 });
        break;
    case 19:
        pinmux_pin_set(d2k_pinmux_dev, 6, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 19, 6, "A5", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 0, 0 });
        break;
    default:;
    }
#endif
#if defined(CONFIG_BOARD_ARDUINO_101)
    switch(pin) {
    case 3:
    	pinmux_pin_set(pinmux_dev, 63, PINMUX_FUNC_C);
        mraa_set_pininfo(board, 3, 17, "IO3", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
        break;
    case 5:
        pinmux_pin_set(pinmux_dev, 64, PINMUX_FUNC_C);
        mraa_set_pininfo(board, 5, 15, "IO5", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
        break;
    case 10:
        pinmux_pin_set(pinmux_dev, 0, PINMUX_FUNC_A);
        break;
    case 11:
        pinmux_pin_set(pinmux_dev, 3, PINMUX_FUNC_A);
        break;
    case 12:
        pinmux_pin_set(pinmux_dev, 1, PINMUX_FUNC_A);
        break;
    case 13:
        pinmux_pin_set(pinmux_dev, 2, PINMUX_FUNC_A);
        break;
    case 6:
    case 9:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
        printf("Pin %d not enabled/Can't be enabled\n", pin);
        return NULL;
    default:;
    	// do nothing
    }
#endif
#if defined(CONFIG_BOARD_ARDUINO_101_SSS)
    switch(pin) {
    case 14:
        pinmux_pin_set(pinmux_dev, 10, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 14, 2, "A0  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
        break;
    case 15:
        pinmux_pin_set(pinmux_dev, 11, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 15, 3, "A1  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
        break;
    case 16:
        pinmux_pin_set(pinmux_dev, 12, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 16, 4, "A2  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
        break;
    case 17:
        pinmux_pin_set(pinmux_dev, 13, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 17, 5, "A3  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
        break;
    case 18:
        pinmux_pin_set(pinmux_dev, 9, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 19, 1, "A5  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 1, 0 });
        break;
    case 19:
        pinmux_pin_set(pinmux_dev, 9, PINMUX_FUNC_A);
        mraa_set_pininfo(board, 19, 1, "A5  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 1, 0 });
        break;
    default:;
    	// do nothing
    }
#endif

    if (board->pins[pin].capabilites.gpio != 1) {
        printf("gpio: pin %i not capable of gpio\n", pin);
        return NULL;
    }
    if (board->pins[pin].gpio.mux_total > 0) {
        if (mraa_setup_mux_mapped(board->pins[pin].gpio) != MRAA_SUCCESS) {
            return NULL;
        }
    }

#if defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD)
    if(board->pins[pin].gpio.pinmap >=0 && board->pins[pin].gpio.pinmap <= 28)
        pinmux_pin_pullup(pinmux_dev, board->pins[pin].gpio.pinmap, PINMUX_FUNC_A);
    else if(board->pins[pin].gpio.pinmap >=29 && board->pins[pin].gpio.pinmap <=31)
        pinmux_pin_pullup(pinmux_dev, board->pins[pin].gpio.pinmap, PINMUX_FUNC_B);
#endif

#if defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD_SS)
    switch(pin) {
    case 14:
    	mraa_set_pininfo(board, 14, 2, "14", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    	break;
    case 16:
    	mraa_set_pininfo(board, 16, 3, "16", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    	break;
    case 18:
    	mraa_set_pininfo(board, 18, 4, "18", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    	break;
    case 20:
    	mraa_set_pininfo(board, 20, 5, "20", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    	break;
    case 22:
    	mraa_set_pininfo(board, 22, 6, "22", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    	break;
    case 24:
    	mraa_set_pininfo(board, 24, 7, "24", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 1, 0 });
    	break;
    case 25:
    	mraa_set_pininfo(board, 25, 10, "25", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    	break;
    case 27:
    	mraa_set_pininfo(board, 27, 11, "27", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    	break;
    default:;
    	// do nothing
    }
#endif

#if defined(CONFIG_BOARD_NUCLEO_L476RG)
    if((pin >= 0 && pin <= 2) || 
       (pin >= 7 && pin <= 8) ||
       (pin >= 11 && pin <= 13) ||
       (pin >= 16 && pin <= 18)) {
        memcpy(stm32_drv, "GPIOA", sizeof("GPIOA"));
    } else if((pin >= 3 && pin <= 6) ||
            (pin == 10) ||
            (pin >= 14 && pin <= 15) ||
            (pin == 19)) {
        memcpy(stm32_drv, "GPIOB", sizeof("GPIOB"));
    } else if((pin == 9) ||
            (pin >= 20 && pin <=21)) {
        memcpy(stm32_drv, "GPIOC", sizeof("GPIOC"));
    }
#endif

    mraa_gpio_context dev = mraa_gpio_init_raw(board->pins[pin].gpio.pinmap);
    if (dev) {
        dev->pin = pin;
    }
    return dev;
}

mraa_gpio_context
mraa_gpio_init_raw(int gpiopin)
{
    mraa_gpio_context dev = (mraa_gpio_context) malloc(sizeof(struct _gpio));
    if (!dev) {
        printf("%s: context allocation failed\n", __FUNCTION__);
        return NULL;
    }

    dev->phy_pin = gpiopin;
#if defined(CONFIG_BOARD_NUCLEO_L476RG)
    dev->zdev = device_get_binding(stm32_drv);
#else
    dev->zdev = device_get_binding(GPIO_DRV_NAME);
#endif
    if (dev->zdev == NULL) {
        printf("Unable to initialize the driver: %s\n", GPIO_DRV_NAME);
        free(dev);
        return NULL;
    }
    int ret = gpio_pin_configure(dev->zdev, dev->phy_pin, GPIO_DIR_OUT);
    if (ret) {
        printf("Error %d configuring %s pin %d\n", ret, GPIO_DRV_NAME, dev->phy_pin);
        free(dev);
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
        return (int) value;
}

mraa_result_t
mraa_gpio_write(mraa_gpio_context dev, int value)
{
    if (gpio_pin_write(dev->zdev, dev->phy_pin, (uint32_t) value))
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
    free(dev);

    return MRAA_SUCCESS;
}
