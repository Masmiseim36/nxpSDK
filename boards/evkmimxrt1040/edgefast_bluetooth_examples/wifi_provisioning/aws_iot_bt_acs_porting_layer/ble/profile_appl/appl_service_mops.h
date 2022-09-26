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

#ifdef AIOS
#include "appl_aios_mops.h"
#endif /* AIOS */

#ifdef ANS
#include "appl_ans_mops.h"
#endif /* ANS */

#ifdef BASIC
#include "appl_basic_mops.h"
#endif /* BASIC */

#ifdef PTS
#include "appl_pts_mops.h"
#endif /* PTS */

#ifdef BMS
#include "appl_bms_mops.h"
#endif /* BMS */

#ifdef BPS
#include "appl_bps_mops.h"
#endif /* BPS */

#ifdef CGMS
#include "appl_cgms_mops.h"
#endif /* CGMS */

#ifdef CPMS
#include "appl_cpms_mops.h"
#endif /* CPMS */

#ifdef CSCS
#include "appl_cscs_mops.h"
#endif /* CSCS */

#ifdef CTS
#include "appl_cts_mops.h"
#endif /* CTS */

#ifdef ESS
#include "appl_ess_mops.h"
#endif /* ESS */

#ifdef FMT
#include "appl_fmt_mops.h"
#endif /* FMT */

#ifdef FTMS
#include "appl_ftms_mops.h"
#endif /* FTMS */

#ifdef GLS
#include "appl_gls_mops.h"
#endif /* GLS */

#ifdef HID
#include "appl_hid_mops.h"
#endif /* HID */

#ifdef HRS
#include "appl_hrs_mops.h"
#endif /* HRS */

#ifdef HTS
#include "appl_hts_mops.h"
#endif /* HTS */

#ifdef IPS
#include "appl_ips_mops.h"
#endif /* IPS */

#ifdef IPSPN
#include "appl_ipspn_mops.h"
#endif /* IPSPN */

#ifdef LNS
#include "appl_lns_mops.h"
#endif /* LNS */

#ifdef NDCS
#include "appl_ndcs_mops.h"
#endif /* NDCS */

#ifdef PASS
#include "appl_pass_mops.h"
#endif /* PASS */

#ifdef POS
#include "appl_pos_mops.h"
#endif /* POS */

#ifdef PXR
#include "appl_pxr_mops.h"
#endif /* PXR */

#ifdef RSCS
#include "appl_rscs_mops.h"
#endif /* RSCS */

#ifdef RTUS
#include "appl_rtus_mops.h"
#endif /* RTUS */

#ifdef SPS
#include "appl_sps_mops.h"
#endif /* SPS */

#ifdef TDS
#include "appl_tds_mops.h"
#endif /* TDS */

#ifdef TPMS
#include "appl_tpms_mops.h"
#endif /* TPMS */

#ifdef VSE1
#include "appl_vse1_mops.h"
#endif /* VSE1 */

#ifdef VSE2
#include "appl_vse2_mops.h"
#endif /* VSE2 */

#ifdef WSS
#include "appl_wss_mops.h"
#endif /* WSS */


/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */

void main_service_menu_operations(void);

#endif /* _H_APPL_SERVICE_MOPS_ */

