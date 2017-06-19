/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
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

/* From drivers/pinmux/quark_mcu/pinmux_board_arduino_101.c
 * This is the full pinmap that we have available on the board for configuration
 * including the ball position and the various modes that can be set.  In the
 * _pinmux_defaults we do not spend any time setting values that are using mode
 * A as the hardware brings up all devices by default in mode A.
 */
#include <stdio.h>
#include <pinmux.h>
#include <pinmux/stm32/pinmux_stm32.h>

#include "mraa_internal.h"
#include "mraa/gpio.h"

static mraa_board_t _board;
static const char* platform_name = "STM32 Nucleo L476RG";

mraa_board_t* mraa_stm32_nucleo_l476rg()
{
    mraa_board_t* b = &_board;
    mraa_set_board_config(b);
    b->platform_name = platform_name;
    b->platform_type = MRAA_STM32_NUCLEO_L476RG;

    // Arduino Header
    // valid, gpio, pwm, fast gpio?, spi, i2c, aio, uart
    mraa_set_pininfo(b, 0, 3, "PA_3", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 1, 1 }, STM32_PIN_PA3);
    mraa_set_pininfo(b, 1, 2, "PA_2", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 1, 1 }, STM32_PIN_PA2);
    mraa_set_pininfo(b, 2, 10, "PA_10", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 1 }, STM32_PIN_PA10);
    mraa_set_pininfo(b, 3, 3, "PB_3", (mraa_pincapabilities_t){ 1, 1, 1, 0, 1, 0, 0, 0 }, STM32_PIN_PB3);
    mraa_set_pininfo(b, 4, 5, "PB_5", (mraa_pincapabilities_t){ 1, 1, 1, 0, 1, 0, 0, 0 }, STM32_PIN_PB5);
    mraa_set_pininfo(b, 5, 4, "PB_4", (mraa_pincapabilities_t){ 1, 1, 1, 0, 1, 0, 0, 0 }, STM32_PIN_PB4);
    mraa_set_pininfo(b, 6, 10, "PB_10", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 1, 0, 1 }, STM32_PIN_PB10);
    mraa_set_pininfo(b, 7, 8, "PA_8", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 }, STM32_PIN_PA8);
    mraa_set_pininfo(b, 8, 9, "PA_9", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 1 }, STM32_PIN_PA9);
    mraa_set_pininfo(b, 9, 7, "PA_7", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 1 }, STM32_PIN_PA7);
    mraa_set_pininfo(b, 10, 6, "PB_6", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 1, 0, 0 }, STM32_PIN_PB6);
    mraa_set_pininfo(b, 11, 7, "PA_7", (mraa_pincapabilities_t){ 1, 1, 1, 0, 1, 0, 1, 0 }, STM32_PIN_PA7);
    mraa_set_pininfo(b, 12, 6, "PA_6", (mraa_pincapabilities_t){ 1, 1, 1, 0, 1, 0, 1, 0 }, STM32_PIN_PA6);
    mraa_set_pininfo(b, 13, 5, "PA_5", (mraa_pincapabilities_t){ 1, 1, 1, 0, 1, 0, 1, 0 }, STM32_PIN_PA5);
    mraa_set_pininfo(b, 14, 9, "PB_9", (mraa_pincapabilities_t){ 1, 1, 1, 0, 1, 1, 0, 0 }, STM32_PIN_PB9);
    mraa_set_pininfo(b, 15, 8, "PB_8", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 1, 0, 0 }, STM32_PIN_PB8);
    mraa_set_pininfo(b, 16, 0, "PA_0", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 1, 1 }, STM32_PIN_PA0);
    mraa_set_pininfo(b, 17, 1, "PA_1", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 }, STM32_PIN_PA1);
    mraa_set_pininfo(b, 18, 4, "PA_4", (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 1, 0 }, STM32_PIN_PA4);
    mraa_set_pininfo(b, 19, 0, "PB_0", (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 1, 0 }, STM32_PIN_PB0);
    mraa_set_pininfo(b, 20, 1, "PC_1", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 1, 1 }, STM32_PIN_PC1);
    mraa_set_pininfo(b, 21, 0, "PC_0", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 1, 1 }, STM32_PIN_PC0);

    b->def_i2c_bus = 0;
    // buses are numbered 1,2,3 in this board
    // but mraa prefers 0,1,2
    b->i2c_bus[0].bus_id = 0;
    b->i2c_bus[1].bus_id = 1;
    b->i2c_bus[2].bus_id = 2;
    // l476rg bus 1
    b->pins[14].i2c.mux_total = 0;
    b->pins[15].i2c.mux_total = 0;
    // l476rg bus 3
    b->pins[20].i2c.mux_total = 0;
    b->pins[21].i2c.mux_total = 0;

    b->i2c_bus[0].sda = 14;
    b->i2c_bus[0].scl = 15;
    b->i2c_bus[2].sda = 20;
    b->i2c_bus[2].scl = 21;

    // spi bus numbers start from 1 on this board
    // but mraa prefers to start from 0
    b->spi_bus[0].bus_id = 1;
    b->spi_bus[0].sclk = 13;
    b->spi_bus[0].mosi = 11;
    b->spi_bus[0].miso = 12;
    b->spi_bus[0].cs = 10;

    return b;
}
