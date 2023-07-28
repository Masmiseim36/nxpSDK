
/**
 *  \file appl_mcp_se.h
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of MCP - Client role interfaces exposed by MCP Client library.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_MCP_SE_
#define _H_APPL_MCP_SE_
/* --------------------------------------------- Header File Inclusion */
#include "GA_mcp_se_api.h"
#include "appl_ga_common.h"

#ifdef MCP_SUPPORT_OBJECT_TRANSFER
#include "cry_pl.h"
#else /* MCP_SUPPORT_OBJECT_TRANSFER */
#define cry_perform_crc32_op(...)
#endif /* MCP_SUPPORT_OBJECT_TRANSFER */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

GA_RESULT appl_mcp_se_callback
          (
              /* IN */ GA_ENDPOINT           * device,
              /* IN */ GA_MCP_SE_RSP_CONTEXT * ctx,
              /* IN */ MCP_SE_EVT_ID           evt_id,
              /* IN */ UINT16                  evt_status,
              /* IN */ MCP_SE_EVT            * evt_data
          );

void main_mcp_se_menu_operations(void);

void appl_update_default_vlc_char_values (void);
void appl_update_default_mpc_char_values (void);
void appl_mcp_se_otp_se_init_obj (void);
void appl_mcp_se_generate_segment(UINT8 seg_type);

#endif /* _H_APPL_MCP_SE_ */