
/**
 *  \file BT_hid_host_api.h
 *
 *  This file contains the APIs and definitions exported by
 *  the HID Host Module
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */


#ifndef _H_BT_HID_HOST_API_
#define _H_BT_HID_HOST_API_

/* ----------------------------------- Header File Inclusion */
#include "BT_common.h"

/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup  hid_host_module  HID Host(Human Interface Device Profile - Host)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Human Interface Device Profile Host module to the Application and other upper
 *  layers of the stack.
 */
/* ----------------------------------- Type Definitions */

/**
 * \defgroup  hid_host_defines Defines
 * \{
 * Describes defines for HID Host module.
 */
/**
 * \defgroup hid_host_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/**
 * This value is the default Transmit MTU used in the Connection API when
 * it is set to 0 by the application.
 */
#define BT_HID_DEFAULT_TRANS_MTU                48U

/**
 * This value is the default Receive MTU used in the Connection API when
 * it is set to 0 by the application.
 */
#define BT_HID_DEFAULT_RECV_MTU                 48U

/**
 * This is the invalid or error value of the HID Host Handle. All handles
 * declared must be initialised to this value.
 */
#define BT_HID_INVALID_HANDLE                   0xFFU

/* ----------------------------------- Data Type/Structures/Typedefs */

/**
 * The list of HIDP Message Types
 */
#define    HID_HOST_HANDSHAKE                   0x00U
#define    HID_HOST_CONTROL                     0x01U

#define    HID_HOST_GET_REPORT                  0x04U
#define    HID_HOST_SET_REPORT                  0x05U
#define    HID_HOST_GET_PROTOCOL                0x06U
#define    HID_HOST_SET_PROTOCOL                0x07U
#define    HID_HOST_GET_IDLE                    0x08U
#define    HID_HOST_SET_IDLE                    0x09U
#define    HID_HOST_DATA                        0x0AU
#define    HID_HOST_DATC                        0x0BU

/**
 * The list of opcodes sent as part of the HID_CONTROL operation from the
 * host to the device.
 */
#define     HID_HOST_NOP                        0x00U
#define     HID_HOST_HARD_RESET                 0x01U
#define     HID_HOST_SOFT_RESET                 0x02U
#define     HID_HOST_SUSPEND                    0x03U
#define     HID_HOST_EXIT_SUSPEND               0x04U
#define     HID_HOST_VIRTUAL_CABLE_UNPLUG       0x05U

/**
 * The various types of reports requested by the host as part of the GET_REPORT
 * operation.
 */
#define     HID_HOST_REPORT_OTHER               0x00U
#define     HID_HOST_INPUT_REPORT               0x01U
#define     HID_HOST_OUTPUT_REPORT              0x02U
#define     HID_HOST_FEATURE_REPORT             0x03U

/**
 * The various protocols in which the device can operate.
 */
#define     HID_HOST_BOOT_PROTOCOL              0x00U
#define     HID_HOST_REPORT_PROTOCOL            0x01U

/**
 * List of result codes sent by HID device for Handshake Message.
 */
#define     HID_HOST_SUCCESS                    0x00U
#define     HID_HOST_NOT_READY                  0x01U
#define     HID_HOST_ERR_INVALID_REPORT_ID      0x02U
#define     HID_HOST_ERR_UNSUPPORTED_REQ        0x03U
#define     HID_HOST_ERR_INVALID_PARAM          0x04U
#define     HID_HOST_ERR_UNKNOWN                0x0EU
#define     HID_HOST_ERR_FATAL                  0x0FU

/**
 * The various events that occur in the HID HOST module that are
 * indicated to the upper layer.
 */
#define     HID_HOST_CTRL_CONNECTION            0x00U
/**
 * Control and Interrupt Disconnection values
 * are used as bit mask
 */
#define     HID_HOST_CTRL_DISCONNECTION         0x01U
#define     HID_HOST_INTR_DISCONNECTION         0x02U
#define     HID_HOST_INTR_CONNECTION            0x03U
/**
 * GET/SET Report/Protocol/Idle Response
 * Types has same value as their corresponding
 * Transaction Types.
 */
#define     HID_HOST_GET_REPORT_RSP             0x04U
#define     HID_HOST_SET_REPORT_RSP             0x05U
#define     HID_HOST_GET_PROTOCOL_RSP           0x06U
#define     HID_HOST_SET_PROTOCOL_RSP           0x07U
#define     HID_HOST_GET_IDLE_RSP               0x08U
#define     HID_HOST_SET_IDLE_RSP               0x09U
#define     HID_HOST_VIRTUAL_CABLE_UNPLUG_IND   0x0AU
/** \} */
/**
 * \defgroup hid_host_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
/**
 * Data type to associate a unique handle with HID Host instance
 */
typedef UCHAR HID_HOST_DEV_HANDLE;

/**
 * Data type to specify the HID control operation code
 */
typedef UCHAR HID_CONTROL_OP_CODE;

/**
 * Data type to specify the HID report type
 */
typedef UCHAR HID_REPORT_TYPE;

/**
 * Data type to specify the HID protocol type
 */
typedef UCHAR HID_PROTOCOL_TYPE;

/**
 * Data type to specify the HID Handshake result codes
 */
typedef UCHAR HID_HOST_HANDSHAKE_VALUES;

/**
 * Data type to specify thhe HID host events
 */
typedef UCHAR HID_HOST_EVT_TYPE;

/**
 * Data type to specify the HID message type
 */
typedef UCHAR HID_HOST_MSG_TYPE;

/**
 * The structure of the reports returned to the HID Host application. It
 * consists of the report data and its length. It also indicates the
 * type of the report i.e INPUT, OUTPUT or FEATURE
 */
typedef struct
{
    /** The report data */
    UCHAR            report[BT_HID_HOST_MAX_REPORT_LEN];

    /** The length of the Report */
    UINT16           report_len;

    /** The type of the report */
    HID_REPORT_TYPE  report_type;

} HID_HOST_REPORT;
/** \} */
/** \} */
/**
 * \defgroup hid_host_appl_callback Application Callback
 * \{
 * Describes Application Callback definitions of the module.
 */
/**
 * The callback function which returns all the incoming input reports from the
 * device to the upper layer application.
 */
typedef void ( *HID_INPUT_REP_IND_CALLBACK )
             (
                 HID_HOST_DEV_HANDLE   handle,
                 HID_HOST_REPORT     * report
             );

/**
 * The callback function which is called by HID Host to report
 * various asynchronous events that has occured to the application.
 */
typedef void ( *HID_HOST_EVENT_IND_CALLBACK )
             (
                 HID_HOST_EVT_TYPE      event_type,
                 HID_HOST_DEV_HANDLE    handle,
                 API_RESULT             result,
                 UCHAR               *  event_data,
                 UINT16                 event_datalen
             );
/** \} */

/*  --------------------------------------------- HID Host APIs */
#ifdef __cplusplus
extern "C"
{
#endif
/**
 * \defgroup hid_host_api_definitios API Definitions
 * \{
 * Describes HID Host API definitions of the module.
 */
/**
 *  \brief To initialize HID Host module
 *
 *  \par Description:
 *  This API is used to initialize the HID Host module.
 *  This must be done before using any other API of this module.
 *
 *  \param None
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_init( void );

/**
 *  \brief To start HID Host module
 *
 *  \par Description:
 *  This API is used to start the HID host module. This must be preceded by
 *  the initialization. The module is now ready to provide HID Host services.
 *
 *  \param None
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_start( void );

/**
 *  \brief To register with HID Host module
 *
 *  \par Description:
 *  This API is used to register callbacks for the HID host module.
 *  This must be preceded by the HID host start.
 *
 *  The application can specify the MTUs required. If not specified ( 0 ), the
 *  Receive MTU is configured to BT_HID_DEFAULT_RECV_MTU and Transmit MTU is
 *  configured to BT_HID_DEFAULT_TRANS_MTU. Both of these are currently set to
 *  48 bytes. The report_ind callback function is called every time there is an
 *  input report from the device and the event_ind callback function is called
 *  to deliver events like disconnection, etc.
 *
 *  \param [in] transmitMTU
 *         Transmit MTU
 *
 *  \param [in] recvMTU
 *         Receive MTU
 *
 *  \param [in] report_ind
 *         Callback function for input reports received from the device
 *
 *  \param [in] event_ind
 *         Callback function for events like, disconnection, etc
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT  BT_hid_host_register
            (
                /* IN */  UINT16                       transmitMTU,
                /* IN */  UINT16                       recvMTU,
                /* IN */  HID_INPUT_REP_IND_CALLBACK   report_ind,
                /* IN */  HID_HOST_EVENT_IND_CALLBACK  event_ind
            );

/**
 *  \brief To add a HID Device with relevant information and callbacks
 *
 *  \par Description:
 *  This API is used to indicate to the HID host module the Bluetooth
 *  Device Address of the HID device whose services are to be used.This
 *  API returns a HANDLE which is to be used as parameter to all APIs.
 *
 *  The application can specify the MTUs required. If not specified ( 0 ), the
 *  Receive MTU is configured to BT_HID_DEFAULT_RECV_MTU and Transmit MTU is
 *  configured to BT_HID_DEFAULT_TRANS_MTU. Both of these are currently set to
 *  48 bytes. The report_ind callback function is called every time there is an
 *  input report from the device and the event_ind callback function is called
 *  to deliver events like disconnection, etc.
 *
 *  \param [in] bdaddr
 *         Bluetooth Device Address of peer
 *
 *  \param [in] transmitMTU
 *         Transmit MTU
 *
 *  \param [in] recvMTU
 *         Receive MTU
 *
 *  \param [in] report_ind
 *         Callback function for input reports received from the device
 *
 *  \param [in] event_ind
 *         Callback function for events like, disconnection, etc
 *
 *  \param [out] handle
 *         Pointer to a caller provided HID_HOST_DEV_HANDLE variable,
 *         which will contain the HID Device Handle on return.
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_add_device
           (
               /* IN */  UCHAR                         * bdaddr,
               /* IN */  UINT16                          transmitMTU,
               /* IN */  UINT16                          recvMTU,
               /* IN */  HID_INPUT_REP_IND_CALLBACK      report_ind,
               /* IN */  HID_HOST_EVENT_IND_CALLBACK     event_ind,
               /* OUT */ HID_HOST_DEV_HANDLE           * handle
           );

/**
 *  \brief To get the handle for the added device
 *
 *  \par Description:
 *  This API is used to get the handle of the added device given its
 *  BD address.
 *
 *  \param [in] bdaddr
 *         Bluetooth Device Address of the added device
 *
 *  \param [out] handle
 *         Pointer to a caller provided HID_HOST_DEV_HANDLE variable,
 *         which will contain the HID Device Handle on return.
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_get_handle_from_addr
           (
               /* IN */ UCHAR                 * bdaddr,
               /* OUT */ HID_HOST_DEV_HANDLE  * handle
           );

/**
 *  \brief To get the address of the added device
 *
 *  \par Description:
 *  This API is used to get the BD address of the added device
 *  given its handle.
 *
 *  \param [in] handle
 *         HID Device Handle of the added device
 *
 *  \param [out] bdaddr
 *         Pointer to a caller provided UCHAR array,
 *         which will contain the Bluetooth Device Address on return.
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_get_addr_from_handle
           (
               /* IN */ HID_HOST_DEV_HANDLE    handle,
               /* OUT */ UCHAR               * bdaddr
           );

/**
 *  \brief To connect control channel to a HID Device
 *
 *  \par Description:
 *  This API initiates the control channel connection establishment procedure
 *  with the specified device.
 *  The peer HID Device is specified by the Device handle.
 *  Using the registered callback the result of connection operation will
 *  be indicated to the caller.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_connect_ctrl_ch_to_device
           (
               /* IN */ HID_HOST_DEV_HANDLE handle
           );

/**
 *  \brief To connect interrupt channel to a HID Device
 *
 *  \par Description:
 *  This API initiates the interrupt channel connection establishment procedure
 *  with the specified device.
 *  The peer HID Device is specified by the Device handle.
 *  Using the registered callback the result of connection operation will
 *  be indicated to the caller.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_connect_intr_ch_to_device
           (
               /* IN */ HID_HOST_DEV_HANDLE handle
           );

/**
 *  \brief To send HID Control packet
 *
 *  \par Description:
 *  This API is used to send the HID_CONTROL packet to the device. The
 *  control opcode to be sent is also specified. The device to which the
 *  opcode is to be sent is specified by the device handle.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \param [in] op_code
 *         OpCode for the HID_CONTROL packet
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 *
 *  \note
 *  A HID_CONTROL request does not generate a HANDSHAKE response.
 */
API_RESULT BT_hid_host_send_hid_control
           (
               /* IN */ HID_HOST_DEV_HANDLE handle,
               /* IN */ HID_CONTROL_OP_CODE op_code
           );

/**
 *  \brief To send Get Report Data request
 *
 *  \par Description:
 *  This API is used to perform the GET_REPORT operation. The report type
 *  requested is specified. The report received from the device is indicated
 *  to the application using the registered callback.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \param [in] rep_type
 *         Type of report to be obtained from the device.
 *
 *  \param [out] report
 *         Report received from device
 *
 *  \param [in] report_id
 *         The identifier for the report to be obtained
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 *
 *  \note
 *  Upon receipt of GET_REPORT request, the device will return
 *  a DATA payload on the Control channel containing the requested report.
 */
API_RESULT BT_hid_host_get_report
           (
               /* IN */  HID_HOST_DEV_HANDLE  handle,
               /* IN */  HID_REPORT_TYPE      rep_type,
               /* OUT */ HID_HOST_REPORT*     report,
               /* IN */  UCHAR                report_id
           );

/**
 *  \brief To send Set Report Data request
 *
 *  \par Description:
 *  This API is used to perform the SET_REPORT operation. The report to be
 *  sent along with its type is specified as input. The API returns success
 *  if a succesful HANDSHAKE was received else returns the appropriate error
 *  codes.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \param [in] report
 *         Pointer to the Report being sent to device
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_set_report
           (
               /* IN */ HID_HOST_DEV_HANDLE    handle,
               /* IN */ HID_HOST_REPORT      * report
           );

/**
 *  \brief To send Get Current Protocol request
 *
 *  \par Description:
 *  This API is used to get the Current Protocol of the HID Device
 *  specified by the handle.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 *
 *  \note
 *  The HID Device responds GET_PROTOCOL request with a single byte
 *  DATA payload that indicates the current protocol.
 */
API_RESULT BT_hid_host_get_protocol
           (
               /* IN */  HID_HOST_DEV_HANDLE handle
           );

/**
 *  \brief To send Change Current Protocol request
 *
 *  \par Description:
 *  This API is used to put the specified device on a particular protocol.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \param [in] protocol
 *         The protocol mode to be set on the device.
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 *
 *  \note
 *  If a HID Device does not support SET_PROTOCOL it shall return a HANDSHAKE
 *  packet indicating an unsupported request.
 */
API_RESULT BT_hid_host_set_protocol
           (
               /* IN */ HID_HOST_DEV_HANDLE  handle,
               /* IN */ HID_PROTOCOL_TYPE    protocol
           );

#ifdef HID_SUPPORT_GET_IDLE
/**
 *  \brief To send Get Current Idle Rate request
 *
 *  \par Description:
 *  This API is used to retrieve the idle rate of the specified Device.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 *
 *  \note
 *  HID Device responds GET_IDLE request with a single byte DATA payload
 *  that indicates the current Idle setting.
 */
API_RESULT BT_hid_host_get_idle_rate
           (
               /* IN */  HID_HOST_DEV_HANDLE handle
           );
#endif /* HID_SUPPORT_GET_IDLE */

#ifdef HID_SUPPORT_SET_IDLE
/**
 *  \brief To send Change Current Idle Rate request
 *
 *  \par Description:
 *  This API is used to set the idle rate of the specified device.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \param [in] new_idle_rate
 *         Idle Rate to which the device has to be set.
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_set_idle_rate
           (
               /* IN */ HID_HOST_DEV_HANDLE  handle,
               /* IN */ UCHAR                new_idle_rate
           );
#endif /* HID_SUPPORT_SET_IDLE */

/**
 *  \brief To send Virtual Cable Unplug request
 *
 *  \par Description:
 *  This API is used to initiate disconnection with remote device by sending
 *  the virtual cable unplug request to remote device. On receiving this
 *  request, the remote device will disconnect the control and interrupt
 *  channels.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_virtual_cable_unplug
           (
               /* IN */ HID_HOST_DEV_HANDLE handle
           );

/**
 *  \brief To initiate disconnection with a HID Device
 *
 *  \par Description:
 *  This API is used to disconnect the interrupt and control channels to
 *  the specified device. However the device details, such as PIN are available
 *  with the HID host and the handle to the device is still valid. The
 *  application can connect again using the same Handle.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \param [in] disconn_type
 *         Disconnection Type - HID_HOST_CTRL_DISCONNECTION or HID_HOST_INTR_DISCONNECTION
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT  BT_hid_host_disconnect_device
            (
                /* IN */ HID_HOST_DEV_HANDLE handle,
                /* IN */ UCHAR               disconn_type
            );

/**
 *  \brief To remove a HID Device from HID Host module list
 *
 *  \par Description:
 *  This API is used to remove a device from the HID host list. The handle
 *  hereafter is invalid. This can be done when the application will no longer
 *  connect to this device.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_remove_device
           (
               /* IN */ HID_HOST_DEV_HANDLE  handle
           );

/**
 *  \brief To stop HID Host module
 *
 *  \par Description:
 *  This API is used to stop all operations of the HID host. All existing
 *  connections are closed. Host needs to be restarted using the start API
 *  to be used again.
 *
 *  \param None
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_stop( void );

/**
 *  \brief To send report on an Interrupt Channel
 *
 *  \par Description:
 *  This API is used to send the reports to device on interrupt channel. The
 *  report to be sent along with its type is specified as input. The API
 *  returns success if a succesful HANDSHAKE was received else returns the
 *  appropriate error codes.
 *
 *  \param [in] handle
 *         HID Device Handle
 *
 *  \param [in] report
 *         Pointer to the Report being sent to device
 *
 *  \return API_RESULT:
 *          - API_SUCCESS, on successful initiation of the procedure.
 *          - Error Code, describing the cause of failure.
 */
API_RESULT BT_hid_host_send_report_on_interrupt
           (
               /* IN */ HID_HOST_DEV_HANDLE    handle,
               /* IN */ HID_HOST_REPORT      * report
           );

#ifdef __cplusplus
}
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_HID_HOST_API_ */

