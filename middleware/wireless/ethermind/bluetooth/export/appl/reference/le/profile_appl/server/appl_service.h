
/**
 *  \file appl_service.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_APPL_SERVICE_
#define _H_APPL_SERVICE_

/* --------------------------------------------- Header File Inclusion */
#include "appl_le.h"

/* GATT DB Generic Header Include */
#include "gatt_db.h"
#include "gatt_db_pl.h"

#if ((defined BPS) && (defined GATT_DB))
#include "appl_bps.h"
#endif

#if ((defined CPMS) && (defined GATT_DB))
#include "appl_cpms.h"
#endif

#if ((defined CSCS) && (defined GATT_DB))
#include "appl_cscs.h"
#endif

#if ((defined GLS) && (defined GATT_DB))
#include "appl_gls.h"
#endif

#if ((defined HID) && (defined GATT_DB))
#include "appl_hid.h"
#endif

#if ((defined HPS) && (defined GATT_DB))
#include "appl_hps.h"
#endif

#if ((defined BT_HRS) && (defined GATT_DB))
#include "appl_hrs.h"
#endif

#if ((defined HTS) && (defined GATT_DB))
#include "appl_hts.h"
#endif

#if ((defined IPSPN) && (defined GATT_DB))
#include "appl_ipspn.h"
#endif

#if ((defined PASS) && (defined GATT_DB))
#include "appl_pass.h"
#endif

#if ((defined PXR) && (defined GATT_DB))
#include "appl_pxr.h"
#endif

#if ((defined RSCS) && (defined GATT_DB))
#include "appl_rscs.h"
#endif

#if ((defined TIS) && (defined GATT_DB))
#include "appl_tis.h"
#endif

/* --------------------------------------------- Global Definitions */
/**
* APPL_ENABLE_AUTHORIZATION flag is enabled in this application while running
* PTS testing related to AUTHORIZATION testcases as this embedded application
* does not have an MMI or GIOP mappings.
* APPL_ENABLE_AUTHORIZATION needs to be enabled in Workspace
*/
#ifdef GATT_DB
#define APPL_ENABLE_AUTHORIZATION
#endif /* GATT_DB */

/**
 * Adv/Scan Procedure types
 * - 0: Fast Procedure
 * - 1: Normal Procedure
 * - 2: Slow Procedure
 */
#define APPL_GAP_PROC_FAST                  0U
#define APPL_GAP_PROC_NORMAL                1U
#define APPL_GAP_PROC_SLOW                  2U

#define APPL_GAP_PROC_TYPES                 3U

/** Advertising data maximum length */
#define APPL_GAP_ADVDATA_LEN                31U

/* Advertising Type defines */
#define APPL_GAP_CONNETBLE_UNDIRTED         HCI_ADV_IND
#define APPL_GAP_SCANNABLE_UNDIRTED         HCI_ADV_SCAN_IND
#define APPL_GAP_NON_CONNCTBLE              HCI_ADV_NONCONN_IND
#define APPL_GAP_CONNETBLE_DIRTED_LOW_DUTY  HCI_ADV_DIRECT_LDC_IND

/**
 * This flag governs the feature of giving flexibility to the application
 * to choose weather to start Advertising or Scanning after BT ON Complete
 * and after subsequent disconnections depending on the current GAP Role
 * selected.
 */
/* #define APPL_SERVICE_CONFIG_GAP_STARTUP */

/* --------------------------------------------- Structures/Data Types */
/** Advertising Data type */
typedef struct _APPL_GAP_ADV_DATA
{
    /** Data */
    UCHAR data[APPL_GAP_ADVDATA_LEN];

    /** Data Length */
    UCHAR datalen;

} APPL_GAP_ADV_DATA;

/** Advertising Parameters type */
typedef struct _APPL_GAP_ADV_PARAM
{
    /* Adv. Min Interval */
    UINT16     min_interval;

    /* Adv. Max Interval */
    UINT16     max_interval;

    /* Adv. Channel Map */
    UCHAR      channel_map;

} APPL_GAP_ADV_PARAM;

/** Scan Parameters type */
typedef struct _APPL_GAP_SCAN_PARAM
{
    /* Scan Interval */
    UINT16     scan_interval;

    /* Scan Window */
    UCHAR      scan_window;

} APPL_GAP_SCAN_PARAM;

/* Connection Parameters Type */
typedef struct _APPL_GAP_CONN_PARAM
{
    /* Scan Interval */
    UINT16     scan_interval;

    /* Scan Window */
    UINT16     scan_window;

    /* Min. Connection Interval */
    UINT16     conn_interval_min;

    /* Max. Connection Interval */
    UINT16     conn_interval_max;

    /* Connection Latency */
    UINT16     conn_latency;

    /* Link Supervision Timeout */
    UINT16     supervision_timeout;

    /* Min. Connection Event Length */
    UINT16     min_ce_length;

    /* Max. Connection Event Length */
    UINT16     max_ce_length;

} APPL_GAP_CONN_PARAM;

/* --------------------------------------------- Macros */
#ifdef APPL_ENABLE_AUTHORIZATION
/* Currently only one generic authorization flag is maintained by the
* application. Provision for the Application Macro is provided
* to access authorization flag based on device id if its needed in future
* use-cases.
*/
#define APPL_GET_AUTHORIZATION(i)                         \
        appl_get_gatt_server_authorization_state((i))

#define APPL_SET_AUTHORIZATION(i,j)                       \
       appl_set_gatt_server_authorization_state((i),(j))
#endif /* APPL_ENABLE_AUTHORIZATION */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void appl_service_init (void);
void appl_service_bt_init(void);
void appl_service_bt_deinit(void);
void appl_service_connect (APPL_HANDLE * handle);
void appl_service_disconnect (APPL_HANDLE * handle);
void appl_service_transport_disconnect (APPL_HANDLE * handle, UCHAR reason);
void appl_service_configure_adv
     (
         UCHAR   proc,
         UCHAR   adv_type,
         UCHAR   l_addr_type,
         UCHAR   p_addr_type,
         UCHAR * p_addr,
         UCHAR   filter_policy
     );
void appl_service_enable_adv (UCHAR enable);
void appl_service_configure_scan
     (
         UCHAR proc,
         UCHAR scan_type,
         UCHAR l_addr_type,
         UCHAR filter_policy
     );
void appl_service_enable_scan (UCHAR enable);
API_RESULT appl_service_initiate_conn
           (
               UCHAR   l_addr_type,
               UCHAR   p_addr_type,
               UCHAR * p_addr,
               UCHAR   filter_policy,
               UCHAR   use_config
           );
void appl_update_battery_lvl(GATT_DB_HANDLE  * batt_handle, UCHAR batt_val);
void appl_get_local_time(UCHAR *buf, UINT16 buf_len);
API_RESULT appl_gatt_db_find_service_info_from_inst
           (
                UCHAR           gatt_service_instance,
                GATT_DB_SERVICE * service_info
           );
API_RESULT appl_set_gatt_service_in_sdp_record
           (
               UCHAR service_instance,
               UCHAR sdp_instance
           );
API_RESULT appl_send_serv_changed_ind(GATT_DB_HANDLE * serv_change_handle);
#ifdef APPL_ENABLE_AUTHORIZATION
API_RESULT appl_check_attr_dev_authorization
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * event_data,
               UINT16        event_datalen
           );
API_RESULT appl_handl_authorization_err
           (
               ATT_HANDLE       * handle,
               UCHAR            event_code,
               ATT_ATTR_HANDLE  attr_handle
           );
UCHAR appl_get_gatt_server_authorization_state(DEVICE_HANDLE dq_handle);
void  appl_set_gatt_server_authorization_state
      (
          DEVICE_HANDLE dq_handle,
          UCHAR         state
      );
#endif /* APPL_ENABLE_AUTHORIZATION */

void appl_service_parse_hv_cnf(ATT_HANDLE * handle, UINT16 evt_result);
void appl_service_parse_ntf_tx_complete(ATT_HANDLE * handle, UCHAR * data, UINT16 datalen);
void appl_service_mtu_updt_handler(ATT_HANDLE * handle, UINT16 mtu);

void appl_service_set_gap_proc_state(UCHAR flag);
UCHAR appl_service_get_gap_proc_state(void);

#endif /* _H_APPL_SERVICE_ */

