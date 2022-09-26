
/**
 *  \file appl_hid_host.h
 *
 *  Header file for HID Host Command Line Test Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HID_HOST_
#define _H_APPL_HID_HOST_

/* ----------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_api.h"
#include "BT_hid_host_api.h"


/* ----------------------------------------- Global Definitions */


/* ----------------------------------------- Macros */
#define APPL_HID_HOST_MAX_DESC_LEN           256U

/* ----------------------------------------- Structures/ Data Types */

/* Following structure is updated with the remote SDP database when querried */
typedef struct
{
    /* The handle for the the added device. */
    HID_HOST_DEV_HANDLE handle;

    /* The HIDVirtualCable attribute specified in the record of this device */
    BOOLEAN                     dev_attr_hid_virtual_cable;

    /* The HIDRecoonectInittiate attribute specified in the record */
    BOOLEAN                     dev_attr_hid_reconnect_initiate;

    /* The HIDRemoteWake attribute specified in the record of this device */
    BOOLEAN                     dev_attr_hid_remote_wake;

    /* The HIDSDPDisable attribute specified in the record of this device */
    BOOLEAN                     dev_attr_hid_disable;

    /* The HIDNormallyConnetable attribute specified in the record  */
    BOOLEAN                     dev_attr_hid_normally_connectable;

    /* The HID Report Descriptor */
    UCHAR                       report_desc [APPL_HID_HOST_MAX_DESC_LEN];

    /* The HID Report Descriptor Length */
    UINT16                      report_desc_len;
} APPL_HID_HOST_DEVICE_INFO;


/* ----------------------------------------- Function Declarations */
#define hid_uinput_init()
#define hid_uinput_kbd_irq(data, datalen)
#define hid_uinput_mouse_irq(data, datalen)

void main_hid_host_operations (void);

void appl_hid_host_report_ind_cb
     (
         HID_HOST_DEV_HANDLE handle,
         HID_HOST_REPORT* report
     );

void appl_hid_host_event_ind_cb
     (
         HID_HOST_EVT_TYPE event_type,
         HID_HOST_DEV_HANDLE handle,
         API_RESULT result,
         UCHAR *event_data,
         UINT16 event_data_len
     );

void hid_host_sdp_cb
     (
          /* IN */ UCHAR command,
          /* IN */ UCHAR * data,
          /* IN */ UINT16 length,
          /* IN */ UINT16 status
     );

void hid_device_add_to_handle_list
     (
         HID_HOST_DEV_HANDLE handle
     );

void hid_device_remove_from_handle_list(HID_HOST_DEV_HANDLE handle);

HID_HOST_DEV_HANDLE hid_device_get_handle(void);

#endif /* _H_APPL_HID_HOST_ */

