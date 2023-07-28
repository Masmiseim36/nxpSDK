
/**
 *  \file appl_basic.h
 *
 *  Basic Server Application Header.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_BASIC_
#define _H_APPL_BASIC_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

#include "gatt_db.h"
#include "BT_security.h"

/* --------------------------------------------- Global Defines */
/**
 * Test Service and Test Characteristic UUIDs
 * Currently assigning dummy 16-Bit UUID Values for demonstration purpose.
 */
/** Test Service A UUID */
#define APPL_BASIC_TEST_SERV_A_UUID                 0xAA00
/** Test Service B UUID */
#define APPL_BASIC_TEST_SERV_B_UUID                 0xBB00
/** Test Characteristic A1 UUID */
#define APPL_BASIC_TEST_CHAR_A1_UUID                0x00A1
/** Test Characteristic B1 UUID */
#define APPL_BASIC_TEST_CHAR_B1_UUID                0x00B1
/** Test Characteristic B2 UUID */
#define APPL_BASIC_TEST_CHAR_B2_UUID                0x00B2

/* --------------------------------------------- Macros */
/** BASIC Database Signatures */
/**
 * NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are BAS UUID(0x180F) for Database Set 1
 * - the 2 Lower Bytes are DIS UUID(0x180A) for Database Set 2
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_BASIC_SET_1_GATT_DB_SIGN               (0x12340000U | GATT_BATTERY_SERVICE)
#define APPL_BASIC_SET_2_GATT_DB_SIGN               (0x12340000U | GATT_DEVICE_INFO_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/** BASIC related Defines */
/* Ideally these are GATT related defines, could be moved to API header */
/** Mask value for All Client Specific Feature Characteristics */
#define APPL_BASIC_CSFC_MASK                        0x07
/** Mask value for Client Specific Feature Characteristic Robust Caching */
#define APPL_BASIC_CSFC_ROBUST_CACHE_MASK           0x01
/** Mask value for Client Specific Feature Characteristic EATT Support */
#define APPL_BASIC_CSFC_EATT_MASK                   0x02
/**
 * Mask value for Client Specific Feature Characteristic Multiple Handle Value
 * Notifications Support
 */
#define APPL_BASIC_CSFC_MHVN_MASK                   0x04
/** Maximum allowed length of Client Specific Feature Characteristic */
#define APPL_BASIC_CSFC_LEN                         1

/* --------------------------------------------- Functions */
void appl_basic_init(void);
void appl_basic_bt_init(void);
void appl_basic_bt_deinit(void);
void appl_basic_connect(DEVICE_HANDLE  * dq_handle);
void appl_basic_server_reinitialize (void);

void appl_basic_update_dev_awarness(void);
void appl_basic_switch_gatt_db(void);
void appl_basic_init_set_1_db(void);
void appl_basic_init_set_2_db(void);
void appl_basic_calculate_db_hash(void);
API_RESULT appl_basic_gatt_db_ntf_cb
           (
               UCHAR      gatt_db_event,
               API_RESULT event_result,
               UCHAR      * data_param,
               UINT16     data_len
           );
void appl_basic_send_serv_changed
     (
         /* IN */ DEVICE_HANDLE dq_handl
     );
#ifdef GATT_DB_DYNAMIC
void appl_basic_updt_serv_chngd_cccd(void);
void appl_basic_updt_csfc_value(void);
#else /* GATT_DB_DYNAMIC */
#define appl_basic_updt_serv_chngd_cccd(...)
#define appl_basic_updt_csfc_value(...)
#endif /* GATT_DB_DYNAMIC */
void appl_basic_reset_serv_chngd_cccd(DEVICE_HANDLE dq_handle);
void appl_basic_reset_csfc_value(DEVICE_HANDLE dq_handle);

void appl_basic_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_basic_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_basic_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );
API_RESULT gatt_db_basic_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_basic_gatt_db_register(void);
#else
#define appl_basic_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_basic_init()
#define APPL_PROFILE_BT_INIT() appl_basic_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_basic_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x)\
        appl_basic_connect(&APPL_GET_DEVICE_HANDLE((*x)))
/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_basic_server_reinitialize()
/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_basic_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_basic_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_basic_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_basic_handle_mtu_update_complete((x),(y))

#endif /* _H_APPL_BASIC_ */

