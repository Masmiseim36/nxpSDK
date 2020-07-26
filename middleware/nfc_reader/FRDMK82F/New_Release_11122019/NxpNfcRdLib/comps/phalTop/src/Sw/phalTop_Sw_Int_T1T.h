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

#ifndef PHALTOP_SW_INT_T1T_H
#define PHALTOP_SW_INT_T1T_H

#ifdef NXPBUILD__PHAL_TOP_SW
#ifdef NXPBUILD__PHAL_T1T_SW

#define PHAL_TOP_T1T_INVALID_SEGMENT_ADDR        0xEFU        /**< Invalid segment address */
#define PHAL_TOP_T1T_CC_LENGTH                   0x04U        /**< Length of T1T CC bytes */
#define PHAL_TOP_T1T_MAX_TMS                     0x0800U      /**< Max. memory size for T1T */
#define PHAL_TOP_T1T_MAX_NDEF_TLV_LEN_FIELD      0x03U        /**< Max. Length of NDEF TLV Length field */
#define PHAL_TOP_T1T_NDEF_TLV_HEADER_LEN         0x01U        /**< NDEF TLV header(T field) length */

#define PHAL_TOP_T1T_SEGMENT_SIZE                0x80U        /**< T1T Segment size */
#define PHAL_TOP_T1T_NDEF_NMN                    0xE1U        /**< NDEF Magical Number for T1T */
#define PHAL_TOP_T1T_BLOCKS_PER_SEGMENT          0x10U        /**< Number of blocks per segment for T1T */
#define PHAL_TOP_T1T_SEGMENT_END_INDEX           0x64U        /**< T1T segment end for TLV check */
#define PHAL_TOP_T1T_BLOCK_LOCK0                 0x70U        /**< T1T block for Lock 0 */
#define PHAL_TOP_T1T_BLOCK_LOCK1                 0x71U        /**< T1T block for Lock 1 */
#define PHAL_TOP_T1T_BLOCK_NMN                   0x08U        /**< T1T block for NDEF Magic Number */
#define PHAL_TOP_T1T_BLOCK_VNO                   0x09U        /**< T1T block for version no */
#define PHAL_TOP_T1T_BLOCK_TMS                   0x0AU        /**< T1T block for Tag Memory Size */
#define PHAL_TOP_T1T_BLOCK_RWA                   0x0BU        /**< T1T block for Read write Access  */
#define PHAL_TOP_T1T_SEGMENT_START_INDEX         0x0CU        /**< T1T Segment start for Tlv */

#define PHAL_TOP_T1T_CC_NMN_ADDR                 0x08U        /**< CC NMN byte address */
#define PHAL_TOP_T1T_CC_VNO_ADDR                 0x09U        /**< CC NDEF version number byte address */
#define PHAL_TOP_T1T_CC_TMS_ADDR                 0x0AU        /**< CC Tag Memory Size byte address */
#define PHAL_TOP_T1T_CC_RWA_ADDR                 0x0BU        /**< CC Read-write access byte address */

#define PHAL_TOP_T1T_NULL_TLV                    0x00U        /**< NULL TLV. */
#define PHAL_TOP_T1T_LOCK_CTRL_TLV               0x01U        /**< Lock Control TLV. */
#define PHAL_TOP_T1T_MEMORY_CTRL_TLV             0x02U        /**< Memory Control TLV. */
#define PHAL_TOP_T1T_NDEF_TLV                    0x03U        /**< NDEF Message TLV. */
#define PHAL_TOP_T1T_PROPRIETARY_TLV             0xFDU        /**< Proprietary TLV. */
#define PHAL_TOP_T1T_TERMINATOR_TLV              0xFEU        /**< Terminator TLV. */

#define PHAL_TOP_T1T_CC_RWA_RW                   0x00U        /**< Read/Write access */
#define PHAL_TOP_T1T_CC_RWA_RO                   0x0FU        /**< Read Only access */

/**< Tag operations error - Reserved/lock/OTP byte */
#define PHAL_TOP_T1T_ERR_RESERVED_BYTE   ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0U)

/**< Get the actual byte offset from read segment data */
#define PHAL_TOP_T1T_GET_SEG_DATA_OFFSET(seg, index)   ((seg) != 0 ? ((index) + 1U): ((index) + 2U))

/**< Get length of TLV length field */
#define PHAL_TOP_T1T_GET_TLV_LEN_BYTES(len)   ((len) > 0xFE? 3: 1)

void phalTop_Sw_Int_T1T_CalculateMaxNdefSize(
                                             phalTop_Sw_DataParams_t * pDataParams,
                                             phalTop_T1T_t * pT1T
                                             );

phStatus_t phalTop_Sw_Int_T1T_UpdateLockReservedOtp(
                                                    phalTop_T1T_t * pT1T
                                                    );

uint8_t phalTop_Sw_Int_T1T_CheckLockReservedOtp(
                                                phalTop_T1T_t * pT1T,
                                                uint16_t wIndex
                                                );

phStatus_t phalTop_Sw_Int_T1T_DetectTlvBlocks(
                                              phalTop_Sw_DataParams_t * pDataParams,
                                              phalTop_T1T_t * pT1T
                                              );

phStatus_t phalTop_Sw_Int_T1T_CheckNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pTagState
                                        );

phStatus_t phalTop_Sw_Int_T1T_FormatNdef(
                                         phalTop_Sw_DataParams_t * pDataParams
                                         );

phStatus_t phalTop_Sw_Int_T1T_EraseNdef(
                                        phalTop_Sw_DataParams_t * pDataParams
                                        );

phStatus_t phalTop_Sw_Int_T1T_ReadNdef(
                                       phalTop_Sw_DataParams_t * pDataParams,
                                       uint8_t * pData,
                                       uint16_t * pLength
                                       );

phStatus_t phalTop_Sw_Int_T1T_WriteNdef(
                                        phalTop_Sw_DataParams_t * pDataParams,
                                        uint8_t * pData,
                                        uint16_t wLength
                                        );

phStatus_t phalTop_Sw_Int_T1T_SetReadOnly(
                                          phalTop_Sw_DataParams_t * pDataParams
                                          );

phStatus_t phalTop_Sw_Int_T1T_Read(
                                   phalTop_T1T_t * pT1T,
                                   uint16_t wAddress,
                                   uint8_t * pData,
                                   uint8_t bFreshRead
                                   );

phStatus_t phalTop_Sw_Int_T1T_Write(
                                    phalTop_Sw_DataParams_t * pDataParams,
                                    phalTop_T1T_t * pT1T,
                                    uint16_t wAddress,
                                    uint8_t * pData,
                                    uint16_t pDataIndex
                                    );

phStatus_t phalTop_Sw_Int_T1T_ClearState(
                                         phalTop_Sw_DataParams_t * pDataParams,
                                         phalTop_T1T_t * pT1T
                                         );

#endif /* NXPBUILD__PHAL_T1T_SW */
#endif /* NXPBUILD__PHAL_TOP_SW */

#endif /* PHALTOP_SW_INT_T1T_H */
