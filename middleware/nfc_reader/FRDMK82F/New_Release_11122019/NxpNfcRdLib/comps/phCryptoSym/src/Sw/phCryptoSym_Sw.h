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
* Software specific Symmetric Cryptography Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SLe: Generated 01.12.2009
*  SLe: Alpha Release 10.01.2010
*
*/

#ifndef PHCRYPTOSYM_SW_H
#define PHCRYPTOSYM_SW_H

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phCryptoSym.h>

#define PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_128 10U  /**< AES rounds for 128 bit key. */
#define PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_192 12U  /**< AES rounds for 192 bit key. */
#define PH_CRYPTOSYM_SW_NUM_AES_ROUNDS_256 14U  /**< AES rounds for 256 bit key. */

phStatus_t phCryptoSym_Sw_InvalidateKey(
                                        phCryptoSym_Sw_DataParams_t * pDataParams
                                        );

phStatus_t phCryptoSym_Sw_Encrypt(
                                  phCryptoSym_Sw_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  const uint8_t * pPlainBuffer,
                                  uint16_t  wBufferLength,
                                  uint8_t * pEncryptedBuffer
                                  );

phStatus_t phCryptoSym_Sw_Decrypt(
                                  phCryptoSym_Sw_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  const uint8_t * pEncryptedBuffer,
                                  uint16_t  wBufferLength,
                                  uint8_t * pPlainBuffer
                                  );


phStatus_t phCryptoSym_Sw_CalculateMac(
                                       phCryptoSym_Sw_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       const uint8_t * pData,
                                       uint16_t  wDataLength,
                                       uint8_t * pMac,
                                       uint8_t * pMacLength
                                       );

phStatus_t phCryptoSym_Sw_LoadIv(
                                 phCryptoSym_Sw_DataParams_t * pDataParams,
                                 const uint8_t * pIV,
                                 uint8_t bIVLength
                                 );

phStatus_t phCryptoSym_Sw_LoadKey(
                                  phCryptoSym_Sw_DataParams_t * pDataParams,
                                  uint16_t wKeyNo,
                                  uint16_t wKeyVersion,
                                  uint16_t wKeyType
                                  );

phStatus_t phCryptoSym_Sw_LoadKeyDirect(
                                        phCryptoSym_Sw_DataParams_t * pDataParams,
                                        const uint8_t * pKey,
                                        uint16_t wKeyType
                                        );

phStatus_t phCryptoSym_Sw_DiversifyKey(
                                       phCryptoSym_Sw_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       uint16_t wKeyNo,
                                       uint16_t wKeyVersion,
                                       uint8_t * pDivInput,
                                       uint8_t  bLenDivInput,
                                       uint8_t * pDiversifiedKey
                                       );

phStatus_t phCryptoSym_Sw_DiversifyDirectKey(
    phCryptoSym_Sw_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t * pKey,
    uint16_t wKeyType,
    uint8_t * pDivInput,
    uint8_t bLenDivInput,
    uint8_t * pDiversifiedKey
    );

phStatus_t phCryptoSym_Sw_SetConfig(
                                    phCryptoSym_Sw_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t wValue
                                    );

phStatus_t phCryptoSym_Sw_GetConfig(
                                    phCryptoSym_Sw_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    );

#endif /* PHCRYPTOSYM_SW_H */
