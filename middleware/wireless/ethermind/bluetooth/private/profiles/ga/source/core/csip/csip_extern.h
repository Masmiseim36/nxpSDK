/**
 *  \file csip_extern.h
 *
 *  \brief This file defines the Coordinated Set Identification Profile Interfaces
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */


#ifndef _H_CSIP_EXTERN_
#define _H_CSIP_EXTERN_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */
/** CSIP Contexts */
GA_DECLARE_GLOBAL_ARRAY(CSIP_CONTEXT, csip_context, GA_CONFIG_LIMITS(CSIP_MAX_CONTEXTS));

/** CSIP Entities */
GA_DECLARE_GLOBAL_ARRAY(CSIP_ENTITY, csip_entity, GA_CONFIG_LIMITS(CSIP_MAX_CSIS_ENTITIES));

/** SIRK input variabes */
extern UCHAR csip_sirk[16];
extern UCHAR csip_key[16];

/** CSIP Profile ID with the bearer */
extern GA_BRR_PRF_HANDLE csip_brr_id;

/** SIRK Procedure callback */
extern CSIP_SIRK_CB csip_sirk_callback;

/* Characteristic UUID table for CSIS */
extern DECL_CONST GA_CHAR_UUID csip_csis_uuid[CSIS_CHAR_ID_COUNT];

/* Characterisitc configuration enable mask */
extern UINT32 csip_csis_config;

/* Mutex variable for CSIP */
GA_DEFINE_MUTEX_TYPE(extern, csic_mutex)
GA_DEFINE_MUTEX_TYPE(extern, csis_mutex)

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

#endif /* _H_CSIP_EXTERN_ */

