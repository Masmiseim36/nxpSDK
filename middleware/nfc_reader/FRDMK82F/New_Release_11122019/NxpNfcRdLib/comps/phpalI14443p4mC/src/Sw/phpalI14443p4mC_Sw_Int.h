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
* Software ISO 14443p4mC mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 25. March 2014
*/

#ifndef PHPALI14443P4MC_SW_INT_H
#define PHPALI14443P4MC_SW_INT_H

/** Macro used to Calculate FWT Value based on FWI. */
#define PHPAL_I14443P4MC_SW_CALCULATE_FWT_US(bFWI)                    \
    (PHPAL_I14443P4MC_SW_FWT_MIN_US*((uint32_t)(1U << (bFWI))))

/** Check if PCB indicates WTX Request/Response */
#define PHPAL_I14443P4MC_SW_IS_WTX(bPcb)                              \
    ((((bPcb) & PHPAL_I14443P4MC_SW_PCB_WTX) == PHPAL_I14443P4MC_SW_PCB_WTX) ? 1 : 0)

/* Default values for parameters */
#define PHPALI14443P4MC_FSCI_DEFAULT        0x02U
#define PHPALI14443P4MC_FWI_DEFAULT         0x04U
#define PHPALI14443P4MC_SFGI_DEFAULT        0x00U
#define PHPALI14443P4MC_FSC_MAX_NFC_FORUM   0x08U
#define PHPALI14443P4MC_FSC_MIN_NFC_FORUM   0x02U

/* Command codes */
#define PHPALI14443P4MC_RATS_CMD            0xE0U
#define PHPALI14443P4MC_PPS_CMD             0xD0U
#define PHPALI14443P4MC_HALT_CMD            0x50U

/* Command lengths */
#define PHPALI14443P4MC_RATS_LEN            0x02U
#define PHPALI14443P4MC_PPS_LEN             0x01U
#define PHPALI14443P4MC_NFC_MAX_ATS_LEN     0x14U
#define PHPALI14443P4MC_NFC_MIN_ATS_LEN     0x05U

/* ATS command parameters index */
#define PHPALI14443P4MC_TL_IDX              0x00U
#define PHPALI14443P4MC_T0_IDX              0x01U
#define PHPALI14443P4MC_TA1_IDX             0x02U
#define PHPALI14443P4MC_TB1_IDX             0x03U
#define PHPALI14443P4MC_TC1_IDX             0x04U

/* PPS mask values */
#define PHPALI14443P4MC_PPS_PPS1_MASK       0x10U
#define PHPALI14443P4MC_VALID_PPS0_MASK     0xEFU
#define PHPALI14443P4MC_PPS_PPS2_MASK       0xF0U

/** Valid CID bits within CID field */
#define PHPALI14443P4MC_CID_MASK            0x0FU

/** Valid NAD bits within NAD field */
#define PHPALI14443P4MC_NAD_MASK            0x77U

/* Block mask values */
#define PHPALI14443P4MC_BLOCK_MASK          0xC0U
#define PHPALI14443P4MC_R_NAK_MASK          0x10U
#define PHPALI14443P4MC_BLOCKNUMBER_MASK    0x01U

/* Supported Block types */
#define PHPALI14443P4MC_BLOCK_I             0x02U
#define PHPALI14443P4MC_BLOCK_R             0x82U
#define PHPALI14443P4MC_BLOCK_S             0xC2U

/* Check CID presence in block */
#define PHPALI14443P4MC_BLOCK_HAS_CID       0x08U

/* Check chaining I-Block */
#define PHPALI14443P4MC_BLOCK_IS_CHAIN      0x10U

/* S-Block mask */
#define PHPALI14443P4MC_BLOCK_S_MASK        0x30U

/* S-Block commands */
#define PHPALI14443P4MC_BLOCK_S_DESELECT    0x00U
#define PHPALI14443P4MC_BLOCK_S_WTX         0x30U

/* R-Block ACK command */
#define PHPALI14443P4MC_BLOCK_R_PCB_ACK     0xA2U

/* Number of frame size table entries */
#define PHPALI14443P4MC_FS_INDEX            0x0DU

/* Number of data rate table entries */
#define PHPALI14443P4MC_DR_INDEX            0x04U

/** Byte position of PCB byte */
#define PHPAL_I14443P4MC_SW_PCB_POS               0x00U

/** ISO 14443-4 I-Block */
#define PHPAL_I14443P4MC_SW_I_BLOCK               0x00U
/** ISO 14443-4 I-Block constant bits mask */
#define PHPAL_I14443P4MC_SW_I_BLOCK_CONST_MASK    0x02U
/** ISO 14443-4 I-Block constant bits */
#define PHPAL_I14443P4MC_SW_I_BLOCK_CONST_BITS    0x02U

/** ISO 14443-4 R-Block */
#define PHPAL_I14443P4MC_SW_R_BLOCK               0x80U
/** ISO 14443-4 R-Block constant bits mask */
#define PHPAL_I14443P4MC_SW_R_BLOCK_CONST_MASK    0x24U
/** ISO 14443-4 R-Block constant bits(b6(1) and b3(0)) */
#define PHPAL_I14443P4MC_SW_R_BLOCK_CONST_BITS    0x20U

/** ISO 14443-4 S-Block */
#define PHPAL_I14443P4MC_SW_S_BLOCK               0xC0U
/** ISO 14443-4 S-Block constant bits mask */
#define PHPAL_I14443P4MC_SW_S_BLOCK_CONST_MASK    0x04U
/** ISO 14443-4 S-Block constant bits (b3(0)) */
#define PHPAL_I14443P4MC_SW_S_BLOCK_CONST_BITS    0x00U
/** ISO 14443-4 S-Block WTX Frame. */
#define PHPAL_I14443P4MC_SW_S_BLOCK_WTX_FRAME     0xF2U

/** Bit within PCB which signals presence of NAD byte */
#define PHPAL_I14443P4MC_SW_PCB_NAD_FOLLOWING     0x04U

/** Bit within PCB which signals presence of CID byte */
#define PHPAL_I14443P4MC_SW_PCB_CID_FOLLOWING     0x08U

/** Bit within PCB which signals chaining */
#define PHPAL_I14443P4MC_SW_PCB_CHAINING          0x10U

/** Bit within PCB which signals ACK */
#define PHPAL_I14443P4MC_SW_PCB_ACK               0x00U

/** Bit within PCB which signals NAK */
#define PHPAL_I14443P4MC_SW_PCB_NAK               0x10U

/** Bits within PCB which signal a DESELECT command */
#define PHPAL_I14443P4MC_SW_PCB_DESELECT          0x00U

/** Bits within PCB which signal a WTX command */
#define PHPAL_I14443P4MC_SW_PCB_WTX               0x30U

/** WTX Res RFU Mask of ISO 14443-4 S-Block */
#define PHPAL_I14443P4MC_SW_WTX_RFU_MASK          0xC0U

/** WTX value Mask of ISO 14443-4 S-Block */
#define PHPAL_I14443P4MC_SW_WTX_VALUE_MASK        0x3FU

/** WTX max value */
#define PHPAL_I14443P4MC_SW_MAX_WTX_VALUE         0x3BU

/** WTX min value */
#define PHPAL_I14443P4MC_SW_MIN_WTX_VALUE         0x01U

/** Minimum response waiting time, ISO/IEC 14443-4:2008 section 7.2 */
#define PHPAL_I14443P4MC_SW_FWT_MIN_US            302U

/** Maximum response waiting time, ISO/IEC 14443-4:2008 section 7.2 */
#define PHPAL_I14443P4MC_SW_FWT_MAX_US            4947968UL

/**
* \brief Function to validate ISO/IEC 14443-4 frame.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR  Protocol Error.
*/
phStatus_t phpalI14443p4mC_Sw_Int_ValidateFrame(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,      /**< [In] Pointer to this layer's parameter structure */
    uint8_t * pRxBuffer,                                /**< [In] Pointer to receive Buffer */
    uint16_t wRxLength,                                 /**< [In] Pointer to receive Buffer Length */
    uint8_t ** ppPayload,                               /**< [Out] Pointer to pay load Buffer */
    uint16_t * pPayloadLength                           /**< [Out] Pointer to pay load Buffer Length */
    );

/**
* \brief Function to validate ISO14443-4 I-Block.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR  Protocol Error.
*/
phStatus_t phpalI14443p4mC_Sw_Int_IsValidIBlock(
    uint8_t bOpMode,                                    /**< [In] Operating mode of library (NFC or ISO) */
    uint8_t bCheckCid,                                  /**< [In] Enable/Disable CID Checking*/
    uint8_t bCid,                                       /**< [In] CID Value*/
    uint8_t bCheckNad,                                  /**< [In] Enable/Disable NAD Checking*/
    uint8_t bNad,                                       /**< [In] NAD Value */
    uint8_t * pRxBuffer,                                /**< [In] Pointer to receive Buffer */
    uint16_t wRxLength                                  /**< [In] Receive Buffer Length*/
    );

/**
* \brief Function to validate ISO14443-4 R-Block
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR  Protocol Error.
*/
phStatus_t phpalI14443p4mC_Sw_Int_IsValidRBlock(
    uint8_t bOpMode,                                    /**< [In] Operating mode of library (NFC or ISO) */
    uint8_t bCheckCid,                                  /**< [In] Enable/Disable CID */
    uint8_t bCid,                                       /**< [In] CID Value*/
    uint8_t * pRxBuffer,                                /**< [In] Pointer to receive Buffer */
    uint16_t wRxLength                                  /**< [In] Receive Buffer Length*/
    );

/**
* \brief Function to validate ISO14443-4 S-Block
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR  Protocol Error.
*/
phStatus_t phpalI14443p4mC_Sw_Int_IsValidSBlock(
    uint8_t bOpMode,                                    /**< [In] Operating mode of library (NFC or ISO) */
    uint8_t bCheckCid,                                  /**< [In] Enable/Disable CID */
    uint8_t bCid,                                       /**< [In] CID Value*/
    uint8_t bWTX,                                       /**< [In] Value of WTX */
    uint8_t * pRxBuffer,                                /**< [In] Pointer to receive Buffer */
    uint16_t wRxLength                                  /**< [In] Receive Buffer Length */
    );

/**
* \brief Function to get PDU header Length.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4mC_Sw_Int_GetPduHeaderLength(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,      /**< [In] Pointer to this layer's parameter structure */
    uint8_t bNadInclude,                                /**< [In] NAD Present or not in current frame */
    uint8_t * pProtLength                               /**< [Out] Pointer protocol Length */
    );

/**
* \brief Validate PPS Request and send PPS Response.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other errors depends on lower layers.
*/
phStatus_t phpalI14443p4mC_Sw_Int_PpsRes(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,      /**< [In] Pointer to this layer's parameter structure */
    uint8_t * pPps,                                     /**< [In] Pointer to PPS Response Buffer */
    uint8_t  wPpsLength                                 /**< [In] Length of PPS receive Buffer */
    );

/**
* \brief Perform a ISO14443-4 S-Block Deselect Response.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other errors depends on lower layers.
*/
phStatus_t phpalI14443p4mC_Sw_Int_DslRes(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams       /**< [In] Pointer to this layer's parameter structure */
    );

/**
* \brief Function to send ACK.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other errors depends on lower layers.
*/
phStatus_t phpalI14443p4mC_Sw_Int_Ack(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams       /**< [In] Pointer to this layer's parameter structure */
    );

/**
* \brief Function to handle Receive Chaining
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other errors depends on lower layers.
*/
phStatus_t phpalI14443p4mC_Sw_Int_HandleRxChaining(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams,      /**< [In] Pointer to this layer's parameter structure */
    uint16_t wOption,                                   /**< [In] Receive option */
    uint8_t ** ppRxBuffer,                              /**< [Out] Pointer to receive Buffer */
    uint16_t * pRxLength                                /**< [Out] Pointer to receive Buffer Length */
    );

/**
* \brief Function to start WTX timer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other errors depends on lower layers.
*/
phStatus_t phpalI14443p4mC_Sw_Int_StartWtxTimer(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams       /**< [In] Pointer to this layer's parameter structure */
    );


#endif /* PHPALI14443P4MC_INT_H */
