/*
Copyright 2021 by Retune DSP
Copyright 2022 NXP

NXP Confidential. This software is owned or controlled by NXP and may only be used strictly in accordance with the applicable license terms.  By expressly accepting such terms or by downloading, installing, activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef RDSP_PARAM_BLOB_H
#define RDSP_PARAM_BLOB_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#define RDSP_PARAM_ALIGNMENT
#else
#define RDSP_PARAM_ALIGNMENT __attribute__((aligned(4)))
#endif

const uint8_t RDSP_PARAM_ALIGNMENT param_blob_header[56] = {
0x38, 0x00, 0x00, 0x00, 0xa9, 0x4b, 0xf3, 0xb2, 0x02, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 
0x02, 0x02, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 
0xa9, 0x57, 0x04, 0x00, 0x5c, 0xec, 0x06, 0x00, 0x9f, 0x50, 0x0e, 0x00, 0x50, 0x50, 0x2f, 0x00, 
0xbd, 0x6f, 0xbd, 0x00, 0x26, 0x49, 0xc6, 0x02, 
};

#ifdef __cplusplus
}
#endif
#endif // RDSP_PARAM_BLOB_H
