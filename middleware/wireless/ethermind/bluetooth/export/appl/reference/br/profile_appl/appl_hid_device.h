
/**
 *  \file appl_hid_device.h
 *
 *  Header file for HID Device Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HID_DEVICE_
#define _H_APPL_HID_DEVICE_

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "BT_sm_api.h"
#include "BT_sdp_api.h"
#include "BT_hid_device_api.h"

#include "BT_dbase_api.h"
#include "db_gen.h"

/* HID APPL Definitions */
/** @addtogroup TYPE_DEFINITIONS
 * @{
 */

/** return status */
#define HID_APPL_SUCCESSFUL                  0x00U
#define HID_APPL_FAILURE                     0x01U

/** Default SDP Parameters */
#define HID_APPL_PROFILE_PARSER_VERSION      0x0111U
#define HID_APPL_SDP_DISABLE_ATTR            0x01U
#ifdef HID_MOUSE
#define HID_APPL_COD                         0x000002580U
#else
#define HID_APPL_COD                         0x000002540U
#endif
#define HID_APPL_DISCOVERY_TIME              0x30U

#define HID_APPL_MAX_SNIFF_PARAMS            0x2U
#define HID_APPL_SNIFF_INTERVAL_1            20U
#define HID_APPL_SNIFF_INTERVAL_2            40U
#define HID_APPL_SNIFF_DURATION_1            10U
#define HID_APPL_SNIFF_DURATION_2            30U

#define HID_APPL_VIRTUAL_CABLED              0x1U
#define HID_APPL_RECONNECT_INITIATE          0x0U
#define HID_APPL_REMOTE_WAKE                 0x1U
#define HID_APPL_NORMALLY_CONNECTABLE        0x1U
#define HID_APPL_BOOT_DEVICE                 0x1U
#define HID_APPL_BATTERY_DEVICE              0x1U

#define HID_APPL_COUNTRY_CODE                0x0U
#define HID_APPL_SUB_CLASS                   0x80U
#define HID_APPL_PROFILE_VERSION             0x0011U
#define HID_APPL_RELEASE_NUMBER              0x0100U
#define HID_APPL_SUPERVISION_TO              HID_DEVICE_SUPERVISION_TIMEOUT

/** Mouse Boot Id */
#define HID_APPL_MOUSE_BOOT                  0x02U

/** Maximum length of HID report */
#define HID_APPL_MAX_REPORT_LENGTH           0x10U

/** Maximum size of buffer for SDP Record */
#define HID_APPL_MAX_SDP_SIZE                0x174U

/** Type of mouse */
#define HID_APPL_MOUSE_IMPLEMENTATION_1      0x0U
#define HID_APPL_MOUSE_IMPLEMENTATION_2      0x1U

/** Number of bytes read at a time from UART */
#define HID_APPL_READ_LEN                    0x4U

/* local device name */
#define HID_APPL_DEVICE_NAME                 "ethermind_mouse"
#define HID_APPL_DEVICE_NAME_LEN             15U

#define HID_APPL_CONNECT                     0x1U
#define HID_APPL_REINITIATE_CONNECT          0x2U



/** @} */


/**
 *  Function Definitions
 */

#if 0
/* funtion to create the SDP database */
void hid_appl_sdp_init ( void );

/* funtion to initialize the device aplication */
void hid_appl_init ( void );
#endif /* 0 */

/* Callbacks registered with the profile */
API_RESULT hid_appl_get_request
           (
               /* IN */ UINT16 req_info,
               /* IN */ UINT16 req_datalen,
               /* IN */ UCHAR *req_data,
               /* IN */ UINT16 *req_type
           );

API_RESULT hid_appl_set_request
           (
               /* IN */ UINT16 req_info,
               /* IN */ UCHAR set_req_info,
               /* IN */ UCHAR *req_data,
               /* IN */ UINT16 *req_type
           );

API_RESULT hid_appl_get_event(
                                 /* IN */ UCHAR event_info,
                                 /* IN */ UCHAR *event_data,
                                 /* IN */ UINT16 *event_data_len
                             );


void main_hid_device_operations (void);

void hid_appl_dbase_register (void);

API_RESULT hid_device_hci_event_ind_cb
           (
               /* IN */ UCHAR event_type,
               /* IN */ UCHAR *event_data,
               /* IN */ UCHAR event_data_len
           );

/* To send test ppt reports. */
void hid_send_ppt_report(void);

#endif /* _H_APPL_HID_DEVICE_ */

