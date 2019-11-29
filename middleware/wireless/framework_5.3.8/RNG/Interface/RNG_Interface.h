/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

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