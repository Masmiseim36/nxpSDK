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

#ifndef _CRC_H_
#define _CRC_H_

#include "EmbeddedTypes.h"

/*!
 * @addtogroup DSP
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
 
/*! @brief crcRefIn bit definitions. */
typedef enum _crcCfgCrcRefIn
{
    gCrcInputNoRef = 0U,  /*!< Do not manipulate input data stream. */
    gCrcRefInput = 1U  /*!< Reflect each byte in the input stream bitwise. */
} crcCfgCrcRefIn_t;

/*! @brief crcRefOut bit definitions. */
typedef enum _crcCfgCrcRefOut
{
    gCrcOutputNoRef = 0U,  /*!< Do not manipulate CRC result. */
    gCrcRefOutput = 1U  /*!< CRC result is to be reflected bitwise (operated on entire word). */
} crcCfgCrcRefOut_t;

/*! @brief crcByteOrder bit definitions. */
typedef enum _crcCfgCrcByteOrd
{
    gCrcLSByteFirst = 0U,  /*!< Byte order of the CRC LS Byte first. */
    gCrcMSByteFirst = 1U   /*!< Bit order of the CRC  MS Byte first. */
} crcCfgCrcByteOrd_t;

 /*! @brief CRC configuration structure. */
typedef struct _CRC_config
{
    uint8_t crcSize;  /*!< Number of CRC octets, allowed range is 0..4. */
    uint8_t crcStartByte;  /*!< Start CRC with this byte position. Byte #0 is the first byte of Sync Address. */
    crcCfgCrcRefIn_t crcRefIn;  /*!< CRC reflect input. See "crcCfgCrcRefIn_t". */
    crcCfgCrcRefOut_t crcRefOut;  /*!< CRC reflect output. See "crcCfgCrcRefOut_t". */
    crcCfgCrcByteOrd_t crcByteOrder;  /*!< CRC byte order. See "crcCfgCrcByteOrd_t". */
    uint32_t crcSeed;  /*!< CRC Seed value. Initial value for CRC LFSR. */
    uint32_t crcPoly;  /*!< CRC Polynomial value. */
    uint32_t crcXorOut;  /*!< XOR mask for CRC result (for no mask, should be 0). */
} CRC_config_t;
 
/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name CRC
 * @{
 */

/*!
 * @brief Software CRC function.
 *
 * The function computes the CRC.
 *
 * @note The settings for software CRC are taken from the passed CRC_config_t structure.
 *
 * @param crcConfig configuration structure.
 * @param dataIn input data buffer.
 * @param length input data buffer size.
 * 
 * @retval computed CRC value.
 */
uint32_t CRC_Compute(CRC_config_t crcConfig, uint8_t *dataIn, uint16_t length);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _CRC_H_ */
