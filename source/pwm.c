/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2014, 2015, 2016 Intel Corporation.
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

#include "mraa/pwm.h"
#include "mraa_internal.h"
#include "mraa_internal_types.h"
#include "version.h"
#include <misc/util.h>
#include <pinmux.h>
#include <pwm.h>
#include <stdlib.h>
#include <string.h>

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#else
#include <misc/printk.h>
#define printf printk
#endif

/* about 1 second
   this calculation is based on that fact that
   one clock cycle comes to be about 31.25 ns */
#define MAX_PERIOD 32000000
#define DEFAULT_DUTY_CYCLE 0.5

#if defined(CONFIG_PWM)
#if defined(CONFIG_PWM_QMSI)
#define PWM_DEVICE_NAME CONFIG_PWM_QMSI_DEV_NAME
#elif defined(CONFIG_PWM_DW)
#define PWM_DEVICE_NAME CONFIG_PWM_DW_0_DRV_NAME
#endif
#endif

mraa_pwm_context
mraa_pwm_init(int pin)
{
    mraa_board_t* board = plat;
    if (board == NULL) {
        return NULL;
    }

    if (pin < 0 || pin >= board->phy_pin_count) {
        return NULL;
    }

#if defined(CONFIG_BOARD_QUARK_D2000_CRB)
    d2k_pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    if (pin == 6) {
        pinmux_pin_set(d2k_pinmux_dev, 19, PINMUX_FUNC_C);
        mraa_set_pininfo(board, 6, 0, "IO6", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    } else if (pin == 9) {
        pinmux_pin_set(d2k_pinmux_dev, 24, PINMUX_FUNC_C);
        mraa_set_pininfo(board, 9, 1, "IO9", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    }
#endif
#if defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_ARDUINO_101)
    struct device* pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    if (pinmux_dev == NULL) {
        printf("Failed to get binding for pinmux\n");
        return NULL;
    }

    if (pin == 3) {
        pinmux_pin_set(pinmux_dev, 63, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 3, 0, "IO3", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    } else if (pin == 5) {
        pinmux_pin_set(pinmux_dev, 64, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 5, 1, "IO5", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    } else if (pin == 6) {
        pinmux_pin_set(pinmux_dev, 65, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 6, 2, "IO6", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    } else if (pin == 9) {
        pinmux_pin_set(pinmux_dev, 66, PINMUX_FUNC_B);
        mraa_set_pininfo(board, 9, 3, "IO9", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    } else {
        printf("Pin %d not enabled/Can't be enabled\n", pin);
        return NULL;
    }
#endif

    if (board->pins[pin].capabilites.pwm != 1) {
        return NULL;
    }

    mraa_pwm_context dev = (mraa_pwm_context) malloc(sizeof(struct _pwm));
    dev->pin = pin;
    dev->phy_pin = board->pins[pin].pwm.pinmap;
    dev->zdev = device_get_binding("PWM_0");
    if (dev->zdev == NULL)
        return NULL;
    dev->period = MAX_PERIOD;
    dev->duty_percentage = DEFAULT_DUTY_CYCLE;
    printf("successfully completed init\n");
    return dev;
}

mraa_pwm_context
mraa_pwm_init_raw(int chipid, int pin)
{
    return NULL;
}

mraa_result_t
mraa_pwm_write(mraa_pwm_context dev, float percentage)
{
    if (percentage < 0.0) {
        percentage = 0.0;
    } else if (percentage > 1.0) {
        percentage = 1.0;
    }
    dev->duty_percentage = percentage;

#if defined(CONFIG_PWM_QMSI)
    uint8_t pwm_duty_val = (uint8_t)(100 * percentage);
    if (pwm_pin_set_duty_cycle(dev->zdev, dev->phy_pin, pwm_duty_val) != 0) {
        return MRAA_ERROR_UNSPECIFIED;
    }
#elif defined(CONFIG_PWM_DW)
    uint32_t on_time = (uint32_t)(percentage * dev->period);
    uint32_t off_time = dev->period - on_time;
    if (pwm_pin_set_values(dev->zdev, dev->phy_pin, on_time, off_time) != 0) {
        return MRAA_ERROR_UNSPECIFIED;
    }
#endif
    dev->duty_percentage = percentage;
    return MRAA_SUCCESS;
}

float
mraa_pwm_read(mraa_pwm_context dev)
{
    if (dev->duty_percentage > 1.0) {
        return 1.0;
    } else if (dev->duty_percentage < 0.0) {
        return 0.0;
    } else {
        return dev->duty_percentage;
    }
}

mraa_result_t
mraa_pwm_period(mraa_pwm_context dev, float seconds)
{
    return mraa_pwm_period_ms(dev, seconds * 1000);
}

mraa_result_t
mraa_pwm_period_ms(mraa_pwm_context dev, int ms)
{
    return mraa_pwm_period_us(dev, ms * 1000);
}

mraa_result_t
mraa_pwm_period_us(mraa_pwm_context dev, int us)
{
    dev->period = 32 * us;
#if defined(CONFIG_PWM_QMSI)
/**
 * the qmsi function deals in us so we don't need the
 * number of cycles for this calculation.
 * API CHANGES FROM 1.4 TO 1.5
 */
#if KERNELVERSION >= 0x1050000
    if (pwm_pin_set_period(dev->zdev, dev->phy_pin, us) != 0) {
        return MRAA_ERROR_UNSPECIFIED;
    }
#endif
#elif defined(CONFIG_PWM_DW)
    // nothing to do as of now
    // need to figure out if they put in a function for the
    // dw driver
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
#endif
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_pwm_pulsewidth(mraa_pwm_context dev, float seconds)
{
    return mraa_pwm_pulsewidth_ms(dev, seconds * 1000);
}

mraa_result_t
mraa_pwm_pulsewidth_ms(mraa_pwm_context dev, int ms)
{
    return mraa_pwm_pulsewidth_us(dev, ms * 1000);
}

mraa_result_t
mraa_pwm_pulsewidth_us(mraa_pwm_context dev, int us)
{
    uint32_t on_time = 32 * us;
    if (on_time > dev->period) {
        // the pulsewidth cannot be greater than the period
        return MRAA_ERROR_UNSPECIFIED;
    }
// API CHANGES FROM 1.4 TO 1.5
#if KERNELVERSION >= 0x1050000
    int ret = pwm_pin_set_values(dev->zdev, dev->phy_pin, 0, on_time);
#elif KERNELVERSION <= 0x1040000
    int ret = pwm_pin_set_values(dev->zdev, dev->phy_pin, on_time, dev->period - on_time);
#endif
    if (ret != 0) {
        return MRAA_ERROR_UNSPECIFIED;
    }

    return MRAA_SUCCESS;
}

mraa_result_t
mraa_pwm_enable(mraa_pwm_context dev, int enable)
{
    /**
     * this functionality is not supported by either qmsi
     * or dw, however, it is used a lot by the upm drivers
     * hence, returning success now, but need to come up with a
     * proper solution
     */
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_pwm_owner(mraa_pwm_context dev, mraa_boolean_t owner)
{
    return MRAA_ERROR_FEATURE_NOT_IMPLEMENTED;
}

mraa_result_t
mraa_pwm_close(mraa_pwm_context dev)
{
    free(dev);
    return MRAA_SUCCESS;
}

int
mraa_pwm_get_max_period(mraa_pwm_context dev)
{
    return (int) dev->period;
}

int
mraa_pwm_get_min_period(mraa_pwm_context dev)
{
    /**
     * returning period for now, might need to add another field
     * for min period
     */
    return (int) dev->period;
}
