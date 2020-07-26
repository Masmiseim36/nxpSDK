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
* Generic ISO14443-3A Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHPALI14443P3A_H
#define PHPALI14443P3A_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef NXPBUILD__PHPAL_I14443P3A_SW

/** \defgroup phpalI14443p3a_Sw Component : Software
* @{
*/

#define PHPAL_I14443P3A_SW_ID       0x01U    /**< ID for Software ISO14443-3A layer. */

/**
* \brief Software PAL-ISO14443P3A parameter structure
*/
typedef struct
{
    uint16_t wId;           /**< Layer ID for this component, NEVER MODIFY! */
    void  * pHalDataParams; /**< Pointer to the parameter structure of the underlying layer. */
    uint8_t abUid[10];      /**< Array holding the activated UID. */
    uint8_t bUidLength;     /**< Length of UID stored in \c abUid. */
    uint8_t bUidComplete;   /**< Whether the UID is complete (\c 1) or not (\c 0). */
    uint8_t bOpeMode;       /**< Operation mode. One of NFC, EMVCo, ISO.*/
    uint8_t bPollCmd;       /**< Used to differentiate which command to send REQA or WakeUpA command for polling */
} phpalI14443p3a_Sw_DataParams_t;

/**
* \brief Initialise this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p3a_Sw_Init(
                                  phpalI14443p3a_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                  uint16_t wSizeOfDataParams,                   /**< [In] Specifies the size of the data parameter structure. */
                                  void * pHalDataParams                         /**< [In] Pointer to the parameter structure of the underlying layer. */
                                  );


/** @} */
#endif /* NXPBUILD__PHPAL_I14443P3A_SW */





#ifdef NXPBUILD__PHPAL_I14443P3A

/** \defgroup phpalI14443p3a ISO/IEC 14443-3A
* \brief These Components implement the ISO/IEC 14443-3 Type-A contactless protocol.
* @{
*/

/** Cascade Level 1 code for ISO14443-3A anticollision/select command. */
#define PHPAL_I14443P3A_CASCADE_LEVEL_1     0x93U

/** Cascade Level 2 code for ISO14443-3A anticollision/select command. */
#define PHPAL_I14443P3A_CASCADE_LEVEL_2     0x95U

/** Cascade Level 3 code for ISO14443-3A anticollision/select command. */
#define PHPAL_I14443P3A_CASCADE_LEVEL_3     0x97U

/** Timeout after card selection in milliseconds.  */
#define PHPAL_I14443P3A_TIMEOUT_DEFAULT_MS  10U

/**
* \name Poll Command
*/
/*@{*/

#define PHPAL_I14443P3A_USE_REQA            0x00U         /**< Indicate REQA command to be used for Type A Polling*/
#define PHPAL_I14443P3A_USE_WUPA            0x01U         /**< Indicate WakeupA command to be used for Type A Polling*/

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
#define PHPAL_I14443P3A_CONFIG_OPE_MODE             0x001U

/**
 * Configure which command to be used for Type A Polling. \n
 * Possible values are\n
 *      #PHPAL_I14443P3A_USE_REQA, \n
 *      #PHPAL_I14443P3A_USE_WUPA, \n
 * Default value is #PHPAL_I14443P3A_USE_REQA.
 * */
#define PHPAL_I14443P3A_CONFIG_POLL_CMD             0x002U

/**
 * Configure timeout in microseconds. This shall be used by AL layers to set
 * timeout for commands.
 * */
#define PHPAL_I14443P3A_CONFIG_TIMEOUT_VALUE_US     0x003U

/*@}*/

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalI14443p3a/src/Sw/phpalI14443p3a_Sw.h"

#define phpalI14443p3a_SetConfig( pDataParams, wConfig, wValue) \
        phpalI14443p3a_Sw_SetConfig((phpalI14443p3a_Sw_DataParams_t *) pDataParams, wConfig, wValue)

#define phpalI14443p3a_GetConfig( pDataParams, wConfig, pValue) \
        phpalI14443p3a_Sw_GetConfig((phpalI14443p3a_Sw_DataParams_t *) pDataParams, wConfig, pValue)

#define phpalI14443p3a_RequestA(  pDataParams,   pAtqa )\
        phpalI14443p3a_Sw_RequestA((phpalI14443p3a_Sw_DataParams_t *)pDataParams, pAtqa)

#define phpalI14443p3a_WakeUpA( pDataParams, pAtqa)\
        phpalI14443p3a_Sw_WakeUpA((phpalI14443p3a_Sw_DataParams_t *)pDataParams, pAtqa)

#define phpalI14443p3a_HaltA( pDataParams) \
        phpalI14443p3a_Sw_HaltA((phpalI14443p3a_Sw_DataParams_t *)pDataParams)

#define phpalI14443p3a_Anticollision(pDataParams, bCascadeLevel,  pUidIn, bNvbUidIn, pUidOut, pNvbUidOut) \
        phpalI14443p3a_Sw_Anticollision((phpalI14443p3a_Sw_DataParams_t *)pDataParams, bCascadeLevel, pUidIn, bNvbUidIn, pUidOut, pNvbUidOut)

#define phpalI14443p3a_Select( pDataParams, bCascadeLevel, pUidIn, pSak) \
        phpalI14443p3a_Sw_Select((phpalI14443p3a_Sw_DataParams_t *)pDataParams, bCascadeLevel, pUidIn, pSak)

#define phpalI14443p3a_ActivateCard( pDataParams, pUidIn, bLenUidIn, pUidOut, pLenUidOut, pSak, pMoreCardsAvailable) \
        phpalI14443p3a_Sw_ActivateCard((phpalI14443p3a_Sw_DataParams_t *)pDataParams, pUidIn, bLenUidIn, pUidOut, pLenUidOut, pSak, pMoreCardsAvailable)

#define phpalI14443p3a_Exchange( pDataParams, wOption,  pTxBuffer, wTxLength,  ppRxBuffer,  pRxLength) \
        phpalI14443p3a_Sw_Exchange((phpalI14443p3a_Sw_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, ppRxBuffer, pRxLength)

#define phpalI14443p3a_GetSerialNo( pDataParams, pUidOut, pLenUidOut) \
         phpalI14443p3a_Sw_GetSerialNo((phpalI14443p3a_Sw_DataParams_t *)pDataParams, pUidOut, pLenUidOut)

#else

/**
* \brief Set configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3a_SetConfig(
                                    void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                                    uint16_t wConfig,    /**< [In] Configuration Identifier. */
                                    uint16_t wValue      /**< [In] Configuration Value. */
                                   );

/**
* \brief Get configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3a_GetConfig(
                                    void * pDataParams,  /**< [In] Pointer to this layers parameter structure. */
                                    uint16_t wConfig,    /**< [In] Configuration Identifier. */
                                    uint16_t *pValue      /**<[Out] Configuration Value. */
                                   );

/**
* \brief Perform a ISO14443-3A Request command.
*
* \b Note: The "Request Guard Time" <em>(see 6.2.2, ISO/IEC 14443-3:2009(E))</em> is mandatory and is
* neither implemented here nor implemented in every \ref phhalHw layer.\n
* Make sure that either the used HAL or the used application does comply to this rule.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3a_RequestA(
                                   void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                   uint8_t * pAtqa      /**< [Out] AtqA; uint8_t[2]. */
                                   );

/**
* \brief Perform a ISO14443-3A Wakeup command.
*
* \b Note: The "Request Guard Time" <em>(see 6.2.2, ISO/IEC 14443-3:2009(E))</em> is mandatory and is
* neither implemented here nor implemented in every \ref phhalHw layer.\n
* Make sure that either the used HAL or the used application does comply to this rule.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3a_WakeUpA(
                                  void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                  uint8_t * pAtqa       /**< [Out] AtqA; uint8_t[2]. */
                                  );

/**
* \brief Perform a ISO14443-3A Halt command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3a_HaltA(
                                void * pDataParams  /**< [In] Pointer to this layer's parameter structure. */
                                );

/**
* \brief Perform a ISO14443-3A Anticollision or Select command.
*
* bNvbUidIn != 0x40: Perform Anticollision command.\n
* bNvbUidIn = 0x40: Perform Select command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3a_Anticollision(
                                        void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                        uint8_t bCascadeLevel,  /**< [In] cascade level code.  */
                                        uint8_t * pUidIn,       /**< [In] known Uid, can be NULL; uint8_t[0-4]. */
                                        uint8_t bNvbUidIn,      /**< [In] number of valid bits of UidIn. MSB codes the valid bytes, LSB codes the valid bits.  */
                                        uint8_t * pUidOut,      /**< [Out] complete Uid; uint8_t[4]. */
                                        uint8_t * pNvbUidOut    /**< [Out] number of valid bits of UidOut. MSB codes the valid bytes, LSB codes the valid bits.  */
                                        );

/**
* \brief Perform a ISO14443-3A Select command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3a_Select(
                                 void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                                 uint8_t bCascadeLevel, /**< [In] cascade level code. */
                                 uint8_t * pUidIn,      /**< [In] Uid; uint8_t[4]. */
                                 uint8_t * pSak         /**< [Out] Select Acknowledge; uint8_t. */
                                 );

/**
* \brief Perform ISO14443-3A ReqA or WupA and Anticollision/Select commands for all cascade levels.
*
* If bLenUidIn is '0' --> CardUid not given, a ReqA is performed.
* If CardUid is given, a WupA is performed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid response received.
* \retval #PH_ERR_FRAMING_ERROR Bcc invalid.
* \retval #PH_ERR_LENGTH_ERROR Given \c pUidIn is not complete.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p3a_ActivateCard(
                                       void * pDataParams,              /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t * pUidIn,                /**< [In] known Uid, can be NULL if bLenUidIn is 0; uint8_t[4/7/10]. */
                                       uint8_t bLenUidIn,               /**< [In] Length of known Uid; 0/4/7/10. */
                                       uint8_t * pUidOut,               /**< [Out] complete Uid; uint8_t[10]. */
                                       uint8_t * pLenUidOut,            /**< [Out] Length of Uid; 4/7/10. */
                                       uint8_t * pSak,                  /**< [Out] Select Acknowledge; uint8_t. */
                                       uint8_t * pMoreCardsAvailable    /**< [Out] Whether there are more cards in the field or not; uint8_t. */
                                       );

/**
* \brief Perform ISO14443-3A Data Exchange with Picc.
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
phStatus_t phpalI14443p3a_Exchange(
                                   void * pDataParams,       /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wOption,         /**< [In] Option parameter. */
                                   uint8_t * pTxBuffer,      /**< [In] Data to transmit. */
                                   uint16_t wTxLength,       /**< [In] Length of data to transmit. */
                                   uint8_t ** ppRxBuffer,    /**< [Out] Pointer to received data. */
                                   uint16_t * pRxLength      /**< [Out] number of received data bytes. */
                                   );

/**
* \brief Retrieve the serial number.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION No Serial number available at the moment.
*/
phStatus_t phpalI14443p3a_GetSerialNo(
                                      void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                      uint8_t * pUidOut,    /**< [Out] Last Uid; uint8_t[10]. */
                                      uint8_t * pLenUidOut  /**< [Out] Length of Uid; 4/7/10. */
                                      );

/** @} */
#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_I14443P3A */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALI14443P3A_H */
