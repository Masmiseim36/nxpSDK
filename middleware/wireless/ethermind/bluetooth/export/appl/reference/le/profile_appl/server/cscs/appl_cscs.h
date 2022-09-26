
/**
 *  \file appl_cscs.h
 *
 *  Application Header File for Cycling Speed & Cadence Sensor.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_CSCS_
#define _H_APPL_CSCS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"

typedef struct _APPL_CSCS_CNTRL_POINT_RSP_PARAM
{
    UCHAR          rsp_code;

    UCHAR          op_code;

    APPL_HANDLE    appl_handle;

}APPL_CSCS_CNTRL_POINT_RSP_PARAM;

/* --------------------------------------------- Global Definitions */

/* CPMS Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are CSCS UUID(0x1816)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_CSCS_GATT_DB_SIGN                     (0x12340000U | GATT_CYCLING_SPEED_AND_CADENCE_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_cscs_init(void);
void appl_cscs_bt_init(void);
void appl_cscs_bt_deinit(void);
void appl_manage_trasnfer (GATT_DB_HANDLE handle, UINT16 config);
void appl_timer_expiry_handler (void *data, UINT16 datalen);
void appl_cscs_connect(DEVICE_HANDLE  * dq_handle);
void appl_send_csc_measurement (APPL_HANDLE   * handle);
void appl_cscs_server_reinitialize (void);
API_RESULT gatt_db_cscs_gatt_char_handler
           (
               GATT_DB_HANDLE * handle,
               GATT_DB_PARAMS * params
           );
API_RESULT appl_csc_control_point_handler
           (
                GATT_DB_HANDLE  * handle,
                ATT_VALUE       * value
           );
void appl_csc_cntrl_point_cnfg_handler (GATT_DB_HANDLE  *handle,UINT16  config);
void appl_cscs_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_cscs_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_cscs_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_cscs_gatt_db_register(void);
#else
#define appl_cscs_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_cscs_init()
#define APPL_PROFILE_BT_INIT() appl_cscs_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_cscs_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_POWER_ON_HANDLER()

#define APPL_PROFILE_CONNECT(x) \
        appl_cscs_connect((&APPL_GET_DEVICE_HANDLE((*x))))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x) appl_send_csc_measurement(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_cscs_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                     \
        gatt_db_cscs_gatt_char_handler((h), (p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_cscs_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_cscs_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_cscs_handle_mtu_update_complete((x),(y))

#ifndef USE_BT_PL_TIMER
#define APPL_USE_IDLE_TIMER
#endif
#define APPL_IDLE_TIMEOUT           30
#endif /* _H_APPL_CSCS_ */

