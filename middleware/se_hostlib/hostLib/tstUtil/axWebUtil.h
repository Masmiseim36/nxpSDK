/**
 * @file axWebUtil.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2017
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
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
