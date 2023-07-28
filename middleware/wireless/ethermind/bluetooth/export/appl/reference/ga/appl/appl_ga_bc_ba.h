/**
 *  \file appl_ga_bc_ba.h
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Broadcast source exposed by BAP library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_BC_BA
#define _H_APPL_GA_BC_BA

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "GA_bass_api.h"

#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

#define IS_APPL_GA_BC_ROLE_BA(role) \
        (BC_ROLE_BA == role)

#define IS_APPL_GA_BC_ROLE_BC_SRC(role) \
        (BC_ROLE_BC_SRC == role)

#define IS_APPL_GA_BC_ROLE_BA_PLUS_BC_SRC(role) \
        ((BC_ROLE_BA & role) && (BC_ROLE_BC_SRC & role))
/* --------------------------------------------- APIs */

GA_RESULT appl_ga_bc_ba_hci_cb_handler
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8   event_datalen
          );

GA_RESULT appl_ga_bc_ba_bc_sink_handler
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         ga_event,
              /* IN */ UINT16        ga_status,
              /* IN */ void        * ga_data,
              /* IN */ UINT16        ga_datalen
          );

GA_RESULT appl_ga_bc_src_handler
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         ga_event,
              /* IN */ UINT16        ga_status,
              /* IN */ void        * ga_data,
              /* IN */ UINT16        ga_datalen
          );

void appl_ga_bc_ba_snk_init(void);
void appl_ga_bc_ba_src_init(void);

void main_ba_operations(void);

void appl_ga_bc_set_src_setting(APPL_GA_ROLE  role);

#endif /* _H_APPL_GA_BC_BA */
