/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mraa/spi.h"
#include <device.h>
#include <malloc.h>
#include <spi.h>
#include <sys_clock.h>
#include <zephyr.h>

#define BMI160_REG_CHIPID 0x00
#define BMI160_CMD_SOFT_RESET 0xB6
#define BMI160_REG_CMD 0x7E
#define BMI160_CHIP_ID 0xD1
#define BMI160_REG_DATA_ACC_X 0x12
#define BMI160_REG_DATA_ACC_Y 0x14
#define BMI160_REG_DATA_ACC_Z 0x16

#define BMI160_REG_ACC_CONF 0x40
#define BMI160_REG_ACC_RANGE 0x41

#define SPI_DRV_NAME "SPI_1"
#define SPI_SLAVE 1

#define SET_MODE_MASK 0xfffffffc

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT printf
#else
#include <misc/printk.h>
#define PRINT printk
#endif

void
main(void)
{
    PRINT("This is the MRAA example which accesses the interal\n");
    PRINT("BMI160 on the Arduino 101 board. The chip is internally\n");
    PRINT("connected only to the ARC core SPI bus.\n");
    PRINT("The example reads the chip ID from the chip and prints and confirms it\n");

    // on both the cores, spi bus 1 seems to be the bus of choice for now
    mraa_spi_context dev = mraa_spi_init(1);
    if (dev == NULL) {
        printf("An error has occured while setting up the SPI bus\n");
        return;
    }
    if (mraa_spi_frequency(dev, 360000) != MRAA_SUCCESS) {
        printf("Unable to set up the SPI frequency\n");
        return;
    }

    // adding a 1ms delay
    sys_thread_busy_wait(1000);

    uint8_t tx_buf[2] = { BMI160_REG_CMD & 0x7f, BMI160_CMD_SOFT_RESET };
    mraa_spi_write_buf(dev, tx_buf, 2);

    sys_thread_busy_wait(1000);
    uint8_t rx_buf[2];
    uint8_t tx[3] = { 0 };
    tx[0] = 0x7f | (1 << 7);
    if (mraa_spi_transfer_buf(dev, tx, rx_buf, 2) != MRAA_SUCCESS) {
        printf("Unable to transfer data over the SPI bus\n");
    }

    sys_thread_busy_wait(1000);
    uint8_t rx[3];
    uint8_t tx_buffer[3] = { 0 };
    tx_buffer[0] = BMI160_REG_CHIPID | (1 << 7);
    if (mraa_spi_transfer_buf(dev, tx_buffer, rx, 3) != MRAA_SUCCESS) {
        printf("Unable to transfer data over the SPI bus\n");
    }

    sys_thread_busy_wait(1000);

    if (rx[1] == BMI160_CHIP_ID) {
        printf("The chip ID is: %x\n", rx[1]);
    } else {
        printf("The chip ID is: %x which is incorrect, actual chip ID is: 0xd1\n", rx[1]);
    }
}
