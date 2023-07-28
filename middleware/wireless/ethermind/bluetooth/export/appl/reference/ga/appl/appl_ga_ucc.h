/**
 *  \file appl_ga_ucc.h
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Unicast Client exposed by BAP Client and BAP Server library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_UCC
#define _H_APPL_GA_UCC

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

#define APPL_GA_UCC_ROLE_NONE                    (0x00U)
#define APPL_GA_UC_ROLE_UCC                      (0x01U << 0U)

#define MAX_REM_ASCS_INFO                        3U

/* --------------------------------------------- APIs */

void appl_ga_ucc_init(void);

GA_RESULT appl_ga_ucc_hci_cb_handler
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8   event_datalen
          );

GA_RESULT appl_ga_ucc_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

void main_ucc_operations(void);

#endif /* _H_APPL_GA_UCC */
