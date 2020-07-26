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

#ifndef PHALI18000P3M3_SW_H
#define PHALI18000P3M3_SW_H

#include <ph_Status.h>

phStatus_t phalI18000p3m3_Sw_Ack(
                                 phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                 uint8_t ** ppRxBuffer,
                                 uint16_t * pRxBitLength
                                 );

phStatus_t phalI18000p3m3_Sw_ReqRn(
                                   phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                   uint8_t bOption,
                                   uint8_t ** ppRxBuffer
                                   );

phStatus_t phalI18000p3m3_Sw_Read(
                                  phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint8_t bMemBank,
                                  uint8_t * pWordPtr,
                                  uint8_t bWordPtrLength,
                                  uint8_t bWordCount,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pRxBitLength
                                  );

phStatus_t phalI18000p3m3_Sw_Write(
                                   phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                   uint8_t bOption,
                                   uint8_t bMemBank,
                                   uint8_t * pWordPtr,
                                   uint8_t bWordPtrLength,
                                   uint8_t * pData
                                   );

phStatus_t phalI18000p3m3_Sw_Kill(
                                  phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint8_t bOption,
                                  uint8_t * pPassword,
                                  uint8_t bRecom
                                  );

phStatus_t phalI18000p3m3_Sw_Lock(
                                  phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                  uint8_t * pMask,
                                  uint8_t * pAction
                                  );

phStatus_t phalI18000p3m3_Sw_Access(
                                    phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                    uint8_t bOption,
                                    uint8_t * pPassword
                                    );

phStatus_t phalI18000p3m3_Sw_BlockWrite(
                                        phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                        uint8_t bMemBank,
                                        uint8_t * pWordPtr,
                                        uint8_t bWordPtrLength,
                                        uint8_t bWordCount,
                                        uint8_t * pData
                                        );

phStatus_t phalI18000p3m3_Sw_BlockErase(
                                        phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                        uint8_t bMemBank,
                                        uint8_t * pWordPtr,
                                        uint8_t bWordPtrLength,
                                        uint8_t bWordCount
                                        );

phStatus_t phalI18000p3m3_Sw_BlockPermaLock(
    phalI18000p3m3_Sw_DataParams_t * pDataParams,
    uint8_t bRFU,
    uint8_t bReadLock,
    uint8_t bMemBank,
    uint8_t * pBlockPtr,
    uint8_t bBlockPtrLength,
    uint8_t bBlockRange,
    uint8_t * pMask,
    uint8_t ** ppRxBuffer,
    uint16_t * pRxBitLength
    );

phStatus_t phalI18000p3m3_Sw_SetHandle(
                                       phalI18000p3m3_Sw_DataParams_t * pDataParams,
                                       uint8_t* pHandle
                                       );

#endif /* PHALI18000P3M3_SW_H */
