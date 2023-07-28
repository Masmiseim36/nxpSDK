/*! *********************************************************************************
 * \defgroup ECDH
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
 * \file
 *
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * NXP Confidential Proprietary
 *
 * No part of this document must be reproduced in any form - including copied,
 * transcribed, printed or by any electronic means - without specific written
 * permission from NXP.
 ********************************************************************************** */

#ifndef _BIG_INTEGER_H
#define _BIG_INTEGER_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

uint32_t Bi_Add(const uint32_t *x, const uint32_t *y, uint32_t *result, uint8_t length);
uint32_t Bi_Sub(const uint32_t *x, const uint32_t *y, uint32_t *result, uint8_t length);
void     Bi_Copy(const uint32_t *from, uint32_t *to, uint8_t length);
#ifdef _keepUnused
int8_t Bi_IsSame(const uint32_t *A, const uint32_t *B, uint8_t length);
#endif
void   Bi_SetZero(uint32_t *A, const int length);
int8_t Bi_IsZero(const uint32_t *A);
int8_t Bi_IsOne(const uint32_t *A);
void   Bi_RightShift(uint32_t *A);
#ifdef _keepUnused
void Bi_RightShiftBy(const uint32_t *in, uint8_t in_size, uint32_t *out, uint8_t out_size, uint8_t shift);
#endif
int8_t Bi_Compare(const uint32_t *A, const uint32_t *B, uint8_t length);

#ifdef __cplusplus
}
#endif

#endif /* _BIG_INTEGER_H */

/*! *********************************************************************************
 * @}
 ********************************************************************************** */
