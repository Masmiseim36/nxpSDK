
/**
 *  \file appl_hts.h
 *
 *  Health Thermometer Server Application Header.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_HTS_
#define _H_APPL_HTS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"
#include "gatt_db.h"

/* --------------------------------------------- Macros */
#define APPL_TEMPERATURE_MEASUREMENT_INTERVAL     5U   /* Seconds */
#define APPL_TEMPERATURE_MEASUREMENT_LENGTH       12U
#define APPL_IM_TEMPERATURE_MEASUREMENT_LENGTH    5U
#define APPL_IM_TEMPERATURE_COUNT                 3U
#define APPL_MSRMT_INTRVL_DATA_INTERVAL           1U   /* Seconds */
#define APPL_MSRMT_INTRVL_DATA_LENGTH             2U
#define APPL_MSRMT_INTRVL_DEFAULT_RANGE_MIN       1U   /* Seconds */
#define APPL_MSRMT_INTRVL_DEFAULT_RANGE_MAX       10U  /* Seconds */

/* Profile Sepcific Error Code */
#define APPL_OUT_OF_RANGE_ERROR                   (APPL_ERR_ID | 0x80U)

/* HTS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are HTS UUID(0x1809)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_HTS_GATT_DB_SIGN                     (0x12340000U | GATT_HEALTH_THERMOMETER_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_hts_init(void);
void appl_hts_bt_init(void);
void appl_hts_bt_deinit(void);
void appl_timer_expiry_handler (void *data_param, UINT16 datalen);
void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
void appl_hts_connect(DEVICE_HANDLE  * dq_handle);
void appl_send_temperature_measurement (APPL_HANDLE  * handle);
void appl_hts_server_reinitialize (void);
API_RESULT appl_validate_msrmt_interval (GATT_DB_HANDLE * handle, UINT16 data);
void appl_msrmt_intrvl_timer_expiry_handler (void *data_param, UINT16 datalen);

void appl_hts_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_hts_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_hts_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );
API_RESULT gatt_db_temperature_measurment_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );
API_RESULT gatt_db_hts_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_hts_gatt_db_register(void);
#else
#define appl_hts_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_hts_init()
#define APPL_PROFILE_BT_INIT() appl_hts_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_hts_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_hts_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x) appl_send_temperature_measurement(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_hts_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_hts_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_hts_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_hts_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_hts_handle_mtu_update_complete((x),(y))

#ifndef USE_BT_PL_TIMER
#define APPL_USE_IDLE_TIMER
#endif
#define APPL_IDLE_TIMEOUT           30U

#endif /* _H_APPL_HTS_ */

