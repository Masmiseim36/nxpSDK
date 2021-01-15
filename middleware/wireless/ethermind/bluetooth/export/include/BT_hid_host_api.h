
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

/* ----------------------------------- Type Definitions */

/**
 * This value is the default Transmit MTU used in the Connection API when
 * it is set to 0 by the application.
 */
#define BT_HID_DEFAULT_TRANS_MTU                48

/**
 * This value is the default Receive MTU used in the Connection API when
 * it is set to 0 by the application.
 */
#define BT_HID_DEFAULT_RECV_MTU                 48

/**
 * This is the invalid or error value of the HID Host Handle. All handles
 * declared must be initialised to this value.
 */
#define BT_HID_INVALID_HANDLE                   0xFF

/* ----------------------------------- Data Type/Structures/Typedefs */

/**
 * The list of HIDP Message Types
 */
#define    HID_HOST_HANDSHAKE                   0x00
#define    HID_HOST_CONTROL                     0x01

#define    HID_HOST_GET_REPORT                  0x04
#define    HID_HOST_SET_REPORT                  0x05
#define    HID_HOST_GET_PROTOCOL                0x06
#define    HID_HOST_SET_PROTOCOL                0x07
#define    HID_HOST_GET_IDLE                    0x08
#define    HID_HOST_SET_IDLE                    0x09
#define    HID_HOST_DATA                        0x0A
#define    HID_HOST_DATC                        0x0B

/**
 * The list of opcodes sent as part of the HID_CONTROL operation from the
 * host to the device.
 */
#define     HID_HOST_NOP                        0x00
#define     HID_HOST_HARD_RESET                 0x01
#define     HID_HOST_SOFT_RESET                 0x02
#define     HID_HOST_SUSPEND                    0x03
#define     HID_HOST_EXIT_SUSPEND               0x04
#define     HID_HOST_VIRTUAL_CABLE_UNPLUG       0x05

/**
 * The various types of reports requested by the host as part of the GET_REPORT
 * operation.
 */
#define     HID_HOST_REPORT_OTHER               0x00
#define     HID_HOST_INPUT_REPORT               0x01
#define     HID_HOST_OUTPUT_REPORT              0x02
#define     HID_HOST_FEATURE_REPORT             0x03

/**
 * The various protocols in which the device can operate.
 */
#define     HID_HOST_BOOT_PROTOCOL              0x00
#define     HID_HOST_REPORT_PROTOCOL            0x01

/**
 * List of result codes sent by HID device for Handshake Message.
 */
#define     HID_HOST_SUCCESS                    0x00
#define     HID_HOST_NOT_READY                  0x01
#define     HID_HOST_ERR_INVALID_REPORT_ID      0x02
#define     HID_HOST_ERR_UNSUPPORTED_REQ        0x03
#define     HID_HOST_ERR_INVALID_PARAM          0x04
#define     HID_HOST_ERR_UNKNOWN                0x0E
#define     HID_HOST_ERR_FATAL                  0x0F

/**
 * The various events that occur in the HID HOST module that are
 * indicated to the upper layer.
 */
#define     HID_HOST_CTRL_CONNECTION            0x00
/**
 * Control and Interrupt Disconnection values
 * are used as bit mask
 */
#define     HID_HOST_CTRL_DISCONNECTION         0x01
#define     HID_HOST_INTR_DISCONNECTION         0x02
#define     HID_HOST_INTR_CONNECTION            0x03
/**
 * GET/SET Report/Protocol/Idle Response
 * Types has same value as their corresponding
 * Transaction Types.
 */
#define     HID_HOST_GET_REPORT_RSP             0x04
#define     HID_HOST_SET_REPORT_RSP             0x05
#define     HID_HOST_GET_PROTOCOL_RSP           0x06
#define     HID_HOST_SET_PROTOCOL_RSP           0x07
#define     HID_HOST_GET_IDLE_RSP               0x08
#define     HID_HOST_SET_IDLE_RSP               0x09
#define     HID_HOST_VIRTUAL_CABLE_UNPLUG_IND   0x0A

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
    /* The report data */
    UCHAR            report[BT_HID_HOST_MAX_REPORT_LEN];

    /* The length of the Report */
    UINT16           report_len;

    /* The type of the report */
    HID_REPORT_TYPE  report_type;

} HID_HOST_REPORT;

/**
 * The callback function which returns all the incoming input reports from the
 * device to the upper layer application.
 */
typedef void ( *HID_INPUT_REP_IND_CALLBACK )
             (
                 HID_HOST_DEV_HANDLE,
                 HID_HOST_REPORT*
             );

/**
 * The callback function which is called by HID Host to report
 * various asynchronous events that has occured to the application.
 */
typedef void ( *HID_HOST_EVENT_IND_CALLBACK )
             (
                 HID_HOST_EVT_TYPE,
                 HID_HOST_DEV_HANDLE,
                 API_RESULT,
                 UCHAR *,
                 UINT16
             );


/*  --------------------------------------------- HID Host APIs */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * This API is used to initialise the HID host module. This must be done
 * before using any other APIs of this module.
 */
API_RESULT BT_hid_host_init( void );

/**
 * This API is used to start the HID host module. This must be preceeded by
 * the initialisation. It starts the various threads of the HID host module.
 * the module is now ready to perform HID services.
 */
API_RESULT BT_hid_host_start( void );

/**
 * This API is used to register the default callback functions with the
 * HID host module. This must be preceeded by the HID Host start.
 */
API_RESULT BT_hid_host_register
           (
               /* IN */  UINT16 transmitMTU,
               /* IN */  UINT16 recvMTU,
               /* IN */  HID_INPUT_REP_IND_CALLBACK report_ind,
               /* IN */  HID_HOST_EVENT_IND_CALLBACK event_ind
           );

/**
 * This API is used to indicate to the HID host module the Bluetooth
 * Device Address of the HID device who's services are to be used. This API
 * returns a HANDLE which is to be used as parameter for all the APIs.
 */
API_RESULT BT_hid_host_add_device
           (
               /* IN */  UCHAR *bdaddr,
               /* IN */  UINT16 transmitMTU,
               /* IN */  UINT16 recvMTU,
               /* IN */  HID_INPUT_REP_IND_CALLBACK report_ind,
               /* IN */  HID_HOST_EVENT_IND_CALLBACK event_ind,
               /* OUT */ HID_HOST_DEV_HANDLE*
           );

/**
 * This API is used to get the handle of the added device
 * for given BD address.
 */
API_RESULT BT_hid_host_get_handle_from_addr
           (
               /* IN */  UCHAR *dev_addr,
               /* OUT */ HID_HOST_DEV_HANDLE *handle
           );

/**
 * This API is used to get the BD address of the added device
 * for given handle.
 */
API_RESULT BT_hid_host_get_addr_from_handle
           (
               /* IN */ HID_HOST_DEV_HANDLE handle,
               /* OUT */ UCHAR *dev_addr
           );

/**
 * This API establishes the Control Channel with the specified device.
 * The device is specified by the Device handle.
 * After the connection is established, the channel is configured.
 * The application can specify the MTUs required. If not specified ( 0 ), the
 * Receive MTU is configured to BT_HID_DEFAULT_RECV_MTU and Transmit MTU is
 * configured to BT_HID_DEFAULT_TRANS_MTU. Both of these are currently set to
 * 48 bytes. The API returns success when control channel is successfully
 * configured.
 */
API_RESULT BT_hid_host_connect_ctrl_ch_to_device
           (
               /* IN */ HID_HOST_DEV_HANDLE
           );

/**
 * This API establishes the Interrupt channel with the specified device.
 * The device is specified by the Device  handle.
 * After the connection is established, the channel is configured.
 * The application can specify the MTUs required. If not specified ( 0 ), the
 * Receive MTU is configured to BT_HID_DEFAULT_RECV_MTU and Transmit MTU is
 * configured to BT_HID_DEFAULT_TRANS_MTU. Both of these are currently set to
 * 48 bytes. The API returns success when interrupt channel is successfully
 * configured.
 */
API_RESULT BT_hid_host_connect_intr_ch_to_device
           (
               /* IN */ HID_HOST_DEV_HANDLE
           );

/**
 * This API is used to send the HID_CONTROL packet to the Device. The control
 * opcode to be sent is also specified. The device to which the opcode
 * is to be sent is specified by the device handle.
 */
API_RESULT BT_hid_host_send_hid_control
           (
               /* IN */ HID_HOST_DEV_HANDLE,
               /* IN */ HID_CONTROL_OP_CODE
           );

/**
 * This API is used to perform the GET_REPORT operation. The report type
 * requested is specified. The report received from the Device is returned
 * in the HID_HOST_REPORT structure. A report id can be optionally sent. If
 * the report id value is a non zero value then it is sent.
 */
API_RESULT BT_hid_host_get_report
           (
               /* IN */  HID_HOST_DEV_HANDLE,
               /* IN */  HID_REPORT_TYPE,
               /* OUT */ HID_HOST_REPORT*,
               /* IN */  UCHAR   report_id
           );

/**
 * This API is used to perform the SET_REPORT operation. The report to be
 * sent along with its type is specified as input. The API returns success
 * if a succesful HANDSHAKE was received else returns the appropriate error
 * codes.
 */
API_RESULT BT_hid_host_set_report
           (
               /* IN */ HID_HOST_DEV_HANDLE,
               /* IN */ HID_HOST_REPORT *
           );

/**
 * This API is used to get the current protocol mode of the device specified by the
 * handle.
 */
API_RESULT BT_hid_host_get_protocol
           (
               /* IN */  HID_HOST_DEV_HANDLE
           );

/**
 * This API is used to put the specified device on a particular protocol mode.
 */
API_RESULT BT_hid_host_set_protocol
           (
               /* IN */ HID_HOST_DEV_HANDLE,
               /* IN */ HID_PROTOCOL_TYPE
           );

#ifdef HID_SUPPORT_GET_IDLE
/**
 * This API is used to retrieve the idle rate of the specified Device.
 */
API_RESULT BT_hid_host_get_idle_rate
           (
               /* IN */  HID_HOST_DEV_HANDLE
           );
#endif /* HID_SUPPORT_GET_IDLE */

#ifdef HID_SUPPORT_SET_IDLE
/**
 * This API is used to set the idle rate of the specified device.
 */
API_RESULT BT_hid_host_set_idle_rate
           (
               /* IN */ HID_HOST_DEV_HANDLE,
               /* IN */ UCHAR new_idle_rate
           );
#endif /* HID_SUPPORT_SET_IDLE */

/**
 *  This API is used to initiate disconnection with remote device by sending
 *  the virtual cable unplug request to remote device. On receiving this
 *  request, the remote device will disconnect the control and interrupt
 *  channels.
 */
API_RESULT BT_hid_host_virtual_cable_unplug
           (
               /* IN */ HID_HOST_DEV_HANDLE handle
           );

/**
 * This API is used to disconnect the Interrupt and Control Channels with
 * the specified device. However the device details, such as PIN are available
 * with the HID Host Module and the handle to the device is still valid. The
 * application can conect again using the same Handle.
 */
API_RESULT BT_hid_host_disconnect_device
           (
               /* IN */ HID_HOST_DEV_HANDLE handle,
               /* IN */ UCHAR               disconn_type
           );

/**
 * This API is used to remove a device from the HID Host module list. The
 * handle hereafter is invalid. This can be done when the application will
 * no longer connect to this device.
 */
API_RESULT BT_hid_host_remove_device
           (
               /* IN */ HID_HOST_DEV_HANDLE
           );

/**
 * This API is used to stop all operations of the HID Host Module. All
 * existing connections will be closed. The Module needs to be restarted using
 * the start API to use again.
 */
API_RESULT BT_hid_host_stop( void );

/**
 *  This API is used to send report data on interrupt channel
 */
API_RESULT BT_hid_host_send_report_on_interrupt
           (
               /* IN */ HID_HOST_DEV_HANDLE handle,
               /* IN */ HID_HOST_REPORT *report
           );

#ifdef __cplusplus
}
#endif

#endif /* _H_BT_HID_HOST_API_ */

