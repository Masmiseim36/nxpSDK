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
* Software MIFARE(R) Ultralight contactless IC Application Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 05. October 2009
*
*/

#ifndef PHALMFUL_SW_H
#define PHALMFUL_SW_H

#include <ph_Status.h>

phStatus_t phalMful_Sw_UlcAuthenticate(
                                       phalMful_Sw_DataParams_t * pDataParams,
                                       uint16_t wKeyNumber,
                                       uint16_t wKeyVersion
                                       );

phStatus_t phalMful_Sw_Read(
                            phalMful_Sw_DataParams_t * pDataParams,
                            uint8_t bAddress,
                            uint8_t * pData
                            );

phStatus_t phalMful_Sw_Write(
                             phalMful_Sw_DataParams_t * pDataParams,
                             uint8_t bAddress,
                             uint8_t * pData
                             );

phStatus_t phalMful_Sw_FastWrite(
                                 phalMful_Sw_DataParams_t * pDataParams,
                                 uint8_t * pData
                                 );

phStatus_t phalMful_Sw_CompatibilityWrite(
    phalMful_Sw_DataParams_t * pDataParams,
    uint8_t bAddress,
    uint8_t * pData
    );

phStatus_t phalMful_Sw_IncrCnt(
                               phalMful_Sw_DataParams_t * pDataParams,
                               uint8_t bCntNum,
                               uint8_t * pCnt
                               );

phStatus_t phalMful_Sw_ReadCnt(
                               phalMful_Sw_DataParams_t * pDataParams,
                               uint8_t bCntNum,
                               uint8_t * pCntValue
                               );

phStatus_t phalMful_Sw_PwdAuth(
                               phalMful_Sw_DataParams_t * pDataParams,
                               uint8_t * pPwd,
                               uint8_t * pPack
                               );

phStatus_t phalMful_Sw_GetVersion(
                                  phalMful_Sw_DataParams_t * pDataParams,
                                  uint8_t * pPwd
                                  );

phStatus_t phalMful_Sw_FastRead(
                                phalMful_Sw_DataParams_t * pDataParams,
                                uint8_t  bStartAddr,
                                uint8_t bEndAddr,
                                uint8_t ** pData,
                                uint16_t * pNumBytes
                                );

phStatus_t phalMful_Sw_SectorSelect(
                                    phalMful_Sw_DataParams_t * pDataParams,
                                    uint8_t bSecNo
                                    );

phStatus_t phalMful_Sw_ReadSign(
                                phalMful_Sw_DataParams_t * pDataParams,
                                uint8_t bAddr,
                                uint8_t ** pSignature
                                );

phStatus_t phalMful_Sw_ChkTearingEvent(
                                       phalMful_Sw_DataParams_t * pDataParams,
                                       uint8_t bCntNum,
                                       uint8_t * pValidFlag
                                       );

phStatus_t phalMful_Sw_WriteSign(
                                 phalMful_Sw_DataParams_t * pDataParams,
                                 uint8_t bAddress,
                                 uint8_t * pSignature
                                 );

phStatus_t phalMful_Sw_LockSign(
                                phalMful_Sw_DataParams_t * pDataParams,
                                uint8_t bLockMode
                                );

phStatus_t phalMful_Sw_VirtualCardSelect(
                                         phalMful_Sw_DataParams_t * pDataParams,
                                         uint8_t * pVCIID,
                                         uint8_t bVCIIDLen,
                                         uint8_t * pVCTID
                                         );

#endif /* PHALMFUL_SW_H */
