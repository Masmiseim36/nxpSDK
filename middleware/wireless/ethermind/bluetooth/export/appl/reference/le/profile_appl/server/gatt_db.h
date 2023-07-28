/**
 *  \file gatt_db.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_
#define _H_GATT_DB_

#define APPL_MANUFACTURER_NAME "NXP"
extern unsigned char appl_manufacturer_name_ext[];

#if ((defined BASIC) && (defined GATT_DB))
#include "gatt_db_basic.h"
#endif

#if ((defined BPS) && (defined GATT_DB))
#include "gatt_db_bps.h"
#endif

#if ((defined CPMS) && (defined GATT_DB))
#include "gatt_db_cpms.h"
#endif

#if ((defined CSCS) && (defined GATT_DB))
#include "gatt_db_cscs.h"
#endif

#if ((defined CTS) && (defined GATT_DB))
#include "gatt_db_cts.h"
#endif

#if ((defined GLS) && (defined GATT_DB))
#include "gatt_db_gls.h"
#endif

#if ((defined HID) && (defined GATT_DB))
#include "gatt_db_hid.h"
#endif

#if ((defined HPS) && (defined GATT_DB))
#include "gatt_db_hps.h"
#endif

#if ((defined BT_HRS) && (defined GATT_DB))
#include "gatt_db_hrs.h"
#endif

#if ((defined HTS) && (defined GATT_DB))
#include "gatt_db_hts.h"
#endif

#if ((defined IPSPN) && (defined GATT_DB))
#include "gatt_db_ipspn.h"
#endif

#if ((defined PASS) && (defined GATT_DB))
#include "gatt_db_pass.h"
#endif

#if ((defined PTS) && (defined GATT_DB))
#include "gatt_db_pts.h"
#endif

#if ((defined PXR) && (defined GATT_DB))
#include "gatt_db_pxr.h"
#endif

#if ((defined RSCS) && (defined GATT_DB))
#include "gatt_db_rscs.h"
#endif

#if ((defined TIS) && (defined GATT_DB))
#include "gatt_db_tis.h"
#endif

#endif /* _H_GATT_DB_ */
