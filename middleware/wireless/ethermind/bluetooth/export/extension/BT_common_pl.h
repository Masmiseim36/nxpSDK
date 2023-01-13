
/**
 *  \file BT_common_pl.h
 *
 *  This file contains the Function Declaration, and Constant Definitions
 *  for the EtherMind Bluetooth Stack.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_COMMON_PL_
#define _H_BT_COMMON_PL_

/* ------------------------------------------- Header File Inclusion */


/* ------------------------------------------- Common PL Debug */
#define PL_ERR(...)                             BT_debug_error(0, __VA_ARGS__)

#ifdef PL_DEBUG

#define PL_TRC(...)                             BT_debug_trace(0, __VA_ARGS__)
#define PL_INF(...)                             BT_debug_info(0, __VA_ARGS__)

#else  /* PL_DEBUG */

#define PL_TRC                                  BT_debug_null
#define PL_INF                                  BT_debug_null

#endif /* PL_DEBUG */


/* ------------------------------------------- Global Definitions/Macros */
/* Set Thread's Data Access Permission - Not used */
#define BT_THREAD_SET_DATA_ACCESS_PERMISSIONS()

/* EtherMind Configuration File */
#define BT_CONFIG_FILE                          "ethermind.conf"

/* Feature to control usage of Upper Layer Callbacks for Init and Deinit */
#define BT_COMMON_PL_SUPPORT_UL_CB

/* ------------------------------------------- Data Structures */
typedef struct
{
    CHAR byte[16];
}INT128;

/**
 * BT Common Platform Interface of Initialization Callback to Upper Layer.
 */
typedef void (* BT_COMMON_UL_INIT_CB_PL)(void);
/**
 * BT Common Platform Interface of De-Initialization Callback to Upper Layer.
 */
typedef void (* BT_COMMON_UL_DEINIT_CB_PL)(void);

/* ------------------------------------------- Function Declarations */
/* EtherMind-Init: Platform Lower & Upper Handler */
void ethermind_init_lower_pl(void);
void ethermind_init_upper_pl(void);

#ifdef BT_HAVE_SHUTDOWN
/* EtherMind-Shutdown: Platform Lower and Upper Handler */
void ethermind_shutdown_lower_pl(void);
void ethermind_shutdown_upper_pl(void);
#endif /* BT_HAVE_SHUTDOWN */

/* Bluetooth-ON: Platform Lower & Upper Handler */
void bluetooth_on_lower_pl(void );
void bluetooth_on_upper_pl(void);

/* Bluetooth-OFF: Platform Lower & Upper Handler */
void bluetooth_off_lower_pl(void);
void bluetooth_off_upper_pl(void);

#ifdef BT_COMMON_PL_SUPPORT_UL_CB
/* Common platform registration for Upper Layer callback for Init & Deinit */
void BT_ethermind_register_ul_cb_pl
     (
         /* IN */ BT_COMMON_UL_INIT_CB_PL   bt_on_init_cb,
         /* IN */ BT_COMMON_UL_DEINIT_CB_PL bt_off_deinit_cb
     );
#endif /* BT_COMMON_PL_SUPPORT_UL_CB */

#endif /* _H_BT_COMMON_PL_ */

