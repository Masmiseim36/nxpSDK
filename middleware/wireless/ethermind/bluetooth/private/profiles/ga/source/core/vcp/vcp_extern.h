/**
 *  \file vcp_extern.h
 *
 *  \brief This file defines the Volume Control Profile Interfaces
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */


#ifndef _H_VCP_EXTERN_
#define _H_VCP_EXTERN_

/* --------------------------------------------- Header File Inclusion */
#include "vcp.h"

/* --------------------------------------------- Global Definitions */

/** VCP VCS Contexts */
GA_DECLARE_GLOBAL_ARRAY(VCP_CONTEXT, vcp_context, GA_CONFIG_LIMITS(VCP_MAX_VCS_CONTEXTS));

/** VCP VOCS Contexts */
GA_DECLARE_GLOBAL_ARRAY(VCP_VOCS_CONTEXT, vcp_vocs_context, GA_CONFIG_LIMITS(VCP_MAX_VOCS_CONTEXTS));

/** VCP AICS Contexts */
GA_DECLARE_GLOBAL_ARRAY(VCP_AICS_CONTEXT, vcp_aics_context, GA_CONFIG_LIMITS(VCP_MAX_AICS_CONTEXTS));

extern GA_BRR_PRF_HANDLE vcp_brr_id;

extern GA_CHAR_UUID vcp_vcs_uuid[VCS_CHAR_ID_COUNT];
extern GA_CHAR_UUID vcp_vocs_uuid[VOCS_CHAR_ID_COUNT];
extern GA_CHAR_UUID vcp_aics_uuid[AICS_CHAR_ID_COUNT];

extern UINT32 vcp_vcs_config;
extern UINT32 vcp_vocs_config;
extern UINT32 vcp_aics_config;

/* Global VCP Mutex Variable */
GA_DEFINE_MUTEX_TYPE(extern, vcc_mutex);
GA_DEFINE_MUTEX_TYPE(extern, vcs_mutex);

#ifdef VCP_RENDERER
extern VCS_ENTTITY      vcs_entity;
GA_DECLARE_GLOBAL_ARRAY(VCS_VOCS_ENTITY, vcs_vocs_entities, GA_CONFIG_LIMITS(VCP_MAX_VOCS_ENTITIES));
GA_DECLARE_GLOBAL_ARRAY(VCS_AICS_ENTITY, vcs_aics_entities, GA_CONFIG_LIMITS(VCP_MAX_AICS_ENTITIES));
#endif /* VCP_RENDERER */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

#endif /* _H_VCP_EXTERN_ */
