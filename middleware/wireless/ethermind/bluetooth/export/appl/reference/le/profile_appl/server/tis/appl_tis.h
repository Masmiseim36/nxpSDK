
/**
 *  \file appl_tis.h
 *
 *  Application Header File for Current Time.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_TIS_
#define _H_APPL_TIS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"

#define APPL_CUR_TIME_MEASUREMENT_INTERVAL    1

#define APPL_CUR_TIME_LENGTH                  10
#define APPL_LOC_TIME_LENGTH                  2
#define APPL_REF_TIME_LENGTH                  4
#define APPL_CUR_TIME_ILLEGAL_WRITE           0x80

/* Valid Year param */
#define APPL_YEAR_MAX                        9999
#define APPL_YEAR_MIN                        1582
#define APPL_YEAR_NOT_KNOWN                  0

/* Valid Month param */
#define APPL_MONTH_MAX                        12
#define APPL_MONTH_MIN                        0

/* Valid Day param */
#define APPL_DAY_MAX                          31
#define APPL_DAY_MIN                          0

/* Valid Hours param */
#define APPL_HOURS_MAX                        23
#define APPL_HOURS_MIN                        0

/* Valid Minutes param */
#define APPL_MINUTES_MAX                      59
#define APPL_MINUTES_MIN                      0

/* Valid Minutes param */
#define APPL_SECONDS_MAX                      59
#define APPL_SECONDS_MIN                      0

/* Valid Day of week */
#define APPL_DAY_OF_WEEK_MAX                  7
#define APPL_DAY_OF_WEEK_MIN                  0

/* Valid Timezone param */
#define APPL_TIMEZONE_MAX                     56
#define APPL_TIMEZONE_MIN                    -48
#define APPL_TIMEZONE_NOT_KNOWN              -128

/* Valid DST Offset param */
#define APPL_DSTOFFSET_MAX                    8
#define APPL_DSTOFFSET_MIN                    0
#define APPL_DSTOFFSET_NOT_KNOWN              255

#define APPL_TIME_UPDT_STATE_LEN              2

/*
 * Read Current Time is used to read current time.
 * Read Current Time function is also used by Current Time Service,
 * and it returns Day of Week and Fraction256 fields
 * which are not required for Next DST Change (NDCS).
 *
 * NDCS has one additional field of one octet, DST Offset.
 */
#define APPL_NDCS_TIME_LENGTH                  (8 + 1)

/* Default DST Offset set as Standard Time */
#define APPL_NDCS_DEFAULT_DST_OFFSET           0x00
#define APPL_NDCS_DST_INDEX                    7

/* TIS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are NDCS UUID(0x1807)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_TIS_GATT_DB_SIGN                     (0x12340000U | GATT_NEXT_DST_CHANGE_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_tis_init(void);
void appl_tis_bt_init(void);
void appl_tis_bt_deinit(void);
void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
void appl_timer_expiry_handler (void *data, UINT16 datalen);
void appl_tis_connect(DEVICE_HANDLE  * dq_handle);
void appl_send_cur_time (APPL_HANDLE   * handle);
void appl_tis_server_reinitialize (void);
API_RESULT appl_cts_get_current_time
           (
                GATT_DB_HANDLE    * handle,
                ATT_VALUE         * value
           );
API_RESULT appl_rtus_config_time_update
           (
               GATT_DB_HANDLE    * handle,
               ATT_VALUE         * value
           );
API_RESULT appl_rtus_get_time_update_state
           (
               GATT_DB_HANDLE    * handle,
               ATT_VALUE         * value
           );
API_RESULT appl_update_current_time (ATT_VALUE    * value);
API_RESULT appl_update_local_time (ATT_VALUE    * value);
API_RESULT appl_ndcs_get_current_time
           (
                GATT_DB_HANDLE    * handle,
                ATT_VALUE         * value
           );
void appl_tis_set_gap_central_role(void);

API_RESULT gatt_db_tis_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );

void appl_tis_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_tis_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_tis_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );

#ifndef GATT_DB_DYNAMIC
void appl_tis_gatt_db_register(void);
#endif /* GATT_DB_DYNAMIC */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_tis_init()
#define APPL_PROFILE_BT_INIT() appl_tis_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_tis_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_tis_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x) appl_send_cur_time(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_tis_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_tis_gatt_char_handler((h),(p))


/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_tis_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_tis_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_tis_handle_mtu_update_complete((x),(y))

#define APPL_USE_IDLE_TIMER
#define APPL_IDLE_TIMEOUT           30
#endif /* _H_APPL_TIS_ */


