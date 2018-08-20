/*
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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