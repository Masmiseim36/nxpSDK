/**
 *  \file bass_extern.h
 *
 *  \brief This file defines the BASS Profile Client and Server extern usage.
 */

 /*
  *  Copyright (C) 2020. Mindtree Ltd.
  *  All rights reserved.
  */


#ifndef _H_BASS_EXTERN_
#define _H_BASS_EXTERN_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */
#ifdef GA_BASS_CLIENT
extern GA_BRR_PRF_HANDLE bass_ce_brr_prf_handle;
extern BASS_CE_NTF_CB bass_ce_cb;

/** BASS Context for the Client */
GA_DECLARE_GLOBAL_ARRAY(BASS_CE_CONTEXT, bass_ce_instance, GA_CONFIG_LIMITS(BASS_CE_MAX_INSTANCES));

/* Total BASS Characteristic count per context */
extern  UINT16 bass_char_count;

/* Characteristic UUID table */
extern BASS_CHAR_UUID_ELEMENT bass_uuid[];

/**
 * Characteristic configuration enable mask
 * Maintaining a common setting across all servers as this feature
 * is mainly to adapt to the client platform capabilities.
 */
extern UINT32 bass_config;

/* Global BASS Client mutex variable */
GA_DEFINE_MUTEX_TYPE(extern, bass_ce_mutex)
#endif /* GA_BASS_CLIENT */

#ifdef GA_BASS_SERVER
extern BASS_SE_NTF_CB bass_se_cb;
extern BASS_SE_ENTITY bass_se_entity;

/* Global BASS Server mutex variable */
GA_DEFINE_MUTEX_TYPE(extern, bass_se_mutex)
#endif /* GA_BASS_SERVER */

/* --------------------------------------------- Data types /Structures */


/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

#endif /* _H_BASS_EXTERN_ */
