
/**
 *  \file appl_basic.c
 *
 *  This file contains a reference implementation to test few specific
 *  scenarios.
 *  Sample applications is as detailed below:
 *      a. This Sample application contains 2 Sets of Services,
 *         - Set 1:
 *            > GAP Service
 *            > GATT Service
 *            > BAS Service
 *            > Test Service A [Secondary Service]
 *            > Test Service B [Includes Test Service A]
 *         - Set 2:
 *            > GAP Service
 *            > GATT Service
 *            > BAS Service
 *            > Test Service A [Secondary Service]
 *            > Test Service B [Includes Test Service A]
 *            > DIS Service
 *      c. The GATT Service Exposes:
 *           - Service Changed Characteristic
 *           - Client Supported Feature Characteristic [CSFC]
 *           - Database Hash Characteristic [DB Hash]
 *           - Server Supported Feature Characteristic [SSFC]
 *      d. The Service Changed Characteristic can be Indicated to all the known
 *         Clients when ever there is a change in the Database and there is
 *         Interface for Application to trigger it as it seems fit.
 *      e. The CSFC is maintained by peer device in the application.
 *      f. The DB Hash is calculated each time a new Database in registered.
 *      g. The SSFC is set to a default value of 0x00 i.e. no Enhanced ATT
 *         bearer is supported in this application.
 *      h. Test Service A is a Secondary Service which is visible only on
 *         LE Transport with Security Mode 2, Level 1, Key Size 16 and
 *         it Exposes:
 *         - Characteristic A1: Which has Signed Wwr and Rd property. And its
 *           Value is of 2 bytes long.
 *      i. Test Service B is a Primary Service which Exposes:
 *         > Included Service Test Service A
 *         - Characteristic B1: Which has Wr and Rd Property. And its value is
 *           of 25 bytes long. The total value is beyond the default MTU length
 *         - Characteristic B2: Which has Wwr and Rd Property. And its Value is
 *           of 2 bytes long.
 *      j. Based on the current state of the Database w.r.t to a known peer
 *         device, this application manages the Database Synchronization with
 *         the peer clients.
 *      k. Whenever a known peer device is marked untrusted through
 *         SM/SMP/Security Interface calls, the user should also ensure that
 *         this application specific Globals are reset through the provided
 *         Interface/Wrappers
 */


/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#if (defined ATT && defined BASIC)

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
#ifdef GATT_DB_HAVE_DB_SIGNATURE
GATT_DB_SIGNATURE appl_basic_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */
/* --------------------------------------------- Global Variables */

/* --------------------------------------------- Functions */

void appl_basic_update_dev_awarness(void)
{
    UINT32        i;
    API_RESULT    retval;
    BT_SEC_INFO   info;
    DEVICE_HANDLE dq_handle;
    APPL_HANDLE   appl_handle;

    APPL_TRC("[BASIC]: In appl_basic_update_dev_awarness\n");

    for (i = 0; i < BT_MAX_DEVICE_QUEUE_SIZE; i++)
    {
        dq_handle = (DEVICE_HANDLE)i;

        /*
         * Check if Device Handle is an active connected device
         * or if this is previously Bonded before any
         * operation.
         */
        /* Fetch the Application Handle on this Device Handle */
        if (API_SUCCESS ==
             appl_get_handle_from_device_handle(dq_handle, &appl_handle))
        {
            /* Check if Active Connection Exists */
            if (HCI_INVALID_CONNECTION_HANDLE !=
                 APPL_GET_CONNECTION_HANDLE(appl_handle))
            {
                /* Active Connection Exists */
                retval = API_SUCCESS;
            }
            else
            {
                /* Mark as unknown when no connection exists */
                retval = API_FAILURE;
            }
        }
        else
        {
            retval = BT_security_get_device_status
                     (
                         &dq_handle,
                         DQ_LINK_ANY,
                         &info
                     );

            if ((retval == API_SUCCESS) &&
                (BT_TRUE != BT_SEC_IS_BONDED(info.status)))
            {
                /* If Bonding Bit is not SET, mark failure */
                retval = API_FAILURE;
            }
        }

        /**
         * From the above sequence, an unknown device will be tagged with
         * non-success retval, a known device will be tagged with success.
         */
        if (API_SUCCESS != retval)
        {
            APPL_TRC("[BASIC]: Device Unknown or previously not bonded!\n");

            /* Set default values to
             * - change aware state
             * - ATT Request counter.
             */
            /* Default state is Change Aware */
            appl_basic_change_aware_state[i] = BT_TRUE;

            /* Default state is 0 */
            appl_basic_request_counter[i] = 0;
        }
        else
        {
            APPL_TRC("[BASIC]: Device 0x%02X marked as Change Unaware\n",
            (DEVICE_HANDLE)i);

            /* If Device is Bonded Mark as Change not Aware */
            appl_basic_change_aware_state[i] = BT_FALSE;

            /* Default state is 0 */
            appl_basic_request_counter[i] = 0;
        }
    }
}

/**
 * Basic GATT DB Set 1 consists of
 * 1. GAPS
 * 2. GATTS
 * 3. BAS
 * 4. Test Service A [Secondary Service]
 * 5. Test Service B [Includes Test Service A]
 */
void appl_basic_init_set_1_db(void)
{
#ifdef GATT_DB_DYNAMIC
    API_RESULT retval = API_FAILURE;

    (BT_IGNORE_RETURN_VALUE) appl_basic_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_test_serv_a();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_test_serv_b();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the BASIC GATT DB Signature */
    appl_basic_db_sign = APPL_BASIC_SET_1_GATT_DB_SIGN;
    retval             = BT_gatt_db_dyn_set_db_signature(appl_basic_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    /* Register the GATT DB */
    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[BASIC APPL]: SET 1 GATT Database Registration Status: 0x%04X\n", retval);
#endif /* GATT_DB_DYNAMIC */
}

/**
 * Basic GATT DB Set 2 consists of
 * 1. GAPS
 * 2. GATTS
 * 3. BAS
 * 4. Test Service A [Secondary Service]
 * 5. Test Service B [Includes Test Service A]
 * 6. DIS
 */
void appl_basic_init_set_2_db(void)
{
#ifdef GATT_DB_DYNAMIC
    API_RESULT retval = API_FAILURE;

    (BT_IGNORE_RETURN_VALUE) appl_basic_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_test_serv_a();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_test_serv_b();
    (BT_IGNORE_RETURN_VALUE) appl_basic_add_dis();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the BASIC GATT DB Signature */
    appl_basic_db_sign = APPL_BASIC_SET_2_GATT_DB_SIGN;
    retval             = BT_gatt_db_dyn_set_db_signature(appl_basic_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    /* Register the GATT DB */
    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[BASIC APPL]: SET 2 GATT Database Registration Status: 0x%04X\n", retval);
#endif /* GATT_DB_DYNAMIC */
}

void appl_basic_calculate_db_hash(void)
{
    /* Calculate the DB Hash for the current Database */
    (BT_IGNORE_RETURN_VALUE)BT_gatt_db_calculate_db_hash
                            (
                                appl_basic_gatt_db_ntf_cb
                            );
}

UINT16 appl_basic_check_cccd
       (
           /* IN */ GATT_DB_HANDLE db_handl
       )
{
    API_RESULT retval;
    ATT_VALUE  value;
    UINT16     cli_cnfg;

    /* Initialize */
    cli_cnfg = 0U;

    retval = BT_gatt_db_get_char_cli_cnfg(&db_handl, &value);

    if (retval != API_SUCCESS)
    {
        cli_cnfg = GATT_CLI_CNFG_DEFAULT;
    }
    else
    {
        BT_UNPACK_LE_2_BYTE(&cli_cnfg, value.val);
    }

    return cli_cnfg;
}

void appl_basic_send_serv_changed
     (
         /* IN */ DEVICE_HANDLE dq_handl
     )
{
    GATT_DB_HANDLE serv_chngd_db_hndl;

    /* Initialize with Service Change Char related values */
    serv_chngd_db_hndl.service_id = (UCHAR)GATT_SER_BASIC_GATT_INST;
    serv_chngd_db_hndl.char_id    = (UCHAR)GATT_CHAR_BASIC_SER_CHNGD_INST;


    if (DEVICE_HANDLE_INIT_VAL == dq_handl)
    {
        UINT32 i;

        /* Send to all connected clients */
        for(i=0; i < ATT_MAX_CONNECTION_INSTANCES; i++)
        {
            if (HCI_INVALID_CONNECTION_HANDLE != APPL_GET_CONNECTION_HANDLE(i))
            {
                serv_chngd_db_hndl.device_id = APPL_GET_DEVICE_HANDLE(i);

                /* Send Service Changed Ind */
                if (GATT_CLI_CNFG_INDICATION ==
                    appl_basic_check_cccd(serv_chngd_db_hndl))
                {
                    appl_send_serv_changed_ind(&serv_chngd_db_hndl);
                }
            }
        }
    }
    else
    {
        /* Send to specific client */
        serv_chngd_db_hndl.device_id = dq_handl;

        /* Send Service Changed Ind */
        if (GATT_CLI_CNFG_INDICATION ==
            appl_basic_check_cccd(serv_chngd_db_hndl))
        {
            appl_send_serv_changed_ind(&serv_chngd_db_hndl);
        }
    }
}

void appl_basic_switch_gatt_db(void)
{
    switch(appl_basic_db_sign)
    {
        /* Current DB is SET 1, Toggle to SET 2 */
        case APPL_BASIC_SET_1_GATT_DB_SIGN:
            /* De-initialize the old DB */
            appl_gatt_db_module_deinit();

            /* Initialize the GATT DB module again */
            appl_gatt_db_module_init();

            /* Initialize SET 2 GATT DB for Basic */
            appl_basic_init_set_2_db();

            /* Update/Retain the previous peer specific values of SCV CCCD */
            appl_basic_updt_serv_chngd_cccd();

            /* Update/Retain the previous peer specific values of SCV CCCD */
            appl_basic_updt_csfc_value();

            /**
             * Send Service Changed Indication to all Connection Clients
             */
            appl_basic_send_serv_changed(DEVICE_HANDLE_INIT_VAL);

            /**
             * Update Change Aware State.
             */
            appl_basic_update_dev_awarness();

            /* Calculate the DB Hash Value */
            appl_basic_calculate_db_hash();
            break;

        /* Current DB is SET 2, Toggle to SET 1 */
        case APPL_BASIC_SET_2_GATT_DB_SIGN:
            /* De-initialize the old DB */
            appl_gatt_db_module_deinit();

            /* Initialize the GATT DB module again */
            appl_gatt_db_module_init();

            /* Initialize SET 1 GATT DB for Basic */
            appl_basic_init_set_1_db();

            /* Update/Retain the previous peer specific values of SCV CCCD */
            appl_basic_updt_serv_chngd_cccd();

            /* Update/Retain the previous peer specific values of SCV CCCD */
            appl_basic_updt_csfc_value();

            /**
             * Send Service Changed Indication to all Connection Clients
             */
            appl_basic_send_serv_changed(DEVICE_HANDLE_INIT_VAL);

            /**
             * Update Change Aware State.
             */
            appl_basic_update_dev_awarness();

            /* Calculate the DB Hash Value */
            appl_basic_calculate_db_hash();

            break;

        default:
            APPL_ERR("[BASIC]: Incorrect GATT DB Switch Value\n");
            break;
    }
}

void appl_basic_init(void)
{
#ifndef GATT_DB_DYNAMIC
    /* Register for BASIC services if Dynamic GATT Database is not Used */
    appl_basic_gatt_db_register();
#else /* GATT_DB_DYNAMIC */

    appl_basic_dyn_db_init();

    /* Default Basic DB is Set 1 */
    appl_basic_init_set_1_db();
#endif /* GATT_DB_DYNAMIC */

    /* Register BASIC GATT DB Handler with PL Extension */
    (BT_IGNORE_RETURN_VALUE) gatt_db_init_pl
                             (
                                gatt_db_basic_gatt_char_handler
                             );
}

void appl_basic_bt_init(void)
{
    APPL_TRC("[BASIC]: In appl_basic_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    /* Calculate the Current DB Hash Value */
    appl_basic_calculate_db_hash();

    appl_basic_server_reinitialize();
}

void appl_basic_bt_deinit(void)
{
    APPL_TRC("[BASIC]: In appl_basic_bt_deinit\n");
}

void appl_basic_connect(DEVICE_HANDLE  * dq_handle)
{
    API_RESULT  retval;
    BT_SEC_INFO info;

    APPL_TRC("[BASIC]: In appl_basic_connect\n");

    /* Check if Device Handle is previously Bonded before any operation */
    retval = BT_security_get_device_status
             (
                 dq_handle,
                 DQ_LINK_ANY,
                 &info
             );

    if ((API_SUCCESS != retval) ||
        (BT_TRUE != BT_SEC_IS_BONDED(info.status)))
    {
        APPL_TRC("Device not previously bonded!\n");

        /* Reset any specific entry maintained by application */
        appl_basic_reset_serv_chngd_cccd(*dq_handle);
        appl_basic_reset_csfc_value(*dq_handle);

        /* Set default values to
         * - change aware state
         * - ATT Request counter.
         */
        /* Default state is Change Aware */
        appl_basic_change_aware_state[*dq_handle] = BT_TRUE;
    }

    /* Reset Request Counter on Each Connection */
    appl_basic_request_counter[*dq_handle] = 0;
}

void appl_basic_server_reinitialize (void)
{
#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        /* Configure and Enable Advertising */
        appl_service_configure_adv
        (
            APPL_GAP_PROC_NORMAL,
            HCI_ADV_IND,
            0x00U,
            0x00U,
            NULL,
            0x00U
        );
        
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


void appl_basic_handle_ind_complete
     (
         APPL_HANDLE * handle,
         UINT16      evt_result
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(evt_result);

#ifdef APPL_BASIC_HAVE_DATA_DUMP
    CONSOLE_OUT(
    "\n[BASIC]: IND Completed for Appl Handle 0x%02X with result 0x%04X\n",
    *handle, evt_result);
#endif /* APPL_BASIC_HAVE_DATA_DUMP */

    /** Set the Device Handle as Change Aware */
    /** TODO: shift to a macro for better readability */
    appl_basic_change_aware_state[APPL_GET_DEVICE_HANDLE(*handle)] = BT_TRUE;
}

void appl_basic_handle_ntf_complete
     (
         APPL_HANDLE * handle,
         UCHAR       * event_data,
         UINT16      datalen
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(event_data);
    BT_IGNORE_UNUSED_PARAM(datalen);

#ifdef APPL_BASIC_HAVE_DATA_DUMP
    CONSOLE_OUT(
    "\n[BASIC]: NTF Sent for Appl Handle 0x%02X\n", *handle);

    appl_dump_bytes(event_data, datalen);
#endif /* APPL_BASIC_HAVE_DATA_DUMP */
}

void appl_basic_handle_mtu_update_complete
     (
         APPL_HANDLE * handle,
         UINT16      mtu
     )
{
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(mtu);

    CONSOLE_OUT(
    "\n[BASIC]: Updated MTU is %d for Appl Handle 0x%02X\n", mtu, *handle);
}
#endif /* (defined ATT && defined BASIC) */

