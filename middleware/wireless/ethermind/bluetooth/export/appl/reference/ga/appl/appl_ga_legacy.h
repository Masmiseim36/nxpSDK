
/**
 *  \file appl_ga_legacy.h
 *
 *  \brief This file defines the Generic Audio Bearer Internal Interface - includes
 *  Data Structures and Methods.
 */

/**
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GA_LEGACY_
#define _H_APPL_GA_LEGACY_

/* --------------------------------------------- Header File Inclusion */
#include "GA_api.h"
#include "appl_ga_features.h"

#include "appl_vcp.h"
#include "appl_micp.h"
#include "appl_csip.h"

#include "appl_hci_le.h"
#include "appl_smp.h"
#include "ga_gatt_db.h"
#include "ga_brr_pl.h"

#ifdef APPL_GA_LEGACY_SUPPORT

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- Macros */
/* Flags to enable/disable LC3 and Audio path */
#define APPL_GA_USE_AUDIO_PL_FOR_AUDIO_DATA
#define INCLUDE_ETHERMIND_LC3

void appl_ga_init(void);

/*
 * This will take effect only if the non-standard
 * configuration is enabled by defining the GA feature
 * GA_SUPPORT_BC_ANNOUNCEMENT_END_IN_STREAMING */
/* #define APPL_END_ANNOUNCEMENT_ON_STREAMING */

/* --------------------------------------------- APIs */

int main_ga_legacy_operations(void);

API_RESULT appl_ga_hci_event_callback
           (
               UINT8   event_code,
               UINT8 * event_data,
               UINT8   event_datalen
           );

#endif /* APPL_GA_LEGACY_SUPPORT */

#endif /* _H_APPL_GA_LEGACY_ */

