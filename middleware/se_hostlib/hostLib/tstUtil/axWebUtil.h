/**
 * @file axWebUtil.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Public interface of Web utility functions
 */
#ifndef _AX_WEB_UTIL_H_
#define _AX_WEB_UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// project specific include files
#include "sm_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_INTERNAL_BUF_TOO_SMALL  (0x7060)
#define ERR_NO_MATCHING_VALUE       (0x7061)

#define AX_WEB_MAX_NAME_PATTERN 128

typedef struct {
  char *buf;
  size_t size;
} bodyMemStruct_t;

int axWebUtilJsonSimpleExtract(char *szBuf, char *szName, char *szValue, U16 bufSize);
int axWebUtilReadConvertCert(U8 *cert, U16 *certSize, FILE *fHandle);

#ifdef __cplusplus
}
#endif
#endif // _AX_WEB_UTIL_H_
