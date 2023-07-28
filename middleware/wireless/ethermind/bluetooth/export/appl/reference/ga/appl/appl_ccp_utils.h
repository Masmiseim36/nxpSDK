/**
 *  \file appl_ccp_utils.h
 *
 *  \brief This file defines the common parser APIs for both CCP SE and CCP CE
 *  applications
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_CCP_UTILS_
#define _H_APPL_CCP_UTILS_

#include "GA_api.h"
#include "appl_utils.h"

#include "appl_ga_utils.h"

#ifdef CCP_CLIENT
#include "GA_ccp_ce_api.h"
#endif /* CCP_CLIENT */

#ifdef CCP_SERVER
#include "GA_ccp_se_api.h"
#endif /* CCP_SERVER */

void appl_ccp_display_ga_result(GA_RESULT event_status);

void appl_ccp_utils_parse_and_display_char_uuid(UINT16 char_uuid);

void appl_ccp_utils_parse_and_display_ccp_call_state(UCHAR call_state);

void appl_ccp_utils_parse_and_display_ccp_cp_opcode(UCHAR  cp_opcode);

void appl_ccp_utils_parse_and_display_ccp_call_flags(UCHAR  call_flag);

void appl_ccp_utils_parse_and_display_ccp_brr_tech(UCHAR  brr_tech);

void appl_ccp_utils_parse_and_display_ccp_brr_signal_strength
     (
         UCHAR signal_strength
     );

void appl_ccp_utils_parse_and_display_ccp_status_flags(UINT16  status_flags);

void appl_ccp_utils_parse_and_display_ccp_cp_opt_opcodes(UINT16  opt_cp_opcodes);

#endif /* _H_APPL_CCP_UTILS_ */