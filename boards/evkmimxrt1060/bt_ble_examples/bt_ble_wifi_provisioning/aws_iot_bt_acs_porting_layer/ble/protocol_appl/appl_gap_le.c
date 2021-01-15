
/**
 *  \file appl_gap_le.c
 *
 *  This file contains the LE GAP application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_gap_le.h"

#ifdef BT_LE

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

#if ((defined APPL_GAP_BROADCASTER_SUPPORT) || (defined APPL_GAP_PERIPHERAL_SUPPORT))
API_RESULT appl_gap_start_advertising
     (
         /* IN */ UCHAR     adv_data_id,
         /* IN */ UCHAR     adv_param_id,
         /* IN */ UCHAR     adv_type
     )
{
    API_RESULT retval;

    retval = API_GAP_INCORRECT_STATE;

    /* If Advertising is already intiated, stop and restart */
    if (BT_FALSE != APPL_GAP_ADV_IS_STATE(APPL_GAP_ADV_ADVERTISING))
    {
        appl_gap_stop_advertising();
    }

    if (BT_FALSE != APPL_GAP_ADV_IS_STATE(APPL_GAP_ADV_IDLE))
    {
        retval = BT_hci_le_set_advertising_data
                 (
                      APPL_GAP_ACCESS_ADV_DATA_LEN(adv_data_id),
                      APPL_GAP_ACCESS_ADV_DATA(adv_data_id)
                 );

        if (API_SUCCESS == retval)
        {
            APPL_GAP_ADV_SET_STATE(APPL_GAP_ADV_UPDATING_DATA);
            APPL_GAP_SET_ADV_PARAM_ID(adv_param_id);
            APPL_GAP_SET_ADV_TYPE(adv_type);
        }
    }

    return retval;
}

API_RESULT appl_gap_stop_advertising (void)
{
    BT_hci_le_set_advertising_enable(0);

    APPL_GAP_ADV_SET_STATE(APPL_GAP_ADV_IDLE);

    return API_SUCCESS;
}
#endif /* APPL_GAP_BROADCASTER || APPL_GAP_PERIPHERAL_SUPPORT */


#if ((defined APPL_GAP_OBSERVER_SUPPORT) || (defined APPL_GAP_CENTRAL_SUPPORT))
API_RESULT appl_gap_start_scanning
           (
               /* IN */ UCHAR scan_param_id,
               /* IN */ UCHAR scan_type
           )
{
    API_RESULT retval;

    retval = API_GAP_INCORRECT_STATE;

    if (BT_FALSE != APPL_GAP_SCAN_IS_STATE(APPL_GAP_SCAN_SCANNING))
    {
        appl_gap_stop_scanning ();
    }

    if (BT_FALSE != APPL_GAP_SCAN_IS_STATE(APPL_GAP_SCAN_IDLE))
    {
        APPL_GAP_SET_SCAN_TYPE(scan_type);

        retval = BT_hci_le_set_scan_parameters
                 (
                     APPL_GAP_GET_SCAN_TYPE(),
                     APPL_GAP_GET_SCAN_INTERVAL(scan_param_id),
                     APPL_GAP_GET_SCAN_WINDOW(scan_param_id),
                     APPL_OWN_ADDR_TYPE(),
                     APPL_GAP_GET_SCAN_FILTER_POLICY(scan_param_id)
                 );
        if (API_SUCCESS == retval)
        {
            APPL_GAP_SCAN_SET_STATE(APPL_GAP_SCAN_UPDATING_PARAM);
        }
    }
    return retval;
}

API_RESULT appl_gap_stop_scanning (void)
{
    BT_hci_le_set_scan_enable(0, 1);

    APPL_GAP_SCAN_SET_STATE(APPL_GAP_SCAN_IDLE);

    return API_SUCCESS;
}
#endif /* APPL_GAP_OBSERVER_SUPPORT || APPL_GAP_CENTRAL_SUPPORT */

#ifdef APPL_GAP_CENTRAL_SUPPORT
API_RESULT appl_gap_initiate_connection
           (
               /* IN */ BT_DEVICE_ADDR * bd_addr,
               /* IN */ UCHAR            conn_param_id
           )
{
    API_RESULT retval;

    retval = API_GAP_INCORRECT_STATE;

    retval = BT_hci_le_create_connection
             (
                 APPL_GAP_GET_CONN_SCAN_INTERVAL(conn_param_id),
                 APPL_GAP_GET_CONN_SCAN_WINDOW(conn_param_id),
                 APPL_GAP_GET_CONN_INITIATOR_FILTER_POLICY(conn_param_id),
                 bd_addr->type,
                 bd_addr->addr,
                 APPL_OWN_ADDR_TYPE(),
                 APPL_GAP_GET_CONN_INTERVAL_MIN(conn_param_id),
                 APPL_GAP_GET_CONN_INTERVAL_MAX(conn_param_id),
                 APPL_GAP_GET_CONN_LATENCY(conn_param_id),
                 APPL_GAP_GET_CONN_SUPERVISION_TIMEOUT(conn_param_id),
                 APPL_GAP_GET_CONN_MIN_CE_LENGTH(conn_param_id),
                 APPL_GAP_GET_CONN_MAX_CE_LENGTH(conn_param_id)
             );

    if (API_SUCCESS == retval)
    {

    }
    return retval;
}

#endif /* APPL_GAP_CENTRAL_SUPPORT */

#endif /* BT_LE */
