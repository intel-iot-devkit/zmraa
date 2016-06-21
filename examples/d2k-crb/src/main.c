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
#include <nanokernel.h>
#include "bmc150.h"

static void upm_print_driver_sample(upm_bmc150_magn dev)
{
    double value_x, value_y, value_z;
    mraa_result_t ret = upm_bmc150_magn_get_value(dev, &value_x, &value_y, &value_z);
    if (ret == MRAA_SUCCESS) {
        printf("magn = ( %f  %f  %f )\n", value_x, value_y, value_z);
    } else {
        printf("upm_bmc150_magn_get_value failed. Code = %d\n", ret);
        return;
    }
}

void
main(void)
{
    mraa_result_t status = mraa_init();
    if (status == MRAA_SUCCESS) {
        printf("Version %s on %s\n", mraa_get_version(), mraa_get_platform_name());
        upm_bmc150_magn upm_dev = upm_bmc150_magn_init(0);
        if (upm_dev == NULL) {
            printf("upm_bmc150_magn_init failed\n");
            return;
        }
        while (1) {
            upm_print_driver_sample(upm_dev);
            task_sleep(sys_clock_ticks_per_sec/20);
        }
    } else
        printf("mraa_init() failed with error code %d\n", status);
}
