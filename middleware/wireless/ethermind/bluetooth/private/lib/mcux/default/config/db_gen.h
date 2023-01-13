
/**
 *  \file db_gen.h
 *
 *  [DBG Version: 1.1.1] This is a generated file.
 *  Created On : Mon Jul 11 20:17:33 2022
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_DB_GEN_
#define _H_DB_GEN_

/* ---------------------------------------------- Header File Inclusion */
#include "BT_common.h"
#include "BT_dbase_api.h"

#ifndef SDP_DYNAMIC_DB

/* --------------------------------------- Global Definitions/Constants */
/* Number of UUIDs present in the SDDB */
#define DB_MAX_UUIDS                      65

/* Number of SDP Records in the SDDB */
#define DB_MAX_RECORDS                    64

#ifdef BT_EIR
/* Number of Service Class UUIDs */
#define SERVICE_CLASS_UUID_INDICES_MAX    46
#endif /* BT_EIR */
#endif /* SDP_DYNAMIC_DB */

/* Service Record Indices */
#define DB_RECORD_SDP  0

#define DB_RECORD_SPP  1

#define DB_RECORD_SPP_VS  2

#define DB_RECORD_HFU  3

#define DB_RECORD_HFAG  4

#define DB_RECORD_HID  5

#define DB_RECORD_NAP  6

#define DB_RECORD_GN  7

#define DB_RECORD_PANU  8

#define DB_RECORD_PRINTER_ADMINISTRATIVE_USER_INTERFACE  9

#define DB_RECORD_DID  10

#define DB_RECORD_HDP  11

#define DB_RECORD_MPS  12

#define DB_RECORD_GAP  13

#define DB_RECORD_GATT  14

#define DB_RECORD_DIS  15

#define DB_RECORD_BAS  16

#define DB_RECORD_RSC  17

#define DB_RECORD_CSC  18

#define DB_RECORD_CPS  19

#define DB_RECORD_HPS  20

#define DB_RECORD_CTS  21

#endif /* _H_DB_GEN_ */

