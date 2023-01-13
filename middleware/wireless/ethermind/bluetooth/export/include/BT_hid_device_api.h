
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

/* ----------------------------------- Header File Inclusion */
#include "BT_common.h"

/* ----------------------------------- Global Definitions */
/**
 * \addtogroup bt_profiles Profiles
 * \{
 */
/**
 * \defgroup  hid_module  HID (Human Interface Device Profile)
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Human Interface Device Profile module to the Application and other upper
 *  layers of the stack.
 */

/**
 * \defgroup  hid_device_defines Defines
 * \{
 * Describes defines for HID Device module.
 */
/**
 * \defgroup hid_device_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/**
 * Data Type/Structures/Typedefs
 */


/**
 *  Function Parameter Definition.
 */

/** ------------------------------------  Type Definition */
/** BT-HID Protocol HANDSHAKE Parameter Definitions */
#define HID_HANDSHAKE_SUCCESSFUL                     0U
#define HID_HANDSHAKE_NOT_READY                      1U
#define HID_HANDSHAKE_ERR_INVALID_REPORT_ID          2U
#define HID_HANDSHAKE_ERR_UNSUPPORTED_REQUEST        3U
#define HID_HANDSHAKE_ERR_INVALID_PARAMETER          4U
#define HID_HANDSHAKE_ERR_UNKNOWN                    14U
#define HID_HANDSHAKE_ERR_FATAL                      15U

/** BT-HID Protocol HID_CONTROL Parameter Definitions */
#define HID_CONTROL_NOP                              0U
#define HID_CONTROL_HARD_RESET                       1U
#define HID_CONTROL_SOFT_RESET                       2U
#define HID_CONTROL_SUSPEND                          3U
#define HID_CONTROL_EXIT_SUSPEND                     4U
#define HID_CONTROL_VIRTUAL_CABLE_UNPLUG             5U

/** HID Protocol Report Types */
#define HID_INPUT_REPORT                             0x01U
#define HID_OUTPUT_REPORT                            0x02U
#define HID_FEATURE_REPORT                           0x03U

/** BT-HID Protocol Types */
#define HID_BOOT_PROTOCOL                            0U
#define HID_REPORT_PROTOCOL                          1U

/** HID Device Profile GET_ Request Types */
#define HID_DEVICE_GET_REPORT                        6U
#define HID_DEVICE_GET_PROTOCOL                      7U
#define HID_DEVICE_GET_IDLE_RATE                     8U

/** HID Device Profile SET_, OUT, CONTROL Request Types */
#define HID_DEVICE_SET_REPORT                        9U
#define HID_DEVICE_SET_PROTOCOL                      10U
#define HID_DEVICE_SET_IDLE_RATE                     11U
#define HID_DEVICE_CONTROL                           12U
#define HID_DEVICE_OUT_REPORT                        13U

/** HID Device Profile Specific Event Types */
#define HID_DEVICE_CTRL_CH_CONNECTION                0x01U
#define HID_DEVICE_CTRL_CH_CONNECTION_FAIL           0x02U
#define HID_DEVICE_INTR_CH_CONNECTION                0x03U
#define HID_DEVICE_INTR_CH_CONNECT_FAIL              0x04U

#define HID_DEVICE_CTRL_CH_DISCONNECTION             0x05U
#define HID_DEVICE_CTRL_CH_DISCONNECTION_FAIL        0x06U
#define HID_DEVICE_INTR_CH_DISCONNECTION             0x07U
#define HID_DEVICE_INTR_CH_DISCONNECTION_FAIL        0x08U
#define HID_DEVICE_VIRTUAL_CABLE_UNPLUG_IND          0x09U

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

#define HID_DEVICE_SEND_REPORT                       0x44U
#define HID_DEVICE_INVALID_EVENT                     0xFFU

/** Min Max Beacon Intervals for Park Mode */
#define HID_DEVICE_BEACON_MAX_INTERVAL               50U
#define HID_DEVICE_BEACON_MIN_INTERVAL               30U

/** MTU Size of the underlying L2cap Channels */
#define  HID_DEVICE_CONTROL_IN_MTU                   48U
#define  HID_DEVICE_INTERRUPT_IN_MTU                 48U

/* Maximum size(bytes) of the report the profile shall expect */
#define HID_DEVICE_MAX_REPORT_SIZE                   100U

/** Default supervision timeout period for the device ( 1 second ) */
#define HID_DEVICE_SUPERVISION_TIMEOUT               0x666U

/** Time profile shall wait for disconnecting 2nd l2cap channel */
#define HID_DEVICE_L2CAP_DISONNECT_TIMEOUT           1U

/** Time profile shall wait for disconnecting 2nd l2cap channel */
#define HID_DEVICE_INTERRUPT_TIMEOUT                 0x8020U

/** Default class of device for the profile (KBD ) */
#define HID_DEVICE_DEFAULT_CLASS_OF_DEVICE           0x00002540U

/** Encryption mode type settings */
#define HID_DEVICE_ENCRYPTION_MODE_DISABLED          0x00U
#define HID_DEVICE_ENCRYPTION_MODE_ENABLED_PTP       0x01U
#define HID_DEVICE_ENCRYPTION_MODE_ENABLED_ALL       0x02U

/** Default encryption mode for device */
#define HID_DEVICE_DEFAULT_ENCRYPTION_MODE  \
                            HID_DEVICE_ENCRYPTION_MODE_ENABLED_PTP

/** Authentication tupport type settings */
#define HID_DEVICE_AUTHENTICATION_DISABLED           0x00U
#define HID_DEVICE_AUTHENTICATION_ENABLED            0x01U

/** Default authentication setting for device */
#define HID_DEVICE_DEFAULT_AUTHENTICATION_TYPE   \
                                HID_DEVICE_AUTHENTICATION_DISABLED

/** PIN type settings */
#define HID_DEVICE_PIN_TYPE_VARIABLE                 0x00U
#define HID_DEVICE_PIN_TYPE_FIXED                    0x01U

/** Default pin type for the device */
#define HID_DEVICE_DEFAULT_PIN_TYPE   HID_DEVICE_PIN_TYPE_VARIABLE

/** Default pin type for the device */
#define HID_DEVICE_DEFAULT_NEW_LINK_KEY              0

/** Status of underlying host controller */
#define HID_DEVICE_HOST_CONTROLLER_UNINITIALIZED     0x00U
#define HID_DEVICE_HOST_CONTROLLER_INITIALIZED       0x01U

/* HID Channel types */
#define HID_CH_TYPE_CTRL                             0x01U
#define HID_CH_TYPE_INTR                             0x02U
#define HID_CH_TYPE_INVALID                          0xFFU

/**
 *  Flag to allow role switch for the device at the time of
 *  connection establiment. Described as:
 *  0x1  -  Role Switch Allowed
 *  0x0  -  Role Switch Not Allowed
 */
#define HID_DEVICE_ROLE_SWITCH                       0x01U

/** link policy settings */
#define HID_DEVICE_HOLD_MODE_ENABLE                  0x0002U
#define HID_DEVICE_HOLD_MODE_DISABLE                 0x0000U

#define HID_DEVICE_SNIFF_MODE_ENABLE                 0x0004U
#define HID_DEVICE_HOLD_MODE_DISABLE                 0x0000U

#define HID_DEVICE_PARK_MODE_ENABLE                  0x0008U
#define HID_DEVICE_PARK_MODE_DISABLE                 0x0000U

#define HID_DEVICE_ROLE_SWITCH_ENABLE                0x0001U

/** Packet Types LMP shall use for the ACL connection */
#define HID_DEVICE_PACKET_TYPES  LMP_ACL_DM1 | LMP_ACL_DM3 | LMP_ACL_DM5 | \
                                 LMP_ACL_DH1 | LMP_ACL_DH3 | LMP_ACL_DH5
/** \} */
/**
 *  Device address used to describe an Invalid HID Host.
 *  This address is set as 0xFF : 0xFF : 0xFF : 0xFF : 0xFF : 0xFF
 */
extern const UCHAR HID_DEVICE_INVALID_HOST[];

/** @} */

/* --------------------------------- Data Type/Structures/Typedefs */
/**
 *  \addtogroup hid_device_defines Defines
 *  \{
 */

/**
 * \defgroup hid_device_structures Structures
 * \{
 * Describes Structures defined by the module.
 */

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
/** \} */
/** \} */

/*  --------------------------------------------- HID Device APIs */
/**
 * \defgroup  hid_api API Definitions
 * \{
 * Describes API definitions for this module.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 *  HID Device APIs
 */

/** The API is used to initialize the HID Device Profile */
/**
 *  \brief To initialize the HID device profile.
 *
 *  \par Description:
 *       The API initializes the HID Device Profile.
 *       This API must be called before other operations on the profile are
 *       performed. It is important to set the different values of the
 *       init_params structure to ensure a correct and predictable behavior of
 *       the profile. The init_params are those parameters that may be part of
 *       SDP or that affect the functionality of the profile.
 *       Please refer to structure definition of HID_DEVICE_INIT_PARAMS for more
 *       detailed description of the different parameters that must be
 *       initialized before passing it to this API.
 *
 *  \param [in] init_params
 *         Pointer to structure describing the initialization parameters for the profile.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_init
         (
             /* IN */ HID_DEVICE_INIT_PARAMS *init_params
         );

/** The API is used to start the HID Device Profile */
/**
 *  \brief To start the HID device profile.
 *
 *  \par Description:
 *       The API starts the HID Device Profile.
 *       This must be preceded by the initialization. The module is now ready to
 *       perform HID services.
 *
 *  \param [in] init_params
 *         Pointer to structure describing the initialization parameters for the profile.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_start
           (
               /* IN */ HID_DEVICE_INIT_PARAMS *init_params
           );

/**
 *  \brief To stop the HID device profile
 *
 *  \par Description:
 *       The API stops the HID Device Profile.
 *       This API is used to stop all operations of the HID Device Module.
 *       The existing HID connection is closed. The Module needs to be restarted
 *       using the start API to be used again.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_stop(void);

/**
 *  The API is used to set active status for desired host. An active host
 *  is one with which the virtually cable connection is made. This implies
 *  that when reconnection is desired, it shall be attempted by profile
 *  only towards this host.
 */
/**
 *  \brief To set the active status for desired host.
 *
 *  \par Description:
 *       The API is used to set active status for desired host. An active host
 *       is one with which the virtually cable connection is made. This implies
 *       that when reconnection is desired, it shall be attempted by profile
 *       only towards this host.
 *
 *  \param [in] hid_host_bd_addr
 *         BD_ADDR of the host which has to be set active.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_set_active_host
           (
               /* IN */ UCHAR *hid_host_bd_addr
           );

/**
 *  \brief To register callbacks with the HID Device Profile.
 *
 *  \par Description:
 *       The API is used to register profile callback functions.
 *       The profile callback functions are a way by which the calling
 *       application is given information about the various events that may be
 *       occurring at the profile level. Please refer to the structure
 *       hid_device_cb_struct for a more detailed explanation of the call back.
 *       The callback functions are:
 *       i)hid_device_get_request:relays HID GET_ requests to the application
 *       ii)hid_device_get_request:relay SET_ requests, HID_CONTROL messages and
 *          HID Host OUT reports.
 *       iii)hid_device_events :Callback function to relay HID Device Profile
 *           Specific events
 *
 *  \param [in] hid_device_cb_struct
 *         Pointer to structure storing the profile callbacks.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note NULL parameters passed to the function imply no callbacks will
 *        registered with the profile.
 */
API_RESULT BT_hid_device_register
           (
               /* IN */ HID_DEVICE_CALLBACK_STRUCT *hid_device_cb_struct
           );

/**
 *  \brief To initiate connection from the HID device.
 *
 *  \par Description:
 *       The API is used to initiate connection from the device to the HID host.
 *       This API has been provided to allow the application to connect to a HID
 *       host, if it wants to. In a general deployment, the device never
 *       attempts to connect to a host - so the calling of this API is
 *       completely at the discretion of the application.
 *
 *  \param [in] bd_addr
 *         bd_addr is the BD ADDR of the remote host.
 *
 *  \param [in] ch_type
 *         Type of channel
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_connect
           (
               /* IN */ UCHAR *bd_addr,
               /* IN */ UCHAR ch_type
           );

/**
 *  \brief To send INTERRUPT reports to the remote host.
 *
 *  \par Description:
 *       The API is used to send INTERRUPT reports to the remote host.
 *       Note that the API takes care of appending appropriate BT_HID headers t
 *       the report_data. The input report_data must be formatted correctly
 *       only as per the USB HID specifications.
 *
 *  \param [in] report_data
 *         The HID Report. The report data is constructed from the data read
 *         from the device controller and conforms to the USB HID report format.
 *         This function will append the necessary BT HID protocol headers.
 *
 *  \param [in] report_size
 *         Size of the Report.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_send_report
           (
               /* IN */ UCHAR *report_data,
               /* IN */ UINT16 report_size
           );

/**
 *  \brief To initiate reconnection to the last known HID host
 *
 *  \par Description:
 *       The API is used to initiate reconnection to the last known host.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_initiate_reconnection(void);

/**
 *  \brief To send responses to the HID Host
 *
 *  \par Description:
 *       The API is used to respond to the HID host requests. Whenever a HID
 *       protocol request (e.g. GET_PROTOCOL, SET_PROTOCOL) comes from the host,
 *       this information is relayed to the application that can then decide
 *       how to respond to the request.
 *
 *  \param [in] response_type
 *         Identifier for type of response.
 *
 *  \param [in] response_status
 *         Status of the response - successful/ erroneous.
 *
 *  \param [in] response_len
 *         Size of the response packet buffer.
 *
 *  \param [in] response_data
 *         Response packet buffer.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note The API takes care of appending appropriate BT_HID headers to the response.
 */
API_RESULT BT_hid_device_send_response
           (
               /* IN */ UCHAR response_type,
               /* IN */ UCHAR response_status,
               /* IN */ UINT16 response_len,
               /* IN */ UCHAR *response_data
           );

/**
 *  \brief To initiate disconnect from the remote HID Host.
 *
 *  \par Description:
 *       The API is used to disconnect from the remote host.
 *
 *  \param [in] disconn_type
 *         Type of disconnection
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_disconnect
           (
               /* IN */ UCHAR disconn_type
           );

/**
 *  The API is used to disconnect from the remote host when the virtual
 *  Cable Configuration is employed.
 */
/**
 *  \brief To disconnect from remote host when virtual cable configuration is employed
 *
 *  \par Description:
 *       The API is used to disconnect from the remote host when the virtual
 *       Cable Configuration is employed.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 */
API_RESULT BT_hid_device_vc_unplug(void);

/**
 *  \brief To determine the current active host.
 *
 *  \par Description:
 *       This API is used to determine the current active host.
 *
 *  \param [in] hid_host_bd_addr
 *         BD_ADDR of the host which has to be set active.
 *
 *  \return
 *       API_SUCCESS or one of the error codes as defined in \ref BLE_ERROR_CODES.
 *
 *  \note
 */
API_RESULT BT_hid_device_get_active_host
           (
               /* OUT */ UCHAR *hid_host_bd_addr
           );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_HID_DEVICE_API_ */

