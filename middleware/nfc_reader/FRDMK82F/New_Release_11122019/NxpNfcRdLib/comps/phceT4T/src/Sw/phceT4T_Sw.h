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
* Card Emulation component for Type 4A Tag.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHCET4T_SW_H
#define PHCET4T_SW_H

#include <phceT4T.h>

phStatus_t phceT4T_Sw_Reset(
                            phceT4T_Sw_DataParams_t * pDataParams
                            );

phStatus_t phceT4T_Sw_SetElementaryFile(
                                        phceT4T_Sw_DataParams_t * pDataParams,
                                        uint8_t bFileType,
                                        uint8_t *pFile,
                                        uint16_t wFileId,
                                        uint32_t dwFileSize,
                                        uint32_t dwContentLen
                                        );

phStatus_t phceT4T_Sw_ProcessCmd(
                                 phceT4T_Sw_DataParams_t *pDataParams,
                                 uint16_t wOption,
                                 uint8_t *pRxData,
                                 uint16_t wRxDataLen,
                                 uint16_t *pStatusWord,
                                 uint8_t **pTxData,
                                 uint16_t *pTxDataLen
                                 );

phStatus_t phceT4T_Sw_Activate(
                               phceT4T_Sw_DataParams_t *pDataParams
                               );

phStatus_t phceT4T_Sw_AppProcessCmd(
                                    phceT4T_Sw_DataParams_t *pDataParams,
                                    phceT4T_AppCallback_t pAppCallback
                                    );

phStatus_t phceT4T_Sw_GetSelectedFileInfo(
                                          phceT4T_Sw_DataParams_t *pDataParams,
                                          uint16_t *pFileId,
                                          uint8_t **ppFile,
                                          uint32_t *pFileSize,
                                          uint8_t *pWriteAccess,
                                          uint32_t *pFileOffset
                                          );

phStatus_t phceT4T_Sw_SetConfig(
                                phceT4T_Sw_DataParams_t   *pDataParams,
                                uint16_t                   wConfig,
                                uint16_t                   wValue
                                );

phStatus_t phceT4T_Sw_GetConfig(
                                phceT4T_Sw_DataParams_t   *pDataParams,
                                uint16_t                   wConfig,
                                uint16_t                     *pValue
                                );
#endif /* PHCET4T_SW_H */
