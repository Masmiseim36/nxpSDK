
/**
 *  \file appl_ipspn.h
 *
 *  Internet Protocol Support Profile Node Application Header.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_IPSPN_
#define _H_APPL_IPSPN_

/* ----------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"
#include "gatt_db.h"

/* Have 6LoBTLE and Networking interface */
/* This macro needs to be defined in the Preprocessors */
/* #define IPSP_HAVE_6LO_NIFACE */

/* IPSPN Database Signature */
/* NOTE: Currently having a dummy signature where
 * - the 2 Upper Bytes are 0x1234
 * - the 2 Lower Bytes are IPSPN UUID(0x1821)
 * Application can have any logic for choosing 4Bytes of signature
 */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
#define APPL_IPSPN_GATT_DB_SIGN                     (0x12340000U | GATT_IPS_SERVICE)
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

/* --------------------------------------------- Functions */
void appl_ipspn_init(void);
void appl_ipspn_bt_init(void);
void appl_ipspn_bt_deinit(void);
void appl_ipspn_connect(DEVICE_HANDLE  * dq_handle);
void appl_ipspn_server_reinitialize (void);

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_ipspn_gatt_db_register(void);
#else
#define appl_ipspn_gatt_db_register()
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

API_RESULT gatt_db_ipspn_gatt_char_handler
           (
               GATT_DB_HANDLE    * handle,
               GATT_DB_PARAMS    * params
           );

/* Profile handling */
/* Profile Initialization */
#define APPL_PROFILE_INIT() appl_ipspn_init()
#define APPL_PROFILE_BT_INIT() appl_ipspn_bt_init()
#define APPL_PROFILE_BT_DEINIT() appl_ipspn_bt_deinit()

/* Profile Connection */
#define APPL_PROFILE_CONNECT(x) \
        appl_ipspn_connect(&APPL_GET_DEVICE_HANDLE((*x)))

/* Profile Trigger for Measurements */
#define APPL_SEND_MEASUREMENT(x)

/* Profile Disconnect Handler */
#define APPL_PROFILE_DISCONNECT_HANDLER(x)

/* Transport Disconnect Handler */
#define APPL_TRANSPORT_DISCONNECT_HANDLER(x, r) appl_ipspn_server_reinitialize()

/* GATT Database Specific Handler */
#define GATT_DB_PROFILE_HANDLER(h, p)                      \
        gatt_db_ipspn_gatt_char_handler((h),(p))

/* Handle Value Notification Complete Handler */
#define APPL_PROFILE_HV_NTF_COMPLETE_HANDLER(x,y,z)

/* Handle Value Indication Complete Handler */
#define APPL_PROFILE_HV_IND_COMPLETE_HANDLER(x,y)

/* Profile ATT MTU Update Complete Handler */
#define APPL_PROFILE_MTU_UPDT_COMPLETE_HANDLER(x,y)

#ifndef USE_BT_PL_TIMER
/* #define APPL_USE_IDLE_TIMER */
#endif
#define APPL_IDLE_TIMEOUT           30

#endif /* _H_APPL_IPSPN_ */

