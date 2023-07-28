
/**
 *  \file appl_pts.h
 *
 *  Basic Server Application Header.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_PTS_
#define _H_APPL_PTS_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"
#include "gatt_db.h"

#include "smp_pl.h"
#include "db_gen.h"

/* --------------------------------------------- Global Definitions */
/** PTS Dummy Services and Characteristics UUIDs */
/** Service UUIDs */
#define APPL_PTS_DUMMY_SERVICE_1                           0xED00
#define APPL_PTS_DUMMY_SERVICE_2                           0xEE00
#define APPL_PTS_DUMMY_SERVICE_3                           0xEF00
#define APPL_PTS_DUMMY_SERVICE_4                           0xEA00
#define APPL_PTS_DUMMY_SERVICE_5                           0xEB00
#define APPL_PTS_DUMMY_SERVICE_6                           0xDEAD
#define APPL_PTS_DUMMY_SERVICE_7                           0xEC00
#define APPL_PTS_DUMMY_SERVICE_8                           0xE900
#define APPL_PTS_DUMMY_SERVICE_9                           0xE800

/** Characteristic UUIDs */
#define APPL_PTS_DUMMY_CHARACTERISTIC_1                    0xED01
#define APPL_PTS_DUMMY_CHARACTERISTIC_2                    0xED02
#define APPL_PTS_DUMMY_CHARACTERISTIC_3                    0xED03
#define APPL_PTS_DUMMY_CHARACTERISTIC_4                    0xED04
#define APPL_PTS_DUMMY_CHARACTERISTIC_5                    0xED05
#define APPL_PTS_DUMMY_CHARACTERISTIC_6                    0xED06
#define APPL_PTS_DUMMY_CHARACTERISTIC_7                    0xED07
#define APPL_PTS_DUMMY_CHARACTERISTIC_8                    0xED08
#define APPL_PTS_DUMMY_CHARACTERISTIC_9                    0xED09
#define APPL_PTS_DUMMY_CHARACTERISTIC_10                   0xED0A
#define APPL_PTS_DUMMY_CHARACTERISTIC_11                   0xED0B
#define APPL_PTS_DUMMY_CHARACTERISTIC_12                   0xED0C
#define APPL_PTS_DUMMY_CHARACTERISTIC_13                   0xED0D
#define APPL_PTS_DUMMY_CHARACTERISTIC_14                   0xEE01
#define APPL_PTS_DUMMY_CHARACTERISTIC_15                   0xEF01
#define APPL_PTS_DUMMY_CHARACTERISTIC_16                   0xEA01
#define APPL_PTS_DUMMY_CHARACTERISTIC_17                   0xEA02
#define APPL_PTS_DUMMY_CHARACTERISTIC_18                   0xEB01
#define APPL_PTS_DUMMY_CHARACTERISTIC_19                   0xFEED
#define APPL_PTS_DUMMY_CHARACTERISTIC_20                   0xBEEF
#define APPL_PTS_DUMMY_CHARACTERISTIC_21                   0xEC01
#define APPL_PTS_DUMMY_CHARACTERISTIC_22                   0xE901
#define APPL_PTS_DUMMY_CHARACTERISTIC_23                   0xE801

/** Characteristic Higher Layer Descriptor UUIDs */
#define APPL_PTS_DUMMY_CHAR_HLD_1                          0xAAED
#define APPL_PTS_DUMMY_CHAR_HLD_2                          0xBBED
#define APPL_PTS_DUMMY_CHAR_HLD_3                          0xCCED
#define APPL_PTS_DUMMY_CHAR_HLD_4                          0xDDED
#define APPL_PTS_DUMMY_CHAR_HLD_5                          0xEEED
#define APPL_PTS_DUMMY_CHAR_HLD_6                          0xFFED

/**
 * NOTE:
 * The PTS Database has an Included Service.
 * Service with UUID 0xEE00 i.e. APPL_PTS_DUMMY_SERVICE_2 is Secondary Service.
 * This is included by APPL_PTS_DUMMY_SERVICE_1
 */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

/* --------------------------------------------- Functions */
void appl_pts_init(void);
void appl_pts_bt_init(void);
void appl_pts_bt_deinit(void);
void appl_pts_connect(DEVICE_HANDLE  * dq_handle);
void appl_pts_server_reinitialize (void);
void appl_pts_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     );
void appl_pts_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     );
void appl_pts_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     );
API_RESULT gatt_db_pts_battery_level_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );
API_RESULT gatt_db_pts_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_pts_gatt_db_register(void);
#else
#define appl_pts_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_pts_init()
#define APPL_PROFILE_BT_INIT(...) appl_pts_bt_init()
#define APPL_PROFILE_BT_DEINIT(...) appl_pts_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_pts_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_pts_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_pts_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)\
        appl_pts_handle_ntf_complete((x),(y),(z))

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)\
         appl_pts_handle_ind_complete((x),(y))

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)\
        appl_pts_handle_mtu_update_complete((x),(y))

#ifndef USE_BT_PL_TIMER
#define APPL_USE_IDLE_TIMER
#endif
#define APPL_IDLE_TIMEOUT           30

#endif /* _H_APPL_PTS_ */

