/**
 *  \file appl_ga_ucs.h
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Unicast Server exposed by BAP Client and BAP Server library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_UCS_
#define _H_APPL_GA_UCS_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

#define APPL_GA_UCS_ROLE_NONE                    (0x00U)
#define APPL_GA_UC_ROLE_UCS                      (0x01U << 1U)

#define UCS_CIS_REJECT_REMOTE_USER_TERMINATED    0x13U

#if (MAX_SRC_CODEC_CAP > GA_MAX_SOURCE_PAC_RECORDS)
#error "Maximum supported source Audio Capabilities should be less than Maximum source PAC records"
#endif /* (MAX_SRC_CODEC_CAP > GA_MAX_SOURCE_PAC_RECORDS) */

#if (MAX_SNK_CODEC_CAP > GA_MAX_SINK_PAC_RECORDS)
#error "Maximum supported sink Audio Capabilities should be less than Maximum sink PAC records"
#endif /* (MAX_SNK_CODEC_CAP > GA_MAX_SINK_PAC_RECORDS) */

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_ucs_hci_cb_handler
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8   event_datalen
          );

GA_RESULT appl_ga_ucs_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

void main_ucs_operations(void);

void appl_ga_ucs_init(void);

GA_RESULT appl_ga_ucs_register_ascs(APPL_GA_ROLE role);
#endif /* _H_APPL_GA_UCS_ */
