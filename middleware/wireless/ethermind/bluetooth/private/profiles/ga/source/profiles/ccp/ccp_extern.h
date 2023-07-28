
/**
 *  \file ccp_internal.h
 *
 *  \brief This file defines the Call Control Profile Interface
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_CCP_EXTERN_
#define _H_CCP_EXTERN_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

#ifdef CCP_CLIENT

/* CCP profile instances */
GA_DECLARE_GLOBAL_ARRAY(CCP_CE, ccp_instance, GA_CONFIG_LIMITS(CCP_MAX_CONTEXTS));

/* CCP TBS instances */
GA_DECLARE_GLOBAL_ARRAY(CCP_CE_TBS, ccp_tbs_instance, GA_CONFIG_LIMITS(CCP_MAX_TBS_CONTEXTS));

/* Application Callback */
extern CCP_CE_NTF_CB   ccp_ce_ntf_cb;
extern GA_BRR_PRF_HANDLE ccp_brr_id;

GA_DEFINE_MUTEX_TYPE(extern, ccp_ce_mutex);
#endif /* CCP_CLIENT */

#ifdef CCP_SERVER
/* TBS Server */
GA_DECLARE_GLOBAL_ARRAY(CCP_TBS_ENTITY, tbs_entity, GA_CONFIG_LIMITS(CCP_MAX_TBS_ENTITIES));
extern CCP_SE_NTF_CB   ccp_se_ntf_cb;

/* Global CCP Server mutex variable */
GA_DEFINE_MUTEX_TYPE(extern, ccp_se_mutex);

/* Characteristic UUID table for TBS */
extern DECL_CONST GA_CHAR_UUID ccp_tbs_uuid[CCP_CHAR_ID_COUNT];

/* Characterisitc configuration enable mask */
extern UINT32 ccp_tbs_config;

#endif /* CCP_SERVER */
/* --------------------------------------------- APIs */
#endif /* _H_CCP_EXTERN_ */



