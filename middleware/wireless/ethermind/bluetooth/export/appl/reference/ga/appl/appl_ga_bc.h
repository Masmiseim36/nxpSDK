/**
 *  \file appl_ga_bc.h
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Broadcast exposed by BAP Client and BAP Server library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_BC_
#define _H_APPL_GA_BC_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_bc_src_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

GA_RESULT appl_ga_bc_sink_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

void appl_ga_bc_init(void);

void main_broadcast_operations(void);

#endif /* _H_APPL_GA_BC_ */