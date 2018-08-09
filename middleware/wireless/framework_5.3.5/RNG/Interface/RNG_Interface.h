/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* RNG implementation header file for the ARM CORTEX-M4 processor
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

#ifndef _RNG_INTERFACE_H_
#define _RNG_INTERFACE_H_

#include "EmbeddedTypes.h"


/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define gRngSuccess_d       (0x00)
#define gRngInternalError_d (0x01)
#define gRngNullPointer_d   (0x80)
#define gRngMaxRequests_d   (100000)


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public function prototypes
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief  Initialize the RNG module
*
* \return eror code
*
********************************************************************************** */
uint8_t RNG_Init(void);


/*! *********************************************************************************
* \brief  Returns a 32-bit statistically random number
*
* \param[out] pRandomNo - pointer to location where the RN will be stored
*
********************************************************************************** */
void RNG_GetRandomNo(uint32_t* pRandomNo);


/*! *********************************************************************************
* \brief  Reads a 32-bit statistically random number from HW (if available)
*
* \param[out] pRandomNo - pointer to location where the RN will be stored
*
* \return error code
*
********************************************************************************** */
uint8_t RNG_HwGetRandomNo(uint32_t* pRandomNo);


/*! *********************************************************************************
* \brief  Initialize seed for the PRNG algorithm.
*
* \param[in]  pSeed - Pointer to the seed (32 bytes). 
*                     Can be set using the RNG_GetRandomNo() function
*
********************************************************************************** */
void RNG_SetPseudoRandomNoSeed(uint8_t* pSeed);


/*! *********************************************************************************
* \brief  Generates a NIST FIPS Publication 186-2-compliant 160 bit pseudo-random number
*
* \param[out]     pOut - pointer to the output buffer (max 32 bytes)
* \param[in]      outBytes - the number of bytes to be copyed (1-32)
* \param[in]      pXSEED - optional user SEED (32 bytes). Should be NULL if not used.
*
* \return         The number of bytes copied or -1 if reseed is needed
*
********************************************************************************** */
int16_t RNG_GetPseudoRandomNo(uint8_t* pOut, uint8_t outBytes, uint8_t* pXSEED);

/*! *********************************************************************************
* \brief  Returns a random number beween 0 and 256
*
* \return random number
*
********************************************************************************** */
uint32_t RND_u32GetRand256(void);

/*! *********************************************************************************
* \brief  Returns a random number beween the specified minum and maximum values
*
* \param[in] u32Min  minimum value
* \param[in] u32Max  maximum value
*
* \return random number
*
********************************************************************************** */
uint32_t RND_u32GetRand(uint32_t u32Min, uint32_t u32Max);

#endif /* _RNG_INTERFACE_H_ */