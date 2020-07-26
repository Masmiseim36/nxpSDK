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
* AES implementation of the Symmetric Cryptography Library.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SLe: Generated 01.12.2009
*  SLe: Alpha Release 10.01.2010
*
*/

#ifndef PHCRYPTOSYM_SW_AES_H
#define PHCRYPTOSYM_SW_AES_H

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phCryptoSym.h>

/** \defgroup phCryptoSym_Sw_AES Cipher : AES
* \brief Software implementation of the AES algorithm
* @{
*/

/**
* \brief Performs the key Expansion of the AES key (128, 192, 256 bits).
* According to the size of the key specified, the pDataParams key array is filled up in the following sequence:
* pKey || pRk1 ..... ||pRkN
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoSym_Sw_Aes_KeyExpansion(
    phCryptoSym_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
    const uint8_t * pKey,                       /**< [In] Key array to be loaded into the AES engine. */
    uint8_t bNkCurrent,                         /**< [In] Size of the key provided (in 32 bit granularity). */
    uint8_t bNkMax                              /**< [In] Total amount of 32 bit Values to be loaded. */
    );

/**
* \brief Performs a encryption of an AES Block (16 byte) using an AES Key.
* The key needs to be provided in the pDataParams structure. By specifying the bNumRounds, the amount of AES rounds to be executed is defined.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoSym_Sw_Aes_EncryptBlock(
    phCryptoSym_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pBlock,   /**< [InOut] Block to be enciphered in place. */
    uint8_t bNumRounds                          /**< [In] Number of rounds according to AES algorithm. */
    );

/**
* \brief Performs a decryption of an AES Block (16 byte) using an AES Key.
* The key needs to be provided in the pDataParams structure. By specifying the bNumRounds, the amount of AES rounds to be executed is defined.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoSym_Sw_Aes_DecryptBlock(
    phCryptoSym_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pBlock,   /**< [InOut] Block to be deciphered in place. */
    uint8_t bNumRounds                          /**< [In] Number of rounds according to AES algorithm. */
    );

/** @}
* end of phCryptoSym_Sw_AES group
*/

#endif /* PHCRYPTOSYM_SW_AES_H */
