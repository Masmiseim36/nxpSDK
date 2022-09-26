
/**
 *  \file appl_hrs.h
 *
 *  Application Header File for Heart Rate Sensor.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HRS_
#define _H_APPL_HRS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"
/* ----------------------------------------- Data Types/ Structures */
typedef struct
{
    UCHAR    index;

    UCHAR    length;

}APPL_HRS_OBS_DATA_INFO;

/* --------------------------------------------- Global Definitions */
/* HRS body Sensor Locations */
#define APP_HRS_BODY_SENSOR_LOCATION_OTHER    0x00
#define APP_HRS_BODY_SENSOR_LOCATION_CHEST    0x01
#define APP_HRS_BODY_SENSOR_LOCATION_WRIST    0x02
#define APP_HRS_BODY_SENSOR_LOCATION_FINGER   0x03
#define APP_HRS_BODY_SENSOR_LOCATION_HAND     0x04
#define APP_HRS_BODY_SENSOR_LOCATION_EARLOBE  0x05
#define APP_HRS_BODY_SENSOR_LOCATION_FOOT     0x06

/* Default HRS body Sensor Location */
#define APPL_HRS_BODY_SENSOR_LOCATION         \
        APP_HRS_BODY_SENSOR_LOCATION_CHEST

/** LSB of error code has to be spec defined */
#define APPL_HR_CNTRL_PNT_NOT_SUPPORTED       (APPL_ERR_ID | 0x80)

/* HRS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are HRS UUID(0x180D)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_HRS_GATT_DB_SIGN                     (0x12340000U | GATT_HEART_RATE_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_hrs_init(void);
void appl_hrs_bt_init(void);
void appl_hrs_bt_deinit(void);
void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
void appl_timer_expiry_handler (void *data, UINT16 datalen);
void appl_hrs_connect(APPL_HANDLE  * appl_handle);
void appl_send_heart_rate_measurement (APPL_HANDLE   * handle);
void appl_hrs_server_reinitialize (void);
API_RESULT appl_hr_control_point_handler
           (
                GATT_DB_HANDLE  * handle,
                ATT_VALUE       * value
           );

API_RESULT gatt_db_hrs_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );
void appl_hrs_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_hrs_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_hrs_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_hrs_gatt_db_register(void);
#else
#define appl_hrs_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_hrs_init()
#define APPL_PROFILE_BT_INIT() appl_hrs_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_hrs_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_hrs_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x) appl_send_heart_rate_measurement(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_hrs_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_hrs_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_hrs_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_hrs_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_hrs_handle_mtu_update_complete((x),(y))

#define APPL_USE_IDLE_TIMER
#define APPL_IDLE_TIMEOUT           30
#endif /* _H_APPL_HRS_ */


