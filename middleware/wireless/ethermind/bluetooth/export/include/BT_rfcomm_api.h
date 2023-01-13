
/**
 *  \file BT_rfcomm_api.h
 *
 *  \brief EtherMind RFCOMM API Header File.
 *
 *  This header file contains declaration of RFCOMM APIs, various Macros and
 *  Constant Definitions for use in upper layer profiles/applications.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_RFCOMM_API_
#define _H_BT_RFCOMM_API_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"


/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_protocol Protocols
 * \{
 */
/**
 * \defgroup  rfcomm_module  RFCOMM (Radio Frequency Communication Protocol)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Radio Frequency Communication Protocol module to the Application and other upper
 *  layers of the stack.
 */
/**
 * \defgroup  rfcomm_defines Defines
 * \{
 * Describes defines for RFCOMM module.
 */
/**
 * \defgroup rfcomm_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/**
 * @name RFCOMM RPN "Request" Types
 *
 * Constant Definitions for RFCOMM RPN "Request" Types
 */
/*@{*/
/** RFCOMM RPN "Request" Types */
#define RFCOMM_RPN_REQUEST_OPTION               0x01U
#define RFCOMM_RPN_COMMAND_OPTION               0x02U
#define RFCOMM_RPN_RESPONSE_OPTION              0x03U
/*@}*/
/** RFCOMM Port Parameters - Baud Rates */
/**
 * @name  RFCOMM Port Parameters - Baud Rates
 *
 * Constant Definitions for RFCOMM Port Parameters - Baud Rates
 */
/*@{*/
#define RFCOMM_PORT_BAUD_RATE_2400              0x00U
#define RFCOMM_PORT_BAUD_RATE_4800              0x01U
#define RFCOMM_PORT_BAUD_RATE_7200              0x02U
#define RFCOMM_PORT_BAUD_RATE_9600              0x03U
#define RFCOMM_PORT_BAUD_RATE_19200             0x04U
#define RFCOMM_PORT_BAUD_RATE_38400             0x05U
#define RFCOMM_PORT_BAUD_RATE_57600             0x06U
#define RFCOMM_PORT_BAUD_RATE_115200            0x07U
#define RFCOMM_PORT_BAUD_RATE_230400            0x08U
/*@}*/
/** RFCOMM Port Parameters - Data Bits */
/**
 * @name  RFCOMM Port Parameters - Data Bits
 *
 * Constant Definitions for RFCOMM Port Parameters - Data Bits
 */
/*@{*/
#define RFCOMM_PORT_DATA_BITS_5                 0x00U
#define RFCOMM_PORT_DATA_BITS_6                 0x02U
#define RFCOMM_PORT_DATA_BITS_7                 0x01U
#define RFCOMM_PORT_DATA_BITS_8                 0x03U
/*@}*/
/** RFCOMM Port Parameters - Stop Bit */
/**
 * @name  RFCOMM Port Parameters - Stop Bit
 *
 * Constant Definitions for RFCOMM Port Parameters - Stop Bit
 */
/*@{*/
#define RFCOMM_PORT_STOP_BIT_1                  0x00U
#define RFCOMM_PORT_STOP_BIT_1_5                0x01U
/*@}*/
/** RFCOMM Port Parameters - Parity */
/**
 * @name  RFCOMM Port Parameters - Parity
 *
 * Constant Definitions for RFCOMM Port Parameters - Parity
 */
/*@{*/
#define RFCOMM_PORT_PARITY_NONE                 0x00U
#define RFCOMM_PORT_PARITY_SET                  0x01U
/*@}*/
/** RFCOMM Port Parameters - Parity Type */
/**
 * @name  RFCOMM Port Parameters - Parity Type
 *
 * Constant Definitions for RFCOMM Port Parameters - Parity Type
 */
/*@{*/
#define RFCOMM_PORT_PARITY_TYPE_ODD             0x00U
#define RFCOMM_PORT_PARITY_TYPE_EVEN            0x02U
#define RFCOMM_PORT_PARITY_TYPE_MARK            0x01U
#define RFCOMM_PORT_PARITY_TYPE_SPACE           0x03U
/*@}*/
/** RFCOMM Port Parameters - Flow Control */
/**
 * @name  RFCOMM Port Parameters - Flow Control
 *
 * Constant Definitions for RFCOMM Port Parameters - Flow Control
 */
/*@{*/
#define RFCOMM_PORT_FLC_X_ON_X_OFF_INPUT        0x01U
#define RFCOMM_PORT_FLC_X_ON_X_OFF_OUTPUT       0x02U
#define RFCOMM_PORT_FLC_RTR_INPUT               0x04U
#define RFCOMM_PORT_FLC_RTR_OUTPUT              0x08U
#define RFCOMM_PORT_FLC_RTC_INPUT               0x10U
#define RFCOMM_PORT_FLC_RTC_OUTPUT              0x20U
/*@}*/
/** RFCOMM Port Parameters - Default Port Settings */
/**
 * @name  RFCOMM Port Parameters - Default Port Settings
 *
 * Constant Definitions for RFCOMM Port Parameters - Default Port Settings
 */
/*@{*/
#define RFCOMM_DEFAULT_PORT_BAUD_RATE           RFCOMM_PORT_BAUD_RATE_9600
#define RFCOMM_DEFAULT_PORT_DATA_BITS           RFCOMM_PORT_DATA_BITS_8
#define RFCOMM_DEFAULT_PORT_STOP_BIT            RFCOMM_PORT_STOP_BIT_1
#define RFCOMM_DEFAULT_PORT_PARITY              RFCOMM_PORT_PARITY_NONE
#define RFCOMM_DEFAULT_PORT_PARITY_TYPE         0x00U
#define RFCOMM_DEFAULT_PORT_FLOW_CONTROL        0x00U
#define RFCOMM_DEFAULT_PORT_XON_CHAR            0x11U
#define RFCOMM_DEFAULT_PORT_XOFF_CHAR           0x13U
/*@}*/
/**
 *  RFCOMM API Contexts or RFCOMM Command Owners.
 *  This has the following purposes:
 *  - Identify the User Context (Blocking Mode)
 *  - Identify the User API (Non-Blocking & Blocking Modes)
 *  - Event Identifiers for RFCOMM Notification Callback (Non-Blocking Mode)
 */
#define RFCOMM_NO_OPERATION                     0x00U
#define RFCOMM_OPEN                             0x01U
#define RFCOMM_CLOSE                            0x02U
#define RFCOMM_ACCEPT                           0x03U
#define RFCOMM_READ                             0x04U
#define RFCOMM_WRITE                            0x05U

#define RFCOMM_SEND_PN                          0x11U
#define RFCOMM_SEND_MSC                         0x12U
#define RFCOMM_SEND_RPN                         0x13U
#define RFCOMM_SEND_RLS                         0x14U

#define RFCOMM_SEND_FC_OFF                      0x21U
#define RFCOMM_SEND_FC_ON                       0x22U
#define RFCOMM_SEND_TEST                        0x23U

/* Asynchronous Events for Non-Blocking Mode */
#define RFCOMM_RESET                            0x41U
#define RFCOMM_RECVD_MSC                        0x51U
#define RFCOMM_RECVD_RLS                        0x52U

/** RFCOMM Credit Transmission Enable & Disable - Internal Flags */
#define RFCOMM_CREDIT_TX_ENABLED                0x01U
#define RFCOMM_CREDIT_TX_DISABLED               0x00U

/** RFCOMM Reading Local & Remote Credit Information - Internal Flags */
#define RFCOMM_CREDIT_LOCAL                     0x01U
#define RFCOMM_CREDIT_REMOTE                    0x00U

/** \} */
/** \} */

/* --------------------------------------------- Structures/Data Types */
/**
 *  \addtogroup rfcomm_defines Defines
 *  \{
 */

/**
 * \defgroup  rfcomm_structures Structures
 * \{
 * Describes Structures for RFCOMM  module.
 */
/**
 *  The RFCOMM Handle
 */
typedef struct _RFCOMM_HANDLE
{
    /** RFCOMM Event Notification Callback (Non-Blocking Mode) */
    API_RESULT (* notify_cb)
               (
                   UCHAR                     event_type,  /* Event Identifier */
                   struct _RFCOMM_HANDLE *    handle,      /* RFCOMM Handle */
                   UINT16                    result,      /* Result/Response */
                   UCHAR *                   data,        /* Returned Data */
                   UINT16                    datalen      /* Returned Data Length */
               );

#ifdef RFCOMM_ENABLE_USER_CXT_IN_HANDLE
    /**
     * Application provided User Context to be saved by RFCOMM and provided
     * through application callback.
     */
    void * blob;
#endif /* RFCOMM_ENABLE_USER_CXT_IN_HANDLE */

    /** The Server Channel for the Data Link Connection */
    UCHAR server_channel;

    /** The Data Link Connection Identifier */
    UCHAR dlci;

    /** The Remote Bluetooth Device Address */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

} RFCOMM_HANDLE;
/** \} */
/** \} */


/**
 *  \defgroup rfcomm_cb Application Callback
 *  \{
 *  This Section Describes the module Notification Callback interface offered
 *  to the application
 */

/**
 *  The RFCOMM Event Notification Callback Data Type
 */
typedef API_RESULT (* RFCOMM_NOTIFY_CB)
                   (
                       UCHAR              event_type,  /* Event Identifier */
                       RFCOMM_HANDLE *    handle,      /* RFCOMM Handle */
                       UINT16             result,      /* Result/Response */
                       UCHAR *            data,        /* Returned Data */
                       UINT16             datalen      /* Returned Data Length */
                   );
/** \} */

/**
 *  \addtogroup rfcomm_defines Defines
 *  \{
 */

/**
 *  \addtogroup  rfcomm_structures Structures
 *  \{
 */
/**
 *  The RFCOMM Parameter Negotiation (PN) Data Type
 */
typedef struct
{
    /** Frame Size of the Data Link Connection */
    UINT16 frame_size;

    /** The Data Link Connection Identifier */
    UCHAR dlci;

    /**
     *  The TS 07.10 Convergence Layer.
     *
     *  In RFCOMM Specification v1.1 onwards it is used to claim
     *  configure Credit based Flow Control.
     */
    UCHAR convergence_layer;

    /** The Priority of the Data Link Connection */
    UCHAR priority;

    /**
     *  The TS 07.10 Window Size.
     *
     *  In RFCOMM Specification v1.1 onwards it is used to convey
     *  initial local Credit.
     */
    UCHAR window_size;

} RFCOMM_PN;

/**
 *  The RFCOMM Remote Port Negotiation (RPN) Data Type
 */
typedef struct
{
    /** To indicate whether the RPN is Request, Command or Response */
    UCHAR request;

    /** The Data Link Connection Identifier */
    UCHAR dlci;

    /** RFCOMM Port Setting - Baud Rate */
    UCHAR baud_rate;

    /** RFCOMM Port Setting - Data Bits */
    UCHAR data_bits;

    /** RFCOMM Port Setting - Stop Bit */
    UCHAR stop_bit;

    /** RFCOMM Port Setting - Parity */
    UCHAR parity;

    /** RFCOMM Port Setting - Parity Type */
    UCHAR parity_type;

    /** RFCOMM Port Setting - Flow Control */
    UCHAR flc;

    /** RFCOMM Port Setting - XON Character */
    UCHAR x_on;

    /** RFCOMM Port Setting - XOFF Character */
    UCHAR x_off;

    /**
     *  The Parameter Masks - used to indicate which parameters in
     *  the RPN Frame are negotiated.
     *
     *  For a RPN "Command":
     *      0 -> No Change
     *      1 -> Change
     *
     *  For a RPN "Response":
     *      0 -> Not accepted proposal
     *      1 -> Accepted proposal, and new values are used
     *
     *  Bitmask for the 1st octet of Parameter Mask:
     *      Bit 0 -> Bit/Baud Rate
     *      Bit 1 -> Data Bits
     *      Bit 2 -> Stop Bit
     *      Bit 3 -> Parity
     *      Bit 4 -> Parity Type
     *      Bit 5 -> XON Character
     *      Bit 6 -> XOFF Character
     *      Bit 7 -> (Reserved)
     *
     *  Bitmask for the 2nd octet of Parameter Mask:
     *      Bit 0 -> XON/XOFF in input
     *      Bit 1 -> XON/XOFF in output
     *      Bit 2 -> RTR on input
     *      Bit 3 -> RTR on output
     *      Bit 4 -> RTC on input
     *      Bit 5 -> RTC on output
     *      Bit 6 -> (Reserved)
     *      Bit 7 -> (Reserved)
     */
    UCHAR mask [2U];

} RFCOMM_RPN;

/**
 *  The RFCOMM Modem Status Command (MSC) Data Type
 */
typedef struct
{
    /** The Data Link Connection Identifier */
    UCHAR dlci;

    /** Modem Status Parameter - Flow Control */
    UCHAR fc;

    /** Modem Status Parameter - Ready to Communicate */
    UCHAR rtc;

    /** Modem Status Parameter - Ready to Receive */
    UCHAR rtr;

    /** Modem Status Parameter - Incoming Call */
    UCHAR ic;

    /** Modem Status Parameter - Data valid */
    UCHAR dv;

    /** Boolean Flag to indicate if Break Signal is present */
    UCHAR flag;

    /** To indicate that the Octet encodes a valid Break Signal */
    UCHAR break_present;

    /** Length of Break in units of 200 ms */
    UCHAR break_length;

} RFCOMM_MSC;

/**
 *  The RFCOMM Remote Line Status (RLS) Data Type
 */
typedef struct
{
    /** The Data Link Connection Identifier */
    UCHAR dlci;

    /**
     *  The Line Status
     *
     *  7   6   5   4   3   2   1   0
     *  X   X   X   X   X   X   X   0  =>  No Error
     *  X   X   X   X   0   0   1   1  =>  Overrun Error
     *  X   X   X   X   0   1   0   X  =>  Parity Error
     *  X   X   X   X   1   0   0   X  =>  Framing Error
     *
     *  X = Don't Care
     */
    UCHAR line_status;

} RFCOMM_RLS;

/**
 *  The RFCOMM Non Supported Command (NSC) Data Type
 */
typedef struct
{
    /** C/R-Bit of the Non Supported Command */
    UCHAR cr_bit;

    /** Command Type of the Non Supported Command */
    UCHAR command_type;

} RFCOMM_NSC;

/**
 *  The RFCOMM Test (TEST) Data Type
 */
typedef struct
{
    /** The Test Data Pattern */
    UCHAR *    data;

    /** Size of the Test Data Pattern */
    UINT16     datalen;

} RFCOMM_TEST;
/** \} */
/** \} */

/* --------------------------------------------- Macros */
/**
 * \defgroup  rfcomm_utility_macros Utility Macros
 * \{
 * Describes Utility Macros for RFCOMM module.
 */
/**
 *  This utility macro initializes fields of a RFCOMM Handle.
 *
 *  \param [in] hdl
 *         The RFCOMM Handle to be initialized.
 */
#ifdef RFCOMM_ENABLE_USER_CXT_IN_HANDLE
#define RFCOMM_RESET_HANDLE(hdl) \
        (hdl)->notify_cb = NULL; \
        (hdl)->blob = NULL; \
        (hdl)->dlci = 0x0U; \
        (hdl)->server_channel = 0x0U; \
        BT_mem_set((hdl)->bd_addr, 0x0U, BT_BD_ADDR_SIZE);

#else
#define RFCOMM_RESET_HANDLE(hdl) \
        (hdl)->notify_cb = NULL; \
        (hdl)->dlci = 0x0U; \
        (hdl)->server_channel = 0x0U; \
        BT_mem_set((hdl)->bd_addr, 0x0U, BT_BD_ADDR_SIZE);
#endif /* RFCOMM_ENABLE_USER_CXT_IN_HANDLE */

/**
 *  This utility macro populates required field of a RFCOMM handle,
 *  as given by the parameters.
 *
 *  \param [in] hdl
 *         The RFCOMM Handle to be initialized
 *  \param [in] bd
 *         The remote Bluetooth Device Address
 *  \param [in] ch
 *         The RFCOMM Server Channel
 *  \param [in] cb
 *         The RFCOMM Event Notification (Non-Blocking Mode)
 */
#define RFCOMM_SET_HANDLE(hdl, bd, ch, cb) \
        (hdl)->notify_cb = (cb); \
        (hdl)->server_channel = (ch); \
        BT_COPY_BD_ADDR((hdl)->bd_addr, (bd));

/**
 *  This utility macro populates required field of a RFCOMM handle,
 *  for calling RFCOMM Accept
 *
 *  \param [in] hdl
 *         The RFCOMM Handle to be initialized
 *  \param [in] ch
 *         The RFCOMM Server Channel
 */
#define RFCOMM_SET_HANDLE_ACCEPT(hdl, ch, cb) \
        (hdl)->notify_cb = (cb); \
        (hdl)->server_channel = (ch);

/**
 *  This utility macro compares two RFCOMM Handles.
 *
 *  \param [in] hdl_1
 *  \param [in] hdl_2
 *         The RFCOMM Handles to be compared.
 */
#define BT_rfcomm_compare_handles(hdl_1, hdl_2) \
        (((hdl_1)->dlci == (hdl_2)->dlci) && \
         ((hdl_1)->server_channel == (hdl_2)->server_channel) && \
         (0 == BT_mem_cmp((hdl_1)->bd_addr, (hdl_2)->bd_addr, BT_BD_ADDR_SIZE)))

/**
 *  This utility macro sets the Notification Callback in a RFCOMM Handle
 *
 *  \param [in] hdl
 *         The RFCOMM Handle to be initialized
 *  \param [in] cb
 *         The RFCOMM Event Notification (Non-Blocking Mode)
 */
#define RFCOMM_SET_HANDLE_NOTIFY_CB(hdl, cb) \
        (hdl)->notify_cb = (cb);

/** RFCOMM Credit Transmission Enable */
#define BT_rfcomm_credit_tx_enable(hdl) \
        BT_rfcomm_credit_tx((hdl), RFCOMM_CREDIT_TX_ENABLED)

/** RFCOMM Credit Transmission Disable */
#define BT_rfcomm_credit_tx_disable(hdl) \
        BT_rfcomm_credit_tx((hdl), RFCOMM_CREDIT_TX_DISABLED)

/** RFCOMM Read Local Credit */
#define BT_rfcomm_read_local_credit(hdl, cr) \
        BT_rfcomm_read_credit((hdl), (cr), RFCOMM_CREDIT_LOCAL)

/** RFCOMM Read Remote Credit */
#define BT_rfcomm_read_remote_credit(hdl, cr) \
        BT_rfcomm_read_credit((hdl), (cr), RFCOMM_CREDIT_REMOTE)

/** To Send Session Flow ON */
#define BT_rfcomm_send_fc_on(hdl) \
        BT_rfcomm_send_fc(hdl, RFCOMM_SEND_FC_ON)

/** To Send Session Flow OFF */
#define BT_rfcomm_send_fc_off(hdl) \
        BT_rfcomm_send_fc(hdl, RFCOMM_SEND_FC_OFF)
/** \} */

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------- Internal Functions */

/* \cond ignore_this Not to be documented - Internal Functions */
/** RFCOMM Credit Transmission Enable & Disable - Internal Functions */
API_RESULT BT_rfcomm_credit_tx
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  UCHAR              flag
           );

/** RFCOMM Read Local & Remote Credit Information - Internal Functions */
API_RESULT BT_rfcomm_read_credit
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  UINT16 *           credit,
               /* IN */  UCHAR              flag
           );

/** To Send Session Flow ON/OFF */
API_RESULT BT_rfcomm_send_fc
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  UCHAR              operation
           );
/** \endcond */

/* --------------------------------------------- API Declarations */
/**
 * \defgroup  rfcomm_api API Definations
 * \{
 * Describes API definations for RFCOMM  module.
 */
/**
 *  \brief To accept Data Link Connection for a RFCOMM Server Channel from a
 *         remote device.
 *
 *  \par Description:
 *       This API starts waiting for an incoming RFCOMM Data Link Connection
 *       from a remote device. The Server Channel Parameter identifies the DLC.
 *       The RFCOMM DLC must not already be connected.
 *       This API marks the corresponding RFCOMM DLC for connection acceptance,
 *       and returns.
 *       The "server" side device generally uses this API. The "client" device will
 *       "open" the RFCOMM connection to this "server" using BT_rfcomm_open() API.
 *
 *  \param [in] server_channel
 *         The Server Channel, the corresponding RFCOMM DLC to which the local
 *         RFCOMM will allow connection attempt by the remote RFCOMM entity.
 *
 *  \param [in] handle
 *         RFCOMM handle must be initialized using RFCOMM_RESET_HANDLE() and
 *         RFCOMM_SET_HANDLE() utility macros. The RFCOMM handle must be
 *         populated with correct Notification Callback function pointer,
 *         so that RFCOMM can notify the caller later with the result of the
 *         operation. Also, the Notification Callback function pointer populated
 *         in the handle parameter will be invoked later on by RFCOMM for
 *         indication of asynchronous events, such as RFCOMM_READ,
 *         RFCOMM_RECVD_MSC and RFCOMM_RECVD_RLS.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note When the remote RFCOMM entity connects to this DLC, RFCOMM will call
 *        the Event Notification Callback with the event_type set as RFCOMM_ACCEPT.
 *        The rfcomm_handle parameter will hold the RFCOMM Handle corresponding
 *        to this RFCMM Data Link Connection.
 */
API_RESULT BT_rfcomm_accept
           (
               /* INOUT */  RFCOMM_HANDLE *    handle,
               /* IN */     UCHAR              server_channel
           );

/**
 *  \brief To stop waiting for connection on a RFCOMM Server Channel.
 *
 *  \par Description:
 *       This API can be used if the local RFCOMM entity was waiting for a
 *       connection. This API will cancel that provided the referred Server
 *       Channel is not opened by any remote RFCOMM entity already.
 *
 *  \param [in] server_channel
 *         The Server Channel for which the connection accept needs to be cancelled.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_rfcomm_accept_cancel
           (
               /* IN */  UCHAR    server_channel
           );

/**
 *  \brief To open an RFCOMM Data Link Connection.
 *
 *  \par Description:
 *       This API initiates connection procedure for RFCOMM Data Link Connection
 *       as specified by the Server Channel parameter with the remote device.
 *       RFCOMM Data Link Connection establishment phase starts with establishment
 *       of RFCOMM Session with the RFCOMM Entity on remote device, if it is not
 *       available. Once the RFCOMM Session is available, the local RFCOMM Entity
 *       can perform the optional Parameter Negotiation (PN) and Remote Port
 *       Negotiation.
 *       Actual DLC establishment happens then when local RFCOMM entity sends SABM
 *       (Set Asynchronous Balanced Mode) command and expects
 *       UA (Unnumbered Acknowledgement)
 *       response for it. Finally, both RFCOMM entities exchange Modem Status
 *       Commands regarding their initial Modem Status line status.
 *       The "client" side device generally uses this API. The "server" device
 *       will be waiting for an RFCOMM connection, on the specified server channel.
 *
 *  \param [in] bd_addr
 *         Bluetooth Device Address of the remote device.
 *
 *  \param [in] server_channel
 *         The RFCOMM Server Channel on which the Data Link Connection needs to
 *         be established.
 *  \param [in] handle
 *         RFCOMM handle must be initialized using RFCOMM_RESET_HANDLE() and
 *         RFCOMM_SET_HANDLE() utility macros. The RFCOMM handle should be
 *         populated with correct Notification Callback function pointer,
 *         so that RFCOMM can notify the result of the operation.
 *         Also, the Notification Callback function pointer populated in the handle
 *         parameter will be invoked later on by RFCOMM for indication of
 *         asynchronous events, such as RFCOMM_READ, RFCOMM_RECVD_MSC and
 *         RFCOMM_RECVD_RLS.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Completion of RFCOMM Open procedure will be notified using the RFCOMM
 *        Event Notification Callback with event_type set to the value of
 *        RFCOMM_OPEN. The rfcomm_handle parameter will hold the RFCOMM Handle
 *        corresponding to this DLC.
 */
API_RESULT BT_rfcomm_open
           (
               /* IN */     UCHAR *            bd_addr,
               /* IN */     UCHAR              server_channel,
               /* INOUT */  RFCOMM_HANDLE *    handle
           );

/**
 *  \brief To close an existing RFCOMM Data Link Connection.
 *
 *  \par Description:
 *       This API closes an existing RFCOMM Data Link Connection, as identified
 *       by the handle parameter.
 *       Closure of a RFCOMM Data Link Connection (DLC) involves transmission of
 *       DISC command by the local RFCOMM entity and reception of the UA respons
 *       for it. Additionally, if the DLC happens to be the only active DLC for
 *       the RFCOMM Session, then it also triggers closure of the RFCOMM Session
 *       with the remote RFCOMM entity.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM Channel or DLC which
 *         needs to be closed. The handle parameter must have been obtained as a
 *         result of earlier BT_rfcomm_open() or BT_rfcomm_accept() operation.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note On completion of RFCOMM Close procedure, local RFCOMM entity will call
 *        the RFCOMM Event Notification Callback with event_type set to RFCOMM_CLOSE.
 *        The response parameter of the callback will be set accordingly,
 *        indicating successful or unsuccessful completion.
 */
API_RESULT BT_rfcomm_close
           (
               /* IN */  RFCOMM_HANDLE *    handle
           );

/**
 *  \brief To query validity of an RFCOMM Channel.
 *
 *  \par Description:
 *       This API validates if the RFCOMM Data Link Connection specified by the
 *       RFCOMM handle is open and in connected state.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM DLC which needs to be
 *         validated. The handle parameter must have been obtained as a result
 *         of earlier BT_rfcomm_open() or BT_rfcomm_accept() operation.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_rfcomm_query_state
           (
               /* IN */  RFCOMM_HANDLE *    handle
           );

/**
 *  \brief To Write to an RFCOMM Data Link Connection.
 *
 *  \par Description:
 *       This API enables the application to transmit data over an established
 *       RFCOMM DLC to its peer entity. RFCOMM transmits data according to the
 *       Frame Size set for the DLC during the Parameter Negotiation (PN) phase.
 *       If the data size requested to be written is more than the negotiated
 *       Frame Size, RFCOMM will segment the application requested data into
 *       segments of Frame Size.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM DLC, on which the
 *         requested data needs to transmitted. The handle parameter must have
 *         been obtained as a result of earlier BT_rfcomm_open() or
 *         BT_rfcomm_accept() operation.
 *
 *  \param [in] data
 *         The application buffer location containing data to be transmitted.
 *
 *  \param [in] length
 *         The size of the application data.
 *
 *  \param [out] actual
 *         Not used. It must be set to NULL.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The local RFCOMM entity stores the application data pointer in its
 *        Transmit Queue and returns back. Transmission from the Transmit Queue
 *        happens automatically, but subject to Flow Control conditions.
 *        When the full data has been transmitted RFCOMM will call the
 *        Notification Callback to enable the application to free any allocated
 *        memory. Till then, the original data pointer, which the application
 *        has earlier written, must be resident and accessible to RFCOMM.
 *        When data is written (or, in case of an error), the RFCOMM will
 *        call the Notification Callback with the event_type parameter set to
 *        RFCOMM_WRITE. The event_data and event_datalen parameters will hold
 *        the original data and the amount of data written respectively.
 */
API_RESULT BT_rfcomm_write
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  UCHAR *            data,
               /* IN */  UINT16             length,
               /* OUT */ UINT16 *           actual
           );

/**
 *  \brief To Test RFCOMM Session by sending Test Command.
 *
 *  \par Description:
 *       This API enables the application to ask local RFCOMM entity to transmit
 *       a Test Command (RFCOMM_TEST), with the Test data pattern,
 *       over the RFCOMM Session with a remote RFCOMM entity.
 *       The RFCOMM Session must be available with the remote RFCOMM entity as
 *       identifiable by the Bluetooth Device Address parameter.
 *       The remote RFCOMM entity should respond to the Test Command with a Test
 *       Response, containing the same Test data pattern, as requested by the
 *       application.
 *
 *
 *  \param [in] handle
 *         The RFCOMM Handle. Only Bluetooth Device Address field of the RFCOMM
 *         handle is used. The handle parameter must be initialized using the
 *         RFCOMM_RESET_HANDLE() and RFCOMM_SET_HANDLE() utility macros
 *         (to specify the Notification Callback function pointer that the RFCOMM
 *         needs to call to confirm result of the operation).
 *
 *  \param [in] test
 *         The pointer to the RFCOMM_TEST data type describing the RFCOMM Test
 *         data pattern and size of the same. The memory for the test data
 *         pattern must be allocated by the application.
 *         The memory for the test data pattern must be available and resident
 *         till the Test procedure is ongoing.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Calling the Notification Callback with event_type set to \
 *        RFCOMM_SEND_TEST indicates completion of the RFCOMM Test procedure.
 *        The response parameter holds the result of the procedure - Successful
 *        or Unsuccessful Error Code.
 */
API_RESULT BT_rfcomm_send_test
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  RFCOMM_TEST *      test
           );

/**
 *  \brief To send Parameter Negotiation Frame to the Peer.
 *
 *  \par Description:
 *       This API enables the application to initiate RFCOMM Parameter
 *       Negotiation with the remote RFCOMM entity. The only useful parameters
 *       in the Parameter Negotiation (RFCOMM_PN) block are the DLC Frame Size
 *       and Priority.  Rest of the parameters are either ignored or not used by
 *       RFCOMM.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM Data Link Connection or
 *         Channel for which parameter needs to negotiated.
 *         The Parameter Negotiation may be initiated either before or after
 *         establishment of an RFCOMM Data Link Connection (DLC). If initiated
 *         prior to establishment of a DLC, the handle parameter must be
 *         initialized using RFCOMM_RESET_HANDLE() and RFCOMM_SET_HANDLE()
 *         utility macros. If initiated after establishment of a DLC, the handle
 *         parameter must have been obtained as a result of earlier BT_rfcomm_open()
 *         or BT_rfcomm_accept() operation.
 *
 *  \param [in] pn
 *         The pointer to the RFCOMM_PN structure describing the RFCOMM DLC
 *         Parameters that the local RFCOMM entity should propose to the peer
 *         entity. The frame_size parameter of RFCOMM_PN can be specified to be
 *         Zero. In that case, RFCOMM chooses the Frame Size for the Data Link
 *         Connection to be a value that is 5 octets less than (to account for
 *         the RFCOMM UIH Frame Header) the minimum of L2CAP In
 *         (see BT_rfcomm_register_mtu) and Out MTU values configured for the
 *         RFCOMM Session.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Calling the Notification Callback with event_type set to RFCOMM_SEND_PN
 *        indicates completion of the parameter negotiation procedure. The response
 *        parameter holds the result of the procedure - Successful or Unsuccessful
 *        Error Code.
 */
API_RESULT BT_rfcomm_send_pn
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  RFCOMM_PN *        pn
           );

/**
 *  \brief To initiate RFCOMM Remote Port Negotiation.
 *
 *  \par Description:
 *       This API enables the application to initiate RFCOMM Remote Port
 *       Negotiation (RPN) with the remote RFCOMM entity.
 *       The RPN can be of two types - RPN Request or RPN Command.
 *       The RPN Request attempts to retrieve the remote RFCOMM entity's port
 *       settings, while an RPN Command initiates negotiation of port settings
 *       with the remote RFCOMM entity.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM Data Link Connection or
 *         Channel for which parameter needs to negotiated.
 *         The Remote Port Negotiation (RPN Command) may be initiated either
 *         before or after establishment of an RFCOMM Data Link Connection (DLC).
 *         If initiated prior to establishment of a DLC, the handle parameter must
 *         be initialized using RFCOMM_RESET_HANDLE() and RFCOMM_SET_HANDLE()
 *         utility macros. If initiated after establishment of a DLC, the handle
 *         parameter must have been obtained as a result of earlier BT_rfcomm_open()
 *         or BT_rfcomm_accept() operation.
 *
 *  \param [in] rpn
 *         The pointer to the RFCOMM_RPN structure describing the RFCOMM Port
 *         Parameters that the local RFCOMM entity should propose to the peer
 *         entity.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Calling the Notification Callback with event_type set to RFCOMM_SEND_RPN
 *        indicates completion of the remote port negotiation procedure.
 *        The response parameter holds the result of the procedure - Successful
 *        or Unsuccessful Error Code.
 *        The data parameter will hold the pointer to a RFCOMM_RPN variable that
 *        describes the Remote RFCOMM entity's port settings. The datalen will
 *        be set to size of RFCOMM_RPN. If an RPN Request had been sent to the
 *        peer, the data parameter holds the current port settings of the peer
 *        device for the RFCOMM DLC. If an RPN Command had been sent to the peer,
 *        the data parameter holds the result and port settings that the peer has
 *        accepted or rejected (for more information see description of
 *        RFCOMM_RPN data type).
 */
API_RESULT BT_rfcomm_send_rpn
           (
               /* IN */     RFCOMM_HANDLE *    handle,
               /* INOUT */  RFCOMM_RPN *       rpn
           );

/**
 *  \brief To transmit Modem Status Command.
 *
 *  \par Description:
 *       This API enables the application to transmit Modem Status Commands to
 *       its peer as described by the Modem Status Command (RFCOMM_MSC) data type.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM Data Link Connection or
 *         Channel for which modem status command needs to be transmitted.
 *         The handle parameter must have been obtained as a result of earlier
 *         BT_rfcomm_open() or BT_rfcomm_accept() operation.
 *
 *  \param [in] msc
 *         The pointer of the RFCOMM_MSC structure describing the Modem Status
 *         that the local entity should transmit to its peer.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Calling the Notification Callback with event_type set to
 *        RFCOMM_SEND_MSC indicates completion of the transmission of modem
 *        status command procedure. The response parameter holds the result of
 *        the procedure - Successful or Unsuccessful Error Code.
 */
API_RESULT BT_rfcomm_send_msc
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  RFCOMM_MSC *       msc
           );

/**
 *  \brief To send Remote Line Status.
 *
 *  \par Description:
 *       This API enables the application to transmit Remote Line Status (RFCOMM_RLS)
 *       command to the peer RFCOMM entity.
 *
 *  \param [in] handle
 *         This parameter identifies the RFCOMM Handle corresponding to the
 *         RFCOMM DLC for which Line Status needs to be sent to the peer.
 *         The handle parameter must have been obtained as a result of earlier
 *         BT_rfcomm_open() or BT_rfcomm_accept() operation.
 *
 *  \param [in] rls
 *         The pointer to the RFCOMM_RLS data type which describes the
 *         line_status value that needs to be transmitted to the peer.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note Calling the Notification Callback with event_type set to
 *        RFCOMM_SEND_RLS indicates completion of the transmission of line status
 *        procedure. The response parameter holds the result of the procedure
 *        - Successful or Unsuccessful Error Code.
 */
API_RESULT BT_rfcomm_send_rls
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  RFCOMM_RLS *       rls
           );

/**
 *  \brief To get/retrieve local port settings for a DLC
 *
 *  \par Description:
 *       This API enables the application to get/retrieve the current local port
 *       settings for an RFCOMM data link connection.
 *       The local port settings are initialized to default values when a DLC is
 *       created. The local port settings should be updated by applications after
 *       port settings are negotiated and final values are accepted for use,
 *       by using the BT_rfcomm_send_rpn() API.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM Data Link Connection or
 *         Channel for which local port settings to be retrieved.
 *
 *  \param [out] rpn
 *         The pointer to the RFCOMM_RPN structure onto which the current local
 *         port settings for the data link connection to be copied. The dlci,
 *         request and mask parameters of the RFCOMM_RPN should be ignored.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 */
API_RESULT BT_rfcomm_get_local_rpn
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* OUT */ RFCOMM_RPN *       rpn
           );

/**
 *  \brief To set/update local port settings for a DLC
 *
 *  \par Description:
 *       This API enables the application to set/update the local port settings
 *       for an RFCOMM data link connection.
 *       The local port settings are initialized to default values when a DLC is
 *       created. The local port settings should be updated by applications after
 *       port settings are negotiated and final values are accepted for use,
 *       by using the BT_rfcomm_send_rpn() API.
 *       The RFCOMM uses the local port settings values in two ways:
 *        i)To reply to peer device in response to received RPN "Request"
 *       ii)To reply to peer device in response to received RPN "Command" for
 *       port negotiation. In this case, RFCOMM automatically accepts the changed
 *       port settings that the peer proposes and updates its local port settings.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM Data Link Connection or
 *         Channel for which local port settings to be updated.
 *
 *  \param [in] rpn
 *         The pointer to the RFCOMM_RPN structure describing the port settings
 *         that needs to be set/updated for the data link connection. The dlci,
 *         request and mask parameters of the RFCOMM_RPN are ignored.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 */
API_RESULT BT_rfcomm_set_local_rpn
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* IN */  RFCOMM_RPN *       rpn
           );

/**
 *  \brief To get parameter negotiation (PN) values for a DLC
 *
 *  \par Description:
 *       This API enables the application to get the parameter negotiation (PN)
 *       values for an RFCOMM data link connection.
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM Data Link Connection or
 *         Channel for which parameter negotiation (PN) values to be retrieved.
 *
 *  \param [in] pn
 *         The pointer to the RFCOMM_PN structure onto which the parameter
 *         negotiation (PN) values for the data link connection to be copied.
 *         The frame_size and priority parameters of the RFCOMM_PN are populated.
 *         Other parameters are used internally in RFCOMM, and should be ignored
 *         by application.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_rfcomm_get_local_pn
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* OUT */ RFCOMM_PN *        pn
           );

/**
 *  \brief To get local modem status command values for a DLC
 *
 *  \par Description:
 *       This API enables the application to get the local modem status command
 *       values for an RFCOMM data link connection.
 *       The RFCOMM maintains the local and remote Modem Status (MSC) parameters
 *       for every DLC. The local Modem Status parameter is updated when the local
 *       application sends the Modem Status Command to the peer device using the
 *       BT_rfcomm_send_msc() API. The remote Modem Status parameter is updated
 *       when the RFCOMM receives the Modem Status Command (MSC) from the peer
 *       device (which is indicated to application using RFCOMM_RECVD_MSC event).
 *
 *  \param [in] handle
 *         The RFCOMM Handle corresponding to the RFCOMM Data Link Connection or
 *         Channel for which local modem status (MSC) values to be retrieved.
 *
 *  \param [out] msc
 *         The pointer to the RFCOMM_MSC structure onto which the parameter
 *         negotiation (PN) values for the data link connection to be copied.
 *         The dlci parameter of the RFCOMM_MSC should be ignored by application.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_rfcomm_get_local_msc
           (
               /* IN */  RFCOMM_HANDLE *    handle,
               /* OUT */ RFCOMM_MSC *       msc
           );

/**
 *  \brief To set Incoming Data MTU value for L2CAP channel establishment.
 *
 *  \par Description:
 *       This API enables the application to specify L2CAP MTU size that RFCOMM
 *       will request to configure for the In-MTU (incoming data) parameter
 *       during L2CAP channel establishment (configuration) for an RFCOMM Session.
 *       The MTU can only be set when there exist no RFCOMM Session with any
 *       remote Bluetooth devices.
 *
 *  \param [in] mtu
 *         This parameter specifies the MTU size to be configured. The value of
 *         the MTU must be more than L2CAP_MIN_MTU (48) and less than
 *         L2CAP_MAX_MTU (see BT_limits.h).
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note RFCOMM calculates the Frame Size for all Data Link Connections
 *        (Server Channels) based on the MTU configured for underlying L2CAP
 *        connection. When initiating establishment of a data link connection
 *        (during parameter negotiation phase), the RFCOMM proposes a frame size
 *        value which is 5 octet less than (to account for the RFCOMM UIH Frame Header)
 *        the minimum of L2CAP In and Out MTU values configured for the RFCOMM Session.
 *        If application does not invoke this API to set the In-MTU for the
 *        L2CAP channel, then the default value L2CAP_MTU_DEFAULT (see BT_limits.h)
 *        is used.
 */
API_RESULT BT_rfcomm_register_mtu
           (
               /* IN */  UINT16    mtu
           );

#ifdef RFCOMM_FLUSHABLE_PACKET
/**
 *  \brief To mark the RFCOMM ACL data as flushable PDU.
 *
 *  \par Description:
 *       This API enables the application to set the RFCOMM data to be as
 *       flushable ACL data.
 *
 *  \param [in] handle
 *          The RFCOMM Handle corresponding to the RFCOMM DLC, on which the
 *          requested data configuration needs to be set. The handle parameter
 *          must have been obtained as a result of earlier BT_rfcomm_open()
 *          or BT_rfcomm_accept() operation.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note This API configures the flush-ability with the L2CAP call
 *        l2cap_set_flushability for the LCID corresponding to the RFCOMM DLCI
 *        in handle. This API is protected by the feature flag RFCOMM_FLUSHABLE_PACKET.
 */
API_RESULT BT_rfcomm_set_flushable
           (
               /* IN */ RFCOMM_HANDLE *    handle
           );
#endif /* RFCOMM_FLUSHABLE_PACKET */

#ifdef RFCOMM_USE_L2CAP_ERTM_RUNTIME_CONFIG
/**
 *  \brief To set the L2CAP operation mode for RFCOMM channel at runtime.
 *
 *  \par Description:
 *       This function sets the L2CAP operation mode for RFCOMM channels at runtime
 *
 *  \param [in] op_mode
 *         Takes either L2CAP_MODE_ERTM or L2CAP_MODE_BASIC
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 */
API_RESULT BT_rfcomm_set_l2cap_operation_mode
           (
               /* IN */ UCHAR op_mode
           );
#endif /* RFCOMM_USE_L2CAP_ERTM_RUNTIME_CONFIG */

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */

#endif /* _H_BT_RFCOMM_API_ */

