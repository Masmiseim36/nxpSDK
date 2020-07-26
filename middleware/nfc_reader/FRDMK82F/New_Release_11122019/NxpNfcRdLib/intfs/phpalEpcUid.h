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
* Generic ICode EPC/UID Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 8. September 2009
*
*/

#ifndef PHPALEPCUID_H
#define PHPALEPCUID_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


#ifdef NXPBUILD__PHPAL_EPCUID

/** \defgroup phpalEpcUid ICODE EPC, ICODE UID
* \brief These Components implement the ICode EPC and ICode UID products.
* @{
*/

#define PHPAL_EPCUID_LABEL_EPC              0x00U   /**< EPC label identifier */
#define PHPAL_EPCUID_LABEL_UID              0x01U   /**< UID label identifier */

#define PHPAL_EPCUID_EPC_MAXBLOCKNUMBER     16U     /**< Last EPC block number. */
#define PHPAL_EPCUID_UID_MAXBLOCKNUMBER     23U     /**< Last UID block number. */

#define PHPAL_EPCUID_EPC_LENGTH             12U     /**< EPC length in bytes without CRC. */
#define PHPAL_EPCUID_IDD_LENGTH             19U     /**< IDD length in bytes without UID-CRC. */

#define PHPAL_EPCUID_DESTROYCODE_LENGTH     3U      /**< Length of the destroy code. */

#define PHPAL_EPCUID_CLOSESLOT_NONLAST      0x00U    /**< Send a non last close slot and wait for tag replies afterwards. */
#define PHPAL_EPCUID_CLOSESLOT_LAST         0x01U    /**< Send the last close slot of a reply round, no data is received afterwards. */

#define PHPAL_EPCUID_NUMSLOTS_1             0x00U    /**< Value for \c bNumSlots = 1. */
#define PHPAL_EPCUID_NUMSLOTS_4             0x01U    /**< Value for \c bNumSlots = 4. */
#define PHPAL_EPCUID_NUMSLOTS_8             0x03U    /**< Value for \c bNumSlots = 8. */
#define PHPAL_EPCUID_NUMSLOTS_16            0x07U    /**< Value for \c bNumSlots = 16. */
#define PHPAL_EPCUID_NUMSLOTS_32            0x0FU    /**< Value for \c bNumSlots = 32. */
#define PHPAL_EPCUID_NUMSLOTS_64            0x1FU    /**< Value for \c bNumSlots = 64. */
#define PHPAL_EPCUID_NUMSLOTS_128           0x3FU    /**< Value for \c bNumSlots = 128. */
#define PHPAL_EPCUID_NUMSLOTS_256           0x7FU    /**< Value for \c bNumSlots = 256. */
#define PHPAL_EPCUID_NUMSLOTS_512           0xFFU    /**< Value for \c bNumSlots = 512. */

/** @} */
#endif /* NXPBUILD__PHPAL_EPCUID */

#ifdef NXPBUILD__PHPAL_EPCUID_SW

/** \defgroup phpalEpcUid_Sw Component : Software
* @{
*/

#define PHPAL_EPCUID_SW_ID                  0x01U    /**< ID for Software EPCUID layer */

/**
* \brief Software PAL-EPCUID parameter structure
*/
typedef struct
{
    uint16_t wId;               /**< Layer ID for this component, NEVER MODIFY! */
    void * pHalDataParams;      /**< Pointer to the parameter structure of the underlying HAL layer. */
    uint8_t bUidValid;          /**< Whether current UID is valid or not. */
    uint8_t bLabelType;         /**< Type of selected label. */
    uint8_t bMaskBitLength;        /**< Number of masked bits already written into \c bEpc/bIdd. */
    union
    {
        uint8_t bEpc[PHPAL_EPCUID_EPC_LENGTH];  /**< 12 bytes EPC. */
        uint8_t bIdd[PHPAL_EPCUID_IDD_LENGTH];  /**< 12 bytes user data (UD) + 2 bytes UD-CRC + 5 bytes UID. */
    } LabelInfo;
} phpalEpcUid_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalEpcUid_Sw_Init(
                               phpalEpcUid_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                               uint16_t wSizeOfDataParams,                  /**< [In] Specifies the size of the data parameter structure. */
                               void * pHalDataParams                        /**< [In] Pointer to the parameter structure of the underlying HAL layer. */
                               );

/** @} */
#endif /* NXPBUILD__PHPAL_EPCUID_SW */

#ifdef NXPBUILD__PHPAL_EPCUID

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalEpcUid/src/Sw/phpalEpcUid_Sw.h"

#define phpalEpcUid_ActivateCard( pDataParams, bTagType, bNumSlots, pMask, bMaskLength, bHash, pRxBuffer, pRxLength, pMoreCardsAvailable) \
        phpalEpcUid_Sw_ActivateCard((phpalEpcUid_Sw_DataParams_t *)pDataParams, bTagType, bNumSlots, pMask, bMaskLength, bHash, pRxBuffer, pRxLength, pMoreCardsAvailable)

#define phpalEpcUid_BeginRound( pDataParams, bTagType, bNumSlots, pMask,bMaskLength, bHash, pRxBuffer, pRxLength) \
        phpalEpcUid_Sw_BeginRound((phpalEpcUid_Sw_DataParams_t *)pDataParams, bTagType, bNumSlots, pMask,bMaskLength, bHash, pRxBuffer, pRxLength) \

#define phpalEpcUid_CloseSlot( pDataParams, bOption, pRxBuffer, pRxLength) \
        phpalEpcUid_Sw_CloseSlot((phpalEpcUid_Sw_DataParams_t *)pDataParams, bOption, pRxBuffer, pRxLength)

#define phpalEpcUid_FixSlot( pDataParams, bTagType, pMask, bMaskLength) \
        phpalEpcUid_Sw_FixSlot((phpalEpcUid_Sw_DataParams_t *) pDataParams, bTagType, pMask, bMaskLength)

#define phpalEpcUid_Write( pDataParams, bTagType, bBlockNo, bData) \
        phpalEpcUid_Sw_Write( (phpalEpcUid_Sw_DataParams_t *)pDataParams, bTagType, bBlockNo, bData)

#define phpalEpcUid_Destroy( pDataParams, bTagType,pData, bDataLength, pDestroyCode) \
        phpalEpcUid_Sw_Destroy((phpalEpcUid_Sw_DataParams_t *) pDataParams, bTagType,pData, bDataLength, pDestroyCode)

#define phpalEpcUid_GetSerialNo( pDataParams, pRxBuffer, pRxLength) \
        phpalEpcUid_Sw_GetSerialNo((phpalEpcUid_Sw_DataParams_t *) pDataParams, pRxBuffer, pRxLength)

#else

/** \addtogroup phpalEpcUid
* @{
*/

/**
* \brief Perform BeginRound command and bring first label to FIXED SLOT state.
*
* This command performs a \ref phpalEpcUid_BeginRound command and
* iterates through the given number of slots using the \ref phpalEpcUid_CloseSlot
* command until it receives the first correct response.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalEpcUid_ActivateCard(
                                    void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
                                    uint8_t bTagType,               /**< [In] Tag type; Either #PHPAL_EPCUID_LABEL_EPC or #PHPAL_EPCUID_LABEL_UID. */
                                    uint8_t bNumSlots,              /**< [In] Number of slots. */
                                    uint8_t * pMask,                /**< [In] Fractional or complete EPC/IDD for selecting certain labels. */
                                    uint8_t bMaskBitLength,         /**< [In] Length of the mask in bits. */
                                    uint8_t bHash,                  /**< [In] Used by EPC labels to generate random slot position. */
                                    uint8_t * pRxBuffer,            /**< [Out] Data returned by the tag which was detected first. */
                                    uint8_t * pRxLength,            /**< [Out] Received data length. */
                                    uint8_t * pMoreCardsAvailable   /**< [Out] Indicates if more than one tag was detected */
                                    );

/**
* \brief Perform BeginRound command.
*
* This command sends a BeginRound command to all labels and
* stores the response of each label into a seperate
* pLabelInfo Array.
* Since the round starts with SlotF and not Slot0, the caller has to ensure
* that the number of entries within \c pLabelInfo is \c 1+bNumSlots.
* For each slot, a "Close Slot" Sequence is performed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalEpcUid_BeginRound(
                                  void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t bTagType,         /**< [In] Tag type; Either #PHPAL_EPCUID_LABEL_EPC or #PHPAL_EPCUID_LABEL_UID. */
                                  uint8_t bNumSlots,        /**< [In] Number of slots. */
                                  uint8_t * pMask,          /**< [In] Fractional or complete EPC/IDD for selecting certain labels. */
                                  uint8_t bMaskBitLength,   /**< [In] Length of the mask in bits. */
                                  uint8_t bHash,            /**< [In] Used by EPC labels to generate random slot position. */
                                  uint8_t * pRxBuffer,      /**< [Out] Data returned by the tag which was detected first. */
                                  uint8_t * pRxLength       /**< [Out] Received data length. */
                                  );

/**
* \brief Perform CloseSlot command.
*
* This command closes the actual slot and returnes the answer of all labels
* replying in the next slot. If \c bOption is set to #PHPAL_EPCUID_CLOSESLOT_LAST
* the reply round is closed and no more data is received.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalEpcUid_CloseSlot(
                                 void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t bOption,       /**< [In] Option; Either #PHPAL_EPCUID_CLOSESLOT_NONLAST or #PHPAL_EPCUID_CLOSESLOT_LAST. */
                                 uint8_t * pRxBuffer,   /**< [Out] Data returned by the tag which answers during the next slot. */
                                 uint8_t * pRxLength    /**< [Out] Received data length. */
                                 );

/**
* \brief Perform FixSlot command.
*
* This command sets a lables matching to the given mask into the FIXED state.
* A begin round with a single slot is started and the responding label is fixed by a
* following FixSlot command.
* <em><strong>Remark:</strong> This command is coupled with \ref phpalEpcUid_BeginRound to ensure the
* exact timing 302,04us (using TxWait) between the two commands.</em>
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalEpcUid_FixSlot(
                               void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bTagType,        /**< [In] Tag type; Either #PHPAL_EPCUID_LABEL_EPC or #PHPAL_EPCUID_LABEL_UID. */
                               uint8_t * pMask,         /**< [In] Complete EPC/IDD for target label. */
                               uint8_t bMaskBitLength   /**< [In] Length of the mask in bits. */
                               );

/**
* \brief Write a block.
*
* There is no response from the label after sending this command.
* In order to verify the written data, a
* \ref phpalEpcUid_BeginRound command should be performed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalEpcUid_Write(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t bTagType,      /**< [In] Tag type; Either #PHPAL_EPCUID_LABEL_EPC or #PHPAL_EPCUID_LABEL_UID. */
                             uint8_t bBlockNo,      /**< [In] Destination block number. */
                             uint8_t bData          /**< [In] Data to write */
                             );


/**
* \brief Destroy Label.
*
* There is no response from the label after sending this command.
* In order to verify that the label is destroyed,
* a \ref phpalEpcUid_BeginRound command should be performed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalEpcUid_Destroy(
                               void * pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                               uint8_t bTagType,        /**< [In] Tag type; Either #PHPAL_EPCUID_LABEL_EPC or #PHPAL_EPCUID_LABEL_UID. */
                               uint8_t * pData,         /**< [In] Complete EPC/IDD data of the tag to be destroyed. */
                               uint8_t bDataLength,     /**< [In] Length of EPC/IDD data. */
                               uint8_t * pDestroyCode   /**< [In] 3 bytes destroy code. */
                               );

/**
* \brief Get the tag data.
*
* The command returnes the tag response to the last \ref phpalEpcUid_ActivateCard.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlaying component.
*/
phStatus_t phpalEpcUid_GetSerialNo(
                                   void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t * pUidOut,   /**< [Out] Tag response to the last \ref phpalEpcUid_ActivateCard. */
                                   uint8_t * pLenUidOut /**< [Out] Tag response length. */
                                   );

/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_EPCUID */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALEPCUID_H */
