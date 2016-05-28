/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2014, 2015 Intel Corporation.
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
#include <limits.h>
#include <errno.h>
#include <string.h>
#include "mraa/pwm.h"
#include "mraa_internal.h"

static struct _pwm tmp_pwm;


static mraa_result_t
mraa_pwm_write_period(mraa_pwm_context dev, int period)
{
    return MRAA_SUCCESS;
}

static mraa_result_t
mraa_pwm_write_duty(mraa_pwm_context dev, int duty)
{
    return MRAA_SUCCESS;
}

static int
mraa_pwm_read_period(mraa_pwm_context dev)
{
    int ret = 0;
    return (int) ret;
}

static int
mraa_pwm_read_duty(mraa_pwm_context dev)
{
    int ret = 0;
    return (int) ret;
}

mraa_pwm_context
mraa_pwm_init(int pin)
{
    mraa_board_t* board = plat;
    if (board == NULL) {
        // syslog(LOG_ERR, "pwm_init: Platform Not Initialised");
        return NULL;
    }
    if (pin < 0 || pin > board->phy_pin_count) {
        // syslog(LOG_ERR, "pwm_init: pin %i beyond platform definition", pin);
        return NULL;
    }
    if (board->pins[pin].capabilites.pwm != 1) {
        // syslog(LOG_ERR, "pwm_init: pin %i not capable of pwm", pin);
        return NULL;
    }

    if (board->pins[pin].pwm.mux_total > 0) {
        if (mraa_setup_mux_mapped(board->pins[pin].pwm) != MRAA_SUCCESS) {
            // syslog(LOG_ERR, "pwm_init: Failed to set-up pwm%i multiplexer", pin);
            return NULL;
        }
    }

    int chip = board->pins[pin].pwm.parent_id;
    int pinn = board->pins[pin].pwm.pinmap;

    return mraa_pwm_init_raw(chip, pinn);
}

mraa_pwm_context
mraa_pwm_init_raw(int chipin, int pin)
{
    mraa_pwm_context dev = &tmp_pwm;
    return dev;
}

mraa_result_t
mraa_pwm_write(mraa_pwm_context dev, float percentage)
{
    if (!dev) {
        // syslog(LOG_ERR, "pwm: write: context is NULL");
        return MRAA_ERROR_INVALID_HANDLE;
    }

    if (dev->period == -1) {
        if (mraa_pwm_read_period(dev) <= 0)
            return MRAA_ERROR_NO_DATA_AVAILABLE;
    }

    if (percentage > 1.0f) {
        // syslog(LOG_WARNING, "pwm_write: %i%% entered, defaulting to 100%%",(int) percentage * 100);
        return mraa_pwm_write_duty(dev, dev->period);
    }
    return mraa_pwm_write_duty(dev, percentage * dev->period);
}

float
mraa_pwm_read(mraa_pwm_context dev)
{
    if (!dev) {
        // syslog(LOG_ERR, "pwm: read: context is NULL");
        return MRAA_ERROR_INVALID_HANDLE;
    }

    int period = mraa_pwm_read_period(dev);
    if (period > 0) {
        return (mraa_pwm_read_duty(dev) / (float) period);
    }
    return 0.0f;
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
    int min, max;

    if (!dev) {
        // syslog(LOG_ERR, "pwm: period: context is NULL");
        return MRAA_ERROR_INVALID_HANDLE;
    }

    min = plat->pwm_min_period;
    max = plat->pwm_max_period;
    if (us < min || us > max) {
        // syslog(LOG_ERR, "pwm_period: pwm%i: %i uS outside platform range", dev->pin, us);
        return MRAA_ERROR_INVALID_PARAMETER;
    }
    return mraa_pwm_write_period(dev, us * 1000);
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
    return mraa_pwm_write_duty(dev, us * 1000);
}

mraa_result_t
mraa_pwm_enable(mraa_pwm_context dev, int enable)
{
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_pwm_unexport_force(mraa_pwm_context dev)
{
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_pwm_unexport(mraa_pwm_context dev)
{
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_pwm_close(mraa_pwm_context dev)
{
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_pwm_owner(mraa_pwm_context dev, mraa_boolean_t owner_new)
{
    if (!dev) {
        // syslog(LOG_ERR, "pwm: owner: context is NULL");
        return MRAA_ERROR_INVALID_HANDLE;
    }
    dev->owner = owner_new;
    return MRAA_SUCCESS;
}

int
mraa_pwm_get_max_period(mraa_pwm_context dev)
{
    if (plat == NULL) {
        return -1;
    }

    if (!dev) {
        // syslog(LOG_ERR, "pwm: get_max_period: context is NULL");
        return MRAA_ERROR_INVALID_HANDLE;
    }
    return plat->pwm_max_period;
}

int
mraa_pwm_get_min_period(mraa_pwm_context dev)
{
    if (plat == NULL) {
        return -1;
    }

    if (!dev) {
        // syslog(LOG_ERR, "pwm: get_min_period: context is NULL");
        return MRAA_ERROR_INVALID_HANDLE;
    }
    return plat->pwm_min_period;
}
