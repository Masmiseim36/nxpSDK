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
* Internal functions for Tag Operation Application Layer Component of
* Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHALTOP_SW_INT_T3T_H
#define PHALTOP_SW_INT_T3T_H

#ifdef NXPBUILD__PHAL_TOP_SW
#ifdef NXPBUILD__PHAL_FELICA_SW

#define PHAL_TOP_T3T_BLOCK_SIZE                  0x10U        /**< Number of blocks per segment for T3T */
#define PHAL_TOP_T3T_WRITE_FLAG_ADDRESS          0x09U        /**< Write Flag Byte Location  in Attribute Information Block */

#define PHAL_TOP_T3T_AIB_RWA_RW                  0x01U        /**< Read/Write access */
#define PHAL_TOP_T3T_AIB_RWA_RO                  0x00U        /**< Read Only access */

#define PHAL_TOP_T3T_WRITE_FLAG_SET              0x0FU        /**< Write Flag to SET during Write in Progress */
#define PHAL_TOP_T3T_WRITE_FLAG_RESET            0x00U        /**< Write Flag to RESET after Write is Done */

void phalTop_Sw_Int_T3T_CreateBlockList(
                                        uint8_t * pBlockList,
                                        uint16_t wStartIndex,
                                        uint8_t bCount,
                                        uint8_t * pBlockListLength
                                        );

phStatus_t phalTop_Sw_Int_T3T_ClearState(
                                         phalTop_Sw_DataParams_t * pDataParams,
                                         phalTop_T3T_t * pT3T
                                         );

phStatus_t phalTop_Sw_Int_T3T_CheckNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pTagState
                                        );

phStatus_t phalTop_Sw_Int_T3T_FormatNdef(
                                         phalTop_Sw_DataParams_t * pDataParams
                                         );

phStatus_t phalTop_Sw_Int_T3T_EraseNdef(
                                        phalTop_Sw_DataParams_t * pDataParams
                                        );

phStatus_t phalTop_Sw_Int_T3T_ReadNdef(
                                       phalTop_Sw_DataParams_t * pDataParams,
                                       uint8_t * pData,
                                       uint16_t * pLength
                                       );

phStatus_t phalTop_Sw_Int_T3T_WriteNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pData,
                                        uint16_t wLength
                                        );

#endif /* NXPBUILD__PHAL_FELICA_SW */
#endif /* NXPBUILD__PHAL_TOP_SW */

#endif /* PHALTOP_SW_INT_T3T_H */
