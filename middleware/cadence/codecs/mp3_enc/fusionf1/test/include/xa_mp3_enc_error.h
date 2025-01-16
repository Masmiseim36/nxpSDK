/*
 * Copyright (c) 2022 Cadence Design Systems, Inc.
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


#ifndef __XA_MP3ENC_ERROR_H__
#define __XA_MP3ENC_ERROR_H__

#include "xa_error_handler.h"

/* the module's error info structure */
extern xa_error_info_struct xa_mp3_enc_error_info;

/* the module's error handling initialization function */
VOID xa_mp3_enc_error_handler_init();

/* the message pointers	*/
extern pWORD8 ppb_xa_mp3_enc_api_non_fatal[];
extern pWORD8 ppb_xa_mp3_enc_api_fatal[];
extern pWORD8 ppb_xa_mp3_enc_config_non_fatal[];
extern pWORD8 ppb_xa_mp3_enc_config_fatal[];


#endif /* __XA_MP3ENC_ERROR_H__ */
