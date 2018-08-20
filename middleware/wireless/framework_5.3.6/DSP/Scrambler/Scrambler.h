/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
