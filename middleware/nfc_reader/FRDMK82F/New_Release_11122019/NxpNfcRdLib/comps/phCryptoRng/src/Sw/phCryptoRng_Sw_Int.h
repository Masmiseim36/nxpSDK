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
* Software Implementation of the CTR_DRBG algorithm as specified in NIST SP 800-90 using the AES algorithm.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHCRYPTORNG_SW_INT_H
#define PHCRYPTORNG_SW_INT_H

#include <ph_Status.h>
/** \addtogroup phCryptoRng_Sw_Internals Internals
* \brief Software implementation of the RND Generator
*  @{
*/

/* Note: according to NIST SP800-90 for AES this is 2^48, for TDEA it would be 2^32, for storage
reasons the limit is set to 2^32 in this particular implementation. */
#define PHCRYPTORNG_SW_MAX_REQUESTS 0xFFFFFFFFU

/**
* \brief Block Length for the cipher used.
*/
#define PHCRYPTORNG_SW_OUTLEN   16U

/**
* \brief Block Length for the cipher used.
*/
#define PHCRYPTORNG_SW_KEYLEN   16U

/**
* \brief Seed length to be provided.
*/
#define PHCRYPTORNG_SW_SEEDLEN  (PHCRYPTORNG_SW_OUTLEN + PHCRYPTORNG_SW_KEYLEN)
#define PHCRYPTORNG_SW_MAX_BITS_DF_FUNCTION   512U

#define PHCRYPTORNG_SW_STATE_INIT       0x00U /* Default State */
#define PHCRYPTORNG_SW_STATE_WORKING    0x01U /* Working State */

/**
* \brief Implements the update function according to NIST SP800-90 section 10.2.1.2.
* Using provided data generated in one of #phCryptoRng_Sw_Instantiate, #phCryptoRng_Sw_Reseed
* and #phCryptoRng_Sw_Generate function the values of pDataParams->V and the key loaded in the
* AES module are updated.
* Provided data can be null, if this is the case, no XOR is performed. This is not recommendet for
* use during Instantiate functionality!.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoRng_Sw_Update(
                                 phCryptoRng_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                 uint8_t * pProvidedData                    /**< [In] Provided data of length #PHCRYPTORNG_SW_SEEDLEN and
                                                                                generated using #phCryptoRng_Sw_Instantiate, #phCryptoRng_Sw_Reseed
                                                                                and #phCryptoRng_Sw_Generate function (can be NULL). */
                                 );

/**
* \brief Implements the instantiate function according to NIST SP800-90 section 10.2.1.3.2 (using derivation function).
* Note: the length of all inputs together needs to be exactly #PHCRYPTORNG_SW_SEEDLEN
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoRng_Sw_Instantiate(
                                phCryptoRng_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                                uint8_t * pEntropyInput,                    /**< [In] Entropy input of size bEntropyInputLength. */
                                uint16_t wEntropyInputLength,               /**< [In] Length of the entropy input. */
                                uint8_t * pNonce,                           /**< [In] Nonced as specified in Section 8.6.7. of NIST SP800-90. */
                                uint8_t bNonceLength,                       /**< [In] Length of the nonce provided. */
                                uint8_t * pPersonalizationString,           /**< [In] Personalization string of size bPersonalizationString. */
                                uint8_t bPersonalizationString              /**< [In] Length of the Personalization string. */
                                );

/**
* \brief Implements the reseed function according to section 10.2.1.4.2 (using derivation function).
* Note: the length of all inputs together needs to be exactly #PHCRYPTORNG_SW_SEEDLEN
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoRng_Sw_Reseed(
                                phCryptoRng_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                                uint8_t * pEntropyInput,                    /**< [In] Entropy input of size bEntropyInputLength. */
                                uint16_t wEntropyInputLength,               /**< [In] Length of the entropy input. */
                                uint8_t * pAdditionalInput,                 /**< [In] Additional Input. */
                                uint8_t bAdditionalInputLength              /**< [In] Length of Additional Input provided. */
                                );

/**
* \brief Implements the generate function according to section 10.2.1.5.2 (using derivation function).
* Note: the length of the additional input needs to be exactly #PHCRYPTORNG_SW_SEEDLEN.
* If an application does not support additional input, the pointer has to be set to NULL.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoRng_Sw_Generate(
                                phCryptoRng_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                                uint8_t * pAdditionalInput,                 /**< [In] Additional Input can be NULL). */
                                uint16_t wNumBytesRequested,                /**< [In] Amount of bytes requested. */
                                uint8_t * pRndBytes                         /**< [Out] Random bytes generated. */
                                );

/**
* \brief Implements the BlockCipherDf according to NIST SP800-90 section 10.4.2.
* Note: inside there are 10 encryptions performed. Although this takes quite some time, the implication on
* overall system performance is rather low as this function is only called at startup and during reseeding.
* Note: The input string needs to be exactly #PHCRYPTORNG_SW_SEEDLEN bytes.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoRng_Sw_BlockCipherDf(
                                phCryptoRng_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                                uint8_t * pIoString                         /**< [InOut] pIoString of PHCRYPTORNG_SW_SEEDLEN bytes - contains input data and later output data. */
                                );

/** @}
* end of phCryptoRng_Sw_Int group
*/


#endif /* PHCRYPTORNG_SW_INT_H */
