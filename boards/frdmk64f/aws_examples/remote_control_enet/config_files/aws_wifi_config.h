/*
 * Amazon FreeRTOS V1.2.0
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file aws_wifi_config.h
 * @brief WiFi module configuration parameters.
 */

#ifndef _AWS_WIFI_CONFIG_H_
#define _AWS_WIFI_CONFIG_H_

/**
 * @brief Max SSID length.
 */
#define wificonfigMAX_SSID_LEN                ( 32 )

/**
 * @brief Max BSSID length.
 */
#define wificonfigMAX_BSSID_LEN               ( 6 )

/**
 * @brief Max passphrase length.
 */
#define wificonfigMAX_PASSPHRASE_LEN          ( 32 )

/**
 * @brief DNS query timeout when getting a host IP.
 */
#define wificonfigDNS_QUERY_TIMEOUT           ( 10000 )

#endif
