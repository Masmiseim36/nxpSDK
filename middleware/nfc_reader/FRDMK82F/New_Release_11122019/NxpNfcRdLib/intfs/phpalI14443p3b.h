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
* Generic ISO14443-3B Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 8. September 2009
*
*/

#ifndef PHPALI14443P3B_H
#define PHPALI14443P3B_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHPAL_I14443P3B_SW

/** \defgroup phpalI14443p3b_Sw Component : Software
* @{
*/

#define PHPAL_I14443P3B_SW_ID   0x01U    /**< ID for Software ISO14443-3B layer     */

/**
* \brief Software PAL-ISO14443P3B parameter structure
*/
typedef struct
{
    uint16_t wId;                       /**< Layer ID for this component, NEVER MODIFY! */
    void  * pHalDataParams;             /**< Pointer to the parameter structure of the underlying layer.*/
    uint8_t bExtAtqb;                   /**< Stores whether last request used extended ATQB. */
    uint8_t pPupi[4];                   /**< Array holding the activated PUPI. */
    uint8_t bPupiValid;                 /**< Whether the stored Pupi is valid (\c 1) or not (\c 0). */
    uint8_t bCidSupported;              /**< Cid Support indicator; Unequal '0' if supported. */
    uint8_t bNadSupported;              /**< Nad Support indicator; Unequal '0' if supported. */
    uint8_t bCid;                       /**< Card Identifier; Ignored if bCidSupported is equal '0'. */
    uint8_t bFwi;                       /**< Frame Waiting Integer. */
    uint8_t bFsci;                      /**< PICC Frame Size Integer; 0-12; */
    uint8_t bFsdi;                      /**< (Current) PCD Frame Size Integer; 0-12; */
    uint8_t bDri;                       /**< (Current) Divisor Receive (PCD to PICC) Integer; 0-3; */
    uint8_t bDsi;                       /**< (Current) Divisor Send (PICC to PCD) Integer; 0-3; */
    uint8_t bAttribParam1;              /**< Param1 parameter for attrib command. */
    uint8_t * pHigherLayerInf;          /**< Pointer to higher layer information buffer. */
    uint16_t wTxWait;                    /**< Tx Wait Time */
    uint16_t  wHigherLayerInfLen;       /**< Length higher layer information. */
    uint8_t * pHigherLayerResp;         /**< Pointer to higher layer response buffer. */
    uint16_t  wHigherLayerRespSize;     /**< Size of higher layer response buffer. */
    uint16_t  wHigherLayerRespLen;      /**< Length of higher layer response. */
    uint8_t bOpeMode;                   /**< Operation mode. One of NFC, EMVCo, ISO. */
    uint8_t bPollCmd;                   /**< Used to differentiate which command to send REQA or WakeUpA command for polling */
    uint8_t bRetryCount;                /**< Retry count for ATTRIB command as per NFC Digital Protocol Version 1.1\n
                                             For mode #RD_LIB_MODE_EMVCO retry count should be 1. */
} phpalI14443p3b_Sw_DataParams_t;

phStatus_t phpalI14443p3b_Sw_Init(
                                  phpalI14443p3b_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                  uint16_t wSizeOfDataParams,                   /**< [In] Specifies the size of the data parameter structure. */
                                  void * pHalDataParams                         /**< [In] Pointer to the parameter structure of the underlying layer.*/
                                  );

/** @} */
#endif /* NXPBUILD__PHPAL_I14443P3B_SW */


#ifdef NXPBUILD__PHPAL_I14443P3B

/** \defgroup phpalI14443p3b ISO/IEC 14443-3B
* \brief These Components implement the ISO/IEC 14443-3 Type-B contactless protocol.
*
* \b Note: The "Timing before the PCD SOF" <em>(see 7.1.7, ISO/IEC 14443-3:2009(E))</em> is mandatory and is
* neither implemented here nor implemented in every \ref phhalHw layer.\n
* Make sure that either the used HAL or the used application does comply to this rule.
*
* @{
*/

/**
* \name 14443-3B constant definitions
*/
/*@{*/
#define PHPAL_I14443P3B_CONFIG_ATTRIB_PARAM1    0x0000U     /**< Set or get the AttribB param1 byte. */
/*@}*/

/**
* \name Configuration
*/
/*@{*/

/**
 * Configure Operation Mode for this Layer. \n
 * Possible values are\n
 *      #RD_LIB_MODE_EMVCO, \n
 *      #RD_LIB_MODE_NFC, \n
 *      #RD_LIB_MODE_ISO,\n
 * Default value is #RD_LIB_MODE_NFC.
 * */
#define PHPAL_I14443P3B_CONFIG_OPE_MODE             0x0001U

/**
 * Configure which command to be used for Type B Polling. \n
 * Possible values are\n
 *      #PHPAL_I14443P3B_USE_WUPB, \n
 *      #PHPAL_I14443P3B_USE_REQB, \n
 * Default value is #PHPAL_I14443P3B_USE_REQB.
 * */
#define PHPAL_I14443P3B_CONFIG_POLL_CMD             0x0002U

/**
 * Configure retry count for ATTRIB command.\n
 * Possible values are\n
 *      #PHPAL_I14443P3B_ATTRIB_RETRY_MIN, \n
 *      #PHPAL_I14443P3B_ATTRIB_RETRY_MAX, \n
 *
 * Default value is #PHPAL_I14443P3B_ATTRIB_RETRY_MAX.\n
 *
 * For mode #RD_LIB_MODE_EMVCO retry count should be 1.
 * */
#define PHPAL_I14443P3B_CONFIG_ATTRIB_RETRY_COUNT   0x0003U

/*@}*/
/**
* \name Poll Command
*/
/*@{*/
#define PHPAL_I14443P3B_USE_REQB            0x00U         /**< Indicate REQB command to be used for Type B Polling*/
#define PHPAL_I14443P3B_USE_WUPB            0x01U         /**< Indicate WakeupB command to be used for Type B Polling*/
/*@}*/

/**
* \name DRI/DSI Values
*/
/*@{*/
#define PHPAL_I14443P3B_DATARATE_106    0x00U   /**< DRI/DSI value for 106 kBit/s */
#define PHPAL_I14443P3B_DATARATE_212    0x01U   /**< DRI/DSI value for 212 kBit/s */
#define PHPAL_I14443P3B_DATARATE_424    0x02U   /**< DRI/DSI value for 424 kBit/s */
#define PHPAL_I14443P3B_DATARATE_848    0x03U   /**< DRI/DSI value for 848 kBit/s */
#define PHPAL_I14443P3B_DATARATE_FORCE  0x80U   /**< Flag that forces the use of the given baud rates */
/*@}*/

/**
* \name Attrib Command Retry Values
*/
/*@{*/
#define PHPAL_I14443P3B_ATTRIB_RETRY_MAX    1U   /**< Maximum retry value of ATTRIB during transmission and timeout error.*/
#define PHPAL_I14443P3B_ATTRIB_RETRY_MIN    0U   /**< Minimum retry value of ATTRIB during transmission and timeout error.*/
/*@}*/

/**
* \name Maximum allowed FSDI/FSCI Value
*/
/*@{*/
#define PHPAL_I14443P3B_FRAMESIZE_MAX       12U   /**< Maximum allowed FSDI/FSCI Value.*/
/*@}*/

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalI14443p3b/src/Sw/phpalI14443p3b_Sw.h"

#define phpalI14443p3b_SetConfig( pDataParams,  wConfig,  wValue) \
        phpalI14443p3b_Sw_SetConfig((phpalI14443p3b_Sw_DataParams_t *)pDataParams, wConfig, wValue)

#define phpalI14443p3b_GetConfig( pDataParams,   wConfig,  pValue ) \
        phpalI14443p3b_Sw_GetConfig((phpalI14443p3b_Sw_DataParams_t *)pDataParams, wConfig, pValue)

#define phpalI14443p3b_RequestB(  pDataParams, bNumSlots, bAfi, bExtAtqb,  pAtqb, pAtqbLen) \
        phpalI14443p3b_Sw_RequestB((phpalI14443p3b_Sw_DataParams_t *)pDataParams, bNumSlots, bAfi, bExtAtqb, pAtqb, pAtqbLen)

#define phpalI14443p3b_WakeUpB( pDataParams, bNumSlots, bAfi, bExtAtqb, pAtqb, pAtqbLen) \
        phpalI14443p3b_Sw_WakeUpB((phpalI14443p3b_Sw_DataParams_t *)pDataParams, bNumSlots, bAfi, bExtAtqb, pAtqb, pAtqbLen)

#define phpalI14443p3b_SlotMarker( pDataParams, bSlotNumber, pAtqb, pAtqbLen) \
        phpalI14443p3b_Sw_SlotMarker((phpalI14443p3b_Sw_DataParams_t *)pDataParams, bSlotNumber, pAtqb, pAtqbLen)

#define phpalI14443p3b_HaltB( pDataParams) \
        phpalI14443p3b_Sw_HaltB((phpalI14443p3b_Sw_DataParams_t *)pDataParams)

#define phpalI14443p3b_Attrib( pDataParams, pAtqb, bAtqbLen, bFsdi, bCid, bDri, bDsi, pMbli) \
        phpalI14443p3b_Sw_Attrib((phpalI14443p3b_Sw_DataParams_t *)pDataParams, pAtqb, bAtqbLen, bFsdi, bCid, bDri, bDsi, pMbli)

#define phpalI14443p3b_ActivateCard( pDataParams, pPupi, bPupiLength, bNumSlots, bAfi, \
                                     bExtAtqb, bFsdi, bCid, bDri, bDsi, pAtqb,  pAtqbLen, pMbli, pMoreCardsAvailable) \
        phpalI14443p3b_Sw_ActivateCard((phpalI14443p3b_Sw_DataParams_t *)pDataParams,pPupi, bPupiLength, bNumSlots,  bAfi, bExtAtqb, \
            bFsdi, bCid,  bDri,  bDsi, pAtqb, pAtqbLen,pMbli, pMoreCardsAvailable)

#define phpalI14443p3b_Exchange( pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength) \
        phpalI14443p3b_Sw_Exchange((phpalI14443p3b_Sw_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength)

#define phpalI14443p3b_GetSerialNo( pDataParams, pPupi) \
        phpalI14443p3b_Sw_GetSerialNo((phpalI14443p3b_Sw_DataParams_t *)pDataParams, pPupi)

#define phpalI14443p3b_SetSerialNo( pDataParams, pPupi) \
        phpalI14443p3b_Sw_SetSerialNo((phpalI14443p3b_Sw_DataParams_t *)pDataParams, pPupi)

#define phpalI14443p3b_SetHigherLayerInf( pDataParams, pTxBuffer,  wTxLength, pRxBuffer, wRxBufSize) \
        phpalI14443p3b_Sw_SetHigherLayerInf((phpalI14443p3b_Sw_DataParams_t *)pDataParams, pTxBuffer, wTxLength, pRxBuffer, wRxBufSize)

#define phpalI14443p3b_GetHigherLayerResp( pDataParams, ppRxBuffer, pRxLength) \
        phpalI14443p3b_Sw_GetHigherLayerResp((phpalI14443p3b_Sw_DataParams_t *)pDataParams, ppRxBuffer, pRxLength)

#define phpalI14443p3b_GetProtocolParams( pDataParams, pCidEnabled, pCid, pNadSupported, pFwi, pFsdi, pFsci) \
        phpalI14443p3b_Sw_GetProtocolParams((phpalI14443p3b_Sw_DataParams_t *)pDataParams, pCidEnabled, pCid, pNadSupported, pFwi, pFsdi, pFsci)

#else

/**
* \brief Set configuration value.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3b_SetConfig(
                                    void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                    uint16_t wConfig,   /**< [In] Configuration Identifier. */
                                    uint16_t wValue     /**< [In] Configuration Value. */
                                    );

/**
* \brief Get configuration value.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3b_GetConfig(
                                    void * pDataParams, /**< [In] Pointer to this layers parameter structure. */
                                    uint16_t wConfig,   /**< [In] Configuration Identifier. */
                                    uint16_t* pValue    /**< [Out] Configuration Value. */
                                    );

/**
* \brief Perform a ISO14443-3B Request command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3b_RequestB(
                                   void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                                   uint8_t bNumSlots,   /**< [In] Number of slots. */
                                   uint8_t bAfi,        /**< [In] AFI; Application Family Indentifier. */
                                   uint8_t bExtAtqb,    /**< [In] Enable Extended AtqB. */
                                   uint8_t * pAtqb,     /**< [Out] AtqB; uint8_t[13]. */
                                   uint8_t * pAtqbLen     /**< [Out] length of ATQB (12/13 bytes). */
                                   );

/**
* \brief Perform a ISO14443-3B Wakeup command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3b_WakeUpB(
                                  void * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                                  uint8_t bNumSlots,    /**< [In] Number of slots. */
                                  uint8_t bAfi,         /**< [In] AFI; Application Family Indentifier. */
                                  uint8_t bExtAtqb,     /**< [In] Enable Extended AtqB. */
                                  uint8_t * pAtqb,      /**< [Out] AtqB; uint8_t[12/13]. */
                                  uint8_t * pAtqbLen    /**< [Out] length of ATQB (12/13 bytes). */
                                  );

/**
* \brief Perform a ISO14443-3B Slot-MARKER command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3b_SlotMarker(
                                     void * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
                                     uint8_t bSlotNumber,   /**< [In] Slot Number. */
                                     uint8_t * pAtqb,       /**< [Out] AtqB; uint8_t[12/13]. */
                                     uint8_t * pAtqbLen     /**< [Out] length of ATQB (12/13 bytes). */
                                     );

/**
* \brief Perform a ISO14443-3B Halt command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3b_HaltB(
                                void * pDataParams  /**< [In] Pointer to this layers parameter structure. */
                                );

/**
* \brief Perform a ISO14443-3B Attrib command.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3b_Attrib(
                                 void * pDataParams,    /**< [In] Pointer to this layers parameter structure. */
                                 uint8_t * pAtqb,       /**< [In] Atqb from ReqB/WupB; uint8_t[12/13]. */
                                 uint8_t bAtqbLen,      /**< [In] Length of Atqb. */
                                 uint8_t bFsdi,         /**< [In] Frame Size Integer; 0-12. */
                                 uint8_t bCid,          /**< [In] Card Identifier; 0-14. */
                                 uint8_t bDri,          /**< [In] Divisor Receive (PCD to PICC) Integer; 0-3. */
                                 uint8_t bDsi,          /**< [In] Divisor Send (PICC to PCD) Integer; 0-3. */
                                 uint8_t * pMbli        /**< [Out] MBLI byte; uint8_t. */
                                 );

/**
* \brief Perform ISO14443-3B ReqB or WupB and Anticollision/Select commands for all cascade levels.
*
* If bLenUidIn is '0' --> CardUid not given, a ReqB is performed.
* If CardUid is given, a WupB is performed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3b_ActivateCard(
                                       void * pDataParams,              /**< [In] Pointer to this layers parameter structure. */
                                       uint8_t * pPupi,                 /**< [In] Pointer to the known PUPI; uint8_t[4]. */
                                       uint8_t bPupiLength,             /**< [In] Length of given PUPI, only a value of 0 or 4 is allowed. */
                                       uint8_t bNumSlots,               /**< [In] Number of slots. */
                                       uint8_t bAfi,                    /**< [In] AFI; Application Family Indentifier. */
                                       uint8_t bExtAtqb,                /**< [In] Enable Extended AtqB. */
                                       uint8_t bFsdi,                   /**< [In] Frame Size Integer; 0-12. */
                                       uint8_t bCid,                    /**< [In] Card Identifier; 0-14. */
                                       uint8_t bDri,                    /**< [In] Divisor Receive (PCD to PICC) Integer; 0-3. */
                                       uint8_t bDsi,                    /**< [In] Divisor Send (PICC to PCD) Integer; 0-3. */
                                       uint8_t * pAtqb,                 /**< [Out] AtqB; uint8_t[13]. */
                                       uint8_t * pAtqbLen,              /**< [Out] length of ATQB (12/13 bytes). */
                                       uint8_t * pMbli,                 /**< [Out] MBLI byte; uint8_t. */
                                       uint8_t * pMoreCardsAvailable    /**< [Out] Whether there are more cards in the field or not; uint8_t. */
                                       );

/**
* \brief Perform ISO14443-3B Data Exchange with Picc.
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
phStatus_t phpalI14443p3b_Exchange(
                                   void * pDataParams,      /**< [In] Pointer to this layers parameter structure. */
                                   uint16_t wOption,        /**< [In] Option parameter. */
                                   uint8_t * pTxBuffer,     /**< [In] Data to transmit. */
                                   uint16_t wTxLength,      /**< [In] Length of data to transmit. */
                                   uint8_t ** ppRxBuffer,   /**< [Out] Pointer to received data. */
                                   uint16_t * pRxLength     /**< [Out] number of received data bytes. */
                                   );

/**
* \brief Retrieve the serial number.
* This function will provide 4 byte PUPI.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION No PUPI available.
*/
phStatus_t phpalI14443p3b_GetSerialNo(
                                      void * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                                      uint8_t * pPupi       /**< [Out] Most recent PUPI. */
                                      );

/**
* \brief This function will update PUPI in this layer's parameter structure.
* This function must be called with valid PUPI.
* Once set the subsequent type B commands use this PUPI.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p3b_SetSerialNo(
                                      void * pDataParams,   /**< [In] Pointer to this layers parameter structure. */
                                      uint8_t * pPupi       /**< [In] 4 byte PUPI. */
                                      );

/**
* \brief Set higher layer information.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION Feature not available.
*/
phStatus_t phpalI14443p3b_SetHigherLayerInf(
    void * pDataParams,     /**< [In] Pointer to this layers parameter structure. */
    uint8_t * pTxBuffer,    /**< [In] Higher layer Information; Can be NULL if /c wTxLength is 0. */
    uint16_t  wTxLength,    /**< [In] Length of the Higher layer Information. Can be 0. */
    uint8_t * pRxBuffer,    /**< [In] Higher layer response buffer. */
    uint16_t wRxBufSize     /**< [In] Size of Higher layer response buffer. */
    );

/**
* \brief Get higher layer response.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION Feature not available.
*/
phStatus_t phpalI14443p3b_GetHigherLayerResp(
    void * pDataParams,     /**< [In] Pointer to this layers parameter structure. */
    uint8_t ** ppRxBuffer,  /**< [Out] Higher layer response. */
    uint16_t * pRxLength    /**< [Out] Length of Higher layer response. */
    );

/**
* \brief Retrieve the ISO14443-3B protocol parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p3b_GetProtocolParams(
    void * pDataParams,         /**< [In] Pointer to this layers parameter structure. */
    uint8_t * pCidEnabled,      /**< [Out] Unequal '0' if Card Identifier is enabled. */
    uint8_t * pCid,             /**< [Out] Card Identifier. */
    uint8_t * pNadSupported,    /**< [Out] Node Address Support; Unequal '0' if supported. */
    uint8_t * pFwi,             /**< [Out] Frame Waiting Integer. */
    uint8_t * pFsdi,            /**< [Out] PCD Frame Size Integer; 0-12. */
    uint8_t * pFsci             /**< [Out] PICC Frame Size Integer; 0-12. */
    );

/** @} */
#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_I14443P3B */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALI14443P3B_H */
