/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic ISO18092 target mode Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 27. October 2014
*/

#ifndef PHPALI18092MT_H
#define PHPALI18092MT_H

#include <ph_Status.h>
#include <phhalHw.h>

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef NXPBUILD__PHPAL_I18092MT_SW

/** \defgroup phpalI18092mT_Sw Component : Software
* @{
*/

#define PHPAL_I18092MT_SW_ID                               0x01U        /**< ID for software I18092 MT component */
#define PHPAL_I18092MT_NFCID3_LENGTH                       0x0AU        /**< Length of NFCID3. */

/**< Event posted by response time extension timer ISR. Reader library thread listens to this event and sends the RTOx packet. */
#ifndef E_PH_OSAL_EVT_RTO_TIMEOUT
#    define E_PH_OSAL_EVT_RTO_TIMEOUT         (1U << 3U)
#endif


/** Typedef for the RTOX call back function */
typedef void(*pRtoxTimerCallback)(void);

/**
* \brief ISO18092 mode passive target parameter structure
*/
typedef struct
{
    uint16_t wId;                                                      /**< Layer ID for this component, NEVER MODIFY */
    void * pHalDataParams;                                             /**< Pointer to the parameter structure of underlying hardware component */
    void * ovrTask;                                                    /**< Task context. */
    pRtoxTimerCallback pRtoxCallback;                                  /**< Holds RTOX Callback function address */
    uint16_t wLastTxLen;                                               /**< Last successful DEP send length. */
    uint8_t aNfcid3i[PHPAL_I18092MT_NFCID3_LENGTH];                    /**< Array holding 10 bytes long NFCID3. */
    uint8_t aNfcid3t[PHPAL_I18092MT_NFCID3_LENGTH];                    /**< Array holding 10 bytes long NFCID3 of the PCD. */
    uint8_t bStateNow;                                                 /**< Current Exchange-State. */
    uint8_t bDid;                                                      /**< Device Identifier; '0' to disable usage else 1-14. */
    uint8_t bNadEnabled;                                               /**< Indicates presence of Node Address. 1 = present. */
    uint8_t bNadVal;                                                   /**< Node Address; Ignored if bNadEnabled is equal '0'. */
    uint8_t bFsl;                                                      /**< Frame Length Byte; 0-3. */
    uint8_t bPni;                                                      /**< Current Packet Number (Packet number information); 0-3. */
    uint8_t bDst;                                                      /**< Divisor Send Integer; 0-2. */
    uint8_t bDrt;                                                      /**< Divisor Receive Integer; 0-2. */
    uint8_t bBsi;                                                      /**< Send bit-rate supported by the PCD. */
    uint8_t bBri;                                                      /**< Receive bit-rate supported by the PCD. */
    uint8_t bLri;                                                      /**< Length reduction value is stored here. */
    uint8_t bBst;                                                      /**< Send bit-rate supported by the PCD. */
    uint8_t bBrt;                                                      /**< Receive bit-rate supported by the PCD. */
    uint8_t bLrt;                                                      /**< Length reduction value is stored here. */
    uint8_t bTo;                                                       /**< Hold the time out value. */
    uint8_t bRtoxDisabled;                                             /**< RTOX Disabler (i.e. '0' if RTOX enabled). */
    uint8_t bRtoxExtnVal;                                              /**< Response timeout extension value. */
    uint8_t bCmdtype;                                                  /**< Holds the type of Command send by target. */
    uint8_t bOpeMode;                                                  /**< Operation mode. Currently only NFC mode is supported and yet to support ISO mode.*/
    uint8_t bPropPSLMode;                                              /**< Enable/Disable proprietary PSL mode.*/
    phOsal_EventObj_t Pal18092mTEventObj;                              /**< Handle for Event. */
}phpalI18092mT_Sw_DataParams_t;

/**
* \brief Initialize this layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Wrong pointer to this layer's parameter structure is sent.
*/
phStatus_t phpalI18092mT_Sw_Init(
    phpalI18092mT_Sw_DataParams_t * pDataParams,                       /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wSizeOfDataParams,                                        /**< [In] Specifies the size of the data parameter structure. */
    void * pHalDataParams,                                             /**< [In] Pointer to the parameter structure of the underlying HAL. */
    pRtoxTimerCallback pRtoxCallback                                   /**< [In] Callback to be executed by the RTOX timer. Implementation provided by user. */
    );

/** @} */
#endif  /* NXPBUILD__PHPAL_I18092MT_SW */

#ifdef NXPBUILD__PHPAL_I18092MT

/** \defgroup phpalI18092mT ISO/IEC 18092 Mode Target
* \brief This Component implements the ISO/IEC ISO18092 Target mode contact-less protocol.
* Using this component it is possible to communicate with an active/passive Initiator.
* @{
*/
#define PHPAL_I18092MT_DID_MAX          0x0EU         /**< The last valid DID. */
#define PHPAL_I18092MT_WT_MAX           0x0EU         /**< The maximum allowed WT value, NFCForum-TS-DigitalProtocol-1.0 section 14.11. */
#define PHPAL_I18092MT_FSL_MAX          0x03U         /**< The maximum allowed FSL value. */
#define PHPAL_I18092MT_NAD_MAX          0x0FU         /**< The maximum allowed NAD value. */

#define PHPAL_I18092MT_MASK_NAD_ENABLE  0xFF00U       /**< Mask used for NAD Enabled/Disabled. */
#define PHPAL_I18092MT_MASK_NAD_VALUE   0x00FFU       /**< Mask used for NAD value. */
/**
* \name ISO18092 Target Mode Configuration Parameters
*/
/*@{*/
/**
* \brief Enable / Disable RTOX feature.
*/
#define PHPAL_I18092MT_CONFIG_RTOX                     0x0000U
/**
* \brief RTOX value.
*/
#define PHPAL_I18092MT_CONFIG_RTOX_VALUE               0x0001U
/**
* \brief Set / Get Node Address.
*
* \c wValue is parsed as follows:
* \verbatim
* NadEnabled = (wValue & 0xFF00U) ? 1 : 0;
* Nad = (wValue & 0x000FU);
* \endverbatim
*/
#define PHPAL_I18092MT_CONFIG_NAD                      0x0002U
/**
* \brief Get DID value. Indicates if DID is used by the PAL.
*/
#define PHPAL_I18092MT_CONFIG_DID                      0x0003U
/**
* \brief Get FSL value.
* Indicates either Lrt or Lri value whichever is lesser in case PSL is not exchanged.
* If PSL is exchanged then the agreed FSL value is returned.
*/
#define PHPAL_I18092MT_CONFIG_FSL                      0x0004U
/**
* \brief Get Time Out (TO) value used by the Target to communicate FWT.
*/
#define PHPAL_I18092MT_CONFIG_TO                       0x0005U

/*@}*/

/**
* \name ISO18092 Target Mode Parameters
*/
/*@{*/
/** Mask Bit Rates Supported by Initiator. */
#define PHPAL_I18092MT_BIT_RATE_MASK                   0xF0U

/** RTOX max value */
#define PHPAL_I18092MT_SW_MAX_RTOX_VALUE               0x3BU

/** RTOX min value */
#define PHPAL_I18092MT_SW_MIN_RTOX_VALUE               0x01U
/*@}*/

/**
* \name DRI/DSI (BRI/BSI) Values
*/
/*@{*/
#define PHPAL_I18092MT_DATARATE_106     0x0000U   /**< DRT(I)/DST(I) value for 106 kBit/s. */
#define PHPAL_I18092MT_DATARATE_212     0x0001U   /**< DRT(T)/DST(T) value for 212 kBit/s. */
#define PHPAL_I18092MT_DATARATE_424     0x0002U   /**< DRI(T)/DSI(T) value for 424 kBit/s. */
#define PHPAL_I18092MT_DATARATE_848     0x0003U   /**< DRI(T)/DSI(T) value for 848 kBit/s. */
/*@}*/

/**
* \name FSL/LRI Values
*/
/*@{*/
#define PHPAL_I18092MT_FRAMESIZE_64   0x00U   /**< FSL value for max. frame size of 64 Bytes.  */
#define PHPAL_I18092MT_FRAMESIZE_128  0x01U   /**< FSL value for max. frame size of 128 Bytes. */
#define PHPAL_I18092MT_FRAMESIZE_192  0x02U   /**< FSL value for max. frame size of 192 Bytes. */
#define PHPAL_I18092MT_FRAMESIZE_254  0x03U   /**< FSL value for max. frame size of 254 Bytes. */
/*@}*/

/**
* \name Maximum Values
*/
/*@{*/
#define PHPAL_I18092MT_MAX_GI_LENGTH   0x30U    /**< Maximum length of Gi data.  */
/*@}*/

#ifdef  NXPRDLIB_REM_GEN_INTFS
#include "../comps/phpalI18092mT/src/Sw/phpalI18092mT_Sw.h"

#define phpalI18092mT_ResetProtocol( pDataParams)\
        phpalI18092mT_Sw_ResetProtocol( (phpalI18092mT_Sw_DataParams_t *) pDataParams)

#define phpalI18092mT_Activate( pDataParams,  pAtrReq, wAtrReqLength, pAtrRes, wAtrResLength,  pGeneralDataBytes,wGeneralDataBytesLength) \
        phpalI18092mT_Sw_Activate( (phpalI18092mT_Sw_DataParams_t *) pDataParams,  pAtrReq, wAtrReqLength, pAtrRes, wAtrResLength,  pGeneralDataBytes,wGeneralDataBytesLength)

#define phpalI18092mT_Transmit( pDataParams, wOption, pTransportDataBytes, wTransportDataBytesLength) \
        phpalI18092mT_Sw_Transmit(( phpalI18092mT_Sw_DataParams_t *) pDataParams,wOption, pTransportDataBytes,wTransportDataBytesLength)


#define phpalI18092mT_Receive(pDataParams,  wOption, ppTransportDataBytes, pTransportDataBytesLength) \
        phpalI18092mT_Sw_Receive(( phpalI18092mT_Sw_DataParams_t *) pDataParams, wOption, ppTransportDataBytes, pTransportDataBytesLength )

#define phpalI18092mT_Rtox( pDataParams) \
        phpalI18092mT_Sw_Rtox((phpalI18092mT_Sw_DataParams_t *) pDataParams)

#define phpalI18092mT_SetConfig( pDataParams, wConfig, wValue) \
        phpalI18092mT_Sw_SetConfig((phpalI18092mT_Sw_DataParams_t *) pDataParams, wConfig, wValue)

#define phpalI18092mT_GetConfig( pDataParams, wConfig, pValue) \
        phpalI18092mT_Sw_GetConfig((phpalI18092mT_Sw_DataParams_t *) pDataParams, wConfig, pValue)

#else
/**
* \brief Reset the ISO18092 protocol parameters.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phpalI18092mT_ResetProtocol(
                                       void * pDataParams   /**< [In] Pointer to this layer's parameter structure. */
                                       );

/**
* \brief Handles ISO/IEC 18092 compliant activation sequence.
*
* This function takes the data returned by \ref phhalHw_Autocoll as a parameter, along with the ATR_RES.\n
* Validates the received ATR_REQ and sends the ATR_RES to PCD.
* If the ATR_REQ was received with error, then stays mute and waits for the valid ATR_REQ and then responds with ATR_RES.\n
* If General information bytes are present in the ATR_REQ then it will be returned back to the caller.\n
* ATR_RES format : byte 1-10:nfcid, byte 11:BSt, byte 12:BRt, byte13:TO, byte14:PPt, byte:15..n[Gt1..n].\n
*
* \retval #PH_ERR_SUCCESS Operation successful, target activated.
* \retval #PH_ERR_PROTOCOL_ERROR Received response is not ISO/IEC 18092 compliant.
* \retval #PH_ERR_EXT_RF_ERROR RF Turned OFF in passive communication mode
* \retval #PH_ERR_INVALID_PARAMETER Invalid parameter supplied.
* \retval Others from the underlying layer.
*/
phStatus_t phpalI18092mT_Activate(
    void * pDataParams,                                 /**< [In] Pointer to this layer's parameter structure. */
    uint8_t * pAtrReq,                                  /**< [In] Data received from Autocoll. Pointer to ATR_REQ. */
    uint16_t wAtrReqLength,                             /**< [In] Length of ATR_REQ. Including GT byte.*/
    uint8_t * pAtrRes,                                  /**< [In] Pointer to ATR_RES. Refer function description for the format.*/
    uint16_t wAtrResLength,                             /**< [In] Length of ATR_RES. */
    uint8_t ** pGeneralDataBytes,                       /**< [Out] Pointer to the general data bytes of ATR_REQ. */
    uint16_t *wGeneralDataBytesLength                   /**< [Out] Number of received general data bytes of ATR_REQ. */
);

/**
* \brief Receives ISO/IEC 18092 frame from Initiator.
*
* This will be the next call in sequence after the \ref phpalI18092mT_Activate.\n
* If the next frame is PSL_REQ it will be validated and handled in this function and this will send the PSL_RES else it will come
* out after validating the DEP_REQ received as the next frame.\n
* If it is a chained frame and more data is expected and RxBuffer is sufficiently large to hold the next chained frame, sends an
* ACK to receive the next chained frame i.e., it will keep on getting the next chunk of data after sending ACKs to the PCD and adding
* the chunks to the RxBuffer making it a contiguous set of data.\n
* If there is more data to be received but the RxBuffer cannot handle the next chained frame, returns to the caller with status
* PH_ERR_SUCCESS_CHAINING. In this case, the application or the higher layer has to copy the received buffer and call the phpalI18092_Receive
* again with wOption = PH_RECEIVE_CHAINING. In this case this function will send an ACK to the Initiator to start receiving the rest of the data.
* If the data received is DSL_REQ, then sends the DSL_RES and set the state to MIFARE product Halted state in case of Type A in passive communication
* mode or else will remain in receive mode till a WUP_REQ is received in active communication mode.\n
*
* This function will also handles the RLS_REQ and sends a RLS_RES
* In case of Active communication mode this function waits for WUP_REQ after DSL_REQ is received.
* If ISO 18092 RAW is activated, the RTOx timer will be started in this function before the data is handled over to the caller.
*
* \c wOption can be one of:\n
* \li #PH_RECEIVE_DEFAULT
* \li #PH_RECEIVE_CHAINING
* \li #PH_RECEIVE_CHAINING_BUFSIZE
*
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_SUCCESS_CHAINING Rx chaining not complete.
* \retval #PH_ERR_SUCCESS_ACK Received ACK for a previous Transmit and no valid data received in the OUT parameters.
* \retval #PH_ERR_SUCCESS_DESELECTED PCD has sent DSL_REQ.
* \retval #PH_ERR_SUCCESS_RELEASED PCD has sent RLS_REQ.
* \retval #PH_ERR_EXT_RF_ERROR External RF turned OFF in passive communication mode.
* \retval #PH_ERR_BUFFER_OVERFLOW Attempt to write beyond buffer size.
* \retval Others from the underlying layer.
*/
phStatus_t phpalI18092mT_Receive(
    void * pDataParams,                                 /**< [In] Pointer to this layer's parameter structure. */
    uint16_t   wOption,                                 /**< [In] Option parameter. */
    uint8_t ** ppTransportDataBytes,                    /**< [Out] Pointer to the received transport data bytes. */
    uint16_t * pTransportDataBytesLength                /**< [Out] Number of received transport data bytes. */
    );

/**
* \brief Sends a DEP_RES I-PDU to an ISO/IEC 18092 compliant Initiator.
*
* This is to be called by the application or the reader library thread to transmit a response to the PCD.\n
* This will internally call the HAL transmit API.\n
* If the pTxData length is bigger than the ISO 18092 Tx buffer (HAL Tx buffer), then this function will fragment and send this data to the PCD.\n
* In addition PH_TRANSMIT_TXCHAINING may be used to enable providing the TxData in chunks i.e., the application itself may have received
* the first chunk only from an external Host and it could receive more data from the Host in the next iteration. In such cases, this will be useful.
*
* \c wOption can be one of:\n
* \li #PH_TRANSMIT_DEFAULT
* \li #PH_TRANSMIT_TXCHAINING
*
* Additionally, the following options are also available:\n
* \li #PH_TRANSMIT_BUFFER_FIRST
* \li #PH_TRANSMIT_BUFFER_CONT
* \li #PH_TRANSMIT_BUFFER_LAST
*
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_EXT_RF_ERROR External RF turned OFF in passive communication mode.
* \retval Others from the underlying layer.
*/
phStatus_t phpalI18092mT_Transmit(
    void * pDataParams,                                 /**< [In] Pointer to this layer's parameter structure. */
    uint16_t   wOption,                                 /**< [In] Option parameter. */
    uint8_t * pTransportDataBytes,                      /**< [In] Transport Data Bytes to be transmitted as part of an DEP_REQ I-PDU. */
    uint16_t   wTransportDataBytesLength                /**< [In] Number of transport data bytes. */
    );


/**
* \brief Transmits an RTOX PDU to extend waiting time and receives RTOX response from Initiator.
*
* This is the API that is called by the application on expire of the RTO timer.\n
* This sends RTOx packet to the PCD and then waits for a valid response.\n
* If the received data is a NACK, then the RTOx packet is re-transmitted.\n
*
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_EXT_RF_ERROR RF Turned OFF in passive communication mode
* \retval Others from the underlying layer.
*/
phStatus_t phpalI18092mT_Rtox(
       void * pDataParams                                 /**< [In] Pointer to this layer's parameter structure. */
       );

/**
* \brief Set configuration parameter.
*
* \c wOption can be one of:\n
* \li #PHPAL_I18092MT_CONFIG_RTOX
* \li #PHPAL_I18092MT_CONFIG_RTOX_VALUE
* \li #PHPAL_I18092MT_CONFIG_NAD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Invalid parameter supplied.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER wrong wOption parameter supplied.
*/
phStatus_t phpalI18092mT_SetConfig(
                                    void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wConfig,    /**< [In] Configuration Identifier. */
                                    uint16_t wValue      /**< [In] Configuration Value. */
                                    );

/**
* \brief Get configuration parameter.
*
* \c wOption can be :\n
* \li #PHPAL_I18092MT_CONFIG_RTOX
* \li #PHPAL_I18092MT_CONFIG_NAD
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER wrong wOption parameter supplied.
*/
phStatus_t phpalI18092mT_GetConfig(
                                    void * pDataParams,  /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wConfig,    /**< [In] Configuration Identifier. */
                                    uint16_t * pValue    /**< [Out] Configuration Value. */
                                    );
/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHPAL_I18092MT */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif  /* PHPALI18092MT_H */
