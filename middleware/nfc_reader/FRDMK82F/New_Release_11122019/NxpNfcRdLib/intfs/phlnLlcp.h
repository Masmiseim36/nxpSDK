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
* Generic LLCP Link layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 18. Feb 2014
*
*/
#ifndef PHLNLLCP_H
#define PHLNLLCP_H

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phOsal.h>
#include "phTools.h"

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef NXPBUILD__PHLN_LLCP_SW

/** \defgroup phlnLlcp_Sw Component : Software
 * \brief Implementation of NFCForum Logical Link Control Protocol (LLCP) 1.2
* @{
*/

#define PHLN_LLCP_SW_ID      0x01U                                                            /**< ID for Software LLCP layer. */

/**< Event posted by Reader library thread to the application thread once LLCP is activated. */
#define E_PH_OSAL_EVT_LLCP_ACTIVATED      (1U << 6U)

/* *****************************************************************************************************************
* Types/Structure Declarations
* ***************************************************************************************************************** */

/**
* \brief Protocol Data Unit (PDU) type.
*/
#if 0
typedef enum /*phlnLlcp_PType*/{
    PHLN_LLCP_PTYPE_SYMM = 0x00,                                                             /**< Symmetry PDU. */
    PHLN_LLCP_PTYPE_PAX,                                                                     /**< Parameter Exchange concerning the LLCP Link configuration. */
    PHLN_LLCP_PTYPE_AGF,                                                                     /**< Aggregated Frame. */
    PHLN_LLCP_PTYPE_UI,                                                                      /**< Unnumbered Information to transfer service data units to the peer LLC. */
    PHLN_LLCP_PTYPE_CONNECT,                                                                 /**< CONNECT PDU is an unnumbered PDU which is used to request a data link connection. */
    PHLN_LLCP_PTYPE_DISC,                                                                    /**< Disconnect is an unnumbered PDU which is used to terminate a data link connection. */
    PHLN_LLCP_PTYPE_CC,                                                                      /**< Connection Complete is used by an LLC to acknowledge the receipt and acceptance of the CONNECT. */
    PHLN_LLCP_PTYPE_DM,                                                                      /**< Disconnected Mode is used to report status indicating that the LLC is logically disconnected. */
    PHLN_LLCP_PTYPE_FRMR,                                                                    /**< Frame Reject is used to report the receipt of a inappropriate PDU. */
    PHLN_LLCP_PTYPE_SNL,                                                                     /**< Service Name Lookup used to discover the availability of named services. */
    PHLN_LLCP_PTYPE_INFO = 0x0C,                                                             /**< Information is used to transfer service data units. */
    PHLN_LLCP_PTYPE_RR,                                                                      /**< Receive Ready to acknowledge one or more received I PDUs and indicate that the LLC is able to
                                                                                                  receive subsequent Information PDUs (I PDUs). */
    PHLN_LLCP_PTYPE_RNR                                                                      /**< Receive Not Ready is used by LLC to indicate a temporary inability to process subsequent I PDUs. */
} phlnLlcp_PType_t;
#endif

#define PHLN_LLCP_PTYPE_SYMM 	(uint8_t) 0x00                                              /**< Symmetry PDU. */
#define PHLN_LLCP_PTYPE_PAX    	(PHLN_LLCP_PTYPE_SYMM + 1U)                                	/**< Parameter Exchange concerning the LLCP Link configuration. */
#define PHLN_LLCP_PTYPE_AGF		(PHLN_LLCP_PTYPE_SYMM + 2U)                                 /**< Aggregated Frame. */
#define PHLN_LLCP_PTYPE_UI		(PHLN_LLCP_PTYPE_SYMM + 3U)                                 /**< Unnumbered Information to transfer service data units to the peer LLC. */
#define PHLN_LLCP_PTYPE_CONNECT	(PHLN_LLCP_PTYPE_SYMM + 4U)                                 /**< CONNECT PDU is an unnumbered PDU which is used to request a data link connection. */
#define PHLN_LLCP_PTYPE_DISC	(PHLN_LLCP_PTYPE_SYMM + 5U)                                 /**< Disconnect is an unnumbered PDU which is used to terminate a data link connection. */
#define PHLN_LLCP_PTYPE_CC		(PHLN_LLCP_PTYPE_SYMM + 6U)                                 /**< Connection Complete is used by an LLC to acknowledge the receipt and acceptance of the CONNECT. */
#define PHLN_LLCP_PTYPE_DM		(PHLN_LLCP_PTYPE_SYMM + 7U)                                 /**< Disconnected Mode is used to report status indicating that the LLC is logically disconnected. */
#define PHLN_LLCP_PTYPE_FRMR	(PHLN_LLCP_PTYPE_SYMM + 8U)                                 /**< Frame Reject is used to report the receipt of a inappropriate PDU. */
#define PHLN_LLCP_PTYPE_SNL		(PHLN_LLCP_PTYPE_SYMM + 9U )                                /**< Service Name Lookup used to discover the availability of named services. */
#define PHLN_LLCP_PTYPE_INFO 	(uint8_t) 0x0C                                              /**< Information is used to transfer service data units. */
#define PHLN_LLCP_PTYPE_RR		(PHLN_LLCP_PTYPE_INFO + 1U )                                /**< Receive Ready to acknowledge one or more received I PDUs and indicate that the LLC is able to
                                                                                              receive subsequent Information PDUs (I PDUs). */
#define PHLN_LLCP_PTYPE_RNR      (PHLN_LLCP_PTYPE_INFO + 2U )                               /**< Receive Not Ready is used by LLC to indicate a temporary inability to process subsequent I PDUs. */
/**
* \brief Socket Type.
*/
typedef enum /*phlnLlcp_Transport_Socket_Type*/{
    PHLN_LLCP_TRANSPORT_CONNECTIONLESS             = 0xA0,                                   /**< Connection-less communication */
    PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED,                                           /**< Connection-oriented Server */
    PHLN_LLCP_TRANSPORT_CLIENT_CONNECTIONORIENTED                                            /**< Connection-oriented Client */
} phlnLlcp_Transport_Socket_Type_t;

/**
* \brief Socket Sequence.
*/
typedef struct /*phlnLlcp_Socket_Seq*/ {
    uint8_t bSendState_Vs;                                                                   /**< Denote the sequence number of the next in-sequence I PDU to be sent. */
    uint8_t bSendAck_Vsa;                                                                    /**< Denote the most recently received N(R) value for a specific data link connection. */
    uint8_t bRxState_Vr;                                                                     /**< Denote the sequence number of the next in-sequence I PDU to be received. */
    uint8_t bRxAck_Vra;                                                                      /**< Denote the most recently sent N(R) value for a specific data link connection. */
} phlnLlcp_Socket_Seq_t;

/**
* \brief Socket structure of LLCP transport.
*/
typedef struct phlnLlcp_Transport_Socket{
    phOsal_SemObj_t                    xSema;                                                /**< Handle to the semaphore. */
    phOsal_SemObj_t                    xRxSema;                                              /**< Handle to the Receive semaphore. */
    phlnLlcp_Socket_Seq_t              sSeq;                                                 /**< Sequence of the I PDU. */
    struct phlnLlcp_Transport_Socket  *pNext;                                                /**< Pointer to the next Registered Socket. */
    uint8_t                           *pUri;                                                 /**< Pointer to URI (Uniform Resource Identifier). */
    uint32_t                           dwBufLen;                                             /**< The size of the Socket Rx Buffer. */
    uint8_t                           *pbRxBuffer;                                           /**< Pointer to the Socket Rx buffer. Size of the Socket Buffer should be less than or
                                                                                                  equal to 248 Bytes and less than HAL Rx Buffer.
                                                                                                  Note: To achieve better performance Socket Buffer Size should be multiple of 248. */
    uint32_t                           dwLength;                                             /**< Length of the data received. */
    uint16_t                           wRMiu;                                                /**< Remote Link's MIU. */
    /**
     * Ready to receive flag.
     * <b>Note</b>: This Flag needs to be set 'True' by the Application after each Socket Receive to receive next Packet as RW (Receive Window) is 1.
     */
    uint8_t                            fReady;
    phlnLlcp_Transport_Socket_Type_t   eSocketType;                                          /**< Based on this, I PDU (Connection-Oriented) or UI PDU (Connection-Less) will be sent. */
    phStatus_t                         wStatus;                                              /**< Determines the status of the Socket. */
    uint8_t                            bLsap;                                                /**< Local SAP (Service Access Point) address. */
    uint8_t                            bRsap;                                                /**< Remote SAP address. */
    uint8_t                            bState;                                               /**< Socket state can be as per <b>LLCP Socket States</b>. */
} phlnLlcp_Transport_Socket_t;

/**
* \brief Socket State of LLCP transport.
*/
typedef enum /*phlnLlcp_Transport_Socket_State*/{
    PHLN_LLCP_TRANSPORT_UNINITIALIZED = 0,
    PHLN_LLCP_TRANSPORT_INITIALIZED,
    PHLN_LLCP_TRANSPORT_REGISTERED
} phlnLlcp_Transport_Socket_State_t;

/**
* \brief Link Management parameters that are expected by LLC.
*/
typedef struct /*phlnLlcp_LMDataParams*/{
    uint8_t bVersion;                                                                        /**< Denotes major and minor release levels of the LLCP specification. */
    uint8_t bLto;                                                                            /**< Link Timeout (LTO) specifies the local link timeout interval guarantee. */
    uint8_t bOpt;                                                                            /**< Option. */
    uint16_t wMiu;                                                                           /**< maximum information unit (MIU) is max number of octets in information field of an LLC PDU. */
    uint16_t wWks;                                                                           /**< Well-Known Service List. */
    uint16_t bAvailableTlv;                                                                  /**< Bit mask for TLVs availability. */
} phlnLlcp_LMDataParams_t;

/**
* \brief LLCP parameter structure
*/
typedef struct /* phlnLlcp_Sw_DataParams */
{
    uint16_t                  wId;                                                           /**< Layer ID for this component, NEVER MODIFY!. */
    void                    * pPalI18092DataParams;                                          /**< Pointer to PAL 18092mPI/18092mT parameter structure. */
    phlnLlcp_LMDataParams_t   sLocalLMParams;                                                /**< Local General Bytes. */
    phlnLlcp_LMDataParams_t   sRemoteLMParams;                                               /**< Remote General Bytes. */
    uint8_t                   bAgreedVersion;                                                /**< Agreed Version number after Version Agreement. */
    uint8_t                   bCurrentClientCnt;                                             /**< Number of Registered Client count. */
    uint8_t                   bSdpClientSAP;                                                 /**< SAP Address of the Client requested for SDP. */
    uint8_t                   bMacType;                                                      /**< MAC type initialized either PHLN_LLCP_INITIATOR OR PHLN_LLCP_TARGET. */
    phOsal_EventObj_t LlcpEventObj;                            /**< Event Object. */
    phOsal_TimerObj_t LlcpSymTimerObj;
    phOsal_TimerObj_t LlcpLtoTimerObj;
    phOsal_SemObj_t xSema;

} phlnLlcp_Sw_DataParams_t;

/**
* \brief Initialize LLCP layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phlnLlcp_Sw_Init(
                            phlnLlcp_Sw_DataParams_t  *pDataParams,                          /**< [In] Pointer to this layer's parameter structure. */
                            uint16_t wSizeOfDataParams,                                      /**< [In] Specifies the size of the data parameter structure. */
                            uint8_t *pLocalGenBytes,                                         /**< [Out] Pointer to General data bytes buffer (Length of the buffer should be
                                                                                                        between min 6bytes and max 20bytes based on number the TLV to be framed). */
                            uint8_t *pLocalGenBytesLen                                       /**< [Out] Length of General data bytes. */
                            );

/** @} */
#endif /* NXPBUILD__PHLN_LLCP_SW */

#ifdef NXPBUILD__PHLN_LLCP


/** \defgroup phlnLlcp NFCForum-LLCP
* \brief This Component implements the NFCForum Logical Link Control Protocol (LLCP) version 1.2.
*
* <strong>Limitations:</strong>
*
* <strong>1.</strong> LLCP will not perform any handling of MAC (18092) chaining feature as this requires
* an additional buffer for LLCP to perform this task. LLCP will use MAC chaining feature using HAL
* receive buffer. To handle N times of MAC chaining using HAL Rx buffer, size of the HAL Rx buffer provided
* by the application should be N+1 times the Frame Size. So to increase MIU of LLCP, both socket buffer and
* HAL Rx buffer size needs to be increase proportionately.\n
* <em>Example:</em> If HAL Rx Buffer Size = 1024Bytes. Then MAC chaining of 3 Frames of 254 bytes (762Bytes)
* can be achieved with LLCP.
*
* <strong>2.</strong> In target mode, if remote peer sends CONNECT PDU immediately after ATR_RES is sent by
* RdLib Stack and if application LLCP Server task could not register socket before CONNECT PDU is received
* then DM PDU will be transmitted by RdLib LLCP Stack and connection will not be established with application
* LLCP Server. In this case either FreeRTOS timer task priority should be lower than any other task in system
* (which is defined in FreeRTOSConfig.h in FreeRTOS project) or the remote peer should send CONNECT PDU once
* again to establish connection with application LLCP Server.
*
* This layer follows a multi-threaded RTOS based design using one \ref phlnLlcp_Activate
* "reader library task" and one LLCP Server/Client application task.
* Reader Library Task calls \ref phlnLlcp_Activate API to establish Logical Link Connection on
* which Connection-Oriented or Connection-Less packets can be exchanged.
*
* A pictorial representation of Reader Library task and LLCP Server Application task
* used to communicate with Peer LLCP Client is as shown below.
*
* \image html LLCP_Server_Sequence.png
*
* A pictorial representation of Reader Library task and LLCP Client Application task
* used to communicate with Peer LLCP Server is as shown below.
*
* \image html LLCP_Client_Sequence.png
*
* @{
*/

/* *****************************************************************************************************************
* Types Declarations
* ***************************************************************************************************************** */
/**
* \name LLCP Socket States
*/
/*@{*/
#define PHLN_LLCP_SOCKET_WAITONCONN              0x01U                                        /**< Socket is blocked on new remote connection. */
#define PHLN_LLCP_SOCKET_CONN_PEND               0x02U                                        /**< Status indicates Connect PDU to be sent. */
#define PHLN_LLCP_SOCKET_DISC_PEND               0x04U                                        /**< Status indicates Disconnect PDU to be sent. */
#define PHLN_LLCP_SOCKET_DISC                    0x10U                                        /**< Received a Disconnect/Disconnect Mode(DM) PDU. */
#define PHLN_LLCP_SOCKET_INFO_EX                 0x20U                                        /**< Information Exchange. */
#define PHLN_LLCP_SOCKET_INFO_SEND_EX            0x40U                                        /**< Send Information and update after next Packet is received. */
#define PHLN_LLCP_SOCKET_SNL                     0x60U                                        /**< SNL Exchange. */
/*@}*/

/**
* \name LLCP specific error codes.
* \note Do not change the macro values.
*/

/*@{*/
/**
 * Status notified to the end point as a return code of \ref phlnLlcp_Transport_Socket_Receive.
 * This means the PDU destined to this SAP contained an invalid send sequence number N(S).
 */
#define PH_ERR_LLCP_PDU_TX_SEQ_ERR               0x81U
/**
 * Status notified to the end point as a return code of \ref phlnLlcp_Transport_Socket_Send.
 * This means the PDU destined to this SAP contained an invalid receive sequence number N(R).
 */
#define PH_ERR_LLCP_PDU_RX_SEQ_ERR               0x82U
/**
 * Status code returned by \ref phlnLlcp_Activate when LLCP is deactivated by calling \ref phlnLlcp_Deactivate
 * The phlnLlcp_Deactivate function can only be called from another task while LLCP flow is
 * active in one task.
 * This error is also returned to end points if they are blocked on any of link management operations like
 * Send, Receive, WaitForConnection, Connect.
 */
#define PH_ERR_LLCP_DEACTIVATED                  0x83U
/**
 * Status notified to the end point as a return code of \ref phlnLlcp_Transport_Socket_Receive.
 * This means the PDU destined to this SAP contained an incorrect or unexpected information field or
 * length of data received is more than socket MIU.
 */
#define PH_ERR_LLCP_PDU_INFO_ERR                 0x84U
/**
 * Returned by \ref phlnLlcp_Activate function when the peer sends DISC PDU over default LLCP Link Management
 * i.e., Local SAP = 0 and Remote SAP = 0.
 * In active mode during LLCP link de-activation if peer device did not respond or if there is an error from
 * lower layers then this error code is returned.
 * Since this happens during LLCP de-activation procedure the earlier data exchange/SNEP PUT has likely succeeded.
 */
#define PH_ERR_PEER_DISCONNECTED                 0x85U
/**
 * Returned by \ref phlnLlcp_Activate function when the peer sends Frame Reject (FRMR) PDU over default LLCP link management
 * i.e., Local SAP = 0 and Remote SAP = 0
 */
#define PH_ERR_LLCP_PDU_FRMR                     0x86U
/**
 * Status notified to the end point. This means the PDU destined to this SAP is not well formed (W Flag) or invalid.
 * This is returned by \ref phlnLlcp_Transport_Socket_Receive function.
 */
#define PH_ERR_LLCP_PDU_INVALID                  0x88U
/**
 * Status notified to the end point as a return code of \ref phlnLlcp_Transport_Socket_Receive, \ref phlnLlcp_Transport_Socket_Send,
 * \ref phlnLlcp_Transport_Socket_Connect. This means the remote peer rejected the PDU with an information field that is not permitted
 * for the PDU TYPE
 */
#define PH_ERR_LLCP_PDU_INFO_PER                 0x8CU

/**
* Status notified to the end point as a return code of \ref phlnLlcp_Transport_Socket_Register. This means a socket which is
* already registered at SAP manager is trying to register once again.
*/
#define PH_ERR_LLCP_SOCKET_REGISTERED            0x8DU

/**
* Status notified to the end point as a return code of \ref phlnLlcp_Transport_Socket_Unregister. This means a socket which is
* passed as an argument is not registered at SAP manager.
*/
#define PH_ERR_LLCP_SOCKET_NOT_REGISTERED        0x8EU

/**
* Status notified to the end point as a return code of \ref phlnLlcp_Transport_Socket_Register. This means all the available SAP that
* can be allocated for clients are exceeded (i.e. Max 31).
*/
#define PH_ERR_LLCP_SAP_EXCEEDED                 0x8FU

/**
* Status notified to the end point as a return code of \ref phlnLlcp_Transport_Socket_Register. This means LLCP is not active and socket
* cannot be registered with SAP manager.
*/
#define PH_ERR_LLCP_SOCKET_REGISTER_FAILED       0x87U
/*@}*/

#define PHLN_LLCP_MAGIC_NUM_BYTE1                0x46U                                        /**< LLCP Magic Byte-1. */
#define PHLN_LLCP_MAGIC_NUM_BYTE2                0x66U                                        /**< LLCP Magic Byte-2. */
#define PHLN_LLCP_MAGIC_NUM_BYTE3                0x6DU                                        /**< LLCP Magic Byte-3. */

/**
* \name Mandatory Byte length of ATR_REQ and ATR_RES
*/
/*@{*/
#define PHLN_LLCP_ATR_REQ_MIN_LEN                16U                                         /**< Used to locate General Bytes Start Position in the received ATR_REQ. */
#define PHLN_LLCP_ATR_RES_MIN_LEN                17U                                         /**< Used to locate General Bytes Start Position in the received ATR_RES. */
/*@}*/

/**
* \name Type-Length-Value (TLV) Parameters
* Below macros can be used by the Application developer to validate Type field which specifies the parameter type, the Length
* field which specifies the length of the value, and the Value field which contains the actual data.
*/
/*@{*/
#define PHLN_LLCP_TLV_TYPE_VERSION               0x01U                                        /**< Type Field value in Version TLV. */
#define PHLN_LLCP_TLV_TYPE_MIUX                  0x02U                                        /**< Type Field value in MIUX (Maximum Information Unit Extension) TLV. */
#define PHLN_LLCP_TLV_TYPE_WKS                   0x03U                                        /**< Type Field value in WKS (Well-Known Service List) TLV. */
#define PHLN_LLCP_TLV_TYPE_LTO                   0x04U                                        /**< Type Field value in LTO (Link Time-Out) TLV. */
#define PHLN_LLCP_TLV_TYPE_RW                    0x05U                                        /**< Type Field value in RW (Receive Window Size) TLV. */
#define PHLN_LLCP_TLV_TYPE_SN                    0x06U                                        /**< Type Field value in SN (Service Name) TLV. */
#define PHLN_LLCP_TLV_TYPE_OPT                   0x07U                                        /**< Type Field value in OPT (Option) TLV. */
#define PHLN_LLCP_TLV_TYPE_SDREQ                 0x08U                                        /**< Type Field value in SDREQ (Service Discovery Request) TLV. */
#define PHLN_LLCP_TLV_TYPE_SDRES                 0x09U                                        /**< Type Field value in SDRES (Service Discovery Response) TLV. */

#define PHLN_LLCP_TLV_LENGTH_VERSION             0x01U                                        /**< Length Field value in Version TLV. */
#define PHLN_LLCP_TLV_LENGTH_MIUX                0x02U                                        /**< Length Field value in MIUX Maximum Information Unit Extension) TLV. */
#define PHLN_LLCP_TLV_LENGTH_WKS                 0x02U                                        /**< Length Field value in WKS (Well-Known Service List) TLV. */
#define PHLN_LLCP_TLV_LENGTH_LTO                 0x01U                                        /**< Length Field value in LTO (Link Time-Out) TLV. */
#define PHLN_LLCP_TLV_LENGTH_RW                  0x01U                                        /**< Length Field value in RW (Receive Window Size) TLV. */
#define PHLN_LLCP_TLV_LENGTH_OPT                 0x01U                                        /**< Length Field value in OPT (Option) TLV. */
#define PHLN_LLCP_TLV_LENGTH_SDRES               0x02U                                        /**< Length Field value in SDRES TLV. */

#define PHLN_LLCP_TLV_MIUX_MASK                  ( 1U << 0x01U )                               /**< Mask used to set MIUX in 'bAvailableTlv' of \ref phlnLlcp_LMDataParams_t of Local Link. */
#define PHLN_LLCP_TLV_WKS_MASK                   ( 1U << 0x02U )                               /**< Mask used to set WKS in 'bAvailableTlv' of \ref phlnLlcp_LMDataParams_t of Local Link. */
#define PHLN_LLCP_TLV_LTO_MASK                   ( 1U << 0x03U )                               /**< Mask used to set LTO in 'bAvailableTlv' of \ref phlnLlcp_LMDataParams_t of Local Link. */
#define PHLN_LLCP_TLV_OPT_MASK                   ( 1U << 0x04U )                               /**< Mask used to set OPT in 'bAvailableTlv' of \ref phlnLlcp_LMDataParams_t of Local Link. */

#define PHLN_LLCP_SNL_SUPPORTED_VERSION_VALUE    0x11U                                        /**< SNL needs to be only supported if the agreed version number is v1.1 and greater then v1.1. */
#define PHLN_LLCP_TLV_VERSION_VALUE              0x12U                                        /**< Major version number is 1, Minor version number is 2. Indicating NFCForum LLPC 1.2 Specification. */
/*@}*/

/**
* \name Maximum Information Unit (MIU).
* Default value of MIU is 248 and HAL Rx Buffer should be approximately around 260bytes or more.
* This value is chosen for better Performance as MAC will support Frame size of 254Bytes.\n
* <b>Restriction:</b> NxpNfcRdlib cannot support MIU more than HAL Rx Buffer size.
*/
/*@{*/
#define PHLN_LLCP_MIU                            248U                                         /**< Maximum number of bytes in the information field of an LLC PDU that the local LLC is able to receive. */
#define PHLN_LLCP_TLV_MIUX_VALUE                 (PHLN_LLCP_MIU - 128U)                       /**< MIUX (Maximum Information Unit Extension) = Maximum Information Unit (MIU) - 128. */
/*@}*/

/**
* \name SYMM timeout as the percentage of LLCP link time out(LTO) / MAC time out(TO).
* User can change this value based on requirement.
* NOTE : High value will impact LLCP performance.
*/
#define PHLN_LLCP_SYMM_VALUE                     10U

/**
* \name Receive Window size of LLCP
* Receive window size of one indicates that the local LLC will acknowledge every I PDU before accepting additional I PDUs.
* This is fixed as per design and cannot be changed.
*/
#define PHLN_LLCP_TLV_RW_VALUE                   0x01U

/**
* \name LLCP Device Type.
* Indicates either NFC Forum Initiator OR Target.
* NOTE : DO NOT CHANGE THE MACRO AS IT IS TIED WITH LLCP LOGIC.
*/
/*@{*/
#define PHLN_LLCP_INITIATOR                        0U
#define PHLN_LLCP_TARGET                           1U
/*@}*/

/**
* \name LLCP De-Aggregation.
* Indicates whether LLCP stack supports dispatching the Aggregated LLC PDUs according to each PDUs destination and
* Source Service Access Point (SSAP) Address.
* If LLCP don't need to handle De-Aggregation then PHLN_LLCP_AGF flag can be set to zero. In which case code for De-Aggregation will not be complied.
*/
/*@{*/
#define PHLN_LLCP_AGF                              1U
/*@}*/

/**
* \name Disconnected Mode Reasons
* These are sent as part of Disconnected Mode (DM) PDU reason field. Refer LLCP1.2 specification section 4.3.8 table 4.
*/
/*@{*/
#define PHLN_LLCP_PDU_DM_DISC         0x00U /**< Indicates the Logical Link Control (LLC) has received a DISC PDU and is now logically disconnected from the data link connection. */
#define PHLN_LLCP_PDU_DM_NO_CONN      0x01U /**< Indicates the LLC has received a connection-oriented PDU but the target service access point has no active connection. */
#define PHLN_LLCP_PDU_DM_NO_SAP       0x02U /**< Indicates the remote LLC has received a CONNECT PDU and there is no service bound to the specified target service access point. */
#define PHLN_LLCP_PDU_DM_CONN_REJ     0x03U /**< Indicates the remote LLC has processed a CONNECT PDU and the request to connect was rejected by the service layer. */
#define PHLN_LLCP_PDU_DM_SAP_REJ      0x10U /**< Indicates the LLC will permanently not accept any CONNECT PDUs with the same target service access point address. */
#define PHLN_LLCP_PDU_DM_ANY_REJ      0x11U /**< Indicates the LLC will permanently not accept any CONNECT PDUs with any target service access point address. */
#define PHLN_LLCP_PDU_DM_TEMP_SAP_REJ 0x20U /**< Indicates the LLC will temporarily not accept any CONNECT PDUs with the specified target service access point address. */
#define PHLN_LLCP_PDU_DM_TEMP_ANY_REJ 0x21U /**< Indicates the LLC will temporarily not accept any CONNECT PDUs with any target service access point address. */
/*@}*/

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phlnLlcp/src/Sw/phlnLlcp_Sw.h"

#define phlnLlcp_Activate(pDataParams, pRemoteGnrlBytes, bRemoteGnrBytesLen, bDevType) \
        phlnLlcp_Sw_Activate((phlnLlcp_Sw_DataParams_t *)pDataParams, pRemoteGnrlBytes, bRemoteGnrBytesLen, bDevType)

#define phlnLlcp_Deactivate(pDataParams) \
        phlnLlcp_Sw_Deactivate((phlnLlcp_Sw_DataParams_t *)pDataParams)

#define phlnLlcp_WaitForActivation(pDataParams) \
        phlnLlcp_Sw_WaitForActivation((phlnLlcp_Sw_DataParams_t *)pDataParams)

#define phlnLlcp_Transport_Socket_Init(pDataParams, psSocket, eSocketType, pSocketRxBuffer, dwSocketRxBufferSize) \
        phlnLlcp_Sw_Transport_Socket_Init((phlnLlcp_Sw_DataParams_t *)pDataParams, psSocket, eSocketType, pSocketRxBuffer, dwSocketRxBufferSize)

#define phlnLlcp_Transport_Socket_Register(pDataParams, pSocket, eSocketType, bLsap, pUri) \
        phlnLlcp_Sw_Transport_Socket_Register((phlnLlcp_Sw_DataParams_t *)pDataParams, pSocket, eSocketType, bLsap, pUri)

#define phlnLlcp_Transport_Socket_WaitForConnection(pDataParams, pServerSocket, pConnServerSocket) \
        phlnLlcp_Sw_Transport_Socket_WaitForConnection((phlnLlcp_Sw_DataParams_t *)pDataParams,  pServerSocket, pConnServerSocket)

#define phlnLlcp_Transport_Socket_Connect( pDataParams, pClientSocket,  bDsap) \
        phlnLlcp_Sw_Transport_Socket_Connect((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket,  bDsap)

#define phlnLlcp_Transport_Socket_ConnectViaUri( pDataParams, pClientSocket,  pUri) \
        phlnLlcp_Sw_Transport_Socket_ConnectViaUri((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket,  pUri)

#define phlnLlcp_Transport_Socket_Receive( pDataParams, psSocket) \
        phlnLlcp_Sw_Transport_Socket_Receive((phlnLlcp_Sw_DataParams_t *)pDataParams, psSocket)

#define phlnLlcp_Transport_Socket_Send(pDataParams, pClientSocket, pTxBuffer, dwTxBufferSize, wFrameOpt) \
        phlnLlcp_Sw_Transport_Socket_Send((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket, pTxBuffer, dwTxBufferSize, wFrameOpt)

#define phlnLlcp_Transport_Socket_DiscoverServices(pDataParams, pSocket, bNumber, bSNBuffSize, psServiceNameList, pSapList) \
        phlnLlcp_Sw_Transport_Socket_DiscoverServices((phlnLlcp_Sw_DataParams_t *)pDataParams, pSocket, bNumber, bSNBuffSize, psServiceNameList, pSapList)

#define phlnLlcp_Transport_Socket_SendTo(pDataParams, pClientSocket, bDsap, pTxBuffer, dwTxBufferSize, wFrameOpt) \
        phlnLlcp_Sw_Transport_Socket_SendTo((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket, bDsap, pTxBuffer, dwTxBufferSize, wFrameOpt)

#define phlnLlcp_Transport_Socket_Disconnect(pDataParams, pClientSocket) \
        phlnLlcp_Sw_Transport_Socket_Disconnect((phlnLlcp_Sw_DataParams_t *)pDataParams, pClientSocket)

#define phlnLlcp_Transport_Socket_Unregister(pDataParams, pSocket) \
        phlnLlcp_Sw_Transport_Socket_Unregister((phlnLlcp_Sw_DataParams_t *)pDataParams, pSocket)

#define phlnLlcp_DeInit(pDataParams) \
        phlnLlcp_Sw_DeInit((phlnLlcp_Sw_DataParams_t *)pDataParams)
#else

/**
* \brief Activates the LLCP layer link by checking General Bytes content of ATR_REQ/ATR_RES.
* Posts LLCP Activation event to threads blocked for this event in a call to \ref phlnLlcp_WaitForActivation API.\n
* Then this API starts processing the Message Queue and signals corresponding Semaphores in the \ref phlnLlcp_Transport_Socket_t.\n
* socket structure to Threads blocking on specific events to occur.\n
* Runs until:\n
* 1. Another thread calls \ref phlnLlcp_Deactivate.\n
* 2. LLCP connection is closed by the other peer using DISC PDU.\n
* 3. LTO (Link Timeout) occurs due to peer device not in range/responding.\n
* 4. External RF Field OFF occurred if activated in Passive Target Mode.\n
* 5. Another thread calls \ref phhalHw_AsyncAbort.\n
* 6. Any exit scenario or error from below MAC layer.\n
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_Sw_Init().\n
*
* Note: If FRMR is received on DSAP and SSAP equal to 0 or an invalid PDU is received on DSAP and SSAP equal to zero, LLCP link
* (DSSAP and SSAP equal to 0) is disconnected and peer need to re-establish the connection as per NFC Forum LLCP Specification 1.2, sec 5.6.4.5.
*
* \return Status code
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     MAC restrictions are not obeyed (Example, DID is set in ATR_REQ).
* \retval #PH_ERR_PROTOCOL_ERROR        Invalid TLV/Version mismatch/bad contents in ATR or Peer device didn't handle MAC restrictions.
* \retval #PH_ERR_PEER_DISCONNECTED     Peer Sent DISC_PDU(LSap=0,DSap=0).
* \retval #PH_ERR_LLCP_DEACTIVATED      Another task/thread calls \ref phlnLlcp_Deactivate.
* \retval #PH_ERR_RESOURCE_ERROR        Message Queue is unavailable or OSAL returned error.
* \retval #PH_ERR_LLCP_PDU_FRMR         Received FRMR on LSAP = 0 and RSAP = 0 received from remote peer,
*                                       MAC link de-activation procedure will not be done by LLCP.
* \retval #PH_ERR_LLCP_PDU_INVALID      Received Invalid PDU over LSAP = 0 and RSAP = 0 or received AGF on LSAP and RSAP other than zero,
*                                       MAC link de-activation procedure will not be done by LLCP.
* \retval #PH_ERR_EXT_RF_ERROR          External RF Field OFF from HAL.
* \retval #PH_ERR_ABORTED               Peer Device did not respond for LTO time or when User aborts by calling \ref phhalHw_AsyncAbort. This code is pre-pended with HAL Component code.
* \retval #PH_ERR_USE_CONDITION         If underlying MAC Components are not part of Build.
* \retval Other                         Depending on underlying component.
*/
phStatus_t phlnLlcp_Activate(
                             void * pDataParams,                                                       /**< [In] Pointer to this layer's parameter structure. */
                             uint8_t * pRemoteGnrlBytes,                                               /**< [In] Pointer to remote general bytes received in ATR REQ/RES. */
                             uint8_t bRemoteGnrBytesLen,                                               /**< [In] Length of general bytes received in ATR REQ/RES. */
                             uint8_t bDevType                                                          /**< [In] Device type PHLN_LLCP_INITIATOR or PHLN_LLCP_TARGET. */
                             );

/**
* \brief Deactivate the LLCP layer link.
* This API should be called from different thread which will post DISC PDU (with LSAP = 0 and RSAP = 0) to Message Queue.\n
* This message is dequeued by the \ref phlnLlcp_Activate API which is running in the Reader Library Thread.\n
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_RESOURCE_ERROR        Message Queue is unavailable or OSAL returned error.
*/
phStatus_t phlnLlcp_Deactivate(
                               void * pDataParams                                                      /**< [In] Pointer to this layer's parameter structure. */
                               );

/**
* \brief Wait for LLCP Activation Event from Reader Library Thread.
* This should be the first API called by Server/Client Thread before calling any other API. This will wait on LLCP Activated Event which will\n
* be posted by the \ref phlnLlcp_Activate API once LLCP Link is established after Link Activation Procedure as defined in NFCForum LLCP 1.2 specification section 5.2. \n
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_IO_TIMEOUT            No LLCP Activate Event detected and Timeout occurred.
* \retval #PH_ERR_RESOURCE_ERROR        Message Queue is unavailable or OSAL returned error.
*/
phStatus_t phlnLlcp_WaitForActivation(
                                      void * pDataParams                                               /**< [In] Pointer to this layer's parameter structure. */
                                      );

/**
* \brief Initializes a new socket.\n
* Socket type is one of:
* - PHLN_LLCP_TRANSPORT_CONNECTIONLESS\n
* - PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED\n
* - PHLN_LLCP_TRANSPORT_CLIENT_CONNECTIONORIENTED\n
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_RESOURCE_ERROR        Message Queue is unavailable or OSAL returned error.
*/
phStatus_t phlnLlcp_Transport_Socket_Init(
                                          void * pDataParams,                                          /**< [In] Pointer to this layer's parameter structure. */
                                          phlnLlcp_Transport_Socket_t * psSocket,                      /**< [In] Pointer to Socket allocated by the Application. */
                                          phlnLlcp_Transport_Socket_Type_t eSocketType,                /**< [In] Type of Socket to be Initialized, indicating the End Point. */
                                          uint8_t * pSocketRxBuffer,                                   /**< [In] Pointer to Rx Buffer allocated by the Application to corresponding Socket. */
                                          uint32_t dwSocketRxBufferSize                                /**< [In] Length of the Rx Buffer. */
                                          );

/**
* \brief Register a socket at the SAP manager.\n
* If Socket type is Connection-oriented server, bLsap and/or pUri have to be provided.\n
* If Socket type is Connection-oriented client, ignores bLsap and pUri parameter but a random SAP will be generated and assigned to the Socket.\n
* If Socket type is Connection-less, bLsap and/or pUri parameters have to be provided. \n
* If this API is called before event is posted by \ref phlnLlcp_Activate or while main LLCP task is performing \ref phlnLlcp_DeInit,
* then #PH_ERR_LLCP_SOCKET_REGISTER_FAILED will be returned.
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init()\n
*
* <b>NOTE</b>: bLsap value should not be '0' or '1' as these are reserved SAP used internally by LLCP.
*
* \return Status code
* \retval #PH_ERR_SUCCESS                     Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS         Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER           Invalid input parameters.
* \retval #PH_ERR_LLCP_SAP_EXCEEDED           Exceeded available number of Client SAPs that can be registered.
* \retval #PH_ERR_LLCP_SOCKET_REGISTERED      Socket is already registered with SAP Manager.
* \retval #PH_ERR_LLCP_SOCKET_REGISTER_FAILED Socket cannot be registered as LLCP is not active or some other registered socket is associated with the passed Lsap value.
*/
phStatus_t phlnLlcp_Transport_Socket_Register(
                                              void * pDataParams,                                      /**< [In] Pointer to this layer's parameter structure. */
                                              phlnLlcp_Transport_Socket_t* pSocket,                    /**< [In] Pointer of Socket to be Registered. */
                                              phlnLlcp_Transport_Socket_Type_t eSocketType,            /**< [In] Type Socket to be used. */
                                              uint8_t bLsap,                                           /**< [In] Local SAP address. */
                                              uint8_t* pUri                                            /**< [In] Pointer of Uniform Resource Identifier (URI).
                                                                                                                 NULL terminated URI string should be provided. */
                                              );

/**
* \brief WaitForConnection is a Blocking call used by a Connection-oriented Server socket waiting for an incoming client connection on the registered pServerSocket.
* pConnServerSocket is an Initialized but not Registered socket. It will be filled with the remote/connected Client information once a remote socket has established a connection.\n
* pConnServerSocket socket has to be used then for further communication (send/receive).\n
*
* <em>NOTE:</em> If pConnServerSocket is NULL then pServerSocket is used for further communication with the connected Remote Client and no further clients can establish connection with the Server.\n
* If pConnServerSocket is a valid Initialized socket then once a connection with Remote Client is established, a new thread needs to be created by the Application to continue\n
* communication with the Connected Remote Client and the current thread can listen for a new Client Connection over next Initialized pConnServerSocket socket.\n
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init() with pServerSocket\n
* 3. phlnLlcp_Transport_Socket_Register() with pServerSocket\n
* 4. phlnLlcp_Transport_Socket_Init() with pConnServerSocket\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
*/
phStatus_t phlnLlcp_Transport_Socket_WaitForConnection(
                                                       void   *pDataParams,                            /**< [In] Pointer to this layer's parameter structure. */
                                                       phlnLlcp_Transport_Socket_t* pServerSocket,     /**< [In] Pointer of Server socket. */
                                                       phlnLlcp_Transport_Socket_t* pConnServerSocket  /**< [In] Pointer of Connection Server socket. */
                                                       );

/**
* \brief Connect is a Blocking call used by a Connection-oriented client socket to connect to a remote server with a DSAP given with bDsap.
* This API will post Connect PDU to Message Queue, \ref phlnLlcp_Activate API will send this PDU to remote peer.\n
* Waits until CC PDU is received and Semaphore is signaled by the \ref phlnLlcp_Activate.\n
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init() with pClientSocket\n
* 3. phlnLlcp_Transport_Socket_Register() with pClientSocket\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
* \retval #PH_ERR_LLCP_PDU_INFO_PER     PDU with an information field that is not permitted.
* \retval #PH_ERR_LLCP_DEACTIVATED      Connect was not successful, received DM PDU as selected server using Destination SAP does not exist on Remote Peer.
*/
phStatus_t phlnLlcp_Transport_Socket_Connect(
                                             void  *pDataParams,                                       /**< [In] Pointer to this layer's parameter structure. */
                                             phlnLlcp_Transport_Socket_t* pClientSocket,               /**< [In] Pointer of Client socket. */
                                             uint8_t bDsap                                             /**< [In] Address of Destination SAP to which Client has to connect. */
                                             );

/**
* \brief ConnectViaUri is a Blocking call used by a Connection-oriented client socket to connect to a remote server with a Uniform Resource Identifier (URI) given with pUri.
* This API will post Connect PDU to Message Queue, \ref phlnLlcp_Activate API will send this PDU to remote peer.\n
* Waits until CC PDU is received and Semaphore is signaled by the \ref phlnLlcp_Activate.\n
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init() with pClientSocket\n
* 3. phlnLlcp_Transport_Socket_Register() with pClientSocket\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
* \retval #PH_ERR_LLCP_PDU_INFO_PER     PDU with an information field that is not permitted.
* \retval #PH_ERR_LLCP_DEACTIVATED      Connect was not successful, received DM PDU as selected server using Destination SAP does not exist on Remote Peer.
*/
phStatus_t phlnLlcp_Transport_Socket_ConnectViaUri(
                                                   void * pDataParams,                                 /**< [In] Pointer to this layer's parameter structure. */
                                                   phlnLlcp_Transport_Socket_t * pClientSocket,        /**< [In] Pointer of Client socket. */
                                                   uint8_t * pUri                                      /**< [In] Pointer to URI of Destination SAP to which Client has to connect.
                                                                                                                 NULL terminated URI string should be provided. */
                                                   );

/**
* \brief Receive is a Blocking call used to receive data on a socket.
* Can be used by any socket type. Function blocks until dwRxBufferSize bytes have been received or an error occurred.\n
* Once Data is received on the Socket, User need to set 'fReady' flag of the respective Socket to 'True' in-order to receive next Packet.
* If the fReady flag is not True then the Link Management will send Receiver Not Ready (RNR) PDU to the remote SAP if it receives an Information
* PDU for this end point.
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init() with pServerSocket\n
* 3. phlnLlcp_Transport_Socket_Register() with pServerSocket\n
* 4. phlnLlcp_Transport_Socket_Init() with pConnServerSocket\n
* 5. phlnLlcp_Transport_Socket_WaitForConnection()
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
* \retval #PH_ERR_PEER_DISCONNECTED     Peer sent DISC PDU over Socket Link.
* \retval #PH_ERR_LLCP_DEACTIVATED      Local LLC shut down.
* \retval #PH_ERR_LLCP_PDU_TX_SEQ_ERR   Received Information PDU (I-PDU) with Sequence error.
* \retval #PH_ERR_LLCP_PDU_INFO_ERR     Received Information length is greater than MIU.
* \retval #PH_ERR_LLCP_PDU_INVALID      Invalid PDU received.
* \retval #PH_ERR_LLCP_PDU_INFO_PER     PDU with an information field that is not permitted.
*/
phStatus_t phlnLlcp_Transport_Socket_Receive(
                                             void  *pDataParams,                                       /**< [In] Pointer to this layer's parameter structure. */
                                             phlnLlcp_Transport_Socket_t* psSocket                     /**< [In] Pointer of the socket. */
                                             );


/**
* \brief Send is a Blocking call used to send data on a Connection-oriented socket.
* Function blocks until complete buffer has been sent out and a RR PDU is received by \ref phlnLlcp_Activate which will signal the Semaphore.\n
* \b wOption can be one of:\n
* \li #PH_TRANSMIT_DEFAULT
*
* \b wOption can be combined with:\n
* \li #PH_TRANSMIT_BUFFERED_BIT
* \li #PH_TRANSMIT_LEAVE_BUFFER_BIT
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init()\n
* 3. phlnLlcp_Transport_Socket_Register()\n
* 4. phlnLlcp_Transport_Socket_Connect()\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_SUCCESS_INFO_RECEIVED Received I-PDU in response to I-PDU. \ref phlnLlcp_Transport_Socket_Receive need
*                                       not be called to Receive next Packet as Socket Buffer already contains the received Data.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_UNSUPPORTED_COMMAND   In-case Socket is other than #PHLN_LLCP_TRANSPORT_SERVER_CONNECTIONORIENTED or #PHLN_LLCP_TRANSPORT_CLIENT_CONNECTIONORIENTED Socket Type.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
* \retval #PH_ERR_TX_NAK_ERROR          Received RNR PDU as Remote LLCP is busy. Application can Re-send once again if required.
* \retval #PH_ERR_PEER_DISCONNECTED     Received DISC PDU from the Remote LLCP.
* \retval #PH_ERR_LLCP_DEACTIVATED      Local LLC shut down.
* \retval #PH_ERR_LLCP_PDU_RX_SEQ_ERR   Received RR/RNR PDU with sequence error.
* \retval #PH_ERR_LLCP_PDU_INFO_PER     PDU with an information field that is not permitted.
*/
phStatus_t phlnLlcp_Transport_Socket_Send(
                                          void * pDataParams,                                          /**< [In] Pointer to this layer's parameter structure. */
                                          phlnLlcp_Transport_Socket_t * pClientSocket,                 /**< [In] Pointer of Client Socket. */
                                          uint8_t * pTxBuffer,                                         /**< [In] Pointer of Buffer to be Transmitted. */
                                          uint32_t dwTxBufferSize,                                     /**< [In] Length of the Buffer to be Transmitted. */
                                          uint16_t wFrameOpt                                           /**< [In] Option parameter. */
                                          );

/**
* \brief Get SAP of remote services using their URIs (Uniform Resource Identifier).
* This function sends SDP queries to the remote peer to get the SAP address for a given service name.
* The queries are sent in a single LLC packet. If list of URI's cannot fit into single packet then this API needs to be called multiple times.
* Only one task can call this API at a time. If this API is called by different tasks simultaneously then #PH_ERR_USE_CONDITION error will be returned.
* As mentioned in LLCP specification, a SAP of 0 means that the service name has not been found which is indicated in the parameter pnSapList.
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init()\n
* 3. phlnLlcp_Transport_Socket_Register()\n
* 4. phlnLlcp_Transport_Socket_DiscoverServices()\n
* 5. phlnLlcp_Transport_Socket_Connect() can be called with the Discovery SAP.
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
* \retval #PH_ERR_PEER_DISCONNECTED     Received DISC PDU from the Remote LLCP.
* \retval #PH_ERR_LLCP_DEACTIVATED      Local LLC shut down.
* \retval #PH_ERR_USE_CONDITION         More than one client has simultaneously called this API.
*/
phStatus_t phlnLlcp_Transport_Socket_DiscoverServices(
                                                      void * pDataParams,                              /**< [In] Pointer to this layer's parameter structure. */
                                                      phlnLlcp_Transport_Socket_t * pSocket,           /**< [In] Pointer of Socket. */
                                                      uint8_t  bNumber,                                /**< [In] Number of Service Names and SAP list. */
                                                      uint8_t  bSNBuffSize,                            /**< [In] Length of Service Name (SN) Buffer Size containing Length and String of URI. */
                                                      uint8_t *  psServiceNameList,                    /**< [In] The list of the Service names to discover. First Byte contains length
                                                                                                                 of SN followed by respective SN Bytes. Followed by next SN Length. */
                                                      uint8_t *  pSapList                             /**< [Out] The list of the corresponding SAP numbers (one byte each), in the same order as the service names list. */
                                                      );

/**
* \brief SendTo is a used to send data on a Connection-less socket.
* Sends data to a remote socket on DSAP Address using bDsap parameter. Delivery of data is not guaranteed.
* Function blocks until the UI PDU is transmitted by the main LLCP task.\n
* \b wOption can be one of:\n
* \li #PH_TRANSMIT_DEFAULT
*
* \b wOption can be combined with:\n
* \li #PH_TRANSMIT_BUFFERED_BIT
* \li #PH_TRANSMIT_LEAVE_BUFFER_BIT
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init()\n
* 3. phlnLlcp_Transport_Socket_Register()\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_UNSUPPORTED_COMMAND   In-case Socket is other than #PHLN_LLCP_TRANSPORT_CONNECTIONLESS Socket Type.
* \retval #PH_ERR_RESOURCE_ERROR        Message Queue is unavailable or OSAL returned error.
*/
phStatus_t phlnLlcp_Transport_Socket_SendTo(
                                            void * pDataParams,                                        /**< [In] Pointer to this layer's parameter structure. */
                                            phlnLlcp_Transport_Socket_t * pClientSocket,               /**< [In] Pointer of Client Socket. */
                                            uint8_t   bDsap,                                           /**< [In] Destination SAP to which Data needs to be sent. */
                                            uint8_t * pTxBuffer,                                       /**< [In] Pointer of Buffer to be Transmitted. */
                                            uint32_t dwTxBufferSize,                                   /**< [In] Length of the Buffer to be Transmitted. */
                                            uint16_t wFrameOpt                                         /**< [In] Option parameter. */
                                            );

/**
* \brief Disconnect is a Blocking call used to close a Connection-oriented socket and its related data link.
* This API will post DISC PDU to Message Queue with respective Local SAP and Remote SAP, \ref phlnLlcp_Activate API will send this PDU to remote peer.\n
* Waits until DM PDU is received and Semaphore is signaled by the \ref phlnLlcp_Activate.\n
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init()\n
* 3. phlnLlcp_Transport_Socket_Register()\n
* 4. phlnLlcp_Transport_Socket_Connect()\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
* \retval #PH_ERR_LLCP_DEACTIVATED      Local LLC shut down.
*/
phStatus_t phlnLlcp_Transport_Socket_Disconnect(
                                                void* pDataParams,                                     /**< [In] Pointer to this layer's parameter structure. */
                                                phlnLlcp_Transport_Socket_t * pClientSocket            /**< [In] Pointer of Client Socket. */
                                                );

/**
* \brief Unregister a socket from the SAP Manager.
* This API needs to be called on all the Initialized sockets to deallocate Semaphores allocated to it by the respective Threads even in case of Error scenarios.\n
* If a socket is Registered with SAP then this API will De-Register with the SAP Manager.\n
*
* * <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_WaitForActivation()\n
* 2. phlnLlcp_Transport_Socket_Init()\n
* 3. phlnLlcp_Transport_Socket_Register()\n
* 4. phlnLlcp_Transport_Socket_Connect() or phlnLlcp_Transport_Socket_WaitForConnection()\n
* 5. phlnLlcp_Transport_Socket_Send() or phlnLlcp_Transport_Socket_Receive()\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
*/
phStatus_t phlnLlcp_Transport_Socket_Unregister(
                                                void * pDataParams,                                    /**< [In] Pointer to this layer's parameter structure. */
                                                phlnLlcp_Transport_Socket_t * pSocket                  /**< [In] Pointer of Socket to be Unregistered. */
                                                );

/**
* \brief DeInit is used to release all the acquired resources and reset internal parameters of LLCP.
* This API should be called immediately after \ref phlnLlcp_Activate API, irrespective of the return status.\n
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. phlnLlcp_Sw_Init().\n
* 2. phlnLlcp_Activate().\n
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval Other                         Depending on underlying component.
*/
phStatus_t phlnLlcp_DeInit(
                           void * pDataParams
                           );

/** @} */
#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHLN_LLCP */

#ifdef __cplusplus
} /* Extern C */
#endif /* Extern C */

#endif /* PHLNLLCP_H*/
