
/** 
 *  \file bt_pal_avrcp_al_internal.h
 * 
 *  This Header File contains the high internal functions declarations
 *  of avrcp abstraction layer.
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_AVRCP_AL_INTERNAL_
#define _H_AVRCP_AL_INTERNAL_

/* --------------------------------------------- Header File Inclusion */
#include "bt_pal_avrcp_al_api.h"

#if ((defined AVRCP_TG) || (defined AVRCP_CT))

/* ------------------------- AVRCP AL Debug Macros */

#ifndef AVRCP_AL_NO_DEBUG
#define AVRCP_AL_ERR(...)                                BT_debug_error(BT_MODULE_ID_AVRCP, __VA_ARGS__)
#else  /* AVRCP_AL_NO_DEBUG */
#define AVRCP_AL_ERR                                     BT_debug_null
#endif /* AVRCP_AL_NO_DEBUG */

#ifdef AVRCP_AL_DEBUG

#define AVRCP_AL_TRC(...)                                BT_debug_trace(BT_MODULE_ID_AVRCP, __VA_ARGS__)
#define AVRCP_AL_INF(...)                                BT_debug_info(BT_MODULE_ID_AVRCP, __VA_ARGS__)

#else /* AVRCP_DEBUG */

#define AVRCP_AL_TRC                                     BT_debug_null
#define AVRCP_AL_INF                                     BT_debug_null

#endif /* AVRCP_AL_DEBUG */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */
/** Lock the AVRCP global mutex */
#define avrcp_al_lock()         BT_MUTEX_LOCK(avrcp_al_mutex, AVRCP)

/** Unlock the AVRCP global mutex */
#define avrcp_al_unlock()       BT_MUTEX_UNLOCK(avrcp_al_mutex, AVRCP)

/** Lock the AVRCP global mutex */
#define avrcp_al_lock_void()    BT_MUTEX_LOCK_VOID(avrcp_al_mutex, AVRCP)

/** Unlock the AVRCP global mutex */
#define avrcp_al_unlock_void()  BT_MUTEX_UNLOCK_VOID(avrcp_al_mutex, AVRCP)


#define         AVRCP_AL_STATE_INVALID              0xFF
#define         AVRCP_AL_STATE_INITIALIZED          0x00

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

/* AVRCP Notification Callback Handler */
API_RESULT avrcp_al_ntf_cb
           (
               UCHAR  event_type,
               UINT16 event_result,
               AVRCP_HANDLE * avrcp_handle,
               void  * event_data,
               UINT16 event_datalen
           );

#endif /* ((defined AVRCP_TG) || (defined AVRCP_CT)) */

#endif /* _H_AVRCP_AL_INTERNAL_ */

