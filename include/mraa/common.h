/*
 * Copyright (c) 2016 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

/** @file
 *
 * This file defines the basic shared types for libmraa
 * this file is different to common.h in that swig takes this as an input
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * MRAA return codes
 */
typedef enum {
    MRAA_SUCCESS = 0,                             /**< Expected response */
    MRAA_ERROR_FEATURE_NOT_IMPLEMENTED = 1,       /**< Feature TODO */
    MRAA_ERROR_FEATURE_NOT_SUPPORTED = 2,         /**< Feature not supported by HW */
    MRAA_ERROR_INVALID_VERBOSITY_LEVEL = 3,       /**< Verbosity level wrong */
    MRAA_ERROR_INVALID_PARAMETER = 4,             /**< Parameter invalid */
    MRAA_ERROR_INVALID_HANDLE = 5,                /**< Handle invalid */
    MRAA_ERROR_NO_RESOURCES = 6,                  /**< No resource of that type avail */
    MRAA_ERROR_INVALID_RESOURCE = 7,              /**< Resource invalid */
    MRAA_ERROR_INVALID_QUEUE_TYPE = 8,            /**< Queue type incorrect */
    MRAA_ERROR_NO_DATA_AVAILABLE = 9,             /**< No data available */
    MRAA_ERROR_INVALID_PLATFORM = 10,             /**< Platform not recognised */
    MRAA_ERROR_PLATFORM_NOT_INITIALISED = 11,     /**< Board information not initialised */
    MRAA_ERROR_UART_OW_SHORTED = 12,              /**< UART OW Short Circuit Detected*/
    MRAA_ERROR_UART_OW_NO_DEVICES = 13,           /**< UART OW No devices detected */
    MRAA_ERROR_UART_OW_DATA_ERROR = 14,           /**< UART OW Data/Bus error detected */

    MRAA_ERROR_UNSPECIFIED = 99 /**< Unknown Error */
} mraa_result_t;

#ifdef __cplusplus
}
#endif
