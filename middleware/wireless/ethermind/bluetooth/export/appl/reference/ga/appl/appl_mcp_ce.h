
/**
 *  \file appl_mcp_ce.h
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of MCP - Client role interfaces exposed by MCP Client library.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */
#ifndef _H_APPL_MCP_CE_
#define _H_APPL_MCP_CE_
/* --------------------------------------------- Header File Inclusion */
#include "GA_mcp_ce_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */
GA_RESULT appl_mcp_ce_cb
          (
              /* IN */ GA_ENDPOINT   * device,
              /* IN */ MCP_CE_EVT_ID   evt_id,
              /* IN */ UINT16          evt_status,
              /* IN */ MCP_CE_EVT    * evt_data
          );

void main_mcp_ce_menu_operations(void);

#endif /* _H_APPL_MCP_CE_ */
