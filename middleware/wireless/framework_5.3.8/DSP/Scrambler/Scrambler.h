/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _SCRAMBLER_H_
#define _SCRAMBLER_H_

#include "EmbeddedTypes.h"

/*!
 * @addtogroup DSP
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief whitenPolyType bit definitions. */
typedef enum _whitenPolyType
{
    gGaloisPolyType = 0U,  /*!< A Galois type LFSR is used with the whiten polynomial. */
    gFibonnaciPolyType = 1U  /*!< A Fibonacci type LFSR is used with the whiten polynomial. */
} whitenPolyType_t;

/*! 
 * @brief whitenRefIn bit definitions. 
 *
 * @note The input data stream is reflected bit-wise, per byte. Bit 7 becomes bit 0, bit 6 becomes bit 1, etc. 
 * Will only cause the reflection of the payload data bits as they are used in the whiten calculation
 * and will not cause any change in the output bit order.
 */
typedef enum _whitenCfgRefIn
{
    gWhitenInputNoRef = 0U,  /*!< Do not manipulate input data stream. */
    gWhitenRefInput = 1U  /*!< Reflect each byte in the input stream bitwise. */
} whitenCfgRefIn_t;

/*! * @brief Whitener module configure structure. */
typedef struct _Whitener_config
{
    whitenPolyType_t whitenPolyType;  /*!< Whiten polynomial type. See "whitenPolyType_t". */
    whitenCfgRefIn_t whitenRefIn;  /*!< Whiten reflect input. See "whitenCfgRefIn_t". */
    uint8_t whitenStartOffset;  /*!< Configure Whitener start point. */
    uint8_t whitenEndOffset;  /*!< Configure end-of-whitening. */
    uint8_t whitenSize;  /*!< Length of whitener LFSR. Maximum value 9. */
    uint16_t whitenInit;  /*!< Initialization value for Whitening/De-whitening. Maximum 9 bits. */
    uint16_t whitenPoly;  /*!< Whitener polynomial. The polynomial value must be right-justified if smaller than 9-bits. Maximum 9 bits. */    
} Whitener_config_t;
 
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Scrambler
 * @{
 */

/*!
 * @brief Software whitening function.
 *
 * The function whitens/de-whitens the input buffer.
 *
 * @note This function will store the result in the input buffer.
 *
 * @param whitenerConfig whitener configuration structure pointer. See "Whitener_config_t".
 * @param pBuff buffer pointer.
 * @param buffLength buffer length.
 */
void Whiten(Whitener_config_t *whitenerConfig, uint8_t *pBuff, uint8_t buffLength);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _SCRAMBLER_H_ */
