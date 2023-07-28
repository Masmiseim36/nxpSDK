/**
 *  \file appl_ga.h
 *
 *  Reference application for GA Core interfaces
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_
#define _H_APPL_GA_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"

#include "appl_hci_le.h"
#include "appl_smp.h"
#include "ga_gatt_db.h"
#include "ga_brr_pl.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_hci_event_callback
          (
              UINT8   event_code,
              UINT8 * event_data,
              UINT8   event_datalen
          );

GA_RESULT appl_ga_hci_iso_data_ind_cb
          (
              UCHAR  * header,
              UCHAR  * data,
              UINT16  datalen
          );

GA_RESULT appl_ga_cap_uc_client_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

GA_RESULT appl_ga_cap_uc_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

GA_RESULT appl_ga_cap_bc_src_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

GA_RESULT appl_ga_cap_bc_sink_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

int main_ga_operations(void);

GA_RESULT appl_ga_authenticate(UCHAR security_mode);

GA_RESULT appl_ga_disconnect(void);

#endif /* _H_APPL_GA_ */