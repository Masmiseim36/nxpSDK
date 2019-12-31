/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _FEC_H_
#define _FEC_H_

#include "EmbeddedTypes.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief  */
#define mStages_c (2U)

/*! @brief  */
#define mNoStages_c (1 << mStages_c)


/*! @brief Error codes for the FEC driver. */
typedef enum fecStatus_tag
{
    gFecSuccess_c = 0U,
    gFecLengthExceeded_c = 1U,
}fecStatus_t;

/*! @brief Viterbi info structure. */
typedef struct viterbiInfo_tag
{
    const uint8_t * cwLut;
    const uint8_t * decLut;
    const uint8_t * hammingLut;
    uint32_t K;
    uint32_t stages;
} viterbiInfo_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief 
 *
 * This function... 
 *
 * @param pIntrlvLut
 * @param pViterbiLuts
 */
void FEC_LutsInit (const uint32_t **pIntrlvLut, viterbiInfo_t *pViterbiLuts, const uint8_t **pConvencLut);

/*!
 * @brief 
 *
 * This function... 
 *
 * @param pInput
 * @param pOutput
 * @param pIntrlvLut
 * @param length
 */
void FEC_Interleaver(uint8_t *pInput, uint32_t *pOutput, const uint32_t *pIntrlvLut, uint32_t length);

/*!
 * @brief 
 *
 * This function... 
 *
 * @param pInput
 * @param pOutput
 * @param viterbiLuts
 * @param length
 */
fecStatus_t FEC_ViterbiDecoder(uint8_t *pInput, uint8_t *pOutput, viterbiInfo_t viterbiLuts, uint32_t length);

/*!
 * @brief 
 *
 * This function... 
 *
 * @param pInput
 * @param pOutput
 * @param pConvencLut
 * @param length
 */
void FEC_ConvEncoder(uint8_t *pInput, uint8_t *pOutput, const uint8_t *pConvencLut, uint32_t length);

#endif /* _FEC_H_ */ 