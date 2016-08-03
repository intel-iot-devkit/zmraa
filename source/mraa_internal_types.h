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

#include <gpio.h>
#include <i2c.h>
#include <adc.h>
#include <pinmux.h>
#include "mraa/common.h"
#include "board_config.h"

// Pack structures to save memory
#pragma pack(push, 1)

/**
 * A structure representing a gpio pin.
 */
struct _gpio {
    /*@{*/
    uint8_t pin; /**< the pin number, as known to the os. */
    int8_t phy_pin; /**< pin passed to clean init. -1 none and raw*/
    struct device* zdev;  /**< Zephyr device driver object */
    struct gpio_callback zcallback; /**< Zephyr gpio callback object */
    mraa_boolean_t owner; /**< If this context originally exported the pin */
    void (* isr)(void *); /**< the interupt service request */
    void *isr_args; /**< args return when interupt service request triggered */
    /*@}*/
};


/**
 * A structure representing a I2C bus
 */
struct _i2c {
    /*@{*/
    int8_t busnum; /**< the bus number of the /dev/i2c-* device */
    uint8_t addr; /**< the address of the i2c slave */
    struct device* zdev;
    union dev_config zcfg;
    /*@}*/
};

/**
 * A structure representing a PWM pin
 */
struct _pwm {
    /*@{*/
    int pin; /**< the pin number, as known to the os. */
    int8_t phy_pin; /**< pin passed to clean init. -1 none and raw*/
    int period;  /**< Cache the period to speed up setting duty */
    float duty_percentage;
    struct device* zdev;
    /*@}*/
};

/**
 * A structure representing a aio pin.
 */
struct _aio {
    /*@{*/
    uint8_t pin; /**< the pin number, as known to the os. */
    int8_t phy_pin; /**< pin passed to clean init. -1 none and raw*/
    struct device* zdev;  /**< Zephyr device driver object */
    int value_bit; /**< 10 bits by default. Can be increased based on board */
    struct adc_seq_table* table; /** struct to hold list of samples */
    uint8_t seq_buffer[4]; /** buffer to save the analog values that have been read */
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
    uint8_t pincmd; /**< Pin command pincmd_xxxx */
                         /**< At this time not all boards will support it -> TO DO */
    uint8_t pin;    /**< Raw GPIO pin id */
    uint8_t value;  /**< Raw GPIO value */
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
    uint8_t pinmap; /**< Zephyr pin */
    uint8_t parent_id;  /**< Zephyr driver id */
    uint8_t mux_total; /** Numfer of muxes needed for operation of pin */
    mraa_mux_t mux[CONFIG_MRAA_PINMUX_COUNT]; /** Array holding information about mux */
    uint8_t output_enable; /** Output Enable GPIO, for level shifting */
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
#if CONFIG_MRAA_GPIO_COUNT > 0
    mraa_pin_t gpio; /**< GPIO structure */
#endif
#if CONFIG_MRAA_PWM_COUNT > 0
    mraa_pin_t pwm;  /**< PWM structure */
#endif
#if CONFIG_MRAA_AIO_COUNT > 0
    mraa_pin_t aio;  /**< Anaglog Pin */
#endif
#if CONFIG_MRAA_I2C_COUNT > 0
    mraa_pin_t i2c;  /**< i2c bus/pin */
#endif
#if CONFIG_MRAA_SPI_COUNT > 0
    mraa_pin_t spi;  /**< spi bus/pin */
#endif
#if CONFIG_MRAA_UART_COUNT > 0
    mraa_pin_t uart;  /**< uart module/pin */
#endif
    /*@}*/
} mraa_pininfo_t;

/**
 * A Structure representing the physical properties of a i2c bus.
 */
typedef struct {
    /*@{*/
    uint8_t bus_id; /**< ID as exposed in the system */
    uint8_t scl; /**< i2c SCL */
    uint8_t sda; /**< i2c SDA */
    // mraa_drv_api_t drv_type; /**< Driver type */
    /*@}*/
} mraa_i2c_bus_t;

/**
 * A Structure representing the physical properties of a spi bus.
 */
typedef struct {
    /*@{*/
    uint8_t bus_id; /**< The Bus ID as exposed to the system. */
    uint8_t slave_s; /**< Slave select */
    mraa_boolean_t three_wire; /**< Is the bus only a three wire system */
    uint8_t sclk; /**< Serial Clock */
    uint8_t mosi; /**< Master Out, Slave In. */
    uint8_t miso; /**< Master In, Slave Out. */
    uint8_t cs; /**< Chip Select, used when the board is a spi slave */
    /*@}*/
} mraa_spi_bus_t;

/**
 * A Structure representing a uart device.
 */
typedef struct {
    /*@{*/
    uint8_t index; /**< ID as exposed in the system */
    int8_t rx; /**< uart rx */
    int8_t tx; /**< uart tx */
    const char* device_path; /**< To store "/dev/ttyS1" for example */
    /*@}*/
} mraa_uart_dev_t;


/**
 * A Structure representing a platform/board.
 */

typedef struct _board_t {
    /*@{*/
    uint8_t phy_pin_count; /**< The Total IO pins on board */
    uint8_t gpio_count; /**< GPIO Count */
    uint8_t aio_count;  /**< Analog side Count */
    uint8_t i2c_bus_count; /**< Usable i2c Count */
    mraa_i2c_bus_t  i2c_bus[CONFIG_MRAA_I2C_COUNT]; /**< Array of i2c */
    uint8_t def_i2c_bus; /**< Position in array of default i2c bus */
    uint8_t spi_bus_count; /**< Usable spi Count */
    mraa_spi_bus_t spi_bus[CONFIG_MRAA_SPI_COUNT];       /**< Array of spi */
    uint8_t def_spi_bus; /**< Position in array of defult spi bus */
    uint8_t adc_raw; /**< ADC raw bit value */
    uint8_t adc_supported; /**< ADC supported bit value */
    uint8_t def_uart_dev; /**< Position in array of defult uart */
    uint8_t uart_dev_count; /**< Usable spi Count */
    mraa_uart_dev_t uart_dev[CONFIG_MRAA_UART_COUNT]; /**< Array of UARTs */
    mraa_boolean_t no_bus_mux; /**< i2c/spi/adc/pwm/uart bus muxing setup not required */
#if CONFIG_MRAA_PWM_COUNT > 0
    int pwm_default_period; /**< The default PWM period is US */
    int pwm_max_period; /**< Maximum period in us */
    int pwm_min_period; /**< Minimum period in us */
#endif
    mraa_platform_t platform_type; /**< Platform type */
    const char* platform_name; /**< Platform Name pointer */
    const char* platform_version; /**< Platform versioning info */
    mraa_pininfo_t pins[CONFIG_MRAA_PIN_COUNT];     /**< Pointer to pin array */
    /*@}*/
} mraa_board_t;

#pragma pack(pop)
