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
#include <misc/shell.h>
#include "mraa/common.h"
#include "gpio-cmd.h"


void
print_version()
{
    printf("Version %s on %s\n", mraa_get_version(), mraa_get_platform_name());
}

static void
shell_cmd_version(int argc, char* argv[])
{
    print_version();
}

static void
shell_cmd_gpio(int argc, char* argv[])
{
printf("argc = %d\n", argc);
    argc--;
    for (int i = 0; i < argc; ++i)
        argv[i] = argv[i+1];
    if (argc > 0) {
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
        else {
            printf("Unknown gpio command.\n");
            gpio_cmd_help(argc, argv);
        }
    } else
        printf("Must specify gpio command. \n");
}

static void
shell_cmd_i2c(int argc, char* argv[])
{
    printf("i2c commands not yet implemented\n");
}



const struct shell_cmd commands[] = { { "version", shell_cmd_version },
                                      { "gpio", shell_cmd_gpio },
                                      { "i2c", shell_cmd_i2c },
                                      { NULL, NULL } };

void
main(void)
{
    mraa_result_t status = mraa_init();
    if (status == MRAA_SUCCESS) {
        print_version();
        shell_init("mraa> ", commands);
    } else {
        printf("mraa_init() failed with error code %d\n", status);
    }
}
