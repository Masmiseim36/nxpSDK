
/**
 *  \file csip_internal.h
 *
 *  \brief This file defines the Coordinated Set Identification Profile Interface
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_CSIP_INTERNAL_
#define _H_CSIP_INTERNAL_


/* --------------------------------------------- Header File Inclusion */
#include "csip.h"

/* --------------------------------------------- Debug Mapping */
#ifndef CSIP_NO_DEBUG
#define CSIP_ERR(...)                                GA_debug_error(GA_MODULE_ID_CSIP, __VA_ARGS__)
#else  /* CSIP_NO_DEBUG */
#define CSIP_ERR                                     GA_debug_null
#endif /* CSIP_NO_DEBUG */

#ifdef CSIP_DEBUG

#define CSIP_TRC(...)                                GA_debug_trace(GA_MODULE_ID_CSIP, __VA_ARGS__)
#define CSIP_INF(...)                                GA_debug_info(GA_MODULE_ID_CSIP, __VA_ARGS__)

#define CSIP_debug_dump_bytes(data, datalen) GA_debug_dump_bytes(GA_MODULE_ID_CSIP, (data), (datalen))

#else /* CSIP_DEBUG */

#define CSIP_TRC                                     GA_debug_null
#define CSIP_INF                                     GA_debug_null

#define CSIP_debug_dump_bytes(data, datalen)

#endif /* CSIP_DEBUG */

/* --------------------------------------------- Global Definitions */
/** CSIP Instance availability */
#define CSIP_FREE          0x00U
#define CSIP_ALLOCATED     0x01U

/* --------------------------------------------- Data types /Structures */
/**
 * CSIP Context Strucure
 */
typedef struct _CSIP_CONTEXT
{
    /* Device Endpoint */
    GA_ENDPOINT device;

    /** Service Range */
    GA_BRR_SVC_RANGE range;

    /**
     * Characteristic Information
     */
    GA_CHAR_INFO char_info[CSIS_CHAR_ID_COUNT];

    /**
     * Current characteristic ID being configured
     */
    UINT8 char_config_id;

    /** Context current event */
    UCHAR cevent;

    /** Context state */
    UCHAR state;

} CSIP_CONTEXT;


/**
 * CSIS Server
 */
typedef struct _CSIP_ENTITY
{
    /**
     * Flag to indicate if allocated.
     */
    UCHAR state;

    /* Service Id */
    GA_BRR_SVC_INST csis_id;

    /**
     * Set Identity Resolving Key
     */
    GA_BRR_CHR_INST char_id_SIRK;

    /**
     * Coordinated Set Size
     */
    GA_BRR_CHR_INST char_id_CSET_SIZE;

    /**
     * Set Member Lock
     */
    GA_BRR_CHR_INST char_id_SETM_LOCK;

    /**
     * Set Member Rank
     */
    GA_BRR_CHR_INST char_id_SETM_RANK;

} CSIP_ENTITY;


/* --------------------------------------------- Macros */
/* Lock and Unlock macros for CSIP */
#define CSIC_LOCK()              GA_MUTEX_LOCK(csic_mutex, CSIP)
#define CSIC_LOCK_VOID()         GA_MUTEX_LOCK_VOID(csic_mutex, CSIP)
#define CSIC_UNLOCK()            GA_MUTEX_UNLOCK(csic_mutex, CSIP)
#define CSIC_UNLOCK_VOID()       GA_MUTEX_UNLOCK_VOID(csic_mutex, CSIP)

#define CSIS_LOCK()              GA_MUTEX_LOCK(csis_mutex, CSIP)
#define CSIS_LOCK_VOID()         GA_MUTEX_LOCK_VOID(csis_mutex, CSIP)
#define CSIS_UNLOCK()            GA_MUTEX_UNLOCK(csis_mutex, CSIP)
#define CSIS_UNLOCK_VOID()       GA_MUTEX_UNLOCK_VOID(csis_mutex, CSIP)

/* --------------------------------------------- APIs */
CSIP_CONTEXT * csip_find_context
               (
                   GA_ENDPOINT * device,
                   UINT8 * ci
               );

void csip_handle_composition
     (
         /* IN */ UINT8 ci,
         /* IN */ GA_BRR_SERVICE_COMPOSITION * comp
     );

void csip_handle_char_composition
     (
         UINT8 ci,
         GA_BRR_CHARACTERISTIC_COMPOSITION * comp
     );

GA_RESULT csip_profile_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

GA_RESULT csip_service_callback
          (
              /* IN */ GA_BRR_DEVICE * device,
              /* IN */ void * context,
              /* IN */ UINT8 event_type,
              /* IN */ UINT16 event_status,
              /* IN */ void * event_data,
              /* IN */ UINT16 event_datalen
          );

GA_RESULT csip_get_sid_for_char
          (
              /* IN  */ GA_BRR_CHR_INST   c_inst,
              /* OUT */ GA_BRR_SVC_INST * s_inst
          );

void csip_handle_read_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

void csip_handle_write_request
     (
         /* IN */ GA_ENDPOINT * device,
         /* IN */ GA_BRR_CHR_CONTEXT * chr,
         /* IN */ UINT16 event_status,
         /* IN */ UCHAR * event_data,
         /* IN */ UINT16 event_datalen
     );

void csip_s1
     (
         /* IN */  UCHAR * m,
         /* IN */  UINT16  mlen
     );

void csip_s1_complete (void);

void csip_k1
     (
         /* IN */  UCHAR * n,
         /* IN */  UINT16  nlen,
         /* IN */  UCHAR * salt
     );

void csip_k1t_complete (void);
void csip_k1_complete (void);

void ga_csip_encryption_complete(UCHAR index, UCHAR status, UCHAR * data, UINT16 datalen);

UINT8 csip_get_char_id_from_uuid
      (
          /* IN */ UINT16 uuid
      );

UINT8 csip_get_char_id_from_handle
      (
          /* IN */ UINT16 char_handle,
          /* IN */ GA_CHAR_INFO *ga_char_info
      );

UINT8 csip_get_next_char_config
      (
          /* IN */ UINT8  char_id,
          /* IN */ GA_CHAR_INFO *ga_char_info
      );

#endif /* _H_CSIP_INTERNAL */

