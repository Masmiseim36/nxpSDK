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
/*  File Name        : xa_common_error_handler.c                             */
/*                                                                           */
/*  Description      : Error related common functions                        */
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
/* xa_testbench ErrorCode Definitions                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Class 0: Memory & File Manager Errors                                     */
/*****************************************************************************/
/* Non Fatal Errors */

/* Fatal Errors */
const char *ppb_xa_testbench_mem_file_fatal[] = 
{
    "Memory Allocation Error",
    "File Open Failed",
    "Opus Header Read Failed"
};

/*****************************************************************************/
/* Error info structure                                                      */
/*****************************************************************************/

/* The Testbench's Error Info Structure */
xa_error_info_struct xa_testbench_error_info =
{
    /* The Module Name    */
    "xa_testbench",
    {
        /* The Class Names    */
        "Memory & File Manager",
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
/*  Testbench error handler init                                             */
/*****************************************************************************/
VOID xa_testbench_error_handler_init()
{
    /* The Message Pointers    */
    xa_testbench_error_info.ppppb_error_msg_pointers[1][ 0] = ppb_xa_testbench_mem_file_fatal;
}

XA_ERRORCODE xa_error_handler(
                xa_error_info_struct *p_mod_err_info,
                const char *pb_context,
                XA_ERRORCODE code
) 
{
    WORD is_fatal     = XA_ERROR_SEVERITY(code) != 0;
    WORD err_class    = XA_ERROR_CLASS(code);
    WORD err_sub_code = XA_ERROR_SUBCODE(code);

    if (code == XA_NO_ERROR) 
    {
        return XA_NO_ERROR;
    }

    printf("\n");

    if (!is_fatal) 
    {
        printf("non ");
    }
    printf("fatal error: ");

    if ((p_mod_err_info->pb_module_name != NULL) && (p_mod_err_info->pb_module_name[0] != '\0')) 
    {
        printf("%s: ", p_mod_err_info->pb_module_name);
    }

    if (p_mod_err_info->ppb_class_names[err_class] != NULL) 
    {
        printf("%s: ", p_mod_err_info->ppb_class_names[err_class]);
    }

    if (pb_context != NULL) 
    {
        printf("%s: ", pb_context);
    }
    
    printf("%s\n", p_mod_err_info->ppppb_error_msg_pointers[is_fatal][err_class][err_sub_code]);

    return XA_NO_ERROR;
}
/* End of file. */
