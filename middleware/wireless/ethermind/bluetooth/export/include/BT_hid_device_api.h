
/**
 *  \file BT_hid_device_api.h
 *
 *  This file exports the APIs, data structures, typedefs and definitions
 *  of the HID Device Profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_HID_DEVICE_API_
#define _H_BT_HID_DEVICE_API_

/**
 * Header File Inclusion
 */
#include "BT_common.h"

/**
 * Global Definitions
 */

/**
 * Data Type/Structures/Typedefs
 */


/**
 *  Function Parameter Definition.
 */

/** @addtogroup TYPE_DEFINITIONS
 * @{
 */

/** ------------------------------------  Type Definition */

/** BT-HID Protocol HANDSHAKE Parameter Definitions */
#define HID_HANDSHAKE_SUCCESSFUL                     0
#define HID_HANDSHAKE_NOT_READY                      1
#define HID_HANDSHAKE_ERR_INVALID_REPORT_ID          2
#define HID_HANDSHAKE_ERR_UNSUPPORTED_REQUEST        3
#define HID_HANDSHAKE_ERR_INVALID_PARAMETER          4
#define HID_HANDSHAKE_ERR_UNKNOWN                    14
#define HID_HANDSHAKE_ERR_FATAL                      15

/** BT-HID Protocol HID_CONTROL Parameter Definitions */
#define HID_CONTROL_NOP                              0
#define HID_CONTROL_HARD_RESET                       1
#define HID_CONTROL_SOFT_RESET                       2
#define HID_CONTROL_SUSPEND                          3
#define HID_CONTROL_EXIT_SUSPEND                     4
#define HID_CONTROL_VIRTUAL_CABLE_UNPLUG             5

/** HID Protocol Report Types */
#define HID_INPUT_REPORT                             0x01
#define HID_OUTPUT_REPORT                            0x02
#define HID_FEATURE_REPORT                           0x03

/** BT-HID Protocol Types */
#define HID_BOOT_PROTOCOL                            0
#define HID_REPORT_PROTOCOL                          1

/** HID Device Profile GET_ Request Types */
#define HID_DEVICE_GET_REPORT                        6
#define HID_DEVICE_GET_PROTOCOL                      7
#define HID_DEVICE_GET_IDLE_RATE                     8

/** HID Device Profile SET_, OUT, CONTROL Request Types */
#define HID_DEVICE_SET_REPORT                        9
#define HID_DEVICE_SET_PROTOCOL                      10
#define HID_DEVICE_SET_IDLE_RATE                     11
#define HID_DEVICE_CONTROL                           12
#define HID_DEVICE_OUT_REPORT                        13

/** HID Device Profile Specific Event Types */
#define HID_DEVICE_CTRL_CH_CONNECTION                0x01
#define HID_DEVICE_CTRL_CH_CONNECTION_FAIL           0x02
#define HID_DEVICE_INTR_CH_CONNECTION                0x03
#define HID_DEVICE_INTR_CH_CONNECT_FAIL              0x04

#define HID_DEVICE_CTRL_CH_DISCONNECTION             0x05
#define HID_DEVICE_CTRL_CH_DISCONNECTION_FAIL        0x06
#define HID_DEVICE_INTR_CH_DISCONNECTION             0x07
#define HID_DEVICE_INTR_CH_DISCONNECTION_FAIL        0x08
#define HID_DEVICE_VIRTUAL_CABLE_UNPLUG_IND          0x09

/* #define HID_DEVICE_CONNECTED                      0x10 */
/* #define HID_DEVICE_VIRTUALLY_CABLED               0x11 */
/* #define HID_DEVICE_CONNECT_FAIL                   0x12 */
/* #define HID_DEVICE_DISCONNECTED                   0x13 */
/* #define HID_DEVICE_VIRTUALLY_UNPLUGGED            0x14 */
#define HID_DEVICE_LINK_LOST_EVT                     0x15
/* #define HID_DEVICE_CHANGE_LINK_KEY                0x17 */
/* #define HID_DEVICE_LINK_POLICY_FAIL               0x1A */
/* #define HID_DEVICE_LINK_POLICY_SUCCESS            0x1B */
/* #define HID_DEVICE_SCAN_ENABLE_FAIL               0x1C */
/* #define HID_DEVICE_SCAN_ENABLE_SUCCESS            0x1D */
/* #define HID_DEVICE_SUPERVISION_TIMEOUT_FAIL       0x1E */
/* #define HID_DEVICE_SUPERVISION_TIMEOUT_SUCCESS    0x1F */
/* #define HID_DEVICE_EXIT_PARK_FAIL                 0x20 */

#define HID_DEVICE_SEND_REPORT                       0x44
#define HID_DEVICE_INVALID_EVENT                     0xFF

/** Min Max Beacon Intervals for Park Mode */
#define HID_DEVICE_BEACON_MAX_INTERVAL               50
#define HID_DEVICE_BEACON_MIN_INTERVAL               30

/** MTU Size of the underlying L2cap Channels */
#define  HID_DEVICE_CONTROL_IN_MTU                   48
#define  HID_DEVICE_INTERRUPT_IN_MTU                 48

/* Maximum size(bytes) of the report the profile shall expect */
#define HID_DEVICE_MAX_REPORT_SIZE                   100

/** Default supervision timeout period for the device ( 1 second ) */
#define HID_DEVICE_SUPERVISION_TIMEOUT               0x666

/** Time profile shall wait for disconnecting 2nd l2cap channel */
#define HID_DEVICE_L2CAP_DISONNECT_TIMEOUT           1

/** Time profile shall wait for disconnecting 2nd l2cap channel */
#define HID_DEVICE_INTERRUPT_TIMEOUT                 0x8020

/** Default class of device for the profile (KBD ) */
#define HID_DEVICE_DEFAULT_CLASS_OF_DEVICE           0x00002540

/** Encryption mode type settings */
#define HID_DEVICE_ENCRYPTION_MODE_DISABLED          0x00
#define HID_DEVICE_ENCRYPTION_MODE_ENABLED_PTP       0x01
#define HID_DEVICE_ENCRYPTION_MODE_ENABLED_ALL       0x02

/** Default encryption mode for device */
#define HID_DEVICE_DEFAULT_ENCRYPTION_MODE  \
                            HID_DEVICE_ENCRYPTION_MODE_ENABLED_PTP

/** Authentication tupport type settings */
#define HID_DEVICE_AUTHENTICATION_DISABLED           0x00
#define HID_DEVICE_AUTHENTICATION_ENABLED            0x01

/** Default authentication setting for device */
#define HID_DEVICE_DEFAULT_AUTHENTICATION_TYPE   \
                                HID_DEVICE_AUTHENTICATION_DISABLED

/** PIN type settings */
#define HID_DEVICE_PIN_TYPE_VARIABLE                 0x00
#define HID_DEVICE_PIN_TYPE_FIXED                    0x01

/** Default pin type for the device */
#define HID_DEVICE_DEFAULT_PIN_TYPE   HID_DEVICE_PIN_TYPE_VARIABLE

/** Default pin type for the device */
#define HID_DEVICE_DEFAULT_NEW_LINK_KEY              0

/** Status of underlying host controller */
#define HID_DEVICE_HOST_CONTROLLER_UNINITIALIZED     0x00
#define HID_DEVICE_HOST_CONTROLLER_INITIALIZED       0x01

/* HID Channel types */
#define HID_CH_TYPE_CTRL                             0x01
#define HID_CH_TYPE_INTR                             0x02
#define HID_CH_TYPE_INVALID                          0xFF

/**
 *  Flag to allow role switch for the device at the time of
 *  connection establiment. Described as:
 *  0x1  -  Role Switch Allowed
 *  0x0  -  Role Switch Not Allowed
 */
#define HID_DEVICE_ROLE_SWITCH                       0x01

/** link policy settings */
#define HID_DEVICE_HOLD_MODE_ENABLE                  0x0002
#define HID_DEVICE_HOLD_MODE_DISABLE                 0x0000

#define HID_DEVICE_SNIFF_MODE_ENABLE                 0x0004
#define HID_DEVICE_HOLD_MODE_DISABLE                 0x0000

#define HID_DEVICE_PARK_MODE_ENABLE                  0x0008
#define HID_DEVICE_PARK_MODE_DISABLE                 0x0000

#define HID_DEVICE_ROLE_SWITCH_ENABLE                0x0001

/** Packet Types LMP shall use for the ACL connection */
#define HID_DEVICE_PACKET_TYPES  LMP_ACL_DM1 | LMP_ACL_DM3 | LMP_ACL_DM5 | \
                                 LMP_ACL_DH1 | LMP_ACL_DH3 | LMP_ACL_DH5

/**
 *  Device address used to describe an Invalid HID Host.
 *  This address is set as 0xFF : 0xFF : 0xFF : 0xFF : 0xFF : 0xFF
 */
extern const UCHAR HID_DEVICE_INVALID_HOST[];

/** @} */

/** @addtogroup DATA_STRUCTURES
 * @{
 */
/* --------------------------------- Data Type/Structures/Typedefs */

/** Structure to store the Profile Callbacks */
typedef struct
{
    /** Call back function to relay GET_ requests from HID Hosts */
    API_RESULT (*hid_device_get_request)
               (
                   /* IN */    UINT16 request_info,
                   /* IN */    UINT16 request_datalen,
                   /* INOUT */ UCHAR *request_data,
                   /* INOUT */ UINT16 *request_type
               );
    /*
     *  Usage Notes:
     *  The request_type parameter shall be used to determine the type of
     *  request, which can be either of HID_DEVICE_GET_REPORT,
     *  HID_DEVICE_GET_PROTOCOL and HID_DEVICE_GET_IDLE_RATE.
     *
     *  The request_type defines the usage of rest of parameters.
     *
     *  case 1;  request_type = HID_DEVICE_GET_REPORT
     *  request_info :  The 3 LSB bits are the report type requested.
     *                  Rest of the bits represent the report id requested.
     *  request_data :  The buffer to return the requested report.
     *  request_type :  Size of the returned report.
     *
     *  case 2;  request_type = HID_DEVICE_GET_PROTOCOL
     *  request_info :  Not carries any information; may be ignored by the
     *                  aplication.
     *  request_data :  Shall be used to return the protocol the device is
     *                  using.
     *  request_type :  Return Value will be length of valid data bytes.
     *                  For the time being, the profile will ignore bytes
     *                  other than the first.
     *
     *  case 3;  request_type = HID_DEVICE_GET_IDLE_RATE
     *  request_info :  Not carries any information; may be ignored by the
     *                  aplication.
     *  request_data :  Shall be used to return the idle rate the device is
     *                  using.
     *  request_type :  Return Value will be length of valid data bytes.
     *                  For the time being, the profile will ignore bytes
     *                  other than the first.
     *
     */

    /**
     *  Composite Call back function to relay SET_ requests, HID_CONTROL
     *  messages and HID Host OUT reports.
     */
    API_RESULT (*hid_device_set_request)
               (
                   /* IN */    UINT16 request_info,
                   /* IN */    UCHAR set_report_info,
                   /* IN */    UCHAR *request_data,
                   /* INOUT */ UINT16 *request_type
               );
    /*
     *  Usage Notes:
     *  The request_type parameter shall be used to determine the type of
     *  request, which can be either of HID_DEVICE_SET_REPORT,
     *  HID_DEVICE_SET_PROTOCOL, HID_DEVICE_SET_IDLE_RATE, HID_DEVICE_CONTROL
     *  and HID_DEVICE_OUT_REPORT.
     *
     *  The request_type defines the usage of rest of parameters. Only the
     *  relevant parameters are described for each type of request.
     *
     *  case 1;  request_type = HID_DEVICE_SET_REPORT
     *  request_info :  The size of the report.
     *  set_report_info :  Report type.
     *  request_data :  The buffer containing the specified report.
     *
     *  case 2;  request_type = HID_DEVICE_SET_PROTOCOL
     *  request_info :  The Protocol requested.
     *
     *  case 3;  request_type = HID_DEVICE_SET_IDLE_RATE
     *  request_info :  The idle rate requested.
     *
     *  case 4;  request_type = HID_DEVICE_OUT_REPORT
     *  These are the Interrupt Reports send by the HID Host over the
     *  INTERRUPT Channel. Valid fields are:
     *  request_info :  The size of input report.
     *  request_data :  The buffer containing the OUT Report
     *
     *  case 5;  request_type = HID_DEVICE_CONTROL
     *  These are the HID_CONTROL messages send by the HID Host.
     *  Valid fields are:
     *  request_info :  The parameter shall be used by the application to
     *                  determine the control message as specified by the
     *                  BT-HID Protocol. Examples can be switching Power Mode,
     *                  Hard_Reset, Soft_Reset etc.
     */

    /** Call back function to relay HID Device Profile Specific events */
    API_RESULT (*hid_device_events) 
               (
                   /* IN */    UCHAR event_type,
                   /* INOUT */ UCHAR *event_data,
                   /* INOUT */ UINT16 *event_data_len
               );

    /*
     *  Usage Notes:
     *  The event_type parameter shall be used to determine the type of
     *  event, which can be one of the defined constants for profile events.
     *  Examples are - HID_DEVICE_CONNECTED, HID_DEVICE_DISCONNECTED and
     *  HID_DEVICE_USER_PIN.
     *
     *  For the current implementation, the events are used to notify the
     *  application of profile events. Their usage is left upto the application.
     *
     *  Following is the description of Events the hid profile reports to
     *  applications.
     *  Note 1: Actual definitions of all event types precede with HID_DEVICE_
     *          suffix.
     *  Note 2: For space constraints the event type definitions have been
     *          continued in successive line.
     *  ---------------------------------------------------------------------
     *   Event Type                   |  Description        |  Parameters
     *  ---------------------------------------------------------------------
     *   _INITIALIZED                 | Device              |  None
     *                                | Initialization      |
     *                                | Notification        |
     *  ---------------------------------------------------------------------
     *   _WRITE_COD_FAIL              | Success or Failure  |  None
     *   _WRITE_COD_SUCCESS           | of Class of Device  |
     *                                | Setting             |
     *  ---------------------------------------------------------------------
     *   _AUTHENTICATION_COMPLETE     | Success or Failure  |  None
     *   _SUCCEED                     | of Authentication   |
     *   _AUTHENTICATION_COMPLETE_FAIL| Procedure           |
     *  ---------------------------------------------------------------------
     *   _EXIT_PARK_FAIL              | Success or Failure  |  None
     *   _EXIT_PARK_SUCCESS           | of Exit Park Mode   |
     *                                | Procedure           |
     *   --------------------------------------------------------------------
     *   _EXIT_SNIFF_FAIL             | Success or Failure  |  None
     *   _EXIT_SNIFF_SUCCESS          | of Exit Sniff Mode  |
     *                                | Procedure           |
     *   --------------------------------------------------------------------
     *   _WRITE_LAP_FAIL              | Success or Failure  |  None
     *   _WRITE_LAP_SUCCESS           | of LAP Setting      |
     *                                | Procedure           |
     *   --------------------------------------------------------------------
     *   _SCAN_ENABLE_FAIL            | Success or Failure  |  None
     *   _NOT_DISCOVERABLE            | of Scan Setting     |
     *   _DISCOVERABLE                | Procedure           |
     *   --------------------------------------------------------------------
     *   _LINK_POLICY_FAIL            | Success or Failure  |  None
     *   _LINK_POLICY_SUCCESS         | of Link Policy      |
     *                                | Setting Procedure   |
     *   --------------------------------------------------------------------
     *   _SUPERVISION_TIMEOUT_FAIL    | Success or Failure  |  None
     *   _SUPERVISION_TIMEOUT_SUCCESS | of Supervision      |
     *                                | Timeout Setting     |
     *                                | Procedure           |
     *   --------------------------------------------------------------------
     *   _AUTHENTICATION_ENABLE_FAIL  | Success or Failure  |  None
     *   _AUTHENTICATION_ENABLE_SUCCESS Authentication      |
     *                                | Enable Setting      |
     *                                | Procedure           |
     *   --------------------------------------------------------------------
     *   _ENCRYPTION_CHANGE_FAIL      | Success or Failure  |  None
     *   _ENCRYPTION_CHANGE_SUCCESS   | Encryption Mode     |
     *                                | Setting Procedure   |
     *   --------------------------------------------------------------------
     *   _PIN_CODE_REQ_NEG_REPLY_FAIL | Success or Failure  |  None
     *   _PIN_CODE_REQ_NEG_REPLY      | of Pin Code Negative|
     *   _SUCCESS                     | Reply Command       |
     *   --------------------------------------------------------------------
     *   _PIN_CODE_REQ_REPLY_FAIL     | Success or Failure  |  None
     *   _PIN_CODE_REQ_REPLY_SUCCESS  | of Pin Code         |
     *                                | Reply Command       |
     *   --------------------------------------------------------------------
     *   _LINKKEY_REQ_REPLY_FAIL      | Success or Failure  |  None
     *   _LINKKEY_REQ_REPLY_SUCCESS   | of Link Key         |
     *                                | Reply Command       |
     *   --------------------------------------------------------------------
     *   _LINKKEY_REQ_NEG_REPLY_FAIL  | Success or Failure  |  None
     *   _LINKKEY_REQ_NEG_REPLY_SUCCESS of Link Key Negative|
     *                                | Reply Command       |
     *   --------------------------------------------------------------------
     *   _ENCRYPTION_DISABLED         | Success Notification|  None
     *                                | For Encryption      |
     *                                | Disabled Setting    |
     *   --------------------------------------------------------------------
     *   _ENCRYPTION_ENABLED          | Success Notification|  None
     *                                | For Encryption      |
     *                                | Enabled Setting     |
     *   --------------------------------------------------------------------
     *   _AUTHENTICATION_COMPLETE_FAIL| Success or Failure  |  None
     *   _AUTHENTICATION_COMPLETE     | of Authentication   |
     *   _SUCCESS                     | Procedure           |
     *   --------------------------------------------------------------------
     *   _USER_PIN                    | Request Notification|  None
     *                                | For PIN             |
     *   --------------------------------------------------------------------
     *   _STORE_LINK_KEY              | Link Key Used For   | event_data_len:
     *                                | Connection          |   22 Bytes
     *                                |                     | event_data:
     *                                |                     |   6 Bytes of
     *                                |                     |   device address
     *                                |                     |   followed by
     *                                |                     |   16 Bytes of
     *                                |                     |   Link Key
     *   --------------------------------------------------------------------
     *   _LINK_KEY                    | Request Notification| event_data_len:
     *                                | For Link Key        |   6 Bytes
     *                                |                     | event_data:
     *                                |                     |   device address
     *   --------------------------------------------------------------------
     *   _PARK_SUCCESS                | Notification For    |  None
     *                                | Park Mode           |
     *   --------------------------------------------------------------------
     *   _SNIFF_SUCCESS               | Notification For    | evemt_data_len:
     *                                | Sniff Mode          |   1 Byte
     *                                |                     | event_data:
     *                                |                     |   Index of the
     *                                |                     |   Sniff Setting
     *                                |                     |   deployed
     *   --------------------------------------------------------------------
     *   _PARK_FAIL                   | Notification For    |  None
     *                                | Failure to Park     |
     *   --------------------------------------------------------------------
     *   _SNIFF_FAIL                  | Notification For    |  None
     *                                | Failure to Sniff    |
     *   --------------------------------------------------------------------
     *   _CONNECT_FAIL                | Failure Notification|  None
     *                                | For Connect Request |
     *   --------------------------------------------------------------------
     *   _CONNECTED                   | Success Notification| event_data_len:
     *                                | For Connect Request |   9 Bytes
     *                                |                     | event_data:
     *                                |                     |   6 Bytes of
     *                                |                     |   device address
     *                                |                     |   followed by
     *                                |                     |   3 Bytes of
     *                                |                     |   COD
     *                                |                     | ( Notification
     *                                |                     |   but no
     *                                |                     |   paramters for
     *                                |                     |   reconnection )
     *   --------------------------------------------------------------------
     *   _VIRTUALLY_CABLED            | Notification For    |  None
     *                                | Virtually Cabled    |
     *                                | Connection          |
     *   --------------------------------------------------------------------
     *   _LINK_LOST_EVT               | Notification For    |  None
     *                                | Link Loss           |
     *   --------------------------------------------------------------------
     *   _DISCONNECTED                | Notification For    |  None
     *                                | Disconnection       |
     *   --------------------------------------------------------------------
     *   _VIRTUALLY_UNPLUGGED         | Notification For    |  None
     *                                | Virtual Cable Unplug|
     *   --------------------------------------------------------------------
     *
     */

} HID_DEVICE_CALLBACK_STRUCT;


/* Structure to describe the power policy parameters for the application */
typedef struct
{
    /** sniff setting parameter */
    UINT16 sniff_interval;

    /** scheme setting duration */
    UINT16 sniff_duration;

} HID_DEVICE_SNIFF_PARAMS;


/** Structure to describe the initialization parameters for the profile */
typedef struct
{
    /**
     *  device attributes
     *
     *  BIT Fields are described as:
     *
     *    15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
     *   _________________________________________________
     *   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
     *   |__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
     *    <--------->  |  |  |  | <----> |  |  |  |  |  |
     *         |       |  |  |  |    |   |  |  |  |  |  |_ HID SDP Attr
     *         |       |  |  |  |    |   |  |  |  |  |     Virtually Cabled
     *         |       |  |  |  |    |   |  |  |  |  |
     *         |       |  |  |  |    |   |  |  |  |  |_ HID SDP Attr
     *         |       |  |  |  |    |   |  |  |  |     Reconnet Initiate
     *         |       |  |  |  |    |   |  |  |  |
     *         |       |  |  |  |    |   |  |  |  |_ HID SDP Attr
     *         |       |  |  |  |    |   |  |  |     Battery Power
     *         |       |  |  |  |    |   |  |  |
     *         |       |  |  |  |    |   |  |  |_ HID SDP Attr
     *         |       |  |  |  |    |   |  |     Remote Wake
     *         |       |  |  |  |    |   |  |
     *         |       |  |  |  |    |   |  |_ HID SDP Attr
     *         |       |  |  |  |    |   |     Normally Connectable
     *         |       |  |  |  |    |   |
     *         |       |  |  |  |    |   |_ HID SDP Attr
     *         |       |  |  |  |    |      Boot Device
     *         |       |  |  |  |    |
     *         |       |  |  |  |    |_ Encryption Mode
     *         |       |  |  |  |
     *         |       |  |  |  |_ Pin Type
     *         |       |  |  |
     *         |       |  |  |_ Authentication Support
     *         |       |  |
     *         |       |  |_ New Link Key For each Connection
     *         |       |
     *         |       |_Park Flag
     *         |
     *         |_________ Unused
     */
    UINT16 device_attrs;

} HID_DEVICE_INIT_PARAMS;


/** @} */

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  HID Device APIs
 */

/** The API is used to initialize the HID Device Profile */
API_RESULT BT_hid_device_init
         (
             /* IN */ HID_DEVICE_INIT_PARAMS *init_params
         );

/** The API is used to start the HID Device Profile */
API_RESULT BT_hid_device_start
           (
               /* IN */ HID_DEVICE_INIT_PARAMS *init_params
           );

/** The API is used to stop the HID Device Profile */
API_RESULT BT_hid_device_stop(void);

/**
 *  The API is used to set active status for desired host. An active host
 *  is one with which the virtually cable connection is made. This implies
 *  that when reconnection is desired, it shall be attempted by profile
 *  only towards this host.
 */
API_RESULT BT_hid_device_set_active_host
           (
               /* IN */ UCHAR *hid_host_bd_addr
           );

/** The API is used to register callbacks with the HID Device Profile */
API_RESULT BT_hid_device_register
           (
               /* IN */ HID_DEVICE_CALLBACK_STRUCT *hid_device_cb_stuct
           );

/** The API is used to initiate connection from the HID device */
API_RESULT BT_hid_device_connect
           (
               /* IN */ UCHAR *bd_addr,
               /* IN */ UCHAR ch_type
           );

/** The API is used to send INTERRUPT reports to the remote host */
API_RESULT BT_hid_device_send_report
           (
               /* IN */ UCHAR *report_data,
               /* IN */ UINT16 report_size
           );

/** The API is used to initiate reconnection to the last known HID host. */
API_RESULT BT_hid_device_initiate_reconnection(void);

/** The API is used to send responses to the HID Host */
API_RESULT BT_hid_device_send_response
           (
               /* IN */ UCHAR response_type,
               /* IN */ UCHAR response_status,
               /* IN */ UINT16 reponse_len,
               /* IN */ UCHAR *response_data
           );

/** The API is used to initiate disconnect from the remote HID Host */
API_RESULT BT_hid_device_disconnect
           (
               /* IN */ UCHAR disconn_type
           );

/**
 *  The API is used to disconnect from the remote host when the virtual
 *  Cable Configuration is employed.
 */
API_RESULT BT_hid_device_vc_unplug(void);

/** Function to receive bt on complete event. */
API_RESULT hid_device_bt_on_complete(void);

/** Function to determine the current active host */
API_RESULT BT_hid_device_get_active_host
           (
               /* OUT */ UCHAR *hid_host_bd_addr
           );

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_HID_DEVICE_API_ */
