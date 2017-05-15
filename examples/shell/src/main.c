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
#include <shell/shell.h>
#include "gpio-cmd.h"
#include "i2c-cmd.h"
#include "mraa/common.h"

#define MRAA_SHELL_MODULE "mraa"

static int shell_cmd_version(int argc, char* argv[])
{
    printf("ZMRAA %s on %s\n", mraa_get_version(), mraa_get_platform_name());
    return 0;
}

static int shell_cmd_gpio(int argc, char* argv[])
{
    argc--;
    for (int i = 0; i < argc; ++i)
        argv[i] = argv[i+1];
    if (argc > 0)
    {
        if (strcmp(argv[0], "help") == 0)
            gpio_cmd_help(argc, argv);
        else if (strcmp(argv[0], "list") == 0)
            gpio_cmd_list(argc, argv);
        else if (strcmp(argv[0], "get") == 0)
            gpio_cmd_get(argc, argv);
        else if (strcmp(argv[0], "set") == 0)
            gpio_cmd_set(argc, argv);
        else if (strcmp(argv[0], "monitor") == 0)
            gpio_cmd_monitor(argc, argv);
        else
        {
            printf("Unknown gpio command.\n");
            gpio_cmd_help(argc, argv);
        }
    }
    else
        printf("Must specify gpio command. \n");
    return 0;
}

static int shell_cmd_i2c(int argc, char* argv[])
{
    i2c_process_command(argc, argv);
    return 0;
}

const struct shell_cmd commands[] = {
    {"version", shell_cmd_version, NULL},
    {"gpio", shell_cmd_gpio, NULL},
    {"i2c", shell_cmd_i2c, NULL},
    {NULL, NULL, NULL}};

void main(void)
{
    /* Initialize mraa */
    mraa_result_t status = mraa_init();
    if (status != MRAA_SUCCESS)
    {
        printf("mraa_init() failed with error code %d\n", status);
        return;
    }

    /* Register this module w/commands */
    SHELL_REGISTER(MRAA_SHELL_MODULE, commands);

    /* Print the current shell version */
    shell_cmd_version(0, NULL);

    /* Set the default shell module and initialize */
    shell_register_default_module(MRAA_SHELL_MODULE);
}
