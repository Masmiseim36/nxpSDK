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

#ifndef PHALTOP_SW_INT_T4T_H
#define PHALTOP_SW_INT_T4T_H

#ifdef NXPBUILD__PHAL_TOP_SW
#ifdef NXPBUILD__PHAL_MFDF_SW

#define PHAL_TOP_T4T_NDEF_APP_ID                    0x000001U     /* Application id of the NDEF Application */
#define PHAL_TOP_T4T_ISOFILEID_APP                  0x05E1U       /* ISO file id of the NDEF application */
#define PHAL_TOP_T4T_SELECT_EF_ID                   0x02U        /* File Selection by EF Id */
#define PHAL_TOP_T4T_SELECT_DF_NAME                 0x04U        /* File Selection by DF Name */
#define PHAL_TOP_T4T_CCLEN_OFFSET                   0x00U        /* Offset value of CCLEN */
#define PHAL_TOP_T4T_VERSION_OFFSET                 0x02U        /* Offset value of Mapping Version */
#define PHAL_TOP_T4T_MAX_LENGTH_EXPECTED_OFFSET     0x03U        /* Offset value of MLe (bytes) i.e Maximum R-APDU data size */
#define PHAL_TOP_T4T_MAX_CMD_LENGTH_OFFSET          0x05U        /* Offset value of MLc (bytes) i.e Maximum C-APDU data size */
#define PHAL_TOP_T4T_NDEFTLV_OFFSET                 0x07U        /* Offset value of NDEF File Control TLV */

#define PHAL_TOP_T4T_SELECTED_NONE                  0x00U
#define PHAL_TOP_T4T_SELECTED_NDEF_APP              0x01U
#define PHAL_TOP_T4T_SELECTED_CC_FILE               0x02U
#define PHAL_TOP_T4T_SELECTED_NDEF_FILE             0x03U

#define PHAL_TOP_T4T_NDEF_TLV                       0x04U        /**< NDEF Message TLV. */
#define PHAL_TOP_T4T_PROPRIETARY_TLV                0x05U        /**< Proprietary TLV. */

#define PHAL_TOP_T4T_NDEF_FILE_READ_ACCESS          0x00U        /**< Read/Write access */
#define PHAL_TOP_T4T_NDEF_FILE_WRITE_ACCESS         0x00U        /**< Read/Write access */
#define PHAL_TOP_T4T_NDEF_FILE_NO_WRITE_ACCESS      0xFFU        /**< Read Only access */

phStatus_t phalTop_Sw_Int_T4T_ClearState(
                                         phalTop_Sw_DataParams_t * pDataParams,
                                         phalTop_T4T_t * pT4T
                                         );

phStatus_t phalTop_Sw_Int_T4T_CheckNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pTagState
                                        );

phStatus_t phalTop_Sw_Int_T4T_ReadNdef(
                                       phalTop_Sw_DataParams_t * pDataParams,
                                       uint8_t * pData,
                                       uint16_t * pLength
                                       );

phStatus_t phalTop_Sw_Int_T4T_WriteNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pData,
                                        uint16_t wLength
                                        );

phStatus_t phalTop_Sw_Int_T4T_FormatNdef(
                                         phalTop_Sw_DataParams_t * pDataParams
                                         );

phStatus_t phalTop_Sw_Int_T4T_EraseNdef(
                                        phalTop_Sw_DataParams_t * pDataParams
                                        );

#endif /* NXPBUILD__PHAL_MFDF_SW */
#endif /* NXPBUILD__PHAL_TOP_SW */

#endif /* PHALTOP_SW_INT_T4T_H */
