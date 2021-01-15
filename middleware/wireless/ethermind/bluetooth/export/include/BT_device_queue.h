
/**
 *  \file BT_device_queue.h
 *
 *  Module Header File for Device Queue module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_DEVICE_QUEUE_
#define _H_BT_DEVICE_QUEUE_

/* ----------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* ----------------------------------------------- Type Definition */
typedef UCHAR DEVICE_HANDLE;

typedef UCHAR DEVICE_LINK_TYPE;

#define DEVICE_HANDLE_INIT_VAL     0xFF

#define DEVICE_HANDLE_INIT(hndl)\
        (hndl) = DEVICE_HANDLE_INIT_VAL

#define DQ_LINK_NONE                    0x00
#define DQ_BR_LINK                      0x01
#define DQ_LE_LINK                      0x02
#define DQ_LINK_ANY                     0xFF

/* ----------------------------------------------- Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif

/** Init routine */
void em_device_queue_init ( void );

/** Activating routine */
void device_queue_bt_init ( void );

/** Shutdown routine */
void device_queue_bt_shutdown ( void );

#ifdef BT_DUAL_MODE
/** Enqueue routine */
API_RESULT device_queue_alloc
           (
               /* OUT */ DEVICE_HANDLE       * hndl, /* Device Handle */
               /* IN */  BT_DEVICE_ADDR      * addr,  /* Remote Device to be added*/
               /* IN */  DEVICE_LINK_TYPE    link_type
           );

/* Fetch Link Type */
API_RESULT device_queue_get_link_type
           (
               /* OUT */ DEVICE_LINK_TYPE    * link_type,
               /* IN */  DEVICE_HANDLE       * hndl
           );
#else /* BT_DUAL_MODE */
/** Enqueue routine */
API_RESULT device_queue_alloc
           (
               /* OUT */ DEVICE_HANDLE     * hndl, /* Device Handle */
               /* IN */  BT_DEVICE_ADDR    * addr /* Remote Device to be added*/
           );
#endif /* BT_DUAL_MODE */

/* Delete routine */
API_RESULT device_queue_free
           (
               /* IN */ DEVICE_HANDLE    * hndl /* Device to be removed */
           );

#ifdef BT_DUAL_MODE

#define device_queue_search_br_edr_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a),DQ_BR_LINK)

#define device_queue_search_le_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a),DQ_LE_LINK)

#define device_queue_search_any_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a),DQ_LINK_ANY)

/* Search routine */
API_RESULT device_queue_search_remote_addr
           (
               /* OUT */ DEVICE_HANDLE       * hndl,/* Device Reference */
               /* IN */  BT_DEVICE_ADDR      * addr, /* Remote Device to be searched*/
               /* IN */  DEVICE_LINK_TYPE    link_type
           );
#else /* BT_DUAL_MODE */

#define device_queue_search_br_edr_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a))

#define device_queue_search_le_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a))

#define device_queue_search_any_remote_addr(h,a)\
        device_queue_search_remote_addr ((h),(a))

/* Search routine */
API_RESULT device_queue_search_remote_addr
           (
               /* OUT */ DEVICE_HANDLE     * hndl,/* Device Reference */
               /* IN */  BT_DEVICE_ADDR    * addr /* Remote Device to be searched*/
           );
#endif /* BT_DUAL_MODE */

/* Fetch Remote Device Address */
API_RESULT device_queue_get_remote_addr
           (
               /* IN */  DEVICE_HANDLE     * hndl, /* Device Reference */
               /* OUT */ BT_DEVICE_ADDR    * addr  /* Remote Device Address */
           );

/* Fetch Device Address Only */
API_RESULT device_queue_get_remote_addr_only
           (
               /* IN */  DEVICE_HANDLE    * hndl, /* Device Reference */
               /* OUT */ UCHAR            * addr  /* Remote Device Address */
           );

/* Set remote addr routine */
API_RESULT device_queue_set_remote_addr
           (
               /* IN */ DEVICE_HANDLE     * hndl, /* Device Reference */
               /* IN */ BT_DEVICE_ADDR    * addr  /* Remote Device to be set */
           );

/* Fetch Local Device Address */
API_RESULT device_queue_get_local_addr
           (
               /* IN */  DEVICE_HANDLE     * hndl, /* Device Reference */
               /* OUT */ BT_DEVICE_ADDR    * addr  /* Device's address */
           );

/* Set local addr routine */
API_RESULT device_queue_set_local_addr
           (
               /* IN */ DEVICE_HANDLE     * hndl, /* Device Reference */
               /* IN */ BT_DEVICE_ADDR    * addr  /* Local Device to be set*/
           );

API_RESULT device_queue_set_curr_addr_type
           (
               /* IN */ UCHAR    * addr,/* Local Device to be set*/
               /* IN */ UCHAR    type
           );

#ifdef __cplusplus
};
#endif

#endif /* _H_BT_DEVICE_QUEUE_ */

