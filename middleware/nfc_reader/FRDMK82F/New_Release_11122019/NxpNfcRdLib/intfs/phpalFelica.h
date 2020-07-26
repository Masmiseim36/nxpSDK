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
* Generic FeliCa Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  RSn: Generated 25. January 2010
*
*/

#ifndef PHPALFELICA_H
#define PHPALFELICA_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHPAL_FELICA

/** \defgroup phpalFelica FeliCa
* \brief These Components implement the FeliCa (contactless) protocol.
* @{
*/

/** \name Lengths
*/
/*@{*/
/** UID length in bytes. */
#define PHPAL_FELICA_IDM_LENGTH     8U
/** PMm length in bytes. */
#define PHPAL_FELICA_PMM_LENGTH     8U
/** Block length in bytes. */
#define PHPAL_FELICA_BLOCK_LENGTH   4U
/** Number of bytes per page. */
#define PHPAL_FELICA_PAGE_LENGTH    16U
/** Length of ATQC. */
#define PHPAL_FELICA_ATQC_LENGTH    17U
/** Length of Request Data (RD) in ATQC. */
#define PHPAL_FELICA_RD_LENGTH      2U
/** Maximum length of a transmission package. */
#define PHPAL_FELICA_TXLENGTH_MAX   255U
/*@}*/

/** @} */
#endif /* NXPBUILD__PHPAL_FELICA */

#ifdef NXPBUILD__PHPAL_FELICA_SW

/** \defgroup phpalFelica_Sw Component : Software
* @{
*/

#define PHPAL_FELICA_SW_ID      0x01U    /**< ID for Software FeliCa layer */

/**
* \brief FeliCa parameter structure
*/
typedef struct
{
    uint16_t  wId;                                                          /**< Layer ID for this component, NEVER MODIFY! */
    void    * pHalDataParams;                                               /**< Pointer to the parameter structure of the underlying layer. */
    uint8_t   aIDmPMm[PHPAL_FELICA_IDM_LENGTH + PHPAL_FELICA_PMM_LENGTH +PHPAL_FELICA_RD_LENGTH];   /**< The Cards Manufacture ID (IDm), Manufacture Parameters (PMm) and Request Data length. */
    uint8_t   bIDmPMmValid;                                                 /**< Whether the stored IDm and PMm are valid (\c 1) or not (\c 0). */
    uint8_t   bLength;                                                      /**< Current data length, used internally for Exchange function. */
    uint8_t   bRequestCode;                                                 /**< Request code (RC) field of ReqC, default value is '0'. */
    uint8_t   bTotalFrames;                                                 /**< Holds total number of response frame received. Value ranges from 1 to N \n
                                                                                 Ignored if \ref phpalFelica_ReqC function called with slot number #PHPAL_FELICA_NUMSLOTS_1. */
    uint8_t   bPreambleLen;                                                 /**< Preamble length used for reception from tag. */
} phpalFelica_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalFelica_Sw_Init(
                               phpalFelica_Sw_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                               uint16_t wSizeOfDataParams,                  /**< [In] Specifies the size of the data parameter structure. */
                               void * pHalDataParams                        /**< [In] Pointer to the parameter structure of the underlying layer. */
                               );

/** @} */
#endif /* NXPBUILD__PHPAL_FELICA_SW */


#ifdef NXPBUILD__PHPAL_FELICA

/** \addtogroup phpalFelica
* @{
*/

/**
* \name Definitions of the time slots for the ReqC command
*/
/*@{*/
#define PHPAL_FELICA_NUMSLOTS_1     0x00U    /**< Value for \c bNumSlots = 1. */
#define PHPAL_FELICA_NUMSLOTS_2     0x01U    /**< Value for \c bNumSlots = 2. */
#define PHPAL_FELICA_NUMSLOTS_4     0x03U    /**< Value for \c bNumSlots = 4. */
#define PHPAL_FELICA_NUMSLOTS_8     0x07U    /**< Value for \c bNumSlots = 8. */
#define PHPAL_FELICA_NUMSLOTS_16    0x0FU    /**< Value for \c bNumSlots = 16. */
/*@}*/

/** \name Preamble Lengths
 */
/*@{*/
#define PHPAL_FELICA_PREAMBLE_LEN_48BITS    0x00U    /**< Preamble of 48 Bits. */
#define PHPAL_FELICA_PREAMBLE_LEN_56BITS    0x02U    /**< Preamble of 56 Bits. */
#define PHPAL_FELICA_PREAMBLE_LEN_64BITS    0x03U    /**< Preamble of 64 Bits. */
#define PHPAL_FELICA_PREAMBLE_LEN_72BITS    0x04U    /**< Preamble of 72 Bits. */
/*@}*/

/**
* \name System Code
*/
/*@{*/
#define PHPAL_FELICA_SYSTEM_CODE_BYTE_0    0xFFU
#define PHPAL_FELICA_SYSTEM_CODE_BYTE_1    0xFFU
/*@}*/

/**
* \name FeliCa Parameters
*/
/*@{*/
/**
* \brief Set / Get Request Code (RC).
*/
#define PHPAL_FELICA_CONFIG_RC                      0x0000U

/**
* \brief Get total number of response frame received.
* Invalid if \ref phpalFelica_ReqC function called with slot number #PHPAL_FELICA_NUMSLOTS_1
*/
#define PH_PALFELICA_CONFIG_NUM_RESPONSE_FRAMES     0x0001U

/**
* \brief Set / Get Preamble Length.
*/
#define PHPAL_FELICA_PREAMBLE_LENGTH                0x0002U

/*@}*/

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalFelica/src/Sw/phpalFelica_Sw.h"

#define phpalFelica_ReqC( pDataParams, pSystemCode, bTimeSlot, ppRxBuffer, pRxLength) \
        phpalFelica_Sw_ReqC((phpalFelica_Sw_DataParams_t *)pDataParams, pSystemCode, bTimeSlot, ppRxBuffer, pRxLength)

#define phpalFelica_ActivateCard( pDataParams, pIDmPMm, bIDmPMmLength,  pSystemCode, \
                                  bTimeSlot, pRxBuffer, pRxLength, pMoreCardsAvailable) \
        phpalFelica_Sw_ActivateCard( (phpalFelica_Sw_DataParams_t *)pDataParams, pIDmPMm, bIDmPMmLength, pSystemCode, bTimeSlot, pRxBuffer, pRxLength, pMoreCardsAvailable )

#define phpalFelica_Exchange( pDataParams, wOption, wN, pTxBuffer, wTxLength, ppRxBuffer, pRxLength) \
        phpalFelica_Sw_Exchange( (phpalFelica_Sw_DataParams_t *)pDataParams, wOption, wN, pTxBuffer, wTxLength, ppRxBuffer, pRxLength )

#define phpalFelica_SetConfig( pDataParams, wConfig, wValue) \
        phpalFelica_Sw_SetConfig((phpalFelica_Sw_DataParams_t *)pDataParams, wConfig, wValue)

#define phpalFelica_GetConfig( pDataParams, wConfig, pValue) \
        phpalFelica_Sw_GetConfig((phpalFelica_Sw_DataParams_t *)pDataParams, wConfig, pValue)

#define phpalFelica_GetSerialNo( pDataParams, pIDmPMmOut,  pLenIDmPMmOut) \
        phpalFelica_Sw_GetSerialNo( (phpalFelica_Sw_DataParams_t *)pDataParams, pIDmPMmOut, pLenIDmPMmOut )

#define phpalFelica_GetFrameInfo(pDataParams, bFrameNum, pResponseBuffer, pwStatus, ppID, pLen) \
        phpalFelica_Sw_GetFrameInfo( (phpalFelica_Sw_DataParams_t *)pDataParams, bFrameNum, pResponseBuffer, pwStatus, ppID, pLen)

#define phpalFelica_SetSerialNo( pDataParams, pIDmPMmIn) \
        phpalFelica_Sw_SetSerialNo( (phpalFelica_Sw_DataParams_t *)pDataParams, pIDmPMmIn)
#else

/**
* \brief Perform the FeliCa ReqC command.
*
* \b Note: This function waits until all cards in all time slots have had time to reply even though only the first response is returned.\n
* The formula used is <b><em>1208us * (n-1)</em></b> according to JIS X 6319-4:2005.
*
* Response format when bNumTimeSlots greater than #PHPAL_FELICA_NUMSLOTS_1
* <pre>
*   0                                                                           28         31
*   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*   |   LEN  |               PAYLOAD(MAX 19 BYTES)              |     PAD      |   STATUS   |
*   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* </pre>
*
* Format of Status Byte.
* <pre>
*   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*   |      |              |      |L|  |  |  |  |                                          |
*   |      |              |      |e|Co|Pr|Da|CL|                                          |
*   | RFU  |     LEN      | RFU  |n|ll|ot|ta|Er|                  RFU                     |
*   | [7:5]|     [4:0]    | [7:5]|E|De|Er|Er|ro|                                          |
*   |      |              |      |r|t |r |r |r |                                          |
*   |      |              |      |r|  |  |  |  |                                          |
*   +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*   |<-------Byte 28----->|<------Byte 29----->|<------Byte 30----->|<------Byte 31------>|
* </pre>
*
* Response format when bNumTimeSlots is #PHPAL_FELICA_NUMSLOTS_1
* <pre>
*   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*   |                   IDmPMm[16]                       |     RD[Optional]    |
*   ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* </pre>
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalFelica_ReqC(
                            void* pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                            uint8_t * pSystemCode,  /**< [In] System-Code; uint8_t[2]. */
                            uint8_t bNumTimeSlots,  /**< [In] Number of time slots to use. Refer to e.g. #PHPAL_FELICA_NUMSLOTS_1 for valid values. */
                            uint8_t ** ppRxBuffer,  /**< [Out] Pointer to the HAL Rx buffer. 32bytes / slot information in case of multiple slots or else a single response containing IDmPMm. */
                            uint16_t * wLen         /**< [Out] Length of the content in the buffer. */
                            );

/**
* \brief This command activates a FeliCa card.
*
* \b Note: If an IDm is passed to this function, it is stored as the current IDm and no real activation is done.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalFelica_ActivateCard(
                                    void* pDataParams,              /**< [In] Pointer to this layer's parameter structure. */
                                    uint8_t * pIDmPMm,              /**< [In] IDm followed by PMm; If supplied it is stored and no real activation is done. */
                                    uint8_t bIDmPMmLength,          /**< [In] IDm length; 0 or 16. */
                                    uint8_t * pSystemCode,          /**< [In] System-Code; uint8_t[2]. */
                                    uint8_t bNumTimeSlots,          /**< [In] Number of timeslots to use. Refer to e.g. #PHPAL_FELICA_NUMSLOTS_1 for valid values. */
                                    uint8_t * pRxBuffer,            /**< [Out] 8 bytes NFCID2 + 8 bytes PAD; uint8_t[16]. */
                                    uint8_t * pRxLength,            /**< [Out] Length of received data. 0 or 16. */
                                    uint8_t * pMoreCardsAvailable   /**< [Out] Whether there are more cards in the field or not; uint8_t. */
                                    );

/**
* \brief Exchange data with the Picc.
*
* \c wOption can be one of:\n
* \li #PH_EXCHANGE_DEFAULT
* \li #PH_EXCHANGE_BUFFER_FIRST
* \li #PH_EXCHANGE_BUFFER_CONT
* \li #PH_EXCHANGE_BUFFER_LAST
*
* Alternatively, the following bits can be combined:\n
* \li #PH_EXCHANGE_BUFFERED_BIT
* \li #PH_EXCHANGE_LEAVE_BUFFER_BIT
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalFelica_Exchange(
                                void* pDataParams,      /**< [In] Pointer to this layer's parameter structure. */
                                uint16_t wOption,       /**< [In] Option parameter. */
                                uint16_t wN,            /**< [In] N to calculate the response timeout. */
                                uint8_t* pTxBuffer,     /**< [In] Data to transmit; Length and IDm is added automatically. */
                                uint16_t wTxLength,     /**< [In] Length of data to transmit. */
                                uint8_t ** ppRxBuffer,  /**< [Out] Pointer to received data; Length, response code and IDm are removed automatically. */
                                uint16_t * pRxLength    /**< [Out] number of received data bytes. */
                                );

/**
* \brief Set configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalFelica_SetConfig(
                                 void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wConfig,    /**< [In] Configuration Identifier. */
                                 uint16_t wValue      /**< [In] Configuration Value. */
                                 );

/**
* \brief Get configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalFelica_GetConfig(
                                 void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wConfig,    /**< [In] Configuration Identifier. */
                                 uint16_t * pValue    /**< [Out] Configuration Value. */
                                 );
/**
* \brief As the card receives the Get Serial Number command, it shall respond with that one.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION No Serial number available at the moment.
*/
phStatus_t phpalFelica_GetSerialNo(
                                   void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t * pUidOut,   /**< [Out] Last Uid (IDm + PMm); uint8_t[16]. */
                                   uint8_t * pLenUidOut /**< [Out] Length of Uid; 0 or 16. */
                                   );


/**
* \brief This function will provide information of given frame number(1 to N).
* It will provide pointer to IDmPMm if frame status is #PH_ERR_SUCCESS.
* \b pwStatus can be one of:\n
* \li #PH_ERR_SUCCESS
* \li #PH_ERR_PROTOCOL_ERROR
* \li #PH_ERR_COLLISION_ERROR
* \li #PH_ERR_INTEGRITY_ERROR
* \li #PH_ERR_LENGTH_ERROR
*
* \b Note: This function is only valid when function \ref phpalFelica_ReqC is called with slot number greater than #PHPAL_FELICA_NUMSLOTS_1.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t  phpalFelica_GetFrameInfo(
                                     void * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
                                     uint8_t bFrameNum,         /**< [In] Frame number. Frame number 0 is invalid. */
                                     uint8_t * pResponseBuffer, /**< [In] The pointer to the entire FeliCa response as obtained from ReqC API. */
                                     uint16_t *pwStatus,        /**< [Out] Frame status. Status will be Success, or one of collision, length, integrity, protocol errors. */
                                     uint8_t** pID,             /**< [Out] Pointer to IDmPMm in case of wStatus being success. */
                                     uint8_t * pLen             /**< [Out] Length of IDmPMm data. 2 Request Data (RD) bytes are optional. This will indicate exact length. */
                                     );

/**
* \brief This function will update IDmPMm in this layer's parameter structure.
* This function must be call after card or device is activated.
*
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalFelica_SetSerialNo(
                                   void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t * pIDmPMm    /**< [In] 16 byte IDmPMm. */
                                   );
/** @} */
#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_FELICA */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALFELICA_H */
