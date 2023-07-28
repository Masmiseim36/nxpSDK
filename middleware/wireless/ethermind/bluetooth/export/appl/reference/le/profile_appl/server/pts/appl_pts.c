
/**
 *  \file appl_pts.c
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined PTS)

/* --------------------------------------------- Global Definitions */
/**
 * This Flag controls dumps and prints of PTS related Handle Value
 * Indication and Notification Complete Status.
 * This Flag is disabled by default.
 */
/* #define APPL_PTS_HAVE_DATA_DUMP */
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Global Variables */

/* --------------------------------------------- Functions */

void appl_pts_init(void)
{
#ifdef GATT_DB_DYNAMIC
    API_RESULT retval = API_FAILURE;
#endif /* GATT_DB_DYNAMIC */

#ifdef GATT_DB_DYNAMIC
    appl_pts_add_gaps();
    appl_pts_add_gatts();
    appl_pts_add_bas();
    appl_pts_add_dis();

    /**
     * TODO: Add other Dummy Services used in PTS Testing here.
     */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[PTS APPL]: GATT Database Registration Status: 0x%04X\n", retval);
#endif /* GATT_DB_DYNAMIC */

#ifndef GATT_DB_DYNAMIC
    /* Register for PTS services if Dynamic GATT Database is not Used */
    appl_pts_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register PTS GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_pts_gatt_char_handler);

    /* Update GAP Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_PTS_GAP_INST,
        DB_RECORD_GAP
    );
    /* Update GATT Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_PTS_GATT_INST,
        DB_RECORD_GATT
    );
            /* Update BAS Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_PTS_BATTERY_INST,
        DB_RECORD_BAS
    );
    /* Update BAS Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record
    (
        (UCHAR)GATT_SER_PTS_DEV_INFO_INST,
        DB_RECORD_DIS
    );

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    /* Initializing Authorization Flag */
    APPL_SET_AUTHORIZATION(DEVICE_HANDLE_INIT_VAL, BT_TRUE);

    /* Update GAP Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record((UCHAR)GATT_SER_PTS_GAP_INST, DB_RECORD_GAP);

    /* Update GATT Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record((UCHAR)GATT_SER_PTS_GATT_INST, DB_RECORD_GATT);

    /* Update BAS Service Range in the SDP Records */
    appl_set_gatt_service_in_sdp_record((UCHAR)GATT_SER_PTS_BATTERY_INST, DB_RECORD_BAS);

    /* Set the IO Capabilities */
    BT_smp_set_io_cap_pl (SMP_IO_CAPABILITY_DISPLAY_ONLY);
}

void appl_pts_bt_init(void)
{
    APPL_TRC("[PTS]: In appl_pts_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_pts_server_reinitialize();
}

void appl_pts_bt_deinit(void)
{
    APPL_TRC("[PTS]: In appl_pts_bt_deinit\n");
}

void appl_pts_connect(DEVICE_HANDLE  * dq_handle)
{
    BT_IGNORE_UNUSED_PARAM(dq_handle);
}

void appl_pts_server_reinitialize (void)
{
#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        /* Configure and Enable Advertising */
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U, 0x00U, NULL, 0x00U);
        appl_service_enable_adv(0x01U);
    }
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        /* Configure and Enable Scanning */
        appl_service_configure_scan(APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}

void appl_pts_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(evt_result);

#ifdef APPL_PTS_HAVE_DATA_DUMP
    CONSOLE_OUT(
    "\n[PTS]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
#endif /* APPL_PTS_HAVE_DATA_DUMP */
}

void appl_pts_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_PTS_HAVE_DATA_DUMP
    CONSOLE_OUT(
    "\n[PTS]: NTF Sent for Appl Handle 0x%02X", *handle);

    appl_dump_bytes(event_data, datalen);
#endif /* APPL_PTS_HAVE_DATA_DUMP */
}

void appl_pts_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    APPL_TRC("\n[PTS]: Updated MTU is %d for Appl Handle 0x%02X\n",
    mtu, *handle);
}

#endif /* PTS */

