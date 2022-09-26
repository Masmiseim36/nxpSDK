/**
 *  \file appl_service_mops.h
 *
 *  This file contains the Sample Profile Menu Operations header.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_SERVICE_MOPS_
#define _H_APPL_SERVICE_MOPS_

/* --------------------------------------------- Header File Inclusion */
#include "appl_le.h"
#include "appl_utils.h"

/* GATT DB Generic Header Include */
#include "gatt_db.h"

#if ((defined BPS) && (defined GATT_DB))
#include "appl_bps_mops.h"
#endif

#if ((defined CPMS) && (defined GATT_DB))
#include "appl_cpms_mops.h"
#endif

#if ((defined CSCS) && (defined GATT_DB))
#include "appl_cscs_mops.h"
#endif

#if ((defined GLS) && (defined GATT_DB))
#include "appl_gls_mops.h"
#endif

#if ((defined HID) && (defined GATT_DB))
#include "appl_hid_mops.h"
#endif

#if ((defined HPS) && (defined GATT_DB))
#include "appl_hps_mops.h"
#endif

#if ((defined BT_HRS) && (defined GATT_DB))
#include "appl_hrs_mops.h"
#endif

#if ((defined HTS) && (defined GATT_DB))
#include "appl_hts_mops.h"
#endif

#if ((defined IPSPN) && (defined GATT_DB))
#include "appl_ipspn_mops.h"
#endif

#if ((defined PASS) && (defined GATT_DB))
#include "appl_pass_mops.h"
#endif

#if ((defined PXR) && (defined GATT_DB))
#include "appl_pxr_mops.h"
#endif

#if ((defined RSCS) && (defined GATT_DB))
#include "appl_rscs_mops.h"
#endif

#if ((defined TIS) && (defined GATT_DB))
#include "appl_tis_mops.h"
#endif

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

void main_service_menu_operations(void);

#endif /* _H_APPL_SERVICE_MOPS_ */

