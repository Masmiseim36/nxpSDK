/*
*         Copyright (c), NXP Semiconductors Bangalore / India
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
* Software Tag Operation Application Layer Component of
* Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHALTOP_SW_H
#define PHALTOP_SW_H

#ifdef NXPBUILD__PHAL_TOP_SW

typedef phStatus_t (*pphalTop_Sw_CheckNdef) (phalTop_Sw_DataParams_t * pDataParams, uint8_t * pTagState );
typedef phStatus_t (*pphalTop_Sw_FormatNdef) (phalTop_Sw_DataParams_t * pDataParams);
typedef phStatus_t (*pphalTop_Sw_EraseNdef) (phalTop_Sw_DataParams_t * pDataParams);
typedef phStatus_t (*pphalTop_Sw_ReadNdef) (phalTop_Sw_DataParams_t * pDataParams,  uint8_t * pData,  uint16_t * pLength);
typedef phStatus_t (*pphalTop_Sw_WriteNdef) (phalTop_Sw_DataParams_t * pDataParams,  uint8_t * pData,  uint16_t  wLength);
typedef phStatus_t (*pphalTop_Sw_Int_SetReadOnly) (phalTop_Sw_DataParams_t * pDataParams);

phStatus_t phalTop_Sw_CheckNdef(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint8_t * pTagState
                                );

phStatus_t phalTop_Sw_FormatNdef(
                                 phalTop_Sw_DataParams_t * pDataParams
                                 );

phStatus_t phalTop_Sw_EraseNdef(
                                phalTop_Sw_DataParams_t * pDataParams
                                );

phStatus_t phalTop_Sw_Reset(
                            phalTop_Sw_DataParams_t * pDataParams
                            );

phStatus_t phalTop_Sw_SetConfig(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint16_t wConfig,
                                uint16_t wValue
                                );

phStatus_t phalTop_Sw_GetConfig(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint16_t wConfig,
                                uint16_t  * wValue
                                );

phStatus_t phalTop_Sw_ReadNdef(
                               phalTop_Sw_DataParams_t * pDataParams,
                               uint8_t * pData,
                               uint16_t * pLength
                               );

phStatus_t phalTop_Sw_WriteNdef(
                                phalTop_Sw_DataParams_t * pDataParams,
                                uint8_t * pData,
                                uint16_t wLength
                                );

#ifdef __DEBUG
phStatus_t phalTop_Sw_SetPtr(
                             phalTop_Sw_DataParams_t * pDataParams,
                             void * pT1T,
                             void * pT2T,
                             void * pT3T,
                             void * pT4T,
                             void * pT5T,
							 void * pMfcTop
                             );
#endif /* __DEBUG */

#endif /* NXPBUILD__PHAL_TOP_SW */

#endif /* PHALTOP_SW_H */
