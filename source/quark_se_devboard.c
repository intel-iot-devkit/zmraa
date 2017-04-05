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

/* From drivers/pinmux/quark_mcu/pinmux_board_arduino_101.c
 * This is the full pinmap that we have available on the board for configuration
 * including the ball position and the various modes that can be set.  In the
 * _pinmux_defaults we do not spend any time setting values that are using mode
 * A as the hardware brings up all devices by default in mode A.
 */

/* pin, ball, mode A, mode B, mode C */
/* 0  F02, gpio_0, ain_0, spi_s_cs */			/* IO10 */
/* 1  G04, gpio_1, ain_1, spi_s_miso */			/* IO12 */
/* 2  H05, gpio_2, ain_2, spi_s_sck */			/* IO13 */
/* 3  J06, gpio_3, ain_3, spi_s_mosi */			/* IO11 */
/* 4  K06, gpio_4, ain_4, NA */
/* 5  L06, gpio_5, ain_5, NA */
/* 6  H04, gpio_6, ain_6, NA */
/* 7  G03, gpio_7, ain_7, NA */
/* 8  L05, gpio_ss_0, ain_8, uart1_cts */
/* 9  M05, gpio_ss_1, ain_9, uart1_rts */		/* AD5 */
/* 10 K05, gpio_ss_2, ain_10 */				/* AD0 */
/* 11 G01, gpio_ss_3, ain_11 */				/* AD1 */
/* 12 J04, gpio_ss_4, ain_12 */				/* AD2 */
/* 13 G02, gpio_ss_5, ain_13 */				/* AD3 */
/* 14 F01, gpio_ss_6, ain_14 */				/* AD4 */
/* 15 J05, gpio_ss_7, ain_15 */
/* 16 L04, gpio_ss_8, ain_16, uart1_txd */		/* IO1 */
/* 17 M04, gpio_ss_9, ain_17, uart1_rxd */		/* IO0 */
/* 18 K04, uart0_rx, ain_18, NA */
/* 19 B02, uart0_tx, gpio_31, NA */
/* 20 C01, i2c0_scl, NA, NA */
/* 21 C02, i2c0_sda, NA, NA */
/* 22 D01, i2c1_scl, NA, NA */
/* 23 D02, i2c1_sda, NA, NA */
/* 24 E01, i2c0_ss_sda, NA, NA */
/* 25 E02, i2c0_ss_scl, NA, NA */
/* 26 B03, i2c1_ss_sda, NA, NA */
/* 27 A03, i2c1_ss_scl, NA, NA */
/* 28 C03, spi0_ss_miso, NA, NA */
/* 29 E03, spi0_ss_mosi, NA, NA */
/* 30 D03, spi0_ss_sck, NA, NA */
/* 31 D04, spi0_ss_cs0, NA, NA */
/* 32 C04, spi0_ss_cs1, NA, NA */
/* 33 B04, spi0_ss_cs2, gpio_29, NA */
/* 34 A04, spi0_ss_cs3, gpio_30, NA */
/* 35 B05, spi1_ss_miso, NA, NA */
/* 36 C05, spi1_ss_mosi, NA, NA */
/* 37 D05, spi1_ss_sck, NA, NA */
/* 38 E05, spi1_ss_cs0, NA, NA */
/* 39 E04, spi1_ss_cs1, NA, NA */
/* 40 A06, spi1_ss_cs2, uart0_cts, NA */
/* 41 B06, spi1_ss_cs3, uart0_rts, NA */
/* 42 C06, gpio_8, spi1_m_sck, NA */
/* 43 D06, gpio_9, spi1_m_miso, NA */
/* 44 E06, gpio_10, spi1_m_mosi, NA */
/* 45 D07, gpio_11, spi1_m_cs0, NA */
/* 46 C07, gpio_12, spi1_m_cs1, NA */
/* 47 B07, gpio_13, spi1_m_cs2, NA */
/* 48 A07, gpio_14, spi1_m_cs3, NA */
/* 49 B08, gpio_15, i2s_rxd, NA */			/* IO5 */
/* 50 A08, gpio_16, i2s_rscki, NA */			/* IO8 */
/* 51 B09, gpio_17, i2s_rws, NA */			/* IO3 */
/* 52 A09, gpio_18, i2s_tsck, NA */			/* IO2 */
/* 53 C09, gpio_19, i2s_twsi, NA */			/* IO4 */
/* 54 D09, gpio_20, i2s_txd, NA */			/* IO7 */
/* 55 D08, gpio_21, spi0_m_sck, NA */
/* 56 E07, gpio_22, spi0_m_miso, NA */
/* 57 E09, gpio_23, spi0_m_mosi, NA */
/* 58 E08, gpio_24, spi0_m_cs0, NA */
/* 59 A10, gpio_25, spi0_m_cs1, NA */
/* 60 B10, gpio_26, spi0_m_cs2, NA */
/* 61 C10, gpio_27, spi0_m_cs3, NA */
/* 62 D10, gpio_28, NA, NA */
/* 63 E10, gpio_ss_10, pwm_0, NA */			/* IO3 */
/* 64 D11, gpio_ss_11, pwm_1, NA */			/* IO5 */
/* 65 C11, gpio_ss_12, pwm_2, NA */			/* IO6 */
/* 66 B11, gpio_ss_13, pwm_3, NA */			/* IO9 */
/* 67 D12, gpio_ss_14, clkout_32khz, NA */
/* 68 C12, gpio_ss_15, clkout_16mhz, NA */

/* Note:
 * 1. I2C pins on the shield are connected to i2c0_ss_sda and i2c_0_ss_scl,
 *    which are on the sensor subsystem. They are also tied to AD4 and AD5.
 *    Therefore, to use I2C, pin 9 (ain_9) and (ain_14) both need to be set
 *    to PINMUX_FUNC_B, so they will not interfere with I2C operations.
 *    Also, there is no internal pull-up on I2c bus, and thus external
 *    pull-up resistors are needed.
 * 2. IO3/PWM0 is connected to pin 51 and 63.
 * 3. IO5/PWM1 is connected to pin 49 and 64.
 */

#include <stdio.h>

#include <pinmux.h>
#include "mraa_internal.h"
#include "mraa/gpio.h"

static mraa_board_t _board;

mraa_board_t* mraa_intel_quark_se_devboard()
{
    mraa_board_t* b = &_board;
    mraa_set_board_config(b);
    b->platform_name = "Quark SE Devboard/C1000 x86";
b->phy_pin_count = 94;
    //b->platform_type = MRAA_INTEL_QUARK_SE_DEVBOARD;

// header - J14
#if 1
    mraa_set_pininfo(b,  1,  1, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  2,  2, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });    
    mraa_set_pininfo(b,  3,  31, "AP_UART0_TXD_GPIO31",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });  // defaults to UART
    mraa_set_pininfo(b,  4,  0, "ARDUINO_IO_1_UART_TXD",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });  // defaults to UART
    mraa_set_pininfo(b,  5,  31, "AP_UART0_RXD_ADC18",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });  // defaults to UART
    mraa_set_pininfo(b,  6,  18, "ARDUINO_IO_0_UART_RXD",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });  // defaults to UART
    mraa_set_pininfo(b,  7,  2, "AP_SPI1_SS_CS2_N_UART0_CTS_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });  // defaults to UART
    mraa_set_pininfo(b,  8,  8, "ARDUINO_CTS",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });  // defaults to UART
    mraa_set_pininfo(b,  9,  3, "AP_SPI1_SS_CS3_N_UART0_RTS_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });  // defaults to UART
    mraa_set_pininfo(b,  10, 9, "ARDUINO_RTS",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });  // defaults to UART
    mraa_set_pininfo(b,  11, 11, "GND",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  12, 12, "GND",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  13, 4, "AP_GPIO4_ADC4",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });  // defaults to GPIO
    mraa_set_pininfo(b,  14, 14, "AP_GPIO_SS2_ADC10",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 });  // NA on x86 core
    mraa_set_pininfo(b,  15, 5, "AP_GPIO5_ADC5",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });  // defaults to GPIO
    mraa_set_pininfo(b,  16, 16, "AP_GPIO_SS3_ADC11",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 });  // NA on x86 core
    mraa_set_pininfo(b,  17, 6, "AP_GPIO6_ADC6_MUX",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });  // defaults to GPIO
    mraa_set_pininfo(b,  18, 18, "AP_GPIO_SS4_ADC12",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 });  // NA on x86 core
    mraa_set_pininfo(b,  19, 14, "AP_GPIO_SS14_PLT_CLK_0",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });  // defaults to SS GPIO/clk/keep number in mind
    mraa_set_pininfo(b,  20, 20, "AP_GPIO_SS5_ADC13",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 });  // NA on x86 core
    mraa_set_pininfo(b,  21, 15, "AP_GPIO_SS15_PLT_CLK_1",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });  // defaults to SS GPIO/clk/keep number in mind
    mraa_set_pininfo(b,  22, 22, "AP_GPIO_SS6_ADC14",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 });  // NA on X86 core
    mraa_set_pininfo(b,  23, 23, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  24, 20, "AP_GPIO_SS7_ADC15",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 });  // NA on x86 core
    mraa_set_pininfo(b,  25, 0, "AP_GPIO_SS10_PWM0",  (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  26, 26, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  27, 1, "AP_GPIO_SS11_PWM1",  (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  28, 15, "AP_GPIO15_I2S_RXD",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  29, 2, "AP_GPIO_SS12_PWM2",  (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  30, 15, "AP_GPIO16_I2S_RSCK",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  31, 3, "AP_GPIO_SS13_PWM3",  (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  32, 15, "AP_GPIO17_I2S_RWS",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  33, 33, "GND",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  34, 18, "AP_GPIO18_I2S_TSCK",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  35, 0, "AP_GPIO_AON0",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });    // this does not work|couldn't find mapping
    mraa_set_pininfo(b,  36, 19, "AP_GPIO19_I2S_TWS",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  37, 1, "AP_GPIO_AON1",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });    // this does not work|couldn't find mapping
    mraa_set_pininfo(b,  38, 20, "AP_GPIO20_I2S_TXD",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  39, 2, "AP_GPIO_AON2",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });    // this does not work|couldn't find mapping
    mraa_set_pininfo(b,  40, 40, "GND",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  41, 3, "AP_GPIO_AON3",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });    // this does not work|couldn't find mapping
    mraa_set_pininfo(b,  42, 1, "SP_I2C1_SS_SDA",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 1, 0, 0 });  // IMU
    mraa_set_pininfo(b,  43, 4, "AP_GPIO_AON4",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });    // this does not work|couldn't find mapping
    mraa_set_pininfo(b,  44, 1, "SP_I2C1_SS_SCL",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 1, 0, 0 });  // IMU
    mraa_set_pininfo(b,  45, 5, "AP_GPIO_AON5",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });    // this does not work|couldn't find mapping
    mraa_set_pininfo(b,  46, 0, "XVREF",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  47, 7, "AP_GPIO7_ADC7",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  48, 0, "ARD_RESET_N_R",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  49, 49, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  50, 50, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
#endif
// the other big header - J15
    mraa_set_pininfo(b,  51, 51, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  52, 52, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  53, 0, "AP_SPI0_SS_SCK",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  54, 21, "AP_GPIO21_SPI0_M_SCK",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  55, 0, "AP_SPI0_SS_MISO",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  56, 22, "AP_GPIO22_SPI0_M_MISO",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  57, 0, "AP_SPI0_SS_MOSI",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  58, 23, "AP_GPIO23_SPI0_M_MOSI",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  59, 0, "AP_SPI0_SS_CS0_N",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  60, 24, "AP_GPIO24_SPI0_M_CS0_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  61, 0, "AP_SPI0_SS_CS1_N",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  62, 25, "AP_GPIO25_SPI0_M_CS1_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  63, 29, "AP_SPI0_SS_CS2_N_GPIO29",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  64, 26, "AP_GPIO26_SPI0_M_CS2_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  65, 30, "AP_SPI0_SS_CS3_N_GPIO30",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  66, 27, "AP_GPIO27_SPI0_M_CS3_N_MUX",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  67, 67, "GND",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  68, 68, "GND",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  69, 0, "AP_SPI1_SS_SCK",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  70, 8, "AP_GPIO8_SPI1_M_SCK",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  71, 0, "AP_SPI1_SS_MISO",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  72, 9, "AP_GPIO9_SPI1_M_MISO",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  73, 0, "AP_SPI1_SS_MOSI",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  74, 10, "AP_GPIO10_SPI1_M_MOSI",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  75, 0, "AP_SPI1_SS_CS0_N",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  76, 11, "AP_GPIO11_SPI1_M_CS0_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  77, 0, "AP_SPI1_SS_CS1_N",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  78, 12, "AP_GPIO12_SPI1_M_CS1_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  79, 79, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  80, 13, "AP_GPIO13_SPI1_M_CS2_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  81, 0, "AP_I2C0_SCL",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 1, 0, 0 });
    mraa_set_pininfo(b,  82, 14, "AP_GPIO14_SPI1_M_CS3_N",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  83, 0, "AP_I2C0_SDA",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 1, 0, 0 });
    mraa_set_pininfo(b,  84, 84, "VDD_HDR",  (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  85, 0, "AP_I2C0_SS_SCL",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 1, 0, 0 });
    mraa_set_pininfo(b,  86, 2, "AP_GPIO2_ADC2_SPI_S_SCK",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  87, 0, "AP_I2C0_SS_SDA",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 1, 0, 0 });
    mraa_set_pininfo(b,  88, 1, "AP_GPIO1_ADC1_SPI_S_MISO",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  89, 0, "AP_I2C1_SCL",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 1, 0, 0 });
    mraa_set_pininfo(b,  90, 3, "AP_GPIO3_ADC3_SPI_S_MOSI",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b,  91, 0, "AP_I2C1_SDA",  (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 1, 0, 0 });
    mraa_set_pininfo(b,  92, 0, "AP_GPIO0_ADC3_SPI_S_MOSI",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });

// Regular Arduino header
/*
    mraa_set_pininfo(b,  0,  0, "IO0",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });
    mraa_set_pininfo(b,  1,  3, "IO1",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 1 });
    mraa_set_pininfo(b,  2, 18, "IO2",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  3,  0, "IO3",  (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  4, 19, "IO4",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  5,  1, "IO5",  (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  6,  2, "IO6",  (mraa_pincapabilities_t){ 0, 0, 1, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  7, 20, "IO7",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  8, 16, "IO8",  (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b,  9,  3, "IO9",  (mraa_pincapabilities_t){ 1, 1, 1, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b, 10,  0, "IO10", (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b, 11,  3, "IO11", (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b, 12,  1, "IO12", (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b, 13,  2, "IO13", (mraa_pincapabilities_t){ 1, 1, 0, 0, 1, 0, 0, 0 });
    mraa_set_pininfo(b, 14,  0, "A0  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b, 15,  0, "A1  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b, 16,  0, "A2  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b, 17,  0, "A3  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 0, 0, 0 });
    mraa_set_pininfo(b, 18, 14, "A4  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 0, 0 });
    mraa_set_pininfo(b, 19,  9, "A5  ", (mraa_pincapabilities_t){ 1, 1, 0, 0, 0, 1, 0, 0 });
*/
    b->def_i2c_bus = 0;
    b->i2c_bus[0].bus_id = 0;
    b->i2c_bus[1].bus_id = 1;
    b->pins[18].i2c.mux_total = 0;
    b->pins[19].i2c.mux_total = 0;
    b->i2c_bus[0].sda = 18;
    b->i2c_bus[0].scl = 19;
#if defined (CONFIG_SPI_0)
    b->spi_bus[0].bus_id = 0;
#elif defined (CONFIG_SPI_1)
    b->spi_bus[0].bus_id = 1;
#elif defined (CONFIG_SPI_2)
    b->spi_bus[0].bus_id = 2;
#endif
    b->spi_bus[0].sclk = 13;
    b->spi_bus[0].mosi = 11;
    b->spi_bus[0].miso = 12;
    b->spi_bus[0].cs = 10;

    int i2c_raw_gpios[] = { 9, 14, 24, 25 };
    struct device* zdev = device_get_binding("GPIO_0");
    if (zdev != NULL) {
        for (int i = 0; i<4; ++i) {
            int ret = gpio_pin_configure(zdev, i2c_raw_gpios[i], GPIO_PUD_PULL_UP);
            if (ret) {
                //printf("Failed to set pull up for pin %d\n", i2c_raw_gpios[i]);
            }
        }
    }
/*
    for (int i = 0; i<4; ++i) {
        mraa_gpio_context gpio = mraa_gpio_init_raw(i2c_raw_gpios[i]);
        mraa_gpio_dir(gpio, MRAA_GPIO_IN);
    }
*/


#if 0
    struct device* pinmux_dev = device_get_binding(CONFIG_PINMUX_NAME);
    if (pinmux_dev == NULL) {
        printf("Failed to get binding for %s\n", CONFIG_PINMUX_NAME);
        return NULL;
    }
    if (pinmux_pin_set(pinmux_dev, 9, PINMUX_FUNC_B)) {
        printf("Failed to set pinmux for %d\n", 9);
        return NULL;
    }
    if (pinmux_pin_set(pinmux_dev, 14, PINMUX_FUNC_B)) {
        printf("Failed to set pinmux for %d\n", 14);
        return NULL;
    }
    if (pinmux_pin_set(pinmux_dev, 24, PINMUX_FUNC_A)) {
        printf("Failed to set pinmux for %d\n", 24);
        return NULL;
    }
    if (pinmux_pin_set(pinmux_dev, 25, PINMUX_FUNC_A)) {
        printf("Failed to set pinmux for %d\n", 25);
        return NULL;
    }
#endif

    return b;
}
