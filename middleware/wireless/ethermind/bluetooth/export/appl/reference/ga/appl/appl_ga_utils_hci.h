/**
 *  \file appl_ga_utils_hci.c
 *
 *  \brief Reference application for HCI APIs and event callbacks for GA
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_UTILS_HCI
#define _H_APPL_GA_UTILS_HCI

#include "appl_hci.h"
#include "GA_bearer_api.h"
#include "appl_ga_common.h"

/* --------------------------------------------- Header File Inclusion */

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- APIs */

GA_RESULT appl_ga_utils_hci_event_callback
           (
               UINT8   event_code,
               UINT8 * event_data,
               UINT8   event_datalen
           );

GA_RESULT appl_ga_utils_hci_set_host_feature
          (
              /* IN */ UCHAR bit_number,
              /* IN */ UCHAR bit_value
          );

GA_RESULT appl_ga_utils_hci_register_callback(void);
GA_RESULT appl_ga_utils_hci_unregister_callback(void);

void appl_ga_utils_hci_add_ea_to_db(UINT8* addr, UINT8 addr_type);
void appl_ga_utils_hci_clear_ea_db(void);
UINT8 appl_ga_utils_hci_is_ea_in_db(UINT8* addr, UINT8 addr_type);
void appl_ga_utils_hci_extract_addr_from_ea(UINT8* ea_report, UINT16 ea_reportlen, GA_ENDPOINT* device);

#endif /* _H_APPL_GA_UTILS_HCI */