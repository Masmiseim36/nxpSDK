/**
 *  \file appl_ga_uc.h
 *
 *  \brief This file is a generic application example that demonstrates the
 *  usage of BAP - Unicast exposed by BAP Client and BAP Server library.
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_UC_
#define _H_APPL_GA_UC_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_uc_client_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

GA_RESULT appl_ga_uc_server_cb_handler
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          );

void appl_ga_uc_init(UINT8 unicast_role);

void main_unicast_operations(void);

#endif /* _H_APPL_GA_UC_ */