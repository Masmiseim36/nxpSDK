
/**
 *  \file BT_storage.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_BT_STORAGE_
#define _H_BT_STORAGE_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* --------------------------------------------- Global Definitions */
/**
 * \addtogroup bt_utils Utilities
 * \{
 */
/**
 * \defgroup  storage_module  Storage
 * \{
 *  This section describes the interfaces & APIs offered by the EtherMind
 *  Storage module to the Application and other upper layers of the stack.
 *
 */
/**
 * \defgroup  storage_defines Defines
 * \{
 * Describes defines for Storage module.
 */
/**
 * \defgroup storage_constants Constants
 * \{
 * Describes Constants defined by the module.
 */
/* Storage types currently supported */
#define STORAGE_TYPE_PERSISTENT                  0U
#ifndef STORAGE_RETENTION_SUPPORT
#define STORAGE_NUM_TYPES                        1U
#else /* STORAGE_RETENTION_SUPPORT */
#define STORAGE_TYPE_RETENTION                   1U
#define STORAGE_NUM_TYPES                        2U
#endif /* STORAGE_RETENTION_SUPPORT */

/* Storage Operations */
/**
 * @name  Storage Operations
 *
 * Constant Definitions for Storage Operations
 */
/*@{*/
#define STORAGE_STORE                            0x00U
#define STORAGE_LOAD                             0x01U
/*@}*/
/* Core module persistent storage IDs */
/**
 * @name  Core module persistent storage IDs
 *
 * Constant Definitions for Core module persistent storage IDs
 */
/*@{*/
#define STORAGE_DQ_PID                           0U
#define STORAGE_SMP_PID                          1U
#define STORAGE_SM_PID                           2U
#define STORAGE_MCAP_PID                         3U
/*@}*/
/* Number of core module persistent storage IDs */
#define STORAGE_NUM_PIDS                         4U

/* Core module retention storage IDs */

/* Number of core module retention storage IDs */
#define STORAGE_NUM_RIDS                         0U

/**
 *  Storage Events
 *  - AUTH_UPDATE
 *  - SHUTDOWN
 *  - OTHERS
 *
 *  Many granular events can be defined in the future, based on the requirement.
 */

/* Storage Events */
/* Authentication Update */
#define STORAGE_EVENT_AUTH_UPDATE                 0x0001U

/* Shutdown */
#define STORAGE_EVENT_SHUTDOWN                    0x0002U

/* Others */
#define STORAGE_EVENT_OTHERS                      0x0004U

/* All events - used as mask to enable all storage events */
#define STORAGE_EVENTS_ALL                        \
            (STORAGE_EVENT_AUTH_UPDATE) |         \
            (STORAGE_EVENT_SHUTDOWN) |            \
            (STORAGE_EVENT_OTHERS)
/** \} */
/* --------------------------------------------- Structures/Data Types */
/**
 * \defgroup storage_structures Structures
 * \{
 * Describes Structures defined by the module.
 */
typedef void (*STORAGE_CB)(UCHAR id, UCHAR type, UCHAR action) DECL_REENTRANT;
/** \} */
/** \} */
/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
#ifdef __cplusplus
extern "C" {
#endif
/**
 * \defgroup storage_API API Definitions
 * \{
 * Describes API definitions of this module.
 */
#ifdef STORAGE_HAVE_EVENT_MASK
/**
 *  API to enable storage events
 */
API_RESULT BT_storage_enable_events
           (
               UINT16    storage_events
           );
#endif /* STORAGE_HAVE_EVENT_MASK */

API_RESULT BT_storage_register_db
           (
               /* IN */ UCHAR  id,
               /* IN */ UCHAR  type,
               /* IN */ STORAGE_CB cb
           );

API_RESULT BT_storage_update_db
           (
               /* IN */ UCHAR id,
               /* IN */ UCHAR type,
               /* IN */ STORAGE_CB cb
           );

API_RESULT BT_storage_store_db
           (
               /* IN */ UCHAR type,
               /* IN */ UINT16 event_type
           );

#ifdef STORAGE_HAVE_WRITE_CHECK
API_RESULT BT_storage_disable_store(void);
API_RESULT BT_storage_sync_db
           (
               /* IN */ UCHAR type
           );
#else /* STORAGE_HAVE_WRITE_CHECK */
#define BT_storage_disable_store()
#define BT_storage_sync_db(t)
#endif /* STORAGE_HAVE_WRITE_CHECK */

API_RESULT BT_storage_load_db (UCHAR type);

UINT16 BT_storage_write
       (
           UCHAR id,
           UCHAR type,
           void * data_param,
           UINT16 datalen
       );

UINT16 BT_storage_read
       (
           UCHAR id,
           UCHAR type,
           void * data_param,
           UINT16 datalen
       );

#ifdef __cplusplus
};
#endif
/** \} */
/** \} */
/** \} */
#endif /* _H_BT_STORAGE_ */

