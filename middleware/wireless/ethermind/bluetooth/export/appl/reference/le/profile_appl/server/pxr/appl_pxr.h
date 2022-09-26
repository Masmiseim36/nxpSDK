
/**
 *  \file appl_pxr.h
 *
 *  Application Header File for Proximity Reporter.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_PXR_
#define _H_APPL_PXR_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"

/* --------------------------------------------- Global Definitions */
#define APPL_PXR_NO_ALERT                  0
#define APPL_PXR_MILD_ALERT                1
#define APPL_PXR_HIGH_ALERT                2

#define APPL_PXR_LINK_LOSS                 0
#define APPL_IM_ALERT                      1

#define APPL_PXR_INVALID_ALRT_VALUE        (APPL_ERR_ID | 0x0080)

#define APPL_PXR_VALID_ALERT_LEVEL(lvl)     \
    (((lvl) > APPL_PXR_HIGH_ALERT)? BT_FALSE : BT_TRUE)

/* Tx Power Level Notification Timeout [5 Seconds] */
#define APPL_PXR_TPS_MSRMT_TIMEOUT         5

/* Tx Power Level */
#define APPL_PX_TX_POWER_LEVEL_LENGTH      1

/* PXR Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are PXR UUID(0x1804)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_PXR_GATT_DB_SIGN                     (0x12340000U | GATT_TX_POWER_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_pxr_init (void);
void appl_pxr_bt_init(void);
void appl_pxr_bt_deinit(void);
void appl_pxr_reinit
     (
          DEVICE_HANDLE    * handle,
          UCHAR              alert_reason
     );

API_RESULT gatt_db_pxr_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_pxr_gatt_db_register(void);
#else
#define appl_pxr_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

void appl_pxr_alert_user
     (
          DEVICE_HANDLE    * handle,
          UCHAR              alert_reason
     );
char * appl_pxr_get_alert_str (UCHAR  alert_level);
API_RESULT appl_pxr_set_alert_level
           (
                GATT_DB_HANDLE    * handle,
                ATT_VALUE         * value
           );

API_RESULT appl_pxr_get_tx_power_level
           (
                GATT_DB_HANDLE    * handle,
                ATT_VALUE         * value
           );

API_RESULT appl_pxr_get_tx_power_level_complete
           (
                UCHAR     status,
                UINT16    conn_handle,
                UCHAR     tx_power_level
           );
void appl_timer_expiry_handler (void *data_param, UINT16 datalen);
void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
void appl_pxr_connect(DEVICE_HANDLE  * dq_handle);
void appl_send_tx_power_ntf (APPL_HANDLE  * handle);
API_RESULT appl_pxr_fetch_tx_power_level(APPL_HANDLE handle);

void appl_pxr_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_pxr_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_pxr_init()
#define APPL_PROFILE_BT_INIT() appl_pxr_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_pxr_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_pxr_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x) appl_send_tx_power_ntf(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r)\
        appl_pxr_reinit(&APPL_GET_DEVICE_HANDLE(*(x)), (r))

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_pxr_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_pxr_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_pxr_handle_mtu_update_complete((x),(y))

/* Proximitiy does not use Idle Timer */
#ifndef USE_BT_PL_TIMER
#define APPL_USE_IDLE_TIMER
#endif
#define APPL_IDLE_TIMEOUT           30

#endif /* _H_APPL_PXR_ */

