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
* Generic ISO 14443p4mC component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 19. March 2014
*/

#ifndef PHPALI14443P4MC_H
#define PHPALI14443P4MC_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef NXPBUILD__PHPAL_I14443P4MC_SW

/** \defgroup phpalI14443p4mC_Sw Component : Software
* @{
*/

#define PHPAL_I14443P4MC_SW_ID                               0x01U        /**< ID for software I18092 MT component */

/**< Event posted by response time extension timer ISR. Reader library thread listens to this event and sends the RTOx packet. */
#ifndef E_PH_OSAL_EVT_RTO_TIMEOUT
#    define E_PH_OSAL_EVT_RTO_TIMEOUT         (1U << 3U)
#endif

/**
* \brief WTX timer call-back function type.
*/
typedef void(*pWtxTimerCallback)(void);

/**
* \brief ISO14443-4 card mode PAL parameter structure
*/
typedef struct
{
    uint16_t wId;                                /**< Layer ID for this component, NEVER MODIFY. */
    uint16_t wLastTxLen;                         /**< Length of last I block send. */
    void * pHalDataParams;                       /**< Pointer to the parameter structure of underlying hardware component. */
    phOsal_EventObj_t mcEventObj;
    uint8_t  bDisableWtx;                        /**< WTX Disabler (i.e. '0' if WTX enabled). */
    uint8_t  bWtx;                               /**< Frame Waiting Time Extension. */
    uint8_t  bFsdi;                              /**< FSDI parameter. */
    uint8_t  bFsci;                              /**< ATS FSCI parameter. */
    uint8_t  bDr;                                /**< ATS DR parameter. */
    uint8_t  bDs;                                /**< ATS DS parameter. */
    uint8_t  bUnequalDrDs;                       /**< DR and DS are different. */
    uint8_t  bFwi;                               /**< ATS FWI parameter. */
    uint8_t  bSfgi;                              /**< ATS SFGI parameter. */
    uint8_t  bCidEnable;                         /**< CID, Enabled = 1 or Disabled. */
    uint8_t  bCidPresence;                       /**< Flag to indicate CID presence in current command-response pair. */
    uint8_t  bCid;                               /**< CID parameter. */
    uint8_t  bNadEnable;                         /**< NAD, Enabled = 1 or Disabled. */
    uint8_t  bNadPresence;                       /**< Flag to indicate NAD presence in current command-response pair. */
    uint8_t  bNad;                               /**< NAD parameter. */
    uint8_t  bStateNow;                          /**< Current state. */
    uint8_t  bBlockNr;                           /**< Current block number. */
    uint8_t  bChainingRx;                        /**< We are chaining Rx. */
    uint8_t  bChainingTx;                        /**< We are chaining Tx. */
    uint8_t  bOpMode;                            /**< Operating mode - ISO or NFC. */
    uint16_t wWtDelta;                           /**< Holds the configured delta time to be subtracted from the time in WTX timer. */
    uint8_t  bWtPercentage;                      /**< Percentage of actual wait time to be used in WTX timer. */
    pWtxTimerCallback pWtxTimerCallback;         /**< Holds WTX timer call-back function address. */
}phpalI14443p4mC_Sw_DataParams_t;

/**
* \brief Initializes this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4mC_Sw_Init(
    phpalI14443p4mC_Sw_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
    void * pHalDataParams,                         /**< [In] Pointer to the parameter structure of the underlying layer.*/
    pWtxTimerCallback pWtxCallback                 /**< [In] Pointer to WTX timer call-back function */
    );

/** @} */
#endif  /* NXPBUILD__PHPAL_I14443P4MC_SW */

#ifdef NXPBUILD__PHPAL_I14443P4MC

/** \defgroup phpalI14443p4mC ISO/IEC 14443-4mC
* \brief This component implements the PICC side of ISO/IEC 14443-4 Protocol.
* @{
*/

/**
* \name ISO14443-4 card mode data rates
*/
/*@{*/
#define PHPAL_I14443P4MC_106                  0x00U           /**< Set communication speed to 106 kbs. */
#define PHPAL_I14443P4MC_212                  0x01U           /**< Set communication speed to 212 kbs. */
#define PHPAL_I14443P4MC_424                  0x02U           /**< Set communication speed to 424 kbs. */
#define PHPAL_I14443P4MC_848                  0x04U           /**< Set communication speed to 848 kbs. */
/** @} */

/**
* \name ISO14443-4 card mode states
*/
/*@{*/
#define PHPAL_I14443P4MC_STATE_NONE           0x00U          /**< Initialized State. */
#define PHPAL_I14443P4MC_STATE_ATS            0x01U          /**< State once we send successful ATS for RATS. */
#define PHPAL_I14443P4MC_STATE_PPS            0x02U          /**< State once we receive PPS Request. */
#define PHPAL_I14443P4MC_STATE_RX             0x03U          /**< Internal State once we are in Receive State. */
#define PHPAL_I14443P4MC_STATE_TX_I           0x04U          /**< State once we have received successful I (Information) Block. */
#define PHPAL_I14443P4MC_STATE_TX_R           0x05U          /**< State once we have received R Block. */
#define PHPAL_I14443P4MC_STATE_WTX            0x07U          /**< State once we have received S Block (WTX). */
#define PHPAL_I14443P4MC_STATE_DESELECT       0x08U          /**< State once we have received S Block (DESELECT). */
#define PHPAL_I14443P4MC_STATE_PARAMETERS     0x09U          /**< State once we have received S Block (PARAMETERS). */
#define PHPAL_I14443P4MC_STATE_RESEND         0x0AU          /**< State to handle errors, like in case of NAK. */
#define PHPAL_I14443P4MC_STATE_RXCHAINING     0x0BU          /**< State to handle RX Data Chaining. */
#define PHPAL_I14443P4MC_STATE_TXCHAINING     0x0CU          /**< State to handle TX Data Chaining. */
/** @} */

/**
* \name ISO14443-4 card mode configuration parameters
*/
/*@{*/
#define PHPAL_I14443P4MC_CONFIG_WTX           0x0000U        /**< Enable/Disable WTX feature. */
#define PHPAL_I14443P4MC_CONFIG_WTX_VALUE     0x0001U        /**< Set the WTX value. */
#define PHPAL_I14443P4MC_CONFIG_BLOCKNO       0x0002U        /**< Get the value of current ISO14443-4 protocol block number. */
#define PHPAL_I14443P4MC_CONFIG_FSDI          0x0003U        /**< Get the FSDI value. */
#define PHPAL_I14443P4MC_CONFIG_MODE          0x0004U        /**< Set/Get the operation mode (#RD_LIB_MODE_NFC or #RD_LIB_MODE_ISO). */
#define PHPAL_I14443P4MC_CONFIG_FSD           0x0005U        /**< Get the current frame size (FSD) value. */
#define PHPAL_I14443P4MC_CONFIG_WT_PERCENTAGE 0x0006U        /**< Set/Get the percentage of actual time to be used in WTX timer. */
#define PHPAL_I14443P4MC_CONFIG_WT_DELTA      0x0007U        /**< Set/Get the delta time to be subtracted from the time in WTX timer. */
#define PHPAL_I14443P4MC_CONFIG_CID_PRESENCE  0x0008U        /**< Check if CID is present in current received block. 1 - Present; 0 - Not present. */
#define PHPAL_I14443P4MC_CONFIG_NAD_PRESENCE  0x0009U        /**< Check if NAD is present in current received I-block. 1 - Present; 0 - Not present. */
/** @} */

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalI14443p4mC/src/Sw/phpalI14443p4mC_Sw.h"

#define phpalI14443p4mC_ResetProtocol( pDataParams) \
        phpalI14443p4mC_Sw_ResetProtocol((phpalI14443p4mC_Sw_DataParams_t *)pDataParams)

#define phpalI14443p4mC_Activate( pDataParams, pRats, wRatsLength, pAts, wAtsLength) \
        phpalI14443p4mC_Sw_Activate((phpalI14443p4mC_Sw_DataParams_t *)pDataParams, pRats, wRatsLength, pAts, wAtsLength)

#define phpalI14443p4mC_SetConfig( pDataParams, wConfig, wValue) \
        phpalI14443p4mC_Sw_SetConfig((phpalI14443p4mC_Sw_DataParams_t *)pDataParams, wConfig, wValue)

#define phpalI14443p4mC_GetConfig( pDataParams, wConfig,pValue) \
        phpalI14443p4mC_Sw_GetConfig((phpalI14443p4mC_Sw_DataParams_t *)pDataParams, wConfig, pValue)

#define phpalI14443p4mC_Receive( pDataParams, wOption, ppRxBuffer, pRxLength) \
        phpalI14443p4mC_Sw_Receive(( phpalI14443p4mC_Sw_DataParams_t *)pDataParams,  wOption, ppRxBuffer, pRxLength)

#define phpalI14443p4mC_Transmit( pDataParams, wOption,  pTxBuffer,  wTxLength) \
        phpalI14443p4mC_Sw_Transmit((phpalI14443p4mC_Sw_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength)

#define phpalI14443p4mC_Wtx( pDataParams ) \
        phpalI14443p4mC_Sw_Wtx((phpalI14443p4mC_Sw_DataParams_t *) pDataParams)

#else

/**
* \brief Reset protocol parameters.
*
* Removes any state information stored in previous runs.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI14443p4mC_ResetProtocol(
    void * pDataParams          /**< [In] Pointer to this layer's parameter structure. */
    );

/**
* \brief Validates received RATS request and send ATS response.
*
* The user has to call \ref phpalI14443p4mC_Receive() once this function has
* returned with success status.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_EXT_RF_ERROR External field turned OFF.
* \retval #PH_ERR_PROTOCOL_ERROR Protocol Error, RATS is invalid.
* \retval #PH_ERR_INVALID_PARAMETER Invalid input parameter, ATS is invalid.
* \retval Other depending on implementation and underlying components.
*/
phStatus_t phpalI14443p4mC_Activate(
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure */
    uint8_t * pRats,            /**< [In] Pointer to received RATS request */
    uint8_t  bRatsLength,       /**< [In] RATS Length*/
    uint8_t * pAts,             /**< [In] ATS to be sent */
    uint16_t  wAtsLength        /**< [In] ATS length */
    );

/**
* \brief Perform receive operation and validate received block.
*
* Used to receive the next command from PCD after the Activate() is successful.
* Responses to S-block and R-Block are sent internally. Response to I-Block can
* be sent using \ref phpalI14443p4mC_Transmit.
* This function also handles PPS request sent from the reader by sending
* a PPS response. An I-Frame received from the reader is returned back to the
* user.
* Handles DESELECT by sending DESELECT response
* Starts WTx timer before returning with the valid received packet to the caller
*
*
* \c wOption can be one of:\n
* \li #PH_RECEIVE_DEFAULT
* \li #PH_RECEIVE_CHAINING
* \li #PH_RECEIVE_CHAINING_BUFSIZE
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_SUCCESS_CHAINING RX chaining incomplete.
* \retval #PH_ERR_SUCCESS_DESELECTED Card got DESELECTED.
* \retval #PH_ERR_EXT_RF_ERROR External field turned OFF.
* \retval #PH_ERR_BUFFER_OVERFLOW Buffer overflow.
* \retval Other depending on implementation and underlying components.
*/
phStatus_t phpalI14443p4mC_Receive(
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,           /**< [In] Option parameter. */
    uint8_t ** ppRxBuffer,      /**< [Out] Pointer to received data. */
    uint16_t * pRxLength        /**< [Out] Number of received data bytes. */
    );

/**
* \brief Transmit I-Block block
*
* This function transmits an information block.
* Stops the WTx timer if it is running.
*
*
* \c wOption can be one of:\n
* \li #PH_TRANSMIT_DEFAULT
* \li #PH_TRANSMIT_BUFFER_FIRST
* \li #PH_TRANSMIT_BUFFER_CONT
* \li #PH_TRANSMIT_BUFFER_LAST
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_EXT_RF_ERROR External field turned OFF.
* \retval #PH_ERR_BUFFER_OVERFLOW buffer overflow.
* \retval Other depending on implementation and underlying components.
*/
phStatus_t phpalI14443p4mC_Transmit(
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wOption,           /**< [In] Option parameter. */
    uint8_t * pTxBuffer,        /**< [In] I-Block data to transmit. */
    uint16_t  wTxLength         /**< [In] Length of data to transmit. */
    );

/**
* \brief Sends WTX request and receive WTX response.
*
*  Wait time extension request is sent by this function and this will block
*  till it receives a response from the external reader.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_PROTOCOL_ERROR Protocol Error.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4mC_Wtx(
    void * pDataParams          /**< [In] Pointer to this layer's parameter structure */
    );

/**
* \brief Set configuration parameter.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phpalI14443p4mC_SetConfig(
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wConfig,           /**< [In] Configuration identifier. */
    uint16_t wValue             /**< [In] Configuration value. */
    );

/**
* \brief Get configuration parameter.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
phStatus_t phpalI14443p4mC_GetConfig(
    void * pDataParams,         /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wConfig,           /**< [In] Configuration identifier. */
    uint16_t *pValue            /**< [Out] Configuration value. */
    );
/** @} */
#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif/*NXPBUILD__PHPAL_I14443P4MC */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHPALI14443P4MC_H */
