/*
 * Author: Henry Bruce <henry.bruce@intel.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mraa/i2c.h"


void
print_version()
{
    printf("Version %s on %s\n", mraa_get_version(), mraa_get_platform_name());
}

mraa_result_t
i2c_get(int bus, uint8_t device_address, uint8_t register_address, uint8_t* data)
{
    mraa_result_t status = MRAA_SUCCESS;
    mraa_i2c_context i2c = mraa_i2c_init(bus);
    if (i2c == NULL) {
        return MRAA_ERROR_NO_RESOURCES;
    }
    status = mraa_i2c_address(i2c, device_address);
    if (status != MRAA_SUCCESS) {
        goto i2c_get_exit;
    }
    status = mraa_i2c_write_byte(i2c, register_address);
    if (status != MRAA_SUCCESS) {
        goto i2c_get_exit;
    }
    status = mraa_i2c_read(i2c, data, 1) == 1 ? MRAA_SUCCESS : MRAA_ERROR_UNSPECIFIED;
    if (status != MRAA_SUCCESS) {
        goto i2c_get_exit;
    }
i2c_get_exit:
    mraa_i2c_stop(i2c);
    return status;
}


static void
i2c_detect_devices(int bus)
{
    mraa_i2c_context i2c = mraa_i2c_init(bus);
    if (i2c == NULL) {
        return;
    }
    int addr;
    for (addr = 0x0; addr < 0x80; ++addr) {
        uint8_t value;
        if ((addr) % 16 == 0)
            printf("%02x: ", addr);
        if (i2c_get(bus, addr, 0, &value) == MRAA_SUCCESS)
            printf("%02x ", addr);
        else
            printf("-- ");
        if ((addr + 1) % 16 == 0)
            printf("\n");
    }
}

void
main(void)
{
    mraa_result_t status = mraa_init();
    if (status == MRAA_SUCCESS) {
        print_version();
        i2c_detect_devices(0);
    } else {
        printf("mraa_init() failed with error code %d\n", status);
    }
}
