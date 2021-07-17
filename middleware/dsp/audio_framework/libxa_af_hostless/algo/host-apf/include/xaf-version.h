/*
* Copyright (c) 2015-2021 Cadence Design Systems Inc.
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
#ifndef __XAF_VERSION_H__
#define __XAF_VERSION_H__

#include "xa_api_defs.h"
#include "xa_af_hostless_definitions.h"

/********** Structures **********/
/* Library information */
typedef struct _xa_api_info_t
{
    char    p_copyright[XA_API_COPYRIGHT_STR_LEN]; /* Copyright string */
    char    p_lib_name[XA_API_STR_LEN];  /* Library name */
    char    p_lib_ver[XA_API_STR_LEN];   /* Library version */
    char    p_api_ver[XA_API_STR_LEN];   /* API version */
} xa_api_info_t;

#endif  // __XAF_VERSION_H__
