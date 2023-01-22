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
/*  File Name        : xa_ogg_lib_error_handler.c                            */
/*                                                                           */
/*  Description      : Error related functions of Ogg Library                */
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
/* Ogg ErrorCode Definitions                                                 */
/*****************************************************************************/

/*****************************************************************************/
/* Class 0: API Errors                                                       */
/*****************************************************************************/
/* Nonfatal Errors */

/* Fatal Errors */
const char *ppb_xa_ogg_lib_api_fatal[] = 
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
const char *ppb_xa_ogg_lib_config_non_fatal[] = 
{
    "Place holder",
};

/* Fatal Errors */
const char *ppb_xa_ogg_lib_config_fatal[] = 
{
    "Place holder",
};

/*****************************************************************************/
/* Class 2: Execution Errors                                                 */
/*****************************************************************************/

/* Nonfatal Errors */
const char *ppb_xa_ogg_lib_execute_non_fatal[] = 
{
    "Insufficient input data",
    "Spurious Ogg page ignored",
    "Stream changed",
};

/* Fatal Errors */
const char *ppb_xa_ogg_lib_execute_fatal[] = 
{
    "Corrupted Ogg stream - chaining streams with same serial numbers",
    "Output buffer allocated is too small",
    "Max page size allocated is too small",
    "Ogg library not initialized",
    "Extra packets on header page",
    "Extra packets on tags page",
};

/*****************************************************************************/
/* Error info structure                                                      */
/*****************************************************************************/

/* The Ogg Library's Error Info Structure */
xa_error_info_struct xa_ogg_lib_error_info =
{
    /* The Module Name    */
    "Cadence Ogg Library",
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
/*  Ogg Module error handler init                                            */
/*****************************************************************************/
VOID xa_ogg_lib_error_handler_init()
{
    /* The Message Pointers    */
    xa_ogg_lib_error_info.ppppb_error_msg_pointers[1][ 0] = ppb_xa_ogg_lib_api_fatal;
    xa_ogg_lib_error_info.ppppb_error_msg_pointers[0][ 1] = ppb_xa_ogg_lib_config_non_fatal;
    xa_ogg_lib_error_info.ppppb_error_msg_pointers[1][ 1] = ppb_xa_ogg_lib_config_fatal;
    xa_ogg_lib_error_info.ppppb_error_msg_pointers[0][ 2] = ppb_xa_ogg_lib_execute_non_fatal;
    xa_ogg_lib_error_info.ppppb_error_msg_pointers[1][ 2] = ppb_xa_ogg_lib_execute_fatal;
}
/* End of file. */
