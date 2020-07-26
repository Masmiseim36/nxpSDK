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
* Software ISO18000-3 Mode3 Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  MWt: Generated 1. February 2010
*
*/

#ifndef PHPALI18000P3M3_SW_H
#define PHPALI18000P3M3_SW_H

#include <ph_Status.h>

#ifdef NXPBUILD__PHPAL_I18000P3M3_SW

phStatus_t phpalI18000p3m3_Sw_Exchange(
                                       phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                       uint16_t wOption,
                                       uint8_t * pTxBuffer,
                                       uint16_t wTxLength,
                                       uint8_t bTxLastBits,
                                       uint8_t ** ppRxBuffer,
                                       uint16_t * pRxLength,
                                       uint8_t * pRxLastBits
                                       );

phStatus_t phpalI18000p3m3_Sw_Select(
                                     phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                     uint8_t bTarget,
                                     uint8_t bAction,
                                     uint8_t bMemBank,
                                     uint8_t * pPointer,
                                     uint8_t bPointerLength,
                                     uint8_t * pMask,
                                     uint8_t bMaskBitLength,
                                     uint8_t bTruncate
                                     );

#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
phStatus_t phpalI18000p3m3_Sw_BeginRound(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t bDr,
    uint8_t bM,
    uint8_t bTRext,
    uint8_t bSel,
    uint8_t bSession,
    uint8_t bRfu,
    uint8_t bQ,
    uint8_t * pStoredCRC
    );
#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */

phStatus_t phpalI18000p3m3_Sw_CreateBeginRoundCmd(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t bDr,
    uint8_t bM,
    uint8_t bTRext,
    uint8_t bSel,
    uint8_t bSession,
    uint8_t bRfu,
    uint8_t bQ,
    uint8_t * pBeginRnd
    );

phStatus_t phpalI18000p3m3_Sw_CreateSelectCmd(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
     uint8_t bTarget,
     uint8_t bAction,
     uint8_t bMemBank,
     uint8_t * pPointer,
     uint8_t bPointerLength,
     uint8_t * pMask,
     uint8_t bMaskBitLength,
     uint8_t bTruncate,
     uint8_t * pSelectCmd,
     uint8_t * bLen,
     uint8_t * bValidBits
    );

#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
phStatus_t phpalI18000p3m3_Sw_ResizeRound(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t bUpDn,
    uint8_t * pStoredCRC
    );

phStatus_t phpalI18000p3m3_Sw_NextSlot(
                                       phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                       uint8_t * pStoredCRC
                                       );
#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED*/

phStatus_t phpalI18000p3m3_Sw_Ack(
                                  phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint8_t bOption,
                                  uint8_t * pHandle,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pRxBitLength
                                  );

phStatus_t phpalI18000p3m3_Sw_Nak(
                                  phpalI18000p3m3_Sw_DataParams_t * pDataParams
                                  );

phStatus_t phpalI18000p3m3_Sw_ActivateCard(
        phpalI18000p3m3_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
        uint8_t * pSelCmd,                  /**< [In] ISO 18000p3m3 Select command frame. */
        uint8_t bSelCmdLen,                 /**< [In] Select command length in bytes- 1 to 39 bytes. */
        uint8_t bNumValidBitsinLastByte,    /**< [In] Number of valid bits in last byte of pSelCmd. */
        uint8_t * pBeginRndCmd,             /**< [In] ISO 18000p3m3 BeginRound command frame. This is 17bits i.e., 3 bytes are expected. CRC5 should not be provided. */
        uint8_t bTSprocessing,              /**< [In] TimeSlot processing behavior. */
        uint8_t ** ppRxBuffer,              /**< [In] Pointer to HAL Rx Buffer containing handle of the activated tag. */
        uint16_t * pRxBufferLen,            /**< [In] Length of response in hal Rx Buffer. */
        uint8_t * pMoreCardsAvailable       /**< [Out] Indicates if more tags are present in the field. */
        );

#ifdef NXPBUILD__PHPAL_I18000P3M3_DEPRECATED
phStatus_t phpalI18000p3m3_Sw_GetSerialNo(
    phpalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t * pRxBuffer,
    uint8_t * pRxLength
    );
#endif /* NXPBUILD__PHPAL_I18000P3M3_DEPRECATED */

phStatus_t phpalI18000p3m3_Sw_SetConfig(
                                        phpalI18000p3m3_Sw_DataParams_t * pDataParams,
                                        uint16_t wConfig,
                                        uint16_t wValue
                                        );

#endif /* NXPBUILD__PHPAL_I18000P3M3_SW */

#endif /* PHPALI18000P3M3_SW_H */
