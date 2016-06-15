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

// For bmc150 test
#include <device.h>
#include <sensor.h>

void
print_version()
{
    printf("Version %s on %s\n", mraa_get_version(), mraa_get_platform_name());
}


static void print_driver_sample(struct device *dev)
{
    int ret;
    struct sensor_value value_x, value_y, value_z;
    ret = sensor_sample_fetch(dev);
    if (ret) {
        printf("sensor_sample_fetch failed ret %d\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_MAGN_X, &value_x);
    ret = sensor_channel_get(dev, SENSOR_CHAN_MAGN_Y, &value_y);
    ret = sensor_channel_get(dev, SENSOR_CHAN_MAGN_Z, &value_z);
    printf("( x y z ) = ( %f  %f  %f )\n", value_x.dval, value_y.dval, value_z.dval);
}



void
main(void)
{
    mraa_result_t status = mraa_init();
    if (status == MRAA_SUCCESS) {
        print_version();
        struct device *dev = device_get_binding("bmc150_magn");
        if (dev != NULL) {
            while (1) {
                print_driver_sample(dev);
                task_sleep(sys_clock_ticks_per_sec/20);
            }
        } else
            printf("Failed to open %s\n", "upm_bmc150");
    } else {
        printf("mraa_init() failed with error code %d\n", status);
    }
}
