/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

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
