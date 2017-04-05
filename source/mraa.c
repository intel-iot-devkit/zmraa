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

#include "board_config.h"
#include "mraa_internal_types.h"
#include <mraa.h>
#include <stdio.h>
#include <string.h>

#if defined(CONFIG_BOARD_ARDUINO_101)
#include "arduino_101.h"
#elif defined(CONFIG_BOARD_ARDUINO_101_SSS)
#include "arduino_101_sss.h"
#elif defined(CONFIG_BOARD_QUARK_D2000_CRB)
#include "intel_d2k_crb.h"
#elif defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD)
#include "quark_se_devboard.h"
#elif defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD_SS)
#include "quark_se_sss_devboard.h"
#else
#error Board not defined
#endif


mraa_board_t* plat = NULL;
struct _gpio _internalgpios[4];

mraa_result_t
mraa_init()
{
#if defined(CONFIG_BOARD_ARDUINO_101)
    plat = mraa_intel_arduino_101();
#elif defined(CONFIG_BOARD_ARDUINO_101_SSS)
    plat = mraa_intel_arduino_101_sss();
#elif defined(CONFIG_BOARD_QUARK_D2000_CRB)
    plat = mraa_intel_d2k_crb();
#elif defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD)
    plat = mraa_intel_quark_se_devboard();
#elif defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD_SS)
    plat = mraa_intel_quark_se_ss_devboard();
#endif
    return plat != NULL ? MRAA_SUCCESS : MRAA_ERROR_NO_RESOURCES;
}


const char*
mraa_get_platform_name()
{
    return plat != NULL ? plat->platform_name : "Unknown";
}

const char*
mraa_get_version()
{
    return "v0.0.3";
}


mraa_platform_t
mraa_get_platform_type()
{
    return plat != NULL ? plat->platform_type : MRAA_UNKNOWN_PLATFORM;
}

mraa_boolean_t
mraa_has_sub_platform()
{
    return false;
}

mraa_result_t
mraa_setup_mux_mapped(mraa_pin_t meta)
{
    int mi;
    mraa_result_t ret;
    mraa_gpio_context mux_i = NULL;
    int last_pin = -1;

    for (mi = 0; mi < meta.mux_total; mi++) {

        switch (meta.mux[mi].pincmd) {
            case PINCMD_UNDEFINED: // used for backward compatibility
                if (meta.mux[mi].pin != last_pin) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    mux_i = mraa_gpio_init_raw(meta.mux[mi].pin);
                    if (mux_i == NULL)
                        return MRAA_ERROR_INVALID_HANDLE;
                    last_pin = meta.mux[mi].pin;
                }
                // this function will sometimes fail, however this is not critical as
                // long as the write succeeds - Test case galileo gen2 pin2
                mraa_gpio_dir(mux_i, MRAA_GPIO_OUT);
                ret = mraa_gpio_write(mux_i, meta.mux[mi].value);
                if (ret != MRAA_SUCCESS) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    return MRAA_ERROR_INVALID_RESOURCE;
                }
                break;

            case PINCMD_SET_VALUE:
                if (meta.mux[mi].pin != last_pin) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    mux_i = mraa_gpio_init_raw(meta.mux[mi].pin);
                    if (mux_i == NULL)
                        return MRAA_ERROR_INVALID_HANDLE;
                    last_pin = meta.mux[mi].pin;
                }

                ret = mraa_gpio_write(mux_i, meta.mux[mi].value);

                if (ret != MRAA_SUCCESS) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    return MRAA_ERROR_INVALID_RESOURCE;
                }
                break;

            case PINCMD_SET_DIRECTION:
                if (meta.mux[mi].pin != last_pin) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    mux_i = mraa_gpio_init_raw(meta.mux[mi].pin);
                    if (mux_i == NULL)
                        return MRAA_ERROR_INVALID_HANDLE;
                    last_pin = meta.mux[mi].pin;
                }

                ret = mraa_gpio_dir(mux_i, meta.mux[mi].value);

                if (ret != MRAA_SUCCESS) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    return MRAA_ERROR_INVALID_RESOURCE;
                }
                break;

            case PINCMD_SET_IN_VALUE:
                if (meta.mux[mi].pin != last_pin) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    mux_i = mraa_gpio_init_raw(meta.mux[mi].pin);
                    if (mux_i == NULL)
                        return MRAA_ERROR_INVALID_HANDLE;
                    last_pin = meta.mux[mi].pin;
                }

                ret = mraa_gpio_dir(mux_i, MRAA_GPIO_IN);

                if (ret == MRAA_SUCCESS)
                    ret = mraa_gpio_write(mux_i, meta.mux[mi].value);

                if (ret != MRAA_SUCCESS) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    return MRAA_ERROR_INVALID_RESOURCE;
                }
                break;

            case PINCMD_SET_OUT_VALUE:
                if (meta.mux[mi].pin != last_pin) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    mux_i = mraa_gpio_init_raw(meta.mux[mi].pin);
                    if (mux_i == NULL)
                        return MRAA_ERROR_INVALID_HANDLE;
                    last_pin = meta.mux[mi].pin;
                }

                ret = mraa_gpio_dir(mux_i, MRAA_GPIO_OUT);

                if (ret == MRAA_SUCCESS)
                    ret = mraa_gpio_write(mux_i, meta.mux[mi].value);

                if (ret != MRAA_SUCCESS) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    return MRAA_ERROR_INVALID_RESOURCE;
                }
                break;

            case PINCMD_SET_MODE:
                if (meta.mux[mi].pin != last_pin) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    mux_i = mraa_gpio_init_raw(meta.mux[mi].pin);
                    if (mux_i == NULL)
                        return MRAA_ERROR_INVALID_HANDLE;
                    last_pin = meta.mux[mi].pin;
                }

                ret = mraa_gpio_mode(mux_i, meta.mux[mi].value);

                if (ret != MRAA_SUCCESS) {
                    if (mux_i != NULL) {
                        mraa_gpio_owner(mux_i, 0);
                        mraa_gpio_close(mux_i);
                    }
                    return MRAA_ERROR_INVALID_RESOURCE;
                }
                break;

            case PINCMD_SKIP:
                break;

            default:
                // syslog(LOG_NOTICE, "mraa_setup_mux_mapped: wrong command %d on pin %d with value
                // %d", meta.mux[mi].pincmd, meta.mux[mi].pin, meta.mux[mi].value);
                break;
        }
    }

    if (mux_i != NULL) {
        mraa_gpio_owner(mux_i, 0);
        mraa_gpio_close(mux_i);
    }
    return MRAA_SUCCESS;
}

mraa_boolean_t
mraa_pin_mode_test(int pin, mraa_pinmodes_t mode)
{
    if (plat == NULL)
        return 0;
    mraa_board_t* current_plat = plat;
    if (current_plat->platform_type == MRAA_UNKNOWN_PLATFORM) {
        return 0;
    }
    if (pin > (current_plat->phy_pin_count - 1) || pin < 0)
        return 0;

    switch (mode) {
        case MRAA_PIN_VALID:
            if (current_plat->pins[pin].capabilites.valid == 1)
                return 1;
            break;
        case MRAA_PIN_GPIO:
            if (current_plat->pins[pin].capabilites.gpio == 1)
                return 1;
            break;
        case MRAA_PIN_PWM:
            if (current_plat->pins[pin].capabilites.pwm == 1)
                return 1;
            break;
        case MRAA_PIN_FAST_GPIO:
            if (current_plat->pins[pin].capabilites.fast_gpio == 1)
                return 1;
            break;
        case MRAA_PIN_SPI:
            if (current_plat->pins[pin].capabilites.spi == 1)
                return 1;
            break;
        case MRAA_PIN_I2C:
            if (current_plat->pins[pin].capabilites.i2c == 1)
                return 1;
            break;
        case MRAA_PIN_AIO:
            if (current_plat->pins[pin].capabilites.aio == 1)
                return 1;
            break;
        case MRAA_PIN_UART:
            if (current_plat->pins[pin].capabilites.uart == 1)
                return 1;
            break;
        default:
            // syslog(LOG_NOTICE, "requested pinmode invalid");
            break;
    }
    return 0;
}

unsigned int
mraa_adc_raw_bits()
{
    if (plat == NULL)
        return 0;

    if (plat->aio_count == 0)
        return 0;

    return plat->adc_raw;
}

unsigned int
mraa_adc_supported_bits()
{
    if (plat == NULL)
        return 0;

    if (plat->aio_count == 0)
        return 0;

    return plat->adc_supported;
}

int
mraa_get_i2c_bus_count()
{
    if (plat == NULL) {
        return -1;
    }
    return plat->i2c_bus_count;
}

int
mraa_get_i2c_bus_id(unsigned i2c_bus)
{
    if (plat == NULL) {
        return -1;
    }

    if (i2c_bus >= plat->i2c_bus_count) {
        return -1;
    }

    return plat->i2c_bus[i2c_bus].bus_id;
}

unsigned int
mraa_get_pin_count()
{
    if (plat == NULL) {
        return 0;
    }
    return plat->phy_pin_count;
}

unsigned int
mraa_get_platform_pin_count(uint8_t platform_offset)
{
    if (platform_offset == MRAA_MAIN_PLATFORM_OFFSET)
        return mraa_get_pin_count();
    else
        return 0;
}


char*
mraa_get_pin_name(int pin)
{
    if (plat == NULL)
        return NULL;
    if (pin > (plat->phy_pin_count - 1) || pin < 0)
        return NULL;
    return (char*) plat->pins[pin].name;
}

int
mraa_get_default_i2c_bus(uint8_t platform_offset)
{
    if (plat == NULL)
        return -1;
    return plat->def_i2c_bus;
}

mraa_boolean_t
mraa_is_sub_platform_id(int pin_or_bus)
{
    return 0;
}

int
mraa_get_sub_platform_id(int pin_or_bus)
{
    return -1;
}

int
mraa_get_sub_platform_index(int pin_or_bus)
{
    return -1;
}


// Internal functions
mraa_result_t
mraa_set_pininfo(mraa_board_t* board, int mraa_pin, int zephyr_pin, char* name, mraa_pincapabilities_t caps)
{
    mraa_pininfo_t* pin_info = &board->pins[mraa_pin];
    pin_info->gpio.pinmap = zephyr_pin;
#if defined(CONFIG_BOARD_ARDUINO_101_SSS) || defined(CONFIG_BOARD_QUARK_D2000_CRB) || defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD_SS)
    pin_info->aio.pinmap = zephyr_pin;
#endif
#if defined(CONFIG_BOARD_ARDUINO_101) || defined(CONFIG_BOARD_QUARK_D2000_CRB) || defined(CONFIG_BOARD_QUARK_SE_C1000_DEVBOARD)
    pin_info->uart.pinmap = zephyr_pin;
#endif
    pin_info->pwm.pinmap = zephyr_pin;
    pin_info->gpio.mux_total = 0;
    pin_info->name = name;
    pin_info->capabilites = caps;
    return MRAA_SUCCESS;
}


void
mraa_set_board_config(mraa_board_t* board)
{
    memset(board, 0, sizeof(mraa_board_t));
    board->phy_pin_count = CONFIG_MRAA_PIN_COUNT;
    board->gpio_count = CONFIG_MRAA_GPIO_COUNT;
    board->aio_count = CONFIG_MRAA_AIO_COUNT;
    board->i2c_bus_count = CONFIG_MRAA_I2C_COUNT;
    board->spi_bus_count = CONFIG_MRAA_SPI_COUNT;
    board->uart_dev_count = CONFIG_MRAA_UART_COUNT;
}
