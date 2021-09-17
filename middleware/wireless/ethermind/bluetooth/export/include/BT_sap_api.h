
/**
 *  \file BT_sap_api.h
 *
 *  \brief EtherMind SAP API Header File.
 *
 *  This Header File contains declaration of SAP APIs, Macros and
 *  Constant Definitions for use in applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_SAP_API_
#define _H_BT_SAP_API_

/* ----------------------------------------- Header File Inclusion */
#include "BT_api.h"

/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
#if ((defined SAP_SERVER) || (defined SAP_CLIENT))
/**
 * \defgroup sap_module SAP (SIM Access Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  SIM Access Profile (SAP) module to the Application and other upper layers
 *  of the stack.
 */
/**
 * \defgroup sap_defines Defines
 * \{
 * Describes defines for the module.
 */
/**
 * \defgroup sap_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/** \} */

/* -------------------------------------------------- constants */
/**
 * \defgroup sap_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/** SAP Message IDs */
typedef enum _SAP_MSG_ID
{
    /** [0x00] CONNECT_REQ. Client -> Server */
    SAP_MSG_CONNECT_REQ = 0x00U,

    /** [0x01] CONNECT_RESP. Server -> Client */
    SAP_MSG_CONNECT_RESP = 0x01U,

    /** [0x02] DISCONNECT_REQ. Client -> Server */
    SAP_MSG_DISCONNECT_REQ = 0x02U,

    /** [0x03] DISCONNECT_RESP. Server -> Client */
    SAP_MSG_DISCONNECT_RESP = 0x03U,

    /** [0x04] DISCONNECT_IND. Server -> Client */
    SAP_MSG_DISCONNECT_IND = 0x04U,

    /** [0x05] TRANSFER_APDU_REQ. Client -> Server */
    SAP_MSG_TRANSFER_APDU_REQ = 0x05U,

    /** [0x06] TRANSFER_APDU_RESP. Server -> Client */
    SAP_MSG_TRANSFER_APDU_RESP = 0x06U,

    /** [0x07] TRANSFER_ATR_REQ. Client -> Server */
    SAP_MSG_TRANSFER_ATR_REQ = 0x07U,

    /** [0x08] TRANSFER_ATR_RESP. Server -> Client */
    SAP_MSG_TRANSFER_ATR_RESP = 0x08U,

    /** [0x09] POWER_SIM_OFF_REQ. Client -> Server */
    SAP_MSG_POWER_SIM_OFF_REQ = 0x09U,

    /** [0x0A] POWER_SIM_OFF_RESP. Server -> Client */
    SAP_MSG_POWER_SIM_OFF_RESP = 0x0AU,

    /** [0x0B] POWER_SIM_ON_REQ. Client -> Server */
    SAP_MSG_POWER_SIM_ON_REQ = 0x0BU,

    /** [0x0C] POWER_SIM_ON_RESP. Server -> Client */
    SAP_MSG_POWER_SIM_ON_RESP = 0x0CU,

    /** [0x0D] RESET_SIM_REQ. Client -> Server */
    SAP_MSG_RESET_SIM_REQ = 0x0DU,

    /** [0x0E] RESET_SIM_RESP. Server -> Client */
    SAP_MSG_RESET_SIM_RESP = 0x0EU,

    /** [0x0F] TRANSFER_CARD_READER_STATUS_REQ. Client -> Server */
    SAP_MSG_TRANSFER_CARD_READER_STATUS_REQ = 0x0FU,

    /** [0x10] TRANSFER_CARD_READER_STATUS_RESP. Server -> Client */
    SAP_MSG_TRANSFER_CARD_READER_STATUS_RESP = 0x10U,

    /** [0x11] STATUS_IND. Server -> Client */
    SAP_MSG_STATUS_IND = 0x11U,

    /** [0x12] ERROR_RESP. Server -> Client */
    SAP_MSG_ERROR_RESP = 0x12U,

    /** [0x13] SET_TRANSPORT_PROTOCOL_REQ. Client -> Server */
    SAP_MSG_SET_TRANSPORT_PROTOCOL_REQ = 0x13U,

    /** [0x14] SET_TRANSPORT_PROTOCOL_RESP. Server -> Client */
    SAP_MSG_SET_TRANSPORT_PROTOCOL_RESP = 0x14U
}SAP_MSG_ID;

/** SAP Parameter IDs */
typedef enum _SAP_PARAM_ID
{
    /** MaxMsgSize. Param Length: 2 */
    SAP_PID_MAX_MSG_SIZE = 0x00U,

    /** ConnectionStatus. Param Length: 1 */
    SAP_PID_CONNECTION_STATUS = 0x01U,

    /** ResultCode. Param Length: 1 */
    SAP_PID_RESULT_CODE = 0x02U,

    /** DisconnectionType. Param Length: 1 */
    SAP_PID_DISCONNECT_TYPE = 0x03U,

    /** CommandAPDU. Param Length: Varies */
    SAP_PID_COMMAND_APDU = 0x04U,

    /** ResponseAPDU. Param Length: Varies */
    SAP_PID_RESPONSE_APDU = 0x05U,

    /** ATR. Param Length: Varies */
    SAP_PID_ATR = 0x06U,

    /** CardReaderStatus. Param Length: 1 */
    SAP_PID_CARD_READER_STATUS = 0x07U,

    /** StatusChange. Param Length: 1 */
    SAP_PID_STATUS_CHANGE = 0x08U,

    /** Transport Protocol. Param Length: 1 */
    SAP_PID_TRANSPORT_PROTOCOL = 0x09U,

    /** CommandAPDU7816. Param Length: Varies */
    SAP_PID_COMMAND_APDU7816 = 0x10U

}SAP_PARAM_ID;

/** SAP Connection Status */
typedef enum _SAP_CONNECTION_STATUS
{
    /** OK, Server can fulfill requirements */
    SAP_CONN_STATUS_OK_SRV_CAN_FULFILL_REQRMTS = 0x00U,

    /** Error, Server unable to establish connection */
    SAP_CONN_STATUS_ERROR_SRV_UNABLE_TO_ESTABLISH_CONNECTION = 0x01U,

    /** Error, Server does not support maximum message size */
    SAP_CONN_STATUS_SRV_DOES_NOT_SUPPORT_MAX_MSG_SIZE = 0x02U,

    /** Error, maximum message size by Client is too small */
    SAP_CONN_STATUS_MAX_MSG_SIZE_BY_CLIENT_TOO_SMALL = 0x03U,

    /** OK, ongoing call */
    SAP_CONN_STATUS_OK_ONGOING_CALL = 0x04U
}SAP_CONNECTION_STATUS;

/** SAP Status Change */
typedef enum _SAP_STATUS_CHANGE
{
    /** Unknown Error */
    SAP_STATUS_CHANGE_UNKNOWN_ERROR = 0x00U,

    /** Card reset */
    SAP_STATUS_CHANGE_CARD_RESET = 0x01U,

    /** Card not accessible */
    SAP_STATUS_CHANGE_CARD_NOT_ACCESSIBLE = 0x02U,

    /** Card removed */
    SAP_STATUS_CHANGE_CARD_REMOVED = 0x03U,

    /** Card inserted */
    SAP_STATUS_CHANGE_CARD_INSERTED = 0x04U,

    /** Card recovered */
    SAP_STATUS_CHANGE_CARD_RECOVERED = 0x05U
}SAP_STATUS_CHANGE;

/** SAP Disconnection Type */
typedef enum _SAP_DISCONNECTION_TYPE
{
    /** Graceful */
    SAP_GRACEFUL_DISCONNECTION = 0x00U,

    /** Immediate */
    SAP_IMMEDIATE_DISCONNECTION = 0x01U
}SAP_DISCONNECTION_TYPE;

/** SAP Result Code */
typedef enum _SAP_RESULT_CODE
{
    /** OK, request processed correctly */
    SAP_RESULT_OK_REQ_PROCESSED_CORRECTLY = 0x00U,

    /** Error, no reason defined */
    SAP_RESULT_ERROR_NO_REASON_DEFINED = 0x01U,

    /** Error, card not accessible */
    SAP_RESULT_ERROR_CARD_NOT_ACCESSIBLE = 0x02U,

    /** Error, card (already) powered off */
    SAP_RESULT_CARD_ALREADY_POWERED_OFF = 0x03U,

    /** Error, card removed */
    SAP_RESULT_CARD_REMOVED = 0x04U,

    /** Error, card already powered on */
    SAP_RESULT_CARD_ALREADY_POWERED_ON = 0x05U,

    /** Error, data not available */
    SAP_RESULT_DATA_NOT_AVALIABLE = 0x06U,

    /** Error, not supported */
    SAP_RESULT_NOT_SUPPORTED = 0x07U

}SAP_RESULT_CODE;

/** SAP Parameter Structure */
typedef struct _SAP_PARAMETER
{
    /** Parameter Identifier */
    UCHAR   param_id;

    /** Number of octets in Parameter */
    UINT16  param_length;

    /** Parameter Value */
    void *  parameter_value;
}SAP_PARAMETER;

/** SAP Message Structure */
typedef struct _SAP_MESSAGE
{
    /** Message ID */
    UCHAR msg_id;

    /** Number of Parameters */
    UCHAR no_of_parameters;

    /** SAP Parameters */
    SAP_PARAMETER param[SAP_MAX_NO_OF_PARAMS];
}SAP_MESSAGE;

/* -------------------------------------------------- Type Definition */
/** SAP Event Types */
typedef enum _SAP_EVENT_TYPE
{
    /** SAP Event: RFCOMM Open */
    SAP_RFCOMM_OPEN,

    /** SAP Event: RFCOMM Accept */
    SAP_RFCOMM_ACCEPT,

    /** SAP Event: RFCOMM Close */
    SAP_RFCOMM_CLOSE,

    /** SAP Event: RFCOMM Read */
    SAP_RFCOMM_READ,

    /** SAP Event: RFCOMM Write */
    SAP_RFCOMM_WRITE,

    /** SAP Event: Connect */
    SAP_CONNECT
}SAP_EVENT_TYPE;

/** \} */
/** \} */

/**
 *  \defgroup sap_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */
/**
 * SAP Client Application Asynchronous Notification Callback.
 *
 * SAP Client calls the registered callback to indicate events occurred to the
 * application.
 *
 * @param event_type Any of the Event occurred from the list at \ref SAP_EVENT_TYPE.
 * @param event_result \ref API_SUCCESS or an error code in BT_error.h
 * @param event_data Data associated with the event if any or NULL.
 * @param event_datalen Size of the event data. 0 if event data is NULL.
 */
typedef API_RESULT (* SAP_CLIENT_APPL_CALLBACK)
                   (
                       UCHAR    event_type,
                       UINT16   event_result,
                       void   * event_data,
                       UINT16   event_datalen
                   );

/**
 * SAP Server Application Asynchronous Notification Callback.
 *
 * SAP Server calls the registered callback to indicate events occurred to the
 * application.
 *
 * @param event_type Any of the Event occurred from the list at \ref SAP_EVENT_TYPE.
 * @param event_result \ref API_SUCCESS or an error code in BT_error.h
 * @param event_data Data associated with the event if any or NULL.
 * @param event_datalen Size of the event data. 0 if event data is NULL.
 */
typedef API_RESULT (* SAP_SERVER_APPL_CALLBACK)
                   (
                       UCHAR    event_type,
                       UINT16   event_result,
                       void   * event_data,
                       UINT16   event_datalen
                   );
/** \} */

/* --------------------------------------------------- APIs Declarations */
/**
 * \defgroup sap_api API Definitions
 * \{
 * Describes API definitions of this module.
 */
#ifdef __cplusplus
extern "C" {
#endif

/** SAP Server related API */
/**
 *  \brief To start the SIM Access Server.
 *
 *  \par Description:
 *       This API starts the SIM Access Server.
 *
 *  \ingroup SIM_Access_Server_API Handler
 *
 *  \param [in] appl_cb
 *         Callback function to be registered by the caller application.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_server_start
           (
               /* IN */ SAP_SERVER_APPL_CALLBACK appl_cb
           );

/**
 *  \brief To start SIM Access Profile disconnection from the server side.
 *
 *  \par Description:
         This API starts SIM Access Profile disconnection from the server side.
 *
 *  \ingroup SIM_Access_Server_API Handler
 *
 *  \param [in] disconnection_type
 *         Values: 0x00 (Graceful), 0x01 (Immediate)
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_server_disconnect
           (
               /* IN */ UCHAR disconnection_type
           );

/**
 *
 *  \brief To report status of SAP server.
 *
 *  \par Description:
 *       This procedure is used during the connection setup phase
 *       or whenever a change in the physical connection between
 *       Server and SIM occurs.
 *
 *  \ingroup SIM_Access_Server_API Handler
 *
 *  \param [in] status
 *         One of the following values
 *             SAP_STATUS_CHANGE_UNKNOWN_ERROR
 *             SAP_STATUS_CHANGE_CARD_RESET
 *             SAP_STATUS_CHANGE_CARD_NOT_ACCESSIBLE
 *             SAP_STATUS_CHANGE_CARD_REMOVED
 *             SAP_STATUS_CHANGE_CARD_INSERTED
 *             SAP_STATUS_CHANGE_CARD_RECOVERED
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_server_report_status
           (
               /* IN */ UCHAR status
           );

/**
 *  \brief To send APDU Response.
 *
 *  \par Description:
 *       Transfers an APDU Response between the Server and the Client.
 *       APDU transfers are always initiated by the Client.
 *
 *  \ingroup SIM_Access_Server_API Handler
 *
 *  \param  [in] rsp     Response data pointer
 *  \param  [in] rsplen  Response data length
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_server_transfer_apdu_response(UCHAR * rsp, UINT16 rsplen);

/** SAP Client related APIs */
/**
 *  \brief To start the SIM Access Client.
 *
 *  \par Description:
 *       This API starts the SIM Access Client.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \param [in] appl_cb
 *         Callback function to be registered by the caller application.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_start
           (
               /* IN */ SAP_CLIENT_APPL_CALLBACK appl_cb
           );

/**
 *
 *  \brief To start SIM Access Profile connection.
 *
 *  \par Description:
 *       This API starts SIM Access Profile connection and negotiate important parameters
 *       adherent to the connection.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \param [in] bd_addr
 *         Bluetooth Device Address to which SIM Access Connection to be made.
 *
 *  \param [in] remote_server_channel
 *         RFCOMM Server Channel of SAP Server
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_connect
           (
               /* IN */ UCHAR *bd_addr,
               /* IN */ UCHAR remote_server_channel
           );

/**
 *  \brief To start SIM Access Profile disconnection from the client side.
 *
 *  \par Description:
 *       This API starts SIM Access Profile disconnection from the client side.
 *       First SIM Client terminate any existing GSM session which involves
 *       the SIM in the Server and then send DISCONNECT_REQ message to the
 *       Server.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \param void
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_disconnect(void);

/**
 *  \brief To request the server to send ATR from the SIM.
 *
 *  \par Description:
 *       This API requests the Server to send the ATR from the SIM.
 *       The TRANSFER_ATR_REQ message is used for this purpose.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \param  void
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_trasfer_atr(void);

/**
 *  \brief To request the Server to power on the SIM.
 *
 *  \par Description:
 *       This API requests the Server to power on the SIM.
 *       The POWER_SIM_ON_REQ message is used for this purpose.
 *       Upon receiving this message, the Server powers on and eventually
 *       performs PPS procedure. After this has been completed, the Server
 *       sends the POWER_SIM_ON_RESP message to the Client.
 *       If the response indicates that operation is successful, the client
 *       requests the ATR of the SIM.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \param  void
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_power_sim_on(void);

/**
 *  \brief To request the Server to power off the SIM.
 *
 *  \par Description:
 *       This API requests the Server to power off the SIM.
 *       First the client terminates any existing GSM session which involves
 *       the SIM in the server.
 *       The client then send the POWER_SIM_OFF_REQ message to the Server.
 *       Upon receiving this message, the Server powers off the SIM and then
 *       sends the POWER_SIM_OFF_RESP message to the Client.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_power_sim_off(void);

/**
 *  \brief To request the Server to reset the SIM.
 *
 *  \par Description:
 *       This API requests the Server to reset the SIM.
 *
 *  - First the client terminates any existing GSM session which involves
 *  the SIM in the server.
 *
 *  - The RESET_SIM_REQ message is used for this purpose.
 *  Upon receiving this message, the Server resets the SIM and eventually
 *  performs PPS procedure. After this has been completed, the Server
 *  sends the RESET_SIM_RESP message to the Client.
 *  If the response indicates that operation is successful, the client
 *  requests the ATR of the SIM.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_reset_sim(void);

/**
 *  \brief To request the Server to return the Card Reader Status.
 *
 *  \par Description:
 *       This API requests the Server to return the Card Reader Status using
 *       the TRANSFER_CARD_READER_STATUS_REQ message.
 *       Following this request, the Server sends the Client
 *       the Card Reader Status in the TRANSFER_CARD_READER_STATUS_RESP
 *       message.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_transfer_card_reader_status(void);

/**
 *  \brief To request the Server to set transport protocol.
 *
 *  \par Description:
 *       This API requests the Server to set transport protocol using
 *       the SET_TRANSPORT_PROTOCOL_REQ message.
 *       Following this request, the Server sends the Client
 *       the Set Transport Protocol response using SET_TRANSPORT_PROTOCOL_RESP
 *       message.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \param  [in] transport_protocol
 *          0x00: T=0
 *          0x01: T=1
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_set_transport_protocol
           (
               /* IN */ UCHAR transport_protocol
           );

/**
 *  \brief To send APDU Command.
 *
 *  \par Description:
 *       Transfers an APDU Command between the Client and the Server.
 *       APDU transfers are always initiated by the Client.
 *
 *  \ingroup SIM_Access_Client_API Handler
 *
 *  \param  [in] type    APDU Command Type: SAP_PID_COMMAND_APDU or SAP_PID_COMMAND_APDU7816
 *  \param  [in] pdu     PDU data pointer
 *  \param  [in] pdulen  PDU data length
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_client_transfer_apdu_command(UCHAR type, UCHAR * pdu, UINT16 pdulen);


/** SAP Access Common APIs */
/**
 *  \brief To form messages and send it to the peer SIM Access entity.
 *
 *  \par Description:
 *       Forms Messages based on SIM Access Profile specification
 *       and sends it to the peer SIM Access entity.
 *
 *  \ingroup SIM_Access_Common_Functions
 *
 *  \param [in] sap_msg
 *         SAP Message to be sent to peer
 *
 *  \param [in] rfcomm_handle
 *         RFCOMM Connection Handle identifying the SAP connection with peer
 *
 *  \param [in] apdu
 *         Memory buffer provided by the caller to frame the SAP APDU from the 'sap_msg'
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_form_and_send_message
           (
               /* IN */ SAP_MESSAGE    * sap_msg,
               /* IN */ RFCOMM_HANDLE  * rfcomm_handle,
               /* IN */ UCHAR          * apdu
           );

/**
 *  \brief To parses SIM Access Profile Messages.
 *
 *  \par Description:
 *       This API parses SIM Access Profile Messages.
 *
 *  \ingroup SIM_Access_Common_Functions
 *
 *  \param [out] sap_msg
 *         SAP Message structure, filled after parsing 'apdu'.
 *         Memory for this SAP_MESSAGE is provided by the caller.
 *
 *  \param [in] apdu
 *         SAP APDU to be parsed.
 *
 *  \return
 *      API_RESULT: API_SUCCESS or Error code for failure
 */
API_RESULT BT_sap_parse_message
           (
               /* OUT */ SAP_MESSAGE  * sap_msg,
               /* IN */  UCHAR        * apdu
           );

#ifdef __cplusplus
};
#endif

#endif /* ((defined SAP_SERVER) || (defined SAP_CLIENT)) */
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_SAP_API_ */

