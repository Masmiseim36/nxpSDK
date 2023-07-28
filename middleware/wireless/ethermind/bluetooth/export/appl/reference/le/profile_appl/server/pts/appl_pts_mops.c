/**
 *  \file appl_pts_mops.c
 *
 *  This file contains the menu handling for PTS Profile.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------- Header File Inclusion */
#include "appl_service.h"
#include "appl_pts_mops.h"
#include "appl_hci_le.h"
#include "gatt.h"

#if (defined APPL_MENU_OPS && defined PTS)

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
/** Menu for PTS operations */
static UCHAR pts_options[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\n\
    -- GATT Server Special Operations --\n\
    10 - GATT Server Menu\n\
    -- GAP Peripheral Operations --\n\
    20 - Gap Peripheral Menu\n\
    -- GAP Client Operations --\n\
    30 - Gap Central Menu\n\
Your Option?\n\
";

/** Menu for PTS GATT Server operations */
static UCHAR pts_gatt_server_options[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
    2 - Set Authorization State\n\
    3 - Send HV Notification/Indication\n\
    4 - Set Security Level for Data Signing\n\
Your Option?\n\
";

/** Menu for PTS GAP Peripheral operations */
static UCHAR pts_gap_peripheral_options[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\n\
    2 - Update ADV Data\n\
    3 - Update BCAST Data\n\
    4 - Advertise HCI_ADV_IND with RPA\n\
    5 - Set Discovery and Connection Mode\n\
    6 - Stop advertisement\n\
Your Option?\n\
";

/** Menu for PTS GAP Central operations */
static UCHAR pts_gap_central_options[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\n\
    2 - Set Default RPA for Central Role\n\
    3 - Set Default Public Addr for Central Role\n\
Your Option?\n\
";

/** Menu for PTS GAP ADV Selections */
static UCHAR pts_gap_adv_menu[] =
"\n\
    -- ADV Data Selections --\n\
    1- Service UUID AD Type\n\
    2 - Local Name\n\
    3 - Manufacturer Specific Data\n\
    4 - Tx Power Level\n\
    5 - Peripheral Connection Interval Range\n\
    6 - Service Solicitation UUIDs\n\
    7 - Appearance\n\
    8 - Public Target Address\n\
    9 - Random Target Address\n\
   10 - Advertising Interval\n\
   11 - LE Bluetooth Device Address\n\
   12 - LE Role\n\
   13 - URI\n\
   14 - Service Data\n\
   15 - Advertising Interval - Long\n\
Your Option?\n\
";

/** Menu for PTS GAP Broadcast ADV Selections */
static UCHAR pts_gap_bcast_menu[] =
"\n\
    -- ADV Data Selections --\n\
    1- Broadcast ADV Data\n\
    2- Broadcast HCI_ADV_NONCONN_IND with RPA\n\
    3- Broadcast HCI_ADV_NONCONN_IND with NRPA\n\
Your Option?\n\
";

/** Menu for PTS GAP Connection and Discovery Mode Selections */
static UCHAR pts_gap_disc_conn_mode_menu[] =
"\n\
    -- Discovery and Connection mode Selections --\n\
    1 - Non - Disc, Non - Conn Mode\n\
    2 - Non-Disc, Undirected Conn Mode\n\
    3 - Limited Disc, Non-Conn Mode\n\
    4 - Limited Disc, Undirected Conn\n\
    5 - Limited Disc, Non-Conn Mode,LE Only\n\
    6 - Limited Disc, Undirected Conn,LE Only\n\
    7 - General-Disc, Non-Conn Mode\n\
    8 - General-Disc, Undirected Conn Mode\n\
    9 - General-Disc, Non-Conn Mode,LE Only\n\
   10 - General-Disc, Undirected Conn Mode,LE Only\n\
   11 - General-Disc, Directed Conn Mode\n\
Your Option?\n\
";

/** Global to hold Non-resolvable Private Address Selection */
static int         g_nrpa_index;

/** Global to hold current Application Handle */
static APPL_HANDLE appl_pts_appl_handle;

/** Global variable to hold security level for the Data Signing Requirements */
UCHAR g_data_sign_mitm = BT_FALSE;

/* --------------------------------------------- Functions */
void appl_handle_adv_data_setting(UCHAR state);
void appl_handle_bcast_setting(UCHAR state);
void appl_rpa_adv
     (
         UCHAR type,
         UCHAR * data,
         UCHAR len,
         UCHAR * rpa_addr
     );
void appl_update_adv_srsp_data
     (
         UCHAR   srsp_flag,
         UCHAR * data,
         UCHAR   len,
         UCHAR   adv_event_type
     );

/* Dummy Function for now */
#define appl_hci_le_update_l_addr_and_mode(...)

void appl_pts_gatt_server_menu_handler (void)
{
    int             choice;
    API_RESULT      retval;
    ATT_ATTR_HANDLE attr_hndl;
    UCHAR           auth_state;

    BT_IGNORE_UNUSED_PARAM(retval);

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", pts_gatt_server_options);
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
            case 0:
                return;

            case 1:
                CONSOLE_OUT ("\nRefreshing ...\r\n\n");
                break;

            case 2:
                CONSOLE_OUT("\n Enter authorization State:\n");
                CONSOLE_OUT("\n0 - Disable\n1 - Enable\n");
                CONSOLE_IN("%d", &choice);
                auth_state = (((UCHAR)choice) == BT_TRUE) ? BT_TRUE : BT_FALSE;

                /* Update Authorization Flag */
                APPL_SET_AUTHORIZATION(DEVICE_HANDLE_INIT_VAL, auth_state);
                break;

            case 3:
                {
                    UCHAR t_data[20];
                    UCHAR t_hvn;

                    appl_display_all_connected_devices();
                    CONSOLE_OUT("\nEnter the desired application handle[in HEX]:\n");
                    CONSOLE_IN("%x", &choice);
                    appl_pts_appl_handle = ((APPL_HANDLE)choice);

                    CONSOLE_OUT("Enter Attribute handle[HEX] : ");
                    CONSOLE_IN("%x", &choice);
                    attr_hndl = (ATT_ATTR_HANDLE) choice;

                    CONSOLE_OUT(" 0 - Handle Value Indication\n");
                    CONSOLE_OUT(" 1 - Handel Value Notification\n");
                    CONSOLE_OUT("Which one to be sent? ");
                    CONSOLE_IN("%x", &choice);
                    t_hvn = (1 == choice) ? BT_TRUE: BT_FALSE;

                    /* Setting Random Data */
                    BT_mem_set(t_data, 0x20, sizeof(t_data));

                    if (BT_TRUE == t_hvn)
                    {
                        /* Send Notification */
                        retval = gatt_send_hv_ntf
                                 (
                                     &APPL_GET_ATT_INSTANCE(appl_pts_appl_handle),
                                     attr_hndl,
                                     t_data,
                                     sizeof(t_data)
                                 );
                    }
                    else
                    {
                        /* Send Indication */
                        retval = gatt_send_hv_ind
                                 (
                                     &APPL_GET_ATT_INSTANCE(appl_pts_appl_handle),
                                     attr_hndl,
                                     t_data,
                                     sizeof(t_data)
                                 );
                    }
                }
                break;

            case 4:
                CONSOLE_OUT("Set the Security Level for Data Signing\n"
                "0. No MITM(Sec Level 1)\n1. With MITM(Sec Level 2)\n");
                CONSOLE_IN("%u", &choice);
                g_data_sign_mitm = (0 == (UCHAR)choice) ? BT_FALSE : BT_TRUE;
                break;

            default:
                CONSOLE_OUT("Invalid Option!\n");
                break;
        }
    }
}

void appl_pts_gap_peripheral_menu_handler (void)
{
    int         choice;
    API_RESULT  retval;
    UCHAR       adv_state;

    BT_IGNORE_UNUSED_PARAM(retval);

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", pts_gap_peripheral_options);
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
            case 0:
                return;

            case 1:
                CONSOLE_OUT ("\nRefreshing ...\r\n\n");
                break;

            case 2:
                CONSOLE_OUT ("\n Set Desired Type of ADV Data \n");
                CONSOLE_OUT ("%s", pts_gap_adv_menu);
                CONSOLE_IN ("%d", &choice);
                adv_state = (UCHAR) choice;

                appl_handle_adv_data_setting(adv_state);
                break;

            case 3:
                CONSOLE_OUT ("\n Set Desired Type of ADV Broadcast Data \n");
                CONSOLE_OUT ("%s", pts_gap_bcast_menu);
                CONSOLE_IN ("%d", &choice);
                adv_state = (UCHAR) choice;

                if (3 == choice)
                {
                    /* Explicitly update the adv_state to a suitable value */
                    /**
                     * The valid values for adv_state here are:
                     * # adv_state = 1, adv_state = 2 and adv_state = 4
                     */
                    adv_state = 4;

                    CONSOLE_OUT("Select NRPA to advertise\n");
                    CONSOLE_OUT("0. 0x55, 0x44, 0x33, 0x22, 0x11, 0x00\n1. 0x56, 0x44, 0x33, 0x22, 0x11, 0x00\n");
                    CONSOLE_IN("%d", &g_nrpa_index);
                }
                appl_handle_bcast_setting(adv_state);
                break;

            case 4:
                {
                    UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x77, 0x69, 0x34, 0x6E, 0x64, 0x74, 0x72, 0x65, 0x65};
                    /**
                     * Resolvable Private Address to be used when:
                     * 1. IRK : "0A 2D F4 65 E3 BD 7B 49 1E B4 C0 95 95 13 46 73"
                     *    RPA : "56 E5 94 7B 97 65" - 0x56, 0xE5, 0x94, 0x7B, 0x97, 0x65
                     *    RPA : "08 1F 7C FF 4E 4D" - 0x08, 0x1F, 0x7C, 0xFF, 0x4E, 0x4D
                     *    RPA : "16 F3 D0 61 C3 73" - 0x16, 0xF3, 0xD0, 0x61, 0xC3, 0x73
                     *    RPA : "71 33 9E 57 66 6B" - 0x71, 0x33, 0x9E, 0x57, 0x66, 0x6B
                     *
                     * 2. IRK : 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11
                     *    RPA : "B4 7E AA CB 42 74" - 0xB4, 0x7E, 0xaa, 0xcb, 0x42, 0x74
                     */
                    UCHAR rpa1[] = {0x56, 0xE5, 0x94, 0x7B, 0x97, 0x65};
                    UCHAR rpa2[] = {0x08, 0x1F, 0x7C, 0xFF, 0x4E, 0x4D};
                    UCHAR* rpa;

                    CONSOLE_OUT("Select RPA to advertise\n");
                    CONSOLE_OUT("1. 56 E5 94 7B 97 65\n2. 08 1F 7C FF 4E 4D\n");
                    CONSOLE_IN("%d", &choice);
                    rpa = (1 == (UCHAR)choice) ? rpa1 : rpa2;

                    appl_rpa_adv(1, adv_srs_data, sizeof(adv_srs_data), rpa);
                }
                break;

            case 5:
                {
                    CONSOLE_OUT("\n Set Desired Discovery and Connection mode \n");
                    CONSOLE_OUT("%s", pts_gap_disc_conn_mode_menu);
                    CONSOLE_IN("%d", &choice);

                    switch (choice)
                    {
                        case 1:
                        {
                            /**
                             * LE Limited Discoverable Flag and LE General
                             * Discoverable Flag set to 0 in FLAG AD type
                             */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x00, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_NONCONN_IND);
                        }
                        break;
                        case 2:
                        {
                            /**
                             * LE Limited Discoverable Flag and LE General
                             * Discoverable Flag set to 0 in FLAG AD type
                             */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x00, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
                        }
                        break;
                        case 3:
                        {
                            /* Set only LE Limited Discoverable Flag in FLAG AD type */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x01, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_NONCONN_IND);
                        }
                        break;
                        case 4:
                        {
                            /* Set only LE Limited Discoverable Flag in FLAG AD type */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x01, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
                        }
                        break;
                        case 5:
                        {
                            /* Set LE Limited Discoverable and BR/EDR Not Supported in FLAG AD type */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x06, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_NONCONN_IND);
                        }
                        break;
                        case 6:
                        {
                            /* Set LE Limited Discoverable and BR/EDR Not Supported in FLAG AD type */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x05, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
                        }
                        break;
                        case 7:
                        {
                            /* Set LE General Discoverable Mode in FLAG AD type */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x02, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_NONCONN_IND);
                        }
                        break;
                        case 8:
                        {
                            /* Set LE General Discoverable Mode in FLAG AD type */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x02, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
                        }
                        break;
                        case 9:
                        {
                            /* Set LE General Discoverable Mode and BR/EDR Not Supported in FLAG AD type */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x06, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_NONCONN_IND);
                        }
                        break;
                        case 10:
                        {
                            /* Set LE General Discoverable Mode and BR/EDR Not Supported in FLAG AD type */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x06, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
                        }
                        break;
                        case 11:
                        {
                            /**
                             * Set LE General Discoverable Mode, LE Limited
                             * Discoverable Mode Directed Connectable mode
                             */
                            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x06, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18 };

                            appl_update_adv_srsp_data(0x00, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_DIRECT_IND);
                        }
                        break;
                    }
                }
                break;

            case 6:
                /* Disable Advertisements */
                BT_hci_le_set_advertising_enable(0);
                break;

            default:
                CONSOLE_OUT("Invalid Option!\n");
                break;
        }
    }
}

void appl_pts_gap_central_menu_handler (void)
{
    int         choice;
    API_RESULT  retval;

    BT_IGNORE_UNUSED_PARAM(retval);

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", pts_gap_central_options);
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
            case 0:
                return;

            case 1:
                CONSOLE_OUT ("\nRefreshing ...\r\n\n");
                break;

            case 2:
                {
                    UCHAR rpa_addr[BT_BD_ADDR_SIZE] =
                          {0x56, 0xE5, 0x94, 0x7B, 0x97, 0x65};
                    BT_DEVICE_ADDR l_rpa_addr;

                    /* Setting Default RPA Address */
                    /**
                     * NOTE:
                     * This needs to be changed if the IRK used is changed.
                     */
                    BT_COPY_BD_ADDR(&l_rpa_addr.addr, rpa_addr);
                    l_rpa_addr.type = BT_BD_RANDOM_ADDRESS_TYPE;

                    appl_hci_le_update_l_addr_and_mode
                    (
                        &l_rpa_addr,
                        BT_TRUE
                    );
                }
                break;

            case 3:
                /* Set the Own Connection Address to be used as Public */
                appl_hci_le_update_l_addr_and_mode
                (
                    NULL,
                    BT_FALSE
                );
                break;

            default:
                CONSOLE_OUT("Invalid Option!\n");
                break;
        }
    }
}

void appl_pts_menu_handler (void)
{
    int         choice;
    API_RESULT  retval;

    BT_IGNORE_UNUSED_PARAM(retval);

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT("\n");
        CONSOLE_OUT("%s", pts_options);
        CONSOLE_IN("%d", &choice);

        switch(choice)
        {
            case 0:
                return;

            case 1:
                CONSOLE_OUT ("\nRefreshing ...\r\n\n");
                break;

            case 10:
                appl_pts_gatt_server_menu_handler();
                break;

            case 20:
                appl_pts_gap_peripheral_menu_handler();
                break;

            case 30:
                appl_pts_gap_central_menu_handler();
                break;

            default:
                CONSOLE_OUT("Invalid Option!\n");
                break;
        }
    }
}

void appl_update_adv_srsp_data
     (
         UCHAR   srsp_flag,
         UCHAR * data,
         UCHAR   len,
         UCHAR   event_type
     )
{
    BT_DEVICE_ADDR peer_addr;
    int            read;
    UINT16         adv_min_intval = 0x0020;
    UINT16         adv_max_intval = 0x0020;

    /* Initialize */
    BT_INIT_BD_ADDR(&peer_addr);

    if (HCI_ADV_NONCONN_IND == event_type)
    {
        adv_min_intval = 0x00A0;
        adv_max_intval = 0x00A0;
    }

    if (HCI_ADV_DIRECT_IND == event_type)
    {
        CONSOLE_OUT("Enter Peer Address : ");
        appl_get_bd_addr(peer_addr.addr);

        CONSOLE_OUT("Enter Peer Address type : ");
        CONSOLE_IN("%u", &read);
        peer_addr.type = (UCHAR) read;
    }

    /* Disable Advertisements */
    BT_hci_le_set_advertising_enable(0);

    /* Set Normal Advertising Parameters */
    BT_hci_le_set_advertising_parameters
    (
        adv_min_intval,
        adv_max_intval,
        event_type,
        0x00,
        peer_addr.type,
        peer_addr.addr,
        0x07,
        0x00
    );

    BT_hci_le_set_advertising_data
    (
        len,
        data
    );

    if (0x01 == srsp_flag)
    {
        BT_hci_le_set_scan_response_data
        (
            len,
            data
        );
    }

    /* Enable Advertisements */
    BT_hci_le_set_advertising_enable(1);
}

void appl_rpa_adv
     (
         UCHAR type,
         UCHAR * data,
         UCHAR len,
         UCHAR * rpa_addr
     )
{
    /* Disable Advertisements */
    BT_hci_le_set_advertising_enable(0);

    /* Set the Incoming RPA */
    BT_hci_le_set_random_address(rpa_addr);

    /* Set Normal Advertising Parameters */
    BT_hci_le_set_advertising_parameters
    (
        0x0020,
        0x0020,
        HCI_ADV_IND,
        0x01,
        0x00,
        NULL,
        0x07,
        0x00
    );

    BT_hci_le_set_advertising_data
    (
        len,
        data
    );

    if (0x01 == type)
    {
        BT_hci_le_set_scan_response_data
        (
            len,
            data
        );
    }

    /* Enable Advertisements */
    BT_hci_le_set_advertising_enable(1);
}

void appl_handle_adv_data_setting(UCHAR state)
{
    switch(state)
    {
        case 1:
            {
               /* Complete List of 16 - bit Service Class UUIDs */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 2:
            {
                /* Complete Local Name */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x77, 0x69, 0x34, 0x6E, 0x64, 0x74, 0x72, 0x65, 0x65};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 3:
            {
                /* Manufacturer Specific Data*/
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x06, 0xFF, 0x6A, 0x00, 0x77, 0x69, 0x34};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 4:
            {
                /* Tx Power Level */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x02, 0x0A, 0x64, 0x02, 0x11, 0x01};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 5:
            {
                /* Peripheral Connection Interval Range */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x05, 0x12, 0x28, 0x00, 0x38, 0x00};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 6:
            {
                /* List of 16-bit Service Solicitation UUIDs */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x05, 0x14, 0x00, 0x18, 0x01, 0x18, 0x05, 0x16, 0x21, 0x18, 0x11, 0x22};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 7:
            {
                /* Appearance */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x03, 0x19, 0x64, 0x00};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 8:
            {
                /* Public Target Address */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x07, 0x17, 0x7C, 0x5C, 0xF8, 0x05, 0x49, 0x15};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 9:
            {
                /* Random Target Address */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x07, 0x18, 0x78, 0x61, 0x1a, 0x6a, 0x2E, 0xE0};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 10:
            {
                /* Advertising Interval */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x03, 0x1A, 0x1A, 0x00};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 11:
            {
                /* LE Bluetooth Device Address */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x08, 0x1B, 0x00, 0x99, 0x0B, 0x08, 0xDC, 0x1B, 0x00};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 12:
            {
                /* LE Role */
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x02, 0x1C, 0x03};

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 13:
            {
                /** URI
                Example from CSS: "example://z.com/�lborg"
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x06, 0x12, 0x24, 0xC2, 0xB9, 0x2F, 0x2F, 0x7A, 0x2E, 0x63, 0x6F, 0x6D, 0x2F, 0xC3, 0x85, 0x6C, 0x62, 0x6F, 0x72, 0x67};
                Example from CSS: tel+1-816-555-1212 */
                UCHAR adv_srs_data[] = { 0x02, 0x01, 0x06,
                                         0x13, 0x24,
                                         0x74, 0x65, 0x6c, 0x2b,
                                         0x31, 0x2d, 0x38, 0x31, 0x36,
                                         0x2d, 0x35, 0x35, 0x35, 0x2d,
                                         0x31, 0x32, 0x31, 0x32 };

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 14:
            {
                /* Service Data */
                UCHAR adv_srs_data[] = { 0x02, 0x01, 0x06, 0x04, 0x16, 0x01, 0x02, 0x03 };

                appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
            }
            break;

        case 15:
        {
            /**
             * Advertising Interval - Long
             * advInterval = 60s.
             * In Units of 0.625 ms, advInterval value = 0x00017700
             */
            UCHAR adv_srs_data[] = { 0x02, 0x01, 0x06, 0x05, 0x2F, 0x00, 0x77, 0x01, 0x00 };

            appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
        }
        break;

        case 20:
        {
            /** Limited discovery */
            UCHAR adv_srs_data[] = {0x02, 0x01, 0x01, 0x02, 0x1C, 0x03};

            appl_update_adv_srsp_data(0x01, adv_srs_data, sizeof(adv_srs_data), HCI_ADV_IND);
        }
        break;
    }
}

void appl_update_adv_bcast_data
     (
         UCHAR srsp_flag,
         UCHAR * data,
         UCHAR len,
         UCHAR l_addr_type,
         UCHAR p_addr_type,
         UCHAR * p_addr
     )
{
    /* Disable Advertisements */
    BT_hci_le_set_advertising_enable(0);

    if (0x01 == l_addr_type)
    {
        /* Setting Static Random Address */
        UCHAR static_addr[] = {0xC0, 0xFF, 0xEE, 0xC0, 0xFF, 0xEE};

        BT_hci_le_set_random_address(static_addr);
    }
    else if (0x02 == l_addr_type)
    {
        /* Setting Resolvable Private Address */
        UCHAR rpa_addr[] =  {0x56, 0xE5, 0x94, 0x7B, 0x97, 0x65};

        BT_hci_le_set_random_address(rpa_addr);
        l_addr_type = 0x01;
    }
    else if (0x03 == l_addr_type)
    {
        UCHAR rpa_addr1[] = { 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
        UCHAR rpa_addr2[] = { 0x56, 0x44, 0x33, 0x22, 0x11, 0x00 };

        UCHAR * rpa_addr;

        /* Non Resolvable Private Address */
        if (0 == g_nrpa_index)
        {
            rpa_addr = &rpa_addr1[0];
        }
        else
        {
            rpa_addr = &rpa_addr2[0];
        }

        BT_hci_le_set_random_address(rpa_addr);
        l_addr_type = 0x01;
    }

    /* Set Normal Advertising Parameters */
    BT_hci_le_set_advertising_parameters
    (
        0x00A0,
        0x00A0,
        HCI_ADV_NONCONN_IND,
        l_addr_type,
        p_addr_type,
        p_addr,
        0x07,
        0x00
    );

    BT_hci_le_set_advertising_data
    (
        len,
        data
    );

    if (0x01 == srsp_flag)
    {
        BT_hci_le_set_scan_response_data
        (
            len,
            data
        );
    }

    /* Enable Advertisements */
    BT_hci_le_set_advertising_enable(1);
}

void appl_handle_bcast_setting(UCHAR state)
{
    UCHAR adv_srs_data_common[] = { 0x02, 0x01, 0x00, 0x05, 0x03, 0x00, 0x18, 0x01, 0x18, 0x0D, 0x09, 0x50, 0x54, 0x53, 0x2D, 0x47, 0x41, 0x50, 0x2D, 0x30, 0x36, 0x42, 0x38, 0x03, 0x19, 0x00, 0x00 };

    switch(state)
    {
        case 1:
            {
                appl_update_adv_bcast_data(0x00, adv_srs_data_common, sizeof(adv_srs_data_common), 0x00, 0x00, NULL);
            }
            break;

        case 2:
            {
                appl_update_adv_bcast_data(0x00, adv_srs_data_common, sizeof(adv_srs_data_common), 0x02, 0x00, NULL);
            }
            break;

        case 3:
            {
                appl_update_adv_bcast_data(0x01, adv_srs_data_common, sizeof(adv_srs_data_common), 0x02, 0x00, NULL);
            }
            break;

        case 4:
            {
                appl_update_adv_bcast_data(0x00, adv_srs_data_common, sizeof(adv_srs_data_common), 0x03, 0x00, NULL);
            }
            break;

        case 5:
            {
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x01, 0x02, 0x1C, 0x03};

                appl_update_adv_bcast_data(0x00, adv_srs_data, sizeof(adv_srs_data), 0x00, 0x00, NULL);
            }
            break;

        case 6:
            {
                UCHAR adv_srs_data[] = {0x02, 0x01, 0x02, 0x02, 0x1C, 0x03};

                appl_update_adv_bcast_data(0x00, adv_srs_data, sizeof(adv_srs_data), 0x00, 0x00, NULL);
            }
            break;

        default:
            CONSOLE_OUT("Invalid Option!\n");
            break;
    }
}

UCHAR appl_get_data_signing_security_level(void)
{
    return g_data_sign_mitm;
}

#endif /* (defined APPL_MENU_OPS && defined PTS) */



