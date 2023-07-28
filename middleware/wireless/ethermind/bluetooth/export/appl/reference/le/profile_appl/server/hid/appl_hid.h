
/**
 *  \file appl_hid.h
 *
 *  Human Interface Device Server Application Header.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HID_
#define _H_APPL_HID_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"

/* HIDS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are HIDS UUID(0x1812)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_HIDS_GATT_DB_SIGN                     (0x12340000U | GATT_HID_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_hid_init(void);
void appl_hid_bt_init(void);
void appl_hid_bt_deinit(void);
void appl_timer_expiry_handler (void *data, UINT16 datalen);
void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
API_RESULT appl_manage_wwr (GATT_DB_HANDLE * handle, GATT_DB_PARAMS * params);
void appl_hid_connect(DEVICE_HANDLE  * dq_handle);
void appl_send_hid_event (APPL_HANDLE  * handle);
void appl_hid_server_reinitialize (void);

API_RESULT gatt_db_hid_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );
API_RESULT appl_mouse_hid_report_handler(APPL_HANDLE * appl_handle);
API_RESULT appl_keyboard_hid_report_handler(APPL_HANDLE * appl_handle);
API_RESULT appl_notify_hid_report
           (
               APPL_HANDLE     * hndl,
               ATT_ATTR_HANDLE attr_hndl
           );
void appl_hid_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_hid_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_hid_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_hid_gatt_db_register(void);
#else
#define appl_hid_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_hid_init()
#define APPL_PROFILE_BT_INIT() appl_hid_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_hid_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_hid_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x)appl_send_hid_event(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_hid_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_hid_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_hid_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_hid_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_hid_handle_mtu_update_complete((x),(y))

#define APPL_USE_IDLE_TIMER
#define APPL_IDLE_TIMEOUT           30

#endif /* _H_APPL_HID_ */

