
/**
 *  \file appl_ga_bc_sd.h
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Broadcast source exposed by BAP library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_BC_SD
#define _H_APPL_GA_BC_SD

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "GA_bass_api.h"

#ifdef BASS_DB
#include "GA_bass_db.h"
#endif /* BASS_DB */

/* GA Bearer Related */
#include "ga_brr_pl.h"

#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */
#define IS_APPL_GA_BC_ROLE_SD(role) \
        (BC_ROLE_SD == role)

#define IS_APPL_GA_BC_ROLE_BC_SINK(role) \
        (BC_ROLE_BC_SNK == role)

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_bc_sd_hci_cb_handler
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8   event_datalen
          );

GA_RESULT appl_ga_bc_sd_sink_cb_handler
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         ga_event,
              /* IN */ UINT16        ga_status,
              /* IN */ void        * ga_data,
              /* IN */ UINT16        ga_datalen
          );

GA_RESULT appl_ga_bc_sink_handler
          (
              /* IN */ GA_ENDPOINT * device,
              /* IN */ UINT8         ga_event,
              /* IN */ UINT16        ga_status,
              /* IN */ void        * ga_data,
              /* IN */ UINT16        ga_datalen
          );

void main_sd_operations(void);

void appl_ga_bc_sd_init(void);
void appl_ga_bc_snk_init(void);

#endif /* _H_APPL_GA_BC_SD */
