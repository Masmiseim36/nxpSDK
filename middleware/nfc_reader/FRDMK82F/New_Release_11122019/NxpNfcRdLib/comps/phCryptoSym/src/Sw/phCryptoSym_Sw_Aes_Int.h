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
* Internal AES Functions for Crypto Library
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SLe: Generated 19. May 2009
*
*/

#ifndef PHCRYPTOSYM_SW_AES_INT_H
#define PHCRYPTOSYM_SW_AES_INT_H

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phCryptoSym.h>

/** \defgroup phCryptoSym_Sw_AES_Internals Internals
* \brief Software implementation of the AES algorithm
* @{
*/

/**
* \brief Performs the combined SubBytes and Shift Rows step of the AES Algorithm for encryption as defined in Chap. 5.1.1, 5.1.2 of FIPS-197.
* First Row remains untouched, second row is shifted by 1 pos, third row is shifted by 2 pos, fourth row is shifted by 3 pos.
* \return None
*/
void phCryptoSym_Sw_Aes_SubBytesShiftRows(
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState    /**< [InOut] 16 byte array containing the state to operate on */
    );

/**
* \brief Performs the combined SubBytes and Shift Rows step of the AES Algorithm for encryption as defined in Chap. 5.3.1, 5.3.2 of FIPS-197.
* First Row remains untouched, second row is shifted by 3 pos, third row is shifted by 2 pos, fourth row is shifted by 1 pos.
* \return None
*/
void phCryptoSym_Sw_Aes_InvSubBytesShiftRows(
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState    /**< [InOut] 16 byte array containing the state to operate on */
    );

/**
* \brief Performs the MixColumn step of the AES Algorithm for encryption as defined in Chap. 5.1.3 of FIPS-197.
* The input bytes are multiplied as specified in the algorithm. The compile flag #PH_CRYPTOSYM_SW_ROM_OPTIMIZATION
* specifies whether all values to be multiplied with are taken from the lookup tables phCryptoSym_Sw_Aes_times2 and
* phCryptoSym_Sw_Aes_times3 or if phCryptoSym_Sw_Aes_times3 is calculated out of the XOR of phCryptoSym_Sw_Aes_times2 and
* the relevant value itself.
* \return None
*/
void phCryptoSym_Sw_Aes_MixColumns(
                                   uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState /**< [InOut] 16 byte array containing the state to operate on */
                                   );


/**
* \brief Performs the MixColumn step of the AES Algorithm for encryption as defined in Chap. 5.3.3 of FIPS-197.
* The input bytes are multiplied as specified in the algorithm. The compile flag #PH_CRYPTOSYM_SW_ROM_OPTIMIZATION
* specifies whether all values to be multiplied with are taken from the lookup tables phCryptoSym_Sw_Aes_times9,
* phCryptoSym_Sw_Aes_timesB, phCryptoSym_Sw_Aes_timesD and phCryptoSym_Sw_Aes_timesE or if the following simplifications are used:
* phCryptoSym_Sw_Aes_timesD = phCryptoSym_Sw_Aes_timesC XOR the value itself
* phCryptoSym_Sw_Aes_timesE = phCryptoSym_Sw_Aes_timesC XOR phCryptoSym_Sw_Aes_times2
* \return None
*/
void phCryptoSym_Sw_Aes_InvMixColumns(
                                      uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState  /**< [InOut] 16 byte array containing the state to operate on */
                                      );

/**
* \brief Performs the AddRoundKey step of the AES Algorithm for encryption and decryption as defined in Chap. 5.1.4 and 5.3.4 of FIPS-197.
* The input state is XORed with the relevant round key.
* There are two compile options that come together with AddRoundKey:
*
* #PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING is not defined:
*   In that case, the round keys have been calculated already upfront in the phCryptoSym_Sw_Aes_KeyExpansion function. The round keys are stored in
*   pDataParams->pKey using the following framing:
*   ORIGINAL KEY || RK1 || RK2 || ... || RKn
*   In total, there are a maximum of 14 Round keys + the original key = 13*16 + 32 = 240 bytes
*   Thus, the function simply picks the correct 16 byte chunk out of the round keys according to the bCnt provided.
*
* #PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING is defined:
*   In that case, the round keys are calculated online.
*   The pDataParams->pKey array has a size of 32 bytes in that case. This key is copied into a local 32 bytes buffer,
*   which then is provided to the AddRoundKey function to always update the next 16 bytes of round key according to the
*   algorithm. There are three options available for that function:
*   - PH_CRYPTOSYM_KEYSCHEDULE_ENCRYPTION: Increments pNkCurrent
*   - PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION: Decrements pNkCurrent
*   - PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION_PREPARE: Increments pNkCurrent, but does not perform a XOR operation with the state.
* \return None
*/

#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
void phCryptoSym_Sw_Aes_AddRoundKey(
                                    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState,   /**< [InOut] 16 byte array containing the state to operate on */
                                    uint8_t PH_MEMLOC_BUF * pKey,           /**< [In] 240 byte array containing all round keys to be used. */
                                    uint8_t bCnt                            /**< [In] Current round. Used by the function to pick the correct round key*/
                                    );
#else /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */
#define PH_CRYPTOSYM_KEYSCHEDULE_ENCRYPTION 0x0FU
#define PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION 0xC0U
#define PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION_PREPARE 0x30U
void phCryptoSym_Sw_Aes_AddRoundKey(
                                    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState,   /**< [InOut] 16 byte array containing the state to operate on */
                                    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pKey,     /**< [In] 32 byte array containing the current round key buffer. */
                                    /**< NOTE: always the same buffer has to be applied within one encryption.  */
                                    /**< NOTE: if this is the original key buffer, the key is overwritten!. */
                                    uint8_t * pNkCurrent,                       /**< [In] Current Nk value. (= numCurRound*4U) */
                                    uint8_t bNk,                                /**< [In] Nk for the currently loaded key type. AES128: 4, AES192: 6, AES256: 8 */
                                    uint8_t bMode                               /**< [In] Option byte indicating whether we have encryption, decryption or key preparation. */
                                    );
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */

/** @}
* end of phCryptoSym_Sw_AES_Internals group
*/

#endif /* PHCRYPTOSYM_SW_AES_INT_H */
