
/**
 *  \file appl_gap_le.h
 *
 *  LE GAP Application Header File
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_GAP_LE_
#define _H_APPL_GAP_LE_

/* --------------------------------------------- Header File Inclusion */
#include "appl_le.h"

/* --------------------------------------------- Global Definitions */
/**
 * GAP related defines
 */

#define API_GAP_INCORRECT_STATE    (APPL_ERR_ID | 0x50)

#define APPL_GAP_MAX_ADV_DATA_LEN          32

/* Advertisement related defines */
#define APPL_GAP_ADV_IDLE                  0x00
#define APPL_GAP_ADV_UPDATING_DATA         0x01
#define APPL_GAP_ADV_UPDATING_PARAM        0x02
#define APPL_GAP_ADV_ADVERTISING           0x03

/* Scan Related Defines */
#define APPL_GAP_SCAN_IDLE                 0x00
#define APPL_GAP_SCAN_UPDATING_PARAM       0x01
#define APPL_GAP_SCAN_SCANNING             0x02

/* Connection related defines */
#define APPL_GAP_CONN_IDLE                 0x00
#define APPL_GAP_CONN_CONNECTING           0x01
#define APPL_GAP_CONN_CONNECTED            0x02

/* --------------------------------------------- Data Types/ Structures */

typedef struct
{
    /* Describes ADV Data */
    UCHAR data_param[APPL_GAP_MAX_ADV_DATA_LEN];

    /* Length of data, max value is 32 */
    UCHAR len;

}APPL_GAP_ADV_DATA;

typedef struct
{
    UINT16     min_interval;

    UINT16     max_interval;

    UCHAR      channel_map;

    UCHAR      filter_policy;

}APPL_GAP_ADV_PARAM;

typedef struct
{
    UINT16     scan_interval;

    UCHAR      scan_window;

    UCHAR      filter_policy;

}APPL_GAP_SCAN_PARAM;

typedef struct
{
    UINT16     scan_interval;

    UINT16     scan_window;

    UCHAR      initiator_filter_policy;

    UINT16     conn_interval_min;

    UINT16     conn_interval_max;

    UINT16     conn_latency;

    UINT16     supervision_timeout;

    UINT16     min_ce_length;

    UINT16     max_ce_length;

}APPL_GAP_CONN_PARAM;

typedef struct
{
    /* Describes various possibilities of adv data */
    DECL_CONST APPL_GAP_ADV_DATA     * adv_data;

    DECL_CONST APPL_GAP_ADV_PARAM    * adv_param;

    UCHAR                            adv_param_id;

    UCHAR                            adv_type;

    /**
     * Current State of advertising
     * - IDLE
     * - UPDATING_ADV_DATA
     * - UPDATING_ADV_PARAM
     * - ADVERTISING
     */
    UCHAR                            adv_state;

}APPL_GAP_ADV_INFO;

typedef struct
{
    DECL_CONST APPL_GAP_SCAN_PARAM    * scan_param;

    UCHAR                             scan_type;

    UCHAR                             scan_param_id;

    /**
     * Current State of scanning
     * - IDLE
     * - UPDATING_SCAN_PARAM
     * - SCANNING
     */
    UCHAR                             scan_state;

}APPL_GAP_SCAN_INFO;

typedef struct
{
    DECL_CONST APPL_GAP_CONN_PARAM    * conn_param;

    UCHAR                             conn_param_id;

}APPL_GAP_CONN_INFO;

/* --------------------------------------------- Externs */

extern APPL_GAP_ADV_INFO    appl_gap_adv_table;

extern APPL_GAP_SCAN_INFO   appl_gap_scan_table;

extern APPL_GAP_CONN_INFO   appl_gap_conn_table;

#define APPL_GAP_ADV_GET_STATE()\
        appl_gap_adv_table.adv_state

#define APPL_GAP_ADV_SET_STATE(x)\
        APPL_GAP_ADV_GET_STATE() = (x)

#define APPL_GAP_ADV_IS_STATE(x)\
        (((x) == APPL_GAP_ADV_GET_STATE()) ? BT_TRUE : BT_FALSE)

#define APPL_GAP_SCAN_GET_STATE()\
        appl_gap_scan_table.scan_state

#define APPL_GAP_SCAN_SET_STATE(x)\
        APPL_GAP_SCAN_GET_STATE() = (x)

#define APPL_GAP_SCAN_IS_STATE(x)\
        (((x) == APPL_GAP_SCAN_GET_STATE()) ? BT_TRUE : BT_FALSE)


#define APPL_GAP_ACCESS_ADV_DATA_LEN(x)\
        appl_gap_adv_table.adv_data[(x)].len

#define APPL_GAP_ACCESS_ADV_DATA(x)\
        ((UCHAR *)&appl_gap_adv_table.adv_data[(x)].data_param[0])

#define APPL_GAP_GET_ADV_MIN_INTERVAL(x)\
        appl_gap_adv_table.adv_param[(x)].min_interval

#define APPL_GAP_GET_ADV_MAX_INTERVAL(x)\
        appl_gap_adv_table.adv_param[(x)].max_interval

#define APPL_GAP_GET_ADV_CHANNEL_MAP(x)\
        appl_gap_adv_table.adv_param[(x)].channel_map

#define APPL_GAP_GET_ADV_FILTER_POLICY(x)\
        appl_gap_adv_table.adv_param[(x)].filter_policy

#define APPL_GAP_GET_ADV_TYPE()\
        appl_gap_adv_table.adv_type

#define APPL_GAP_SET_ADV_TYPE(x)\
        APPL_GAP_GET_ADV_TYPE() = (x)

#define APPL_GAP_GET_ADV_PARAM_ID\
        appl_gap_adv_table.adv_param_id

#define APPL_GAP_SET_ADV_PARAM_ID(x)\
        APPL_GAP_GET_ADV_PARAM_ID = (x)

#define APPL_GAP_GET_SCAN_TYPE()\
        appl_gap_scan_table.scan_type

#define APPL_GAP_SET_SCAN_TYPE(x)\
        APPL_GAP_GET_SCAN_TYPE() = (x)

#define APPL_GAP_GET_SCAN_INTERVAL(x)\
        appl_gap_scan_table.scan_param[(x)].scan_interval

#define APPL_GAP_GET_SCAN_WINDOW(x)\
        appl_gap_scan_table.scan_param[(x)].scan_window

#define APPL_GAP_GET_SCAN_FILTER_POLICY(x)\
        appl_gap_scan_table.scan_param[(x)].filter_policy

#define APPL_GAP_GET_CONN_SCAN_INTERVAL(x)\
        appl_gap_conn_table.conn_param[(x)].scan_interval

#define APPL_GAP_GET_CONN_SCAN_WINDOW(x)\
        appl_gap_conn_table.conn_param[(x)].scan_window

#define APPL_GAP_GET_CONN_INITIATOR_FILTER_POLICY(x)\
        appl_gap_conn_table.conn_param[(x)].initiator_filter_policy

#define APPL_GAP_GET_CONN_INTERVAL_MIN(x)\
        appl_gap_conn_table.conn_param[(x)].conn_interval_min

#define APPL_GAP_GET_CONN_INTERVAL_MAX(x)\
        appl_gap_conn_table.conn_param[(x)].conn_interval_max

#define APPL_GAP_GET_CONN_LATENCY(x)\
        appl_gap_conn_table.conn_param[(x)].conn_latency

#define APPL_GAP_GET_CONN_SUPERVISION_TIMEOUT(x)\
        appl_gap_conn_table.conn_param[(x)].supervision_timeout

#define APPL_GAP_GET_CONN_MIN_CE_LENGTH(x)\
        appl_gap_conn_table.conn_param[(x)].min_ce_length

#define APPL_GAP_GET_CONN_MAX_CE_LENGTH(x)\
        appl_gap_conn_table.conn_param[(x)].max_ce_length

#define APPL_DIRECT_ADDR_TYPE()               0
#define APPL_OWN_ADDR_TYPE()                  0

#define APPL_ADV_DATA_ID             0
#define APPL_GAP_ADV_NRML_PARAM_ID   0
#define APPL_GAP_ADV_LP_PARAM_ID     2
#define APPL_NRML_SCAN_PARAM_ID      0
#define APPL_LP_SCAN_PARAM_ID        2
#define APPL_PASSIVE_SCAN_TYPE       0

#define APPL_FAST_CONNECT_PARAM_ID   0
/**
 *  NOTE :
 *  Use of Fast and  Normal Connection Parameters are dependent on
 *  use case and may be identical if there is no differentiation
 *  between data operating conncetion requirement and
 *  discovery or first time conncetion requirement.
 */
#define APPL_NRML_CONNECT_PARAM_ID   0
/* --------------------------------------------- Functions */


#define appl_gap_set_adv_data_complete(x)\
        BT_hci_le_set_advertising_parameters\
        (\
            APPL_GAP_GET_ADV_MIN_INTERVAL(x),\
            APPL_GAP_GET_ADV_MAX_INTERVAL(x),\
            APPL_GAP_GET_ADV_TYPE(),\
            APPL_OWN_ADDR_TYPE(),\
            APPL_DIRECT_ADDR_TYPE(),\
            APPL_CONFIG_PEER_DEVICE_ADDR(),\
            APPL_GAP_GET_ADV_CHANNEL_MAP (x),\
            APPL_GAP_GET_ADV_FILTER_POLICY(x)\
        );\
        APPL_GAP_ADV_SET_STATE(APPL_GAP_ADV_UPDATING_PARAM)

#define appl_gap_set_adv_param_complete()\
        BT_hci_le_set_advertising_enable(1);\
        APPL_GAP_ADV_SET_STATE(APPL_GAP_ADV_ADVERTISING)

#define appl_gap_set_scan_param_complete()\
        BT_hci_le_set_scan_enable(1, 1);\
        APPL_GAP_SCAN_SET_STATE(APPL_GAP_SCAN_SCANNING)

#define appl_strt_nrml_undrct_cntbl_adv(adv_data_id)\
        appl_gap_start_advertising\
        (\
             (adv_data_id),\
             APPL_GAP_ADV_NRML_PARAM_ID,\
             0x00\
        )

#define appl_strt_lp_undrct_cntbl_adv(adv_data_id)\
        appl_gap_start_advertising\
        (\
             (adv_data_id),\
             APPL_GAP_ADV_LP_PARAM_ID,\
             0x00\
        )

API_RESULT appl_gap_start_advertising
           (
               /* IN */ UCHAR     adv_data_id,
               /* IN */ UCHAR     adv_param_id,
               /* IN */ UCHAR     adv_type
           );

API_RESULT appl_gap_stop_advertising (void);

#define appl_strt_nrml_psv_scan()\
        appl_gap_start_scanning\
        (\
             APPL_NRML_SCAN_PARAM_ID,\
             APPL_PASSIVE_SCAN_TYPE\
        )


#define appl_strt_lp_psv_scan()\
        appl_gap_start_scanning\
        (\
             APPL_LP_SCAN_PARAM_ID,\
             APPL_PASSIVE_SCAN_TYPE\
        )

API_RESULT appl_gap_start_scanning
           (
               /* IN */ UCHAR     scan_param_id,
               /* IN */ UCHAR     scan_type
           );

API_RESULT appl_gap_stop_scanning (void);

#define appl_init_fast_connection(bd_addr)\
        appl_gap_initiate_connection\
        (\
             bd_addr,\
             APPL_FAST_CONNECT_PARAM_ID\
        )


#define appl_init_nrml_connection(bd_addr)\
        appl_gap_initiate_connection\
        (\
             bd_addr,\
             APPL_NRML_CONNECT_PARAM_ID\
        )

API_RESULT appl_gap_initiate_connection
           (
               /* IN */ BT_DEVICE_ADDR * bd_addr,
               /* IN */ UCHAR            conn_param_id
           );

#endif /* _H_APPL_GAP_LE_ */

