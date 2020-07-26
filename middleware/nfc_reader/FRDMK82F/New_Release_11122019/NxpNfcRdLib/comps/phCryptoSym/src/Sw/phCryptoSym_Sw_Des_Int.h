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
* Internal DES Functions for Crypto Library
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SLe: Generated 19. May 2009
*
*/

#ifndef PHCRYPTOSYM_SW_DES_INT_H
#define PHCRYPTOSYM_SW_DES_INT_H

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phCryptoSym.h>

/** \defgroup phCryptoSym_Sw_DES_Internals Internals
* \brief Software implementation of the DES algorithm
* @{
*/

/**
* \brief Compute a permutation as defined in table IP in section Enciphering of FIPS46.3.
* The implementation is chosen such that the trade off between ROM usage and execution
* performance is optimum for (small) uC architectures. For implementation details refer to
* the detailed documentation inside of the function body.
* \return None
*/
void  phCryptoSym_Sw_Des_Permutate_IP(
                                      uint8_t PH_CRYTOSYM_SW_FAST_RAM * pStateIn, /**< [In] State pointer containing the initial state. */
                                      uint8_t PH_CRYTOSYM_SW_FAST_RAM * pStateOut /**< [Out] State pointer containing the permutated state. */
                                      );

/**
* \brief Compute a permutation as defined in table IP-1 (inverse IP) in section Enciphering of FIPS46.3.
* The implementation is chosen such that the trade off between ROM usage and execution
* performance is optimum for (small) uC architectures. For implementation details refer to
* the detailed documentation inside of the function body.
* \return None
*/
void  phCryptoSym_Sw_Des_Permutate_IP_Inv(
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pStateIn, /**< [In] State pointer containing the computed state. */
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pStateOut /**< [Out] State pointer containing the final state. */
    );

/**
* \brief Compute a permutation as defined in table PC1 of Appendix 1 of FIPS46.3.
* The implementation is chosen such that the trade off between ROM usage and execution
* performance is optimum for (small) uC architectures. For implementation details refer to
* the detailed documentation inside of the function body.
* \return None
*/
void  phCryptoSym_Sw_Des_PC1_Permutation(
    const uint8_t * pKeyIn,                 /**< [In] key pointer containing the original key. */
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pKeyOut  /**< [Out] key  pointer containing the permutated key. */
    );

/**
* \brief Compute a permutation as defined in table PC2 of Appendix 1 of FIPS46.3.
* The implementation is chosen such that the trade off between ROM usage and execution
* performance is optimum for (small) uC architectures. For implementation details refer to
* the detailed documentation inside of the function body.
* \return None
*/
void phCryptoSym_Sw_Des_PC2_Permutation(
                                        uint8_t PH_CRYTOSYM_SW_FAST_RAM * pKeyIn,  /**< [In] round key pointer containing the input. */
                                        uint8_t * pKeyOut                       /**< [Out] round key  pointer containing the output. */
                                        );

/**
* \brief Rotates (bitwise) a four byte array to the left by bNumPos positions.
* Note: Only the first 28 bits are rotated through, in other words, bits 4-7 of byte 3 are untouched
* \return None
*/
void  phCryptoSym_Sw_Des_RotateLeft28(
                                      uint8_t PH_CRYTOSYM_SW_FAST_RAM * pArray,  /**< [InOut] 4 Byte array to be rotated. */
                                      uint8_t bNumPos                         /**< [In] Amount of positions (bits) to rotate through. */
                                      );
#ifdef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
/**
* \brief Rotates (bitwise) a four byte array to the right by bNumPos positions.
* Note: Only the first 28 bits are rotated through, in other words, bits 4-7 of byte 3 are untouched
* \return None
*/
void  phCryptoSym_Sw_Des_RotateRight28(
                                       uint8_t PH_CRYTOSYM_SW_FAST_RAM * pArray,  /**< [InOut] 4 Byte array to be rotated. */
                                       uint8_t bNumPos                         /**< [In] Amount of positions (bits) to rotate through. */
                                       );
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */
/**
* \brief Performes the cipher function F as described in chapter "The Cipher Function f" of FIPS46.3.
* The 32 bit input pR is expanded, XORed with the key, looked up using the SBOX and finally permutated according to
* the standard of the DES algorithm
* \return None
*/
void phCryptoSym_Sw_Des_F(
                          uint8_t PH_CRYTOSYM_SW_FAST_RAM * pR,  /**< [InOut] 4 Byte array to be ciphered. */
                          uint8_t * pRoundKey                 /**< [In] 8 bytes (containing 48 bits) round key used for XORing. */
                          );

/**
* \brief Performes a round as described in Figure 1. Enciphering computation. of FIPS46.3.
* The 8 byte input is first split into two parts, namely L and R. On R, the function F is executed,
* and then finally the result is xored to L.
* \return None
*/
void phCryptoSym_Sw_Des_ComputeRound(
                                     uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState,  /**< [InOut] 8 bytes state to compute the round on. */
                                     uint8_t * pRoundKey                     /**< [In] 8 bytes (containing 48 bits) round key used for XORing. */
                                     );
/**
* \brief Expands the 32 bit input to generate a 48 bit output according to E-Selection Table. of FIPS46.3.
* The 4 byte input is expanded to create a 8 byte output containing 6 bits of expanded data in the individual bytes.
* \return None
*/
void phCryptoSym_Sw_Des_Expand(
                               uint8_t PH_CRYTOSYM_SW_FAST_RAM * pR,   /**< [In] 4 Bytes of input Data. */
                               uint8_t PH_CRYTOSYM_SW_FAST_RAM * pRexp /**< [Out] 8 bytes (containing 48 bits) of expanded data. */
                               );

/**
* \brief Take an array of length 8, put bState[0:3] to bState[4:7] and vice versa.
* \return None
*/
void phCryptoSym_Sw_Des_Swap(
                             uint8_t PH_CRYTOSYM_SW_FAST_RAM *bState /**< [InOut] State to be swapped. */
                             );

/** @}
* end of phCryptoSym_Sw_DES_Internals group
*/

#endif /* PHCRYPTOSYM_SW_DES_INT_H */
