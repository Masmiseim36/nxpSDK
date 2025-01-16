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


 #ifndef __XA_DRM_DEC_DRM_UTILS_H__
#define __XA_DRM_DEC_DRM_UTILS_H__

#include "xa_drm_dec_sample_testbench.h"

#define ENABLE_INTERFACE_HEADER_SUPPORT
#define INTERFACE_HEADER_SIZE      44

XA_ERRORCODE DRM_InputOpen(HANDLE_DRM *hDataSrc, char *filename);
XA_ERRORCODE DRM_InputRead(HANDLE_DRM *drm,buffer_t *inpbuf);

#ifdef ENABLE_INTERFACE_HEADER_SUPPORT
void create_default_config(XA_DRM_DEC_DRM_AUDIO_CONFIG *p_drmconfig);
XA_ERRORCODE inputReadInterfaceHeader(
    HANDLE_DRM *hDataSrc,
    XA_DRM_DEC_DRM_AUDIO_CONFIG *p_drmconfig,
    int *p_headerlength);
#endif/*ENABLE_INTERFACE_HEADER_SUPPORT*/

void DRM_inputClose(HANDLE_DRM *hDataSrc);

#endif // __XA_DRM_DEC_DRM_UTILS_H__

