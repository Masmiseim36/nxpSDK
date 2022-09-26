
/**
 *  \file appl_pass.h
 *
 *  Application Header File for Phone Alert Status Sensor.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_PASS_
#define _H_APPL_PASS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"
/* --------------------------------------------- Global Definitions */
#define ALRT_STATUS_RINGER_STATE_BIT                     0
#define ALRT_STATUS_VIBRATOR_STATE_BIT                   1
#define ALRT_STATUS_DISP_ALRT_STATE_BIT                  2

#define RINGER_SETTING_RINGER_SILENT                     0
#define RINGER_SETTING_RINGER_NORMAL                     1

#define APPL_PASS_RINGER_CNTRL_PNT_SILENT_MODE           1
#define APPL_PASS_RINGER_CNTRL_PNT_MUTE_ONCE             2
#define APPL_PASS_RINGER_CNTRL_PNT_CANCEL_SILENT_MODE    3

#define APPL_AUTO_SEND_NOTIFICATION                      0
#define APPL_MENU_SEND_NOTIFICATION                      1

#define APPL_ALERT_STATUS_INTERVAL                       10
#define APPL_RINGER_SETTING_INTERVAL                     10

#define appl_set_bit_value(var,pos)\
        var = (var & (~(1 << pos)))|(1 << pos)

#define appl_clear_bit_value(var,pos)\
        var = (var & (~(1 << pos)))

#define appl_get_bit_value(var,pos)\
        (var & (1 << pos))?1:0

/* PASS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are PASS UUID(0x180E)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_PASS_GATT_DB_SIGN                     (0x12340000U | GATT_PHONE_ALERT_STATUS_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
/* --------------------------------------------- Functions */
void appl_pass_init(void);
void appl_pass_bt_init(void);
void appl_pass_bt_deinit(void);
void appl_pass_connect(DEVICE_HANDLE  * dq_handle);
void appl_update_alert_status (UCHAR * val);
void appl_update_ringer_setting (UCHAR * val);
void appl_ringer_setting_timer_expiry_handler (void *data, UINT16 datalen);
void appl_alert_status_timer_expiry_handler (void *data, UINT16 datalen);
void appl_start_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
void appl_send_phone_alert_notifications (APPL_HANDLE * handle);
void appl_pass_ringer_control_point_write_handler
     (
        GATT_DB_HANDLE    * handle,
        ATT_VALUE         * value
     );
API_RESULT appl_pass_read_request_handler
           (
               GATT_DB_HANDLE    * handle
           );
void appl_pass_server_reinitialize (void);
API_RESULT gatt_db_pass_gatt_char_handler
           (
               GATT_DB_HANDLE* handle,
               GATT_DB_PARAMS* params
           );
void appl_pass_set_gap_central_role(void);
void appl_pass_menu_handler (void);
void appl_send_alert_status_ntf(void);
void appl_send_ringer_settings_ntf(void);
void appl_pass_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_pass_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_pass_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_pass_gatt_db_register(void);
#else
#define appl_pass_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
#define APPL_PROFILE_INIT() appl_pass_init()
#define APPL_PROFILE_BT_INIT() appl_pass_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_pass_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_POWER_ON_HANDLER()
#define APPL_PROFILE_CONNECT(x) \
    appl_pass_connect(&APPL_GET_DEVICE_HANDLE((*x)))
/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x) appl_send_phone_alert_notifications(x)
/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)
/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_pass_server_reinitialize()
/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                     \
        gatt_db_pass_gatt_char_handler((h), (p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_pass_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_pass_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_pass_handle_mtu_update_complete((x),(y))

#ifndef USE_BT_PL_TIMER
#define APPL_USE_IDLE_TIMER
#endif

#define APPL_USE_IDLE_TIMER
#define APPL_IDLE_TIMEOUT           30
#endif /* _H_APPL_PASS_ */


