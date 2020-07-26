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
* Software MIFARE product Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 31. July 2009
*
*/

#ifndef PHPALMIFARE_SW_H
#define PHPALMIFARE_SW_H

#include <ph_Status.h>

#ifndef NXPRDLIB_REM_GEN_INTFS
#   include <phpalMifare.h>
#endif

phStatus_t phpalMifare_Sw_ExchangeL3(
                                     phpalMifare_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t * pTxBuffer,
                                     uint16_t wTxLength,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pRxLength
                                     );

phStatus_t phpalMifare_Sw_ExchangeL4(
                                     phpalMifare_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t * pTxBuffer,
                                     uint16_t wTxLength,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pRxLength
                                     );

phStatus_t phpalMifare_Sw_ExchangePc(
                                     phpalMifare_Sw_DataParams_t * pDataParams,
                                     uint16_t wOption,
                                     uint8_t * pTxBuffer,
                                     uint16_t wTxLength,
                                     uint8_t ** ppRxBuffer,
                                     uint16_t * pRxLength
                                     );

phStatus_t phpalMifare_Sw_ExchangeRaw(
                                      phpalMifare_Sw_DataParams_t * pDataParams,
                                      uint16_t wOption,
                                      uint8_t * pTxBuffer,
                                      uint16_t wTxLength,
                                      uint8_t bTxLastBits,
                                      uint8_t ** ppRxBuffer,
                                      uint16_t * pRxLength,
                                      uint8_t * pRxLastBits
                                      );

phStatus_t phpalMifare_Sw_MfcAuthenticateKeyNo(
    phpalMifare_Sw_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid
    );

phStatus_t  phpalMifare_Sw_MfcAuthenticate(
    phpalMifare_Sw_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint8_t * pKey,
    uint8_t * pUid
    );

phStatus_t phpalMifare_Sw_SetMinFdtPc(
    phpalMifare_Sw_DataParams_t * pDataParams,
    uint16_t wValue
    );

phStatus_t phpalMifare_Sw_GetConfig(
                                    phpalMifare_Sw_DataParams_t * pDataParams,
                                    uint16_t wConfig,
                                    uint16_t * pValue
                                    );

#endif /* PHPALMIFARE_SW_H */
