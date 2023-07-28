/**
 *  \file hap_extern.h
 *
 *  \brief This file defines the Hearing Access Profile Client and Server extern usage.
 */

 /*
  *  Copyright (C) 2020. Mindtree Ltd.
  *  All rights reserved.
  */


#ifndef _H_HAP_EXTERN_
#define _H_HAP_EXTERN_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */
/* Application callback */
extern HAP_NTF_CB hap_cb;

#ifdef HAP_CLIENT

extern GA_BRR_PRF_HANDLE hap_brr_prf_handle;

/* Characteristic UUID table for HAS */
extern DECL_CONST GA_CHAR_UUID hap_has_uuid[HAP_CHAR_ID_COUNT];

/* Characteristic configuration enable mask */
extern UINT32 hap_has_config;

GA_DECLARE_GLOBAL_ARRAY(HAP_CE, hap_entity, GA_CONFIG_LIMITS(HAP_MAX_HAS_CONTEXTS));
#endif /* HAP_CLIENT */

#ifdef HAP_SERVER
/* HAP Service Entity */
extern HAP_SE has_entity;
#endif /* HAP_SERVER */

/* Mutex variable for HAP */
GA_DEFINE_MUTEX_TYPE(extern, hap_mutex)

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

#endif /* _H_HAP_EXTERN_ */
