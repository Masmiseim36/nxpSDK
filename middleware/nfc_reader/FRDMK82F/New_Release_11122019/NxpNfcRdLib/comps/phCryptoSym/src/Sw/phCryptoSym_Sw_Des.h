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
* DES implementation of the Symmetric Cryptography Library.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SLe: Generated 01.12.2009
*  SLe: Alpha Release 10.01.2010
*
*/

#ifndef PHCRYPTOSYM_SW_DES_H
#define PHCRYPTOSYM_SW_DES_H

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phCryptoSym.h>

/** \defgroup phCryptoSym_Sw_DES Cipher : DES
* \brief Software implementation of the DES algorithm
* @{
*/

/**
* \brief Performs a decryption of a DES Block (8 byte) using a DES Key.
* The key needs to be provided in the pDataParams structure. By specifying the bKeyNumber, either the
* first, the second or the third key is used.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoSym_Sw_Des_DecryptBlock(
    phCryptoSym_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pBlock,   /**< [InOut] Block to be enciphered in place. */
    uint8_t bKeyNumber                          /**< [In] Number of the key to be used (0, 1 or 2) */
    );

/**
* \brief Performs an encryption of a DES Block (8 byte) using a DES Key.
* The key needs to be provided in the pDataParams structure. By specifying the bKeyNumber, either the
* first, the second or the third key is used.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoSym_Sw_Des_EncryptBlock(
    phCryptoSym_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pBlock,   /**< [InOut] Block to be enciphered in place. */
    uint8_t bKeyNumber                          /**< [In] Number of the key to be used (0, 1 or 2) */
    );

/**
* \brief Performs the key Expansion of the DES key(s)
* According to the number of keys specified, the pDataParams key array is filled up in the following sequence:
* Expanded KEY1 [128] || Expanded KEY2 [128] || Expanded KEY3 [128]
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phCryptoSym_Sw_Des_KeyInit(
                                      phCryptoSym_Sw_DataParams_t * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
                                      const uint8_t * pKey,                         /**< [In] Key array to be loaded into the DES engine. */
                                      uint8_t bNumKeys                              /**< [In] Amount of keys provided (1, 2 or 3) */
                                      );

/**
* \brief Decode the KeyVersion of a DES key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phCryptoSym_Sw_Des_DecodeVersion(
    uint8_t * pKey,         /**< [In] Key to decode. */
    uint16_t * pKeyVersion  /**< [Out] Version of the key. */
    );

/**
* \brief Encode the KeyVersion in a DES key.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Operation aborted.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phCryptoSym_Sw_Des_EncodeVersion(
    uint8_t * pKey,         /**< [In] Key to encode. */
    uint16_t wKeyVersion,   /**< [In] Version of the key. */
    uint16_t wKeyType,      /**< [In] Type of Key. */
    uint8_t * pEncodedKey   /**< [Out] Encoded key. */
    );

/** @}
* end of phCryptoSym_Sw_DES group
*/

#endif /* PHCRYPTOSYM_SW_DES_H */
