/*
 * Copyright (c) 2014-2022 Cadence Design Systems, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/*****************************************************************************/
/*                                                                           */
/*  File Name        : xa_opus_codec_error_handler.c                         */
/*                                                                           */
/*  Description      : Error related functions of opus codec             */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* File includes                                                             */
/*****************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "xa_type_def.h"
#include "xa_error_standards.h"
#include "xa_error_handler.h"

/*****************************************************************************/
/* Opus ErrorCode Definitions                                             */
/*****************************************************************************/

/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Nonfatal Errors */

/* Fatal Errors */
const char *ppb_xa_opus_codec_api_fatal[] = 
{
    "NULL Pointer: Memory Allocation Error",
    "Memory Allocation Error: Alignment requirement not met",
    "Invalid Command",
    "Invalid Command Type/Index"
};


/*****************************************************************************/
/* Class 1: Configuration Errors                                             */
/*****************************************************************************/
/* Nonfatal Errors */
const char *ppb_xa_opus_codec_config_non_fatal[] = 
{
    "Invalid encoder channel force setting",
    "Invalid encoder max. bandwidth setting",
    "Invalid encoder bandwidth setting",
    "Invalid encoder complexity setting",
    "Invalid encoder expected packet loss percentage setting",
    "Invalid encoder force mode setting",
    "Invalid encoder signal type setting",
    "Invalid decoder gain adjustment setting",
    "Decoder output stream map ignored for channel mapping 0"
};

/* Fatal Errors */
const char *ppb_xa_opus_codec_config_fatal[] = 
{
    "Sampling frequency not supported",
    "Number of channel not supported",
    "Application mode not supported",
    "Bitrate not supported",
    "Invalid config parameter combination",
    "Number of streams not supported",
    "Only Opus streams with major version 0  are supported",
    "Channel mapping value must be 0 or 1",
    "Bad stream map"
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/

/* Nonfatal Errors */
const char *ppb_xa_opus_codec_execute_non_fatal[] = 
{
    "Invalid argument",
    "Insufficient data",
    "Internal error",
    "Corrupted stream",
    "warning: Unable to enforce the forced mode, hence overriding the forced mode with appropriate mode set"
};

/* Fatal Errors */
const char *ppb_xa_opus_codec_execute_fatal[] = 
{
    "OPUS Init failure. ",
    "OPUS not initialized. ",
    "OPUS parameters changed. OPUS must be reinitialized ",
    "OPUS encoder output buffer too short. ",
    "OPUS decoder Payload error. The compressed data passed is corrupted",
    "OPUS encoder invalid number of input samples"
};

/*****************************************************************************/
/* Error info structure                                                      */
/*****************************************************************************/

/* The OPUS Codec's Error Info Structure */
xa_error_info_struct xa_opus_codec_error_info =
{
    /* The Module Name    */
    "Cadence OPUS Codec",
    {
        /* The Class Names    */
        "API",
        "Configuration",
        "Execution",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        ""
    },
    {
        /* The Message Pointers    */
        {   
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
        },
        {   
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
        }
    }
};

/*****************************************************************************/
/*  OPUS Module error handler init                                             */
/*****************************************************************************/
VOID xa_opus_codec_error_handler_init()
{
    /* The Message Pointers    */
    xa_opus_codec_error_info.ppppb_error_msg_pointers[1][ 0] = ppb_xa_opus_codec_api_fatal;
    xa_opus_codec_error_info.ppppb_error_msg_pointers[0][ 1] = ppb_xa_opus_codec_config_non_fatal;
    xa_opus_codec_error_info.ppppb_error_msg_pointers[1][ 1] = ppb_xa_opus_codec_config_fatal;
    xa_opus_codec_error_info.ppppb_error_msg_pointers[0][ 2] = ppb_xa_opus_codec_execute_non_fatal;
    xa_opus_codec_error_info.ppppb_error_msg_pointers[1][ 2] = ppb_xa_opus_codec_execute_fatal;
}
/* End of file. */
