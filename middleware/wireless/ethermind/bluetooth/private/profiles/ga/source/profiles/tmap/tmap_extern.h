/**
 *  \file tmap_extern.h
 *
 *  \brief This file defines the Telephony and Media Audio Profile Client and
 *  Server extern usage.
 */

 /*
  *  Copyright (C) 2020. Mindtree Ltd.
  *  All rights reserved.
  */


#ifndef _H_TMAP_EXTERN_
#define _H_TMAP_EXTERN_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */
/* Application callback */
extern TMAP_NTF_CB tmap_cb;

#ifdef TMAP_CLIENT

extern GA_BRR_PRF_HANDLE tmap_brr_prf_handle;

GA_DECLARE_GLOBAL_ARRAY(TMAP_CE, tmap_entity, GA_CONFIG_LIMITS(TMAP_MAX_TMAS_CONTEXTS));
#endif /* TMAP_CLIENT */

#ifdef TMAP_SERVER
/* GA TMAP Server Entity */
extern TMAP_SE tmas_entity;
#endif /* TMAP_SERVER */

/* Mutex variable for TMAP */
GA_DEFINE_MUTEX_TYPE(extern, tmap_mutex)

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

#endif /* _H_TMAP_EXTERN_ */
