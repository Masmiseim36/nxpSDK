
/**
 *  \file appl_gls.h
 *
 *  Application Header File for Glucose Sensor.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GLS_
#define _H_APPL_GLS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#ifdef GLS
#include "gatt_db.h"

#ifdef BT_RACP
#include "BT_racp.h"
#endif /* BT_RACP */

/* #define APPL_GLS_SINGLE_BOND_SUPPORT */

#define APPL_RECORD_TRNSFER_INTERVAL               3
#define APPL_UPDATE_RECORD_INTERVAL                15
#define APPL_GLUCOSE_MEASUREMENT_LENGTH            18
#define APPL_GLUCOSE_MEASUREMENT_CONTEXT_LENGTH    17
#define APPL_GLUCOSE_MEASUREMENT_COUNT             9
#define APPL_GLUCOSE_MEASUREMENT_CONTEXT_COUNT     2
#define APPL_STORED_RECORD_COUNT                   9
#define APPL_CONTEXT_INFORMATION_FLAG_MASK         0x10

/* --------------------------------------------- Global Definitions */
typedef struct appl_gl_racp_req_param
{
    UCHAR       req_op_code;

    UCHAR       oprtr;

    UCHAR       operand[20];
}APPL_RACP_REQ_PARAM;

/* Structure to be passed to Timer Handler */
typedef struct appl_gl_racp_timer_param
{
    APPL_HANDLE          handle;

    UCHAR                rsp_code_val;

    APPL_RACP_REQ_PARAM  appl_req_param;

}APPL_TIMER_PARAM;

typedef struct appl_gls_user_facing_time
{
    INT16 hours;

    INT16 minutes;

} APPL_GLS_USER_FACING_TIME;

/* GLS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are GLS UUID(0x1808)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_GLS_GATT_DB_SIGN                     (0x12340000U | GATT_GLUCOSE_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_gls_init (void);
void appl_gls_bt_init(void);
void appl_gls_bt_deinit(void);
void appl_gls_connect(DEVICE_HANDLE  * dq_handle);
void appl_timer_expiry_handler (void *data, UINT16 datalen);
void appl_gls_server_reinitialize (void);
API_RESULT appl_gl_racp_write_handler
           (
                GATT_DB_HANDLE    * handle,
                ATT_VALUE         * value
           );
void appl_gls_manage_transfer(APPL_TIMER_PARAM * appl_timer_param);
API_RESULT appl_gl_racp_operation_hndlr (APPL_TIMER_PARAM    * appl_param);
API_RESULT appl_gl_racp_send_response (APPL_TIMER_PARAM    * appl_param);
API_RESULT appl_gl_racp_rprt_strd_record (APPL_TIMER_PARAM  *appl_timer_param);
UINT16 appl_get_stored_records_count(APPL_TIMER_PARAM * appl_timer_param, UCHAR type);
API_RESULT appl_gl_racp_clear_strd_records(APPL_TIMER_PARAM  *appl_timer_param);
UCHAR appl_check_context_info_flag (void);
void appl_profile_menu_handler (void);
void appl_activate_gls_records (void);
void gls_racp_init(void);

API_RESULT appl_gls_racp_write_handler
           (
                ATT_VALUE         * value
           );
void appl_racp_timer_expiry_handler(void *data, UINT16 datalen);
API_RESULT appl_gl_racp_validate_operands
            (
                /* IN */ UCHAR    oprtr,
                /* IN */ UCHAR  * operand,
                /* IN */ UCHAR    len
            );
void appl_send_rsp_handler
     (
         RACP_REQ_INFO   * gls_racp_req_info,
         UINT16            len,
         UINT16            result
     );

API_RESULT appl_gls_report_stored_records
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           );

API_RESULT appl_gls_report_stored_number_of_records
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           );

API_RESULT appl_gls_delete_stored_record
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           );

API_RESULT appl_gls_abort_operation
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           );

API_RESULT appl_gls_access_record
           (
               RACP_REQ_INFO * req_info,
               UINT16          length
           );
INT16 appl_gls_convert_from_hrs_to_minutes
      (
          APPL_GLS_USER_FACING_TIME * appl_user_facing_time,
          INT16                      prev_record_time_offset
      );

void appl_gls_update_time_offset
     (
         APPL_GLS_USER_FACING_TIME * appl_user_facing_time,
         UCHAR record_no,
         UCHAR total_records
     );
API_RESULT gatt_db_gls_gatt_char_handler
(
    GATT_DB_HANDLE* handle,
    GATT_DB_PARAMS* params
);
void appl_gls_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_gls_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_gls_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );
#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_gls_gatt_db_register(void);
#else
#define appl_gls_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_gls_init()
#define APPL_PROFILE_BT_INIT() appl_gls_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_gls_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_POWER_ON_HANDLER()
#define APPL_PROFILE_CONNECT(x) \
        appl_gls_connect(&APPL_GET_DEVICE_HANDLE((*x)))
/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x)
/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)
/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_gls_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                     \
        gatt_db_gls_gatt_char_handler((h), (p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_gls_handle_ntf_complete((x),(y),(z))
/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_gls_handle_ind_complete((x),(y))
/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_gls_handle_mtu_update_complete((x),(y))
#ifndef USE_BT_PL_TIMER
#define APPL_USE_IDLE_TIMER
#endif
#define APPL_IDLE_TIMEOUT           30

#endif /* GLS */
#endif /* _H_APPL_GLS_ */


