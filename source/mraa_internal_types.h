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

#pragma once

#include "mraa/common.h"
#include "board_config.h"
#include <pinmux.h>

/**
 * A structure representing a gpio pin.
 */
struct _gpio {
    /*@{*/
    int pin; /**< the pin number, as known to the os. */
    int phy_pin; /**< pin passed to clean init. -1 none and raw*/
    struct device* zdev;
    mraa_boolean_t owner; /**< If this context originally exported the pin */
    /*@}*/
};

/**
 * A bitfield representing the capabilities of a pin.
 */
typedef struct {
    /*@{*/
    mraa_boolean_t valid:1;     /**< Is the pin valid at all */
    mraa_boolean_t gpio:1;      /**< Is the pin gpio capable */
    mraa_boolean_t pwm:1;       /**< Is the pin pwm capable */
    mraa_boolean_t fast_gpio:1; /**< Is the pin fast gpio capable */
    mraa_boolean_t spi:1;       /**< Is the pin spi capable */
    mraa_boolean_t i2c:1;       /**< Is the pin i2c capable */
    mraa_boolean_t aio:1;       /**< Is the pin analog input capable */
    mraa_boolean_t uart:1;      /**< Is the pin uart capable */
    /*@}*/
} mraa_pincapabilities_t;


/**
 *  Pin commands definition for mraa_mux_t struc
 */

typedef enum {
    PINCMD_UNDEFINED = 0,       // do not modify, default command for zero value, used for backward compatibility with boards where pincmd is not defined (it will be deleted later)
    PINCMD_SET_VALUE = 1,       // set a pin's value
    PINCMD_SET_DIRECTION = 2,   // set a pin's direction (value like MRAA_GPIO_OUT, MRAA_GPIO_OUT_HIGH...)
    PINCMD_SET_IN_VALUE = 3,    // set input direction and value
    PINCMD_SET_OUT_VALUE = 4,   // set output direction and value
    PINCMD_SET_MODE = 5,        // set pin's mode
    PINCMD_SKIP = 6             // just skip this command, do not apply pin and value
} pincmd_t;


/**
 * A Structure representing a multiplexer and the required value
 */
typedef struct {
    /*@{*/
    unsigned int pincmd; /**< Pin command pincmd_xxxx */
                         /**< At this time not all boards will support it -> TO DO */
    unsigned int pin;    /**< Raw GPIO pin id */
    unsigned int value;  /**< Raw GPIO value */
    /*@}*/
} mraa_mux_t;

typedef struct {
    mraa_boolean_t complex_pin:1;
    mraa_boolean_t output_en:1;
    mraa_boolean_t output_en_high:1;
    mraa_boolean_t pullup_en:1;
    mraa_boolean_t pullup_en_hiz:1;
} mraa_pin_cap_complex_t;

typedef struct {
    /*@{*/
    unsigned int pinmap; /**< sysfs pin */
    // unsigned int parent_id; /** parent chip id */
    unsigned int mux_total; /** Numfer of muxes needed for operation of pin */
    mraa_mux_t mux[1]; /** Array holding information about mux */
    unsigned int output_enable; /** Output Enable GPIO, for level shifting */
    int pinmux;
    /*@}*/
} mraa_pin_t;

/**
 * A Structure representing a physical Pin.
 */
typedef struct {
    /*@{*/
    char *name; /**< Pin's real world name */
    mraa_pincapabilities_t capabilites; /**< Pin Capabiliites */
    mraa_pin_t gpio; /**< GPIO structure */
    mraa_pin_t pwm;  /**< PWM structure */
    mraa_pin_t aio;  /**< Anaglog Pin */
    mraa_pin_t i2c;  /**< i2c bus/pin */
    mraa_pin_t spi;  /**< spi bus/pin */
    mraa_pin_t uart;  /**< uart module/pin */
    /*@}*/
} mraa_pininfo_t;

/**
 * A Structure representing the physical properties of a i2c bus.
 */
typedef struct {
    /*@{*/
    unsigned int bus_id; /**< ID as exposed in the system */
    unsigned int scl; /**< i2c SCL */
    unsigned int sda; /**< i2c SDA */
    // mraa_drv_api_t drv_type; /**< Driver type */
    /*@}*/
} mraa_i2c_bus_t;

/**
 * A Structure representing the physical properties of a spi bus.
 */
typedef struct {
    /*@{*/
    unsigned int bus_id; /**< The Bus ID as exposed to the system. */
    unsigned int slave_s; /**< Slave select */
    mraa_boolean_t three_wire; /**< Is the bus only a three wire system */
    unsigned int sclk; /**< Serial Clock */
    unsigned int mosi; /**< Master Out, Slave In. */
    unsigned int miso; /**< Master In, Slave Out. */
    unsigned int cs; /**< Chip Select, used when the board is a spi slave */
    /*@}*/
} mraa_spi_bus_t;

/**
 * A Structure representing a uart device.
 */
typedef struct {
    /*@{*/
    unsigned int index; /**< ID as exposed in the system */
    int rx; /**< uart rx */
    int tx; /**< uart tx */
    const char* device_path; /**< To store "/dev/ttyS1" for example */
    /*@}*/
} mraa_uart_dev_t;


/**
 * A Structure representing a platform/board.
 */

typedef struct _board_t {
    /*@{*/
    unsigned int phy_pin_count; /**< The Total IO pins on board */
    unsigned int gpio_count; /**< GPIO Count */
    unsigned int aio_count;  /**< Analog side Count */
    unsigned int i2c_bus_count; /**< Usable i2c Count */
    mraa_i2c_bus_t  i2c_bus[CONFIG_MRAA_I2C_COUNT]; /**< Array of i2c */
    unsigned int def_i2c_bus; /**< Position in array of default i2c bus */
    unsigned int spi_bus_count; /**< Usable spi Count */
    mraa_spi_bus_t spi_bus[CONFIG_MRAA_SPI_COUNT];       /**< Array of spi */
    unsigned int def_spi_bus; /**< Position in array of defult spi bus */
    unsigned int adc_raw; /**< ADC raw bit value */
    unsigned int adc_supported; /**< ADC supported bit value */
    unsigned int def_uart_dev; /**< Position in array of defult uart */
    unsigned int uart_dev_count; /**< Usable spi Count */
    mraa_uart_dev_t uart_dev[CONFIG_MRAA_UART_COUNT]; /**< Array of UARTs */
    mraa_boolean_t no_bus_mux; /**< i2c/spi/adc/pwm/uart bus muxing setup not required */
    int pwm_default_period; /**< The default PWM period is US */
    int pwm_max_period; /**< Maximum period in us */
    int pwm_min_period; /**< Minimum period in us */
    mraa_platform_t platform_type; /**< Platform type */
    const char* platform_name; /**< Platform Name pointer */
    const char* platform_version; /**< Platform versioning info */
    mraa_pininfo_t pins[CONFIG_MRAA_PIN_COUNT];     /**< Pointer to pin array */
    /*@}*/
} mraa_board_t;
