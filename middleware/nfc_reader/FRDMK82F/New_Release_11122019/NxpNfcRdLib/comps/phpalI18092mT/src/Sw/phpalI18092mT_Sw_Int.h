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
* Internal definitions for software ISO18092 target mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 27. October 2014
*/

#ifndef PHPALI18092MT_SW_INT_H
#define PHPALI18092MT_SW_INT_H

#include <phpalI18092mT.h>
#include "phpalI18092mT_Sw.h"

/** Macro which checks for invalid block status. */
#define PHPAL_I18092MT_SW_IS_INVALID_BLOCK_STATUS(bStatus)                                              \
    ((                                                                                                  \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT) ||                                                 \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_INTEGRITY_ERROR) ||                                            \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_FRAMING_ERROR) ||                                              \
    (((bStatus) & PH_ERR_MASK) == PH_ERR_COLLISION_ERROR))                                              \
    ? 1 : 0                                                                                             \
    )

/** Macro which checks if given PDU is timeout PDU. */
#define PHPAL_I18092MT_SW_IS_TIMEOUT(bPfb)                                                              \
    ((                                                                                                  \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK) == PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK) &&   \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_OPTION_MASK) == PHPAL_I18092MT_SW_PFB_OPTION_MASK))                \
    ? 1 : 0                                                                                             \
    )

/** Macro which checks if given PDU is ATN PDU. */
#define PHPAL_I18092MT_SW_IS_ATTENTION(bPfb)                                                            \
    ((                                                                                                  \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK) == PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK) &&   \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_OPTION_MASK) == 0x00U))                                             \
    ? 1 : 0                                                                                             \
    )

/** Macro which checks if given PDU is ACK PDU. */
#define PHPAL_I18092MT_SW_IS_ACK(bPfb)                                                                 \
    ((                                                                                                 \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK) == PHPAL_I18092MT_SW_PFB_ACK_PDU_MASK) &&          \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_OPTION_MASK) == 0x00U))                                            \
    ? 1 : 0                                                                                            \
    )

/** Macro which checks if given PDU is NACK PDU. */
#define PHPAL_I18092MT_SW_IS_NACK(bPfb)                                                                 \
    ((                                                                                                  \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK) == PHPAL_I18092MT_SW_PFB_ACK_PDU_MASK) &&           \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_OPTION_MASK) == PHPAL_I18092MT_SW_PFB_OPTION_MASK))                \
    ? 1 : 0                                                                                             \
    )

/** Macro which checks if given I-PDU is chaining. */
#define PHPAL_I18092MT_SW_IS_CHAINING(bPfb)                                                             \
    ((                                                                                                  \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK) == PHPAL_I18092MT_SW_PFB_INFORMATION_PDU_MASK) &&   \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_OPTION_MASK) == PHPAL_I18092MT_SW_PFB_OPTION_MASK))                \
    ? 1 : 0                                                                                             \
    )

/** Macro which checks if given I-PDU is not chaining. */
#define PHPAL_I18092MT_SW_IS_NOT_CHAINING(bPfb)                                                         \
    ((                                                                                                  \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK) == PHPAL_I18092MT_SW_PFB_INFORMATION_PDU_MASK) &&   \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_OPTION_MASK) == 0x00U))                                             \
    ? 1 : 0                                                                                             \
    )

/** Macro which checks if given PDU is an I-PDU. */
#define PHPAL_I18092MT_SW_IS_INFORMATION_PDU(bPfb)                                                      \
    (                                                                                                   \
    (((bPfb) & PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK) == PHPAL_I18092MT_SW_PFB_INFORMATION_PDU_MASK)      \
    ? 1 : 0                                                                                             \
    )

/** Macro used to increment PNI. */
#define PHPAL_I18092MT_SW_INCREMENT_PNI(bPni)                                                          \
    (((bPni) == 3U) ? 0 : ((bPni) + 1U))

/** Macro used to increment PNI. */
#define PHPAL_I18092MT_SW_DECREMENT_PNI(bPni)                                                          \
    (((bPni) == 0U) ? 3U : ((bPni) - 1))

#define PHPAL_I18092MT_SW_CALCULATE_RWT_US(bTo)                                                         \
    (PHPAL_I18092MT_SW_RWT_MIN_US*((uint32_t)(1U << (bTo))))

/** Request command code. */
#define PHPAL_I18092MT_SW_CMD_REQUEST                  0xD4U

/** Response command code. */
#define PHPAL_I18092MT_SW_CMD_RESPONSE                 0xD5U

/** Attribute request command code. */
#define PHPAL_I18092MT_SW_CMD_ATR_REQ                  0x00U

/** Bit position of LRt field within PPt byte for attribute request command. */
#define PHPAL_I18092MT_SW_ATR_PPT_LRT_BIT              0x04U

/** Attribute response command code. */
#define PHPAL_I18092MT_SW_CMD_ATR_RES                  0x01U

/** Attribute response command code. */
#define PHPAL_I18092MT_SW_CMD_WUP_REQ                  0x02U

/** Attribute response command code. */
#define PHPAL_I18092MT_SW_CMD_WUP_RES                  0x03U

/** Parameter select request command code. */
#define PHPAL_I18092MT_SW_CMD_PSL_REQ                  0x04U

/** Parameter select response command code. */
#define PHPAL_I18092MT_SW_CMD_PSL_RES                  0x05U

/** Data exchange protocol request command code. */
#define PHPAL_I18092MT_SW_CMD_DEP_REQ                  0x06U

/** Data exchange protocol response command code. */
#define PHPAL_I18092MT_SW_CMD_DEP_RES                  0x07U

/** Deselect request command code. */
#define PHPAL_I18092MT_SW_CMD_DSL_REQ                  0x08U

/** Deselect response command code. */
#define PHPAL_I18092MT_SW_CMD_DSL_RES                  0x09U

/** Release request command code. */
#define PHPAL_I18092MT_SW_CMD_RLS_REQ                  0x0AU

/** Release response command code. */
#define PHPAL_I18092MT_SW_CMD_RLS_RES                  0x0BU

/** ATN command. */
#define PHPAL_I18092MT_SW_CMD_ATN                      0xFEU

/** NACK command. */
#define PHPAL_I18092MT_SW_CMD_NACK                     0xFDU

/** NACK command. */
#define PHPAL_I18092MT_SW_CMD_ACK                      0xFCU

/** Position of NFCID3 within ATR_REQ. */
#define PHPAL_I18092MT_SW_ATR_REQ_NFCID3_POS           0x02U

/** Position of DID within ATR_REQ. */
#define PHPAL_I18092MT_SW_ATR_REQ_DID_POS              12U

/** Position of BS(T) within ATR_REQ. */
#define PHPAL_I18092MT_SW_ATR_REQ_BSI_POS              13U

/** Position of BR(T) within ATR_REQ. */
#define PHPAL_I18092MT_SW_ATR_REQ_BRI_POS              14U

/** Position of TO within ATR_RES. */
#define PHPAL_I18092MT_SW_ATR_RES_TO_POS               15U

/** Position of PP(I) within ATR_REQ. */
#define PHPAL_I18092MT_SW_ATR_REQ_PPI_POS              15U

/** Position of PP(I) within ATR_RES. */
#define PHPAL_I18092MT_SW_ATR_RES_PPI_POS              16U

/** Gt_available bit within PP(I) byte of ATR_REQ. */
#define PHPAL_I18092MT_SW_ATR_REQ_PPI_GT_AVAILABLE     0x02U

/** Gt_available bit within PP(T) byte of ATR_RES. */
#define PHPAL_I18092MT_SW_ATR_RES_PPT_GT_AVAILABLE     0x02U

/** Position of DID within the DSL_REQ command */
#define PHPAL_I18092MT_SW_DSL_REQ_DID_POS              0x02U

/** The position of DID within PSL_REQ */
#define PHPAL_I18092MT_SW_PSL_REQ_DID_POS              0x02U

/** Position of BRS within PSL_REQ */
#define PHPAL_I18092MT_SW_PSL_REQ_BRS_POS              0x03U

/** Postion of FSL within PSL_REQ */
#define PHPAL_I18092MT_SW_PSL_REQ_FSL_POS              0x04U

/** Position of DID within the RLS_REQ */
#define PHPAL_I18092MT_SW_RLS_REQ_DID_POS              0x02U

/** NAD_used bit within PP(I) byte of ATR_REQ. */
#define PHPAL_I18092MT_SW_ATR_REQ_PPI_NAD_USED         0x01U

/** Gt data starting location in the frame */
#define PHPAL_I18092MT_SW_ATR_REQ_GT_POS               16U

/** Bit mask within PFB identifying an information PDU. */
#define PHPAL_I18092MT_SW_PFB_INFORMATION_PDU_MASK     0x00U

/** Bit mask within PFB identifying an ACK/NACK PDU. */
#define PHPAL_I18092MT_SW_PFB_ACK_PDU_MASK             0x40U

/** Bit mask within PFB identifying a supervisory PDU. */
#define PHPAL_I18092MT_SW_PFB_SUPERVISORY_PDU_MASK     0x80U

/** Bit mask within PFB identifying a protected PDU. */
#define PHPAL_I18092MT_SW_PFB_PROTECTED_PDU_MASK       0x20U

/** Bit mask within PFB to get type of PDU. */
#define PHPAL_I18092MT_SW_PFB_PDU_TYPE_MASK            0xE0U

/** Bit mask within PFB for checking NAD enabler. */
#define PHPAL_I18092MT_SW_PFB_NAD_MASK                 0x08U

/** Bit mask within PFB for checking DID enabler. */
#define PHPAL_I18092MT_SW_PFB_DID_MASK                 0x04U

/** Bit mask within PFB for getting PNI. */
#define PHPAL_I18092MT_SW_PFB_PNI_MASK                 0x03U

/** Bit mask within PDU for getting option field (=ACK or ATTENTION or MI). */
#define PHPAL_I18092MT_SW_PFB_OPTION_MASK              0x10U

/** Activation response waiting time in milliseconds, NFCForum-TS-DigitalProtocol-1.0 section A.10. */
#define PHPAL_I18092MT_SW_DELTA_T_INITIATOR_MS         100U

/** Time extension in microseconds for all commands, NFCForum-TS-DigitalProtocol-1.0 section A.10. */
#define PHPAL_I18092MT_SW_DELTA_RWT_US                 2U

/** Activation response waiting time in milliseconds, NFCForum-TS-DigitalProtocol-1.0 section A.10. deltaRWT is included in this case. */
#define PHPAL_I18092MT_SW_RWT_ACTIVATION_MS            1239U

/** Minimum response waiting time, ISO/IEC 18092:2004(E) section 12.5.1.2.1. */
#define PHPAL_I18092MT_SW_RWT_MIN_US                   302U

/** Maximum response waiting time, ISO/IEC 18092:2004(E) section A.10. */
#define PHPAL_I18092MT_SW_RWT_MAX_US                   4947968UL

/** Minimum frame size, NFCForum-TS-DigitalProtocol-1.0. */
#define PHPAL_I18092MT_SW_MIN_FRAMESIZE                3U

/** Maximum frame size, NFCForum-TS-DigitalProtocol-1.0. */
#define PHPAL_I18092MT_SW_MAX_FRAMESIZE                255U

/** Frame length must be min. 17 bytes during activation. (NFCForum-TS-DigitalProtocol-1.0, section 14.6.1.1. */
#define PHPAL_I18092MT_SW_MIN_ACTIVATION_FRAME_LENGTH  16U

/** Frame length is limited to 64 bytes during activation. (NFCForum-TS-DigitalProtocol-1.0, section 14.6.1.1. */
#define PHPAL_I18092MT_SW_MAX_ACTIVATION_FRAME_LENGTH  64U

/** Frame length of the PSL request command  */
#define PHPAL_I18092MT_SW_PSL_REQ_LENGTH               0x05U

/** DSL request command length */
#define PHPAL_I18092MT_SW_DSL_REQ_LENGTH               0x02U

/** RSL request command length */
#define PHPAL_I18092MT_SW_RLS_REQ_LENGTH               0x02U

/** WUP request command length */
#define PHPAL_I18092MT_SW_WUP_REQ_LENGTH               0x0DU

/** State information PDU to be transmitted. */
#define PHPAL_I18092MT_SW_STATE_INFORMATION_PDU_TX     0x00U

/** State ACK PDU to be transmitted. */
#define PHPAL_I18092MT_SW_STATE_ACK_PDU_TX             0x01U

/** State information PDU to be received. */
#define PHPAL_I18092MT_SW_STATE_INFORMATION_PDU_RX     0x02U

/** Finished state. */
#define PHPAL_I18092MT_SW_STATE_FINISHED               0x03U

/** State mask. */
#define PHPAL_I18092MT_SW_STATE_MASK                   0x0FU

/** Retransmission of information PDU. */
#define PHPAL_I18092MT_SW_STATE_RETRANSMIT_BIT         0x10U

/** Information PDU is chaining. */
#define PHPAL_I18092MT_SW_STATE_CHAINING_BIT           0x20U

/** Information PDU is first block in chain. */
#define PHPAL_I18092MT_SW_STATE_CHAINING_FIRST_BIT     0x40U

/** Information PDU is last block in chain. */
#define PHPAL_I18092MT_SW_STATE_CHAINING_LAST_BIT      0x80U

/** Defines initial/unknown state of the target state machine */
#define PHPAL_I18092MT_SW_STATE_NONE                   0x00U

/** Indicates that the state machine has been activated to ATR */
#define PHPAL_I18092MT_SW_STATE_ATR_ACTIVATED          0x01U

/** Indicates that the target state machine is in receive mode */
#define PHPAL_I18092MT_SW_STATE_RX                     0x02U

/** State indicating that the target state machine is in transmit state */
#define PHPAL_I18092MT_SW_STATE_TX                     0x03U

/** Max length of the Header */
#define PHPAL_I18092MT_SW_MAX_HEADER_LENGTH            0x07U

/** The bit position indicating chaining(MI) within information PDU */
#define PHPAL_I18092MT_SW_MI_BIT_POSIITON              0x04U

/** The bit position indicating presence of NAD */
#define PHPAL_I18092MT_SW_NAD_BIT_POSITION             0x03U

/** The bit position indicating presence of DID */
#define PHPAL_I18092MT_SW_DID_BIT_POSITION             0x02U

/** RTOX Payload Length */
#define PHPAL_I18092MT_SW_RTOX_LENGTH                  0x01U

/** DSL request command length */
#define PHPAL_I18092MT_SW_ATN_RES_LENGTH               0x04U

phStatus_t phpalI18092mT_Sw_ValidateAndNormalizeResponseFrame(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pFrame,
    uint16_t wFrameLength,
    uint8_t bExpectedResponseCode,
    uint8_t ** ppValidatedFrame,
    uint16_t * pValidatedFrameLength,
    uint8_t ** ppPayload,
    uint16_t * pPayloadLength
    );

phStatus_t phpalI18092mT_Sw_WriteFrameHeader(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t wOption,
    uint8_t bCommandCode,
    uint8_t bDataLength
    );

phStatus_t phpalI18092mT_Sw_GetPduHeaderLength(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pProtLength
    );

phStatus_t phpalI18092mT_Sw_UpdatePduHeader(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t bDataLength
    );

phStatus_t phpalI18092mT_Sw_WritePduHeader(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t bPduMask,
    uint8_t bOption,
    uint8_t bDataLength
    );

phStatus_t phpalI18092mT_Sw_PerformTxChaining(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint16_t   wOption,
    uint16_t wBytesToSend,
    uint8_t * pTransportDataBytes,
    uint16_t wTransportDataBytesLength
    );

phStatus_t phpalI18092mT_Sw_ResponseToPfb(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t bPfbMask
    );

phStatus_t phpalI18092mT_Sw_HandlechainedData(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t * pRxBuf,
    uint16_t  wRxLen,
    uint8_t * pRxOverlapped,
    uint16_t  *wRxOverlappedStartPos,
    uint16_t  wHeaderLen,
    uint16_t  wOption,
    uint8_t * bDataRec
    );

phStatus_t phpalI18092mT_Sw_ValidatePfb(
    phpalI18092mT_Sw_DataParams_t * pDataParams,
    uint8_t bPfb,
    uint8_t *bDataRec
    );

#endif  /* PHPALI18092MT_SW_INT_H */
