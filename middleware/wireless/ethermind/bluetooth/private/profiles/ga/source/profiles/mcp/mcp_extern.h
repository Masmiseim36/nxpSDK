/**
 *  \file mcp_extern.h
 *
 *  \brief This file defines the Media Control Profile Client and Server extern usage.
 */

 /*
  *  Copyright (C) 2020. Mindtree Ltd.
  *  All rights reserved.
  */


#ifndef _H_MCP_EXTERN_
#define _H_MCP_EXTERN_

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */
#ifdef MCP_CLIENT
GA_DECLARE_GLOBAL_ARRAY(MCP_CE, mcp_ce_instance, GA_CONFIG_LIMITS(MCP_MAX_CONTEXTS));

GA_DECLARE_GLOBAL_ARRAY(MCP_CE_MCS, mcp_ce_mcs_instance, GA_CONFIG_LIMITS(MCP_MAX_MCS_CONTEXTS));

/* Global MCP Client mutex variable */
GA_DEFINE_MUTEX_TYPE(extern,mcp_ce_mutex)

extern UINT16 config_hndl;

extern GA_BRR_PRF_HANDLE mcp_ce_brr_prf_handle;

/* Application callback */
extern MCP_CE_NTF_CB mcp_ce_cb;

/* Characteristic UUID table for GMCS/MCS */
extern DECL_CONST GA_CHAR_UUID mcp_mcs_uuid[MCP_CHAR_ID_COUNT];

/* Characteristic configuration enable mask */
extern UINT32 mcp_mcs_config;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/* OTC Profile ID registered with the OTC Module */
extern OTP_CE_NTF_CB_ID mcp_ce_otp_ce_ntf_cb_id;
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

#endif /* MCP_CLIENT */

/* --------------------------------------------- Data types /Structures */

#ifdef MCP_SERVER
/* MCS Service Instances added to GATT DB */
GA_DECLARE_GLOBAL_ARRAY(MCP_MCS_ENTITY, mcs_entity, MCP_SE_MAX_MCS_SER_ENTITIES);

/* Global MCP Server mutex variable */
GA_DEFINE_MUTEX_TYPE(extern, mcp_se_mutex)

/* Application callback */
extern MCP_SE_NTF_CB mcp_se_cb;

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
/* Callback registered with GA Bearer for Server Callbacks only */
extern GA_RESULT ots_ga_brr_server_callback
                 (
                     /* IN */ GA_BRR_DEVICE * device,
                     /* IN */ void          * context,
                     /* IN */ OTP_SE_EVT_ID   event_type,
                     /* IN */ UINT16          event_status,
                     /* IN */ void          * event_data,
                     /* IN */ UINT16          event_datalen
                 );
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

#endif /* MCP_SERVER */
/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

#endif /* _H_MCP_EXTERN_ */
