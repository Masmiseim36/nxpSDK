
/**
 *  \file appl_ccp_ce.h
 *
 *  \brief This file is the Call Control Profile Client reference application header
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_CCP_CE_
#define _H_APPL_CCP_CE_

/* --------------------------------------------- Header File Inclusion */
#include "appl_ccp.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */
#define  APPL_GTBS_CE_ENITITES          2U
#define  APPL_TBS_CE_ENITITES           2U

#define  APPL_TBS_CE_BRR_CALL_LIST      3U

#define  SAMPLE_URI                  "tel:+0123456789"

/* --------------------------------------------- APIs */

GA_RESULT appl_ccp_ce_callback
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         event_type,
              /* IN */ UINT16        event_status,
              /* IN */ void        * event_data,
              /* IN */ UINT16        event_datalen
          );

void main_ccp_ce_menu_operations(void);

void appl_ccp_ce_parse_and_display_termination_reason
     (
         UCHAR  termination_reason
     );

void appl_ccp_ce_parse_and_display_cp_ntf_data
     (
         CCP_EVENT_INFO * appl_event_info
     );

void appl_ccp_ce_parse_and_display_cp_result_code(UCHAR  result_code);

#endif /* _H_APPL_CCP_CE_ */

