/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic HostComm Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHCRYPTORNG_H
#define PHCRYPTORNG_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PH_CRYPTORNG_SW

/** \defgroup phCryptoRng_Sw Component : Software
* \brief Software implementation of the phCryptoRng interface
*
* The following standard is implemented:
* - NIST Special Publication 800-90: CTR_DRBG
*
* Architecture overview of the phCryptoRng_Sw Component
* - The AES 128 implementation of the phCryptoSym component is used to generate random numbers.
* - This phCryptoSym instance has to be properly instantiated before generation of random numbers is possible.
* - The phCryptoRng_Sw_Int block implements all functions required by the NIST standard.
* - According to the state of the library component, the seed function choses whether to use phCryptoRng_Sw_Instantiate or phCryptoRng_Sw_Reseed.
* - phCryptoRng_Sw_Update is automatically called when random numbers are generated in order to be ready for retrieving the next chunk of
* random bytes.
* - phCryptoRng_Sw_BlockCipherDf is used to derive the key and V-value for a given seed.
* @{
*/

#define PH_CRYPTORNG_SW_ID             0x01U    /**< ID for Software crypto rng component. */

/**
* \brief Software parameter structure
*/
typedef struct
{
    uint16_t wId;                   /**< Layer ID for this component, NEVER MODIFY! */
    void * pCryptoDataParams;       /**< Data parameter structure for the AES engine */
    uint8_t V[16];
    uint32_t dwRequestCounter;      /**< Counts the amount of requests between two seeding procedures.
                                    Note: according to NIST SP800-90 for AES this is 2^48, for storage
                                    reasons the limit is set to 2^32 in this particular implementation. */
    uint8_t bState;
} phCryptoRng_Sw_DataParams_t;

/**
* \brief Initialise the CryptoRnd component.
*
* For seeding, the following considerations have to be taken:
* - Take a seed of at 32 bytes, which consists of
*   - entropy input
*   - nonce
*   - personalization string.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phCryptoRng_Sw_Init(
                            phCryptoRng_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                            uint16_t wSizeOfDataParams,                /**< [In] Specifies the size of the data parameter structure. */
                            void * pCryptoDataParams                   /**< [In] Pointer to the parameter structure of the symmetric crypto layer. */
                            );

/** @} */
#endif /* NXPBUILD__PH_CRYPTORNG_SW */


#ifdef NXPBUILD__PH_CRYPTORNG

/** \defgroup phCryptoRng CryptoRng
*
* \brief This is only a wrapper layer to abstract the different crypto random number generator implementations.
* With this wrapper it is possible to support more than one crypto random number generator implementation
* in parallel, by adapting this wrapper.
*
* Important hints for users of this component:
* - Before use of any function, the dedicated rng implementation has to be initialized (e.g. #phCryptoRng_Sw_Init)
* - Before random numbers can be generated, the #phCryptoRng_Seed function has to be called
* @{
*/

/**
* \brief Seeds the random number generator with the given seed.
*
* The seed length is dependend on the underlaying implementation. Further details can be found in the detailed description
* of the underlying layers.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phCryptoRng_Seed(
                               void * pDataParams,          /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t * pSeed,             /**< [In] Seed */
                               uint8_t bSeedLength          /**< [In] Size of the seed. */
                               );

/**
* \brief Obtains random bytes from the random source.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phCryptoRng_Rnd(
                            void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
                            uint16_t  wNoOfRndBytes,        /**< [In] number of random bytes to generate */
                            uint8_t * pRnd                  /**< [Out] generated bytes; uint8_t[dwNumBytes] */
                            );
/** @} */
#endif /* NXPBUILD__PH_CRYPTORNG */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHCRYPTORNG_H */
