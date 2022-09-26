
/**
 *  \file appl_bnep.c
 *
 *  Source File for BNEP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_bnep.h"
#include "appl_utils.h"

#ifdef BNEP
/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
static const char bnep_options[] = "\n\
========= BNEP Menu ================= \n\
    0.  Exit.\n\
    1.  Refresh.\n\
 \n\
    2.  Show BNEP Handles.\n\
 \n\
    3.  BNEP Registration.\n\
    4.  BNEP Set Policy.\n\
 \n\
    10. BNEP Accept.\n\
    11. BNEP Connect.\n\
    12. BNEP Setup.\n\
    13. Set BNEP Setup Response.\n\
    14. BNEP Filter.\n\
    15. BNEP Disconnect.\n\
 \n\
    20. BNEP Data.\n\
    21. BNEP Raw Write.\n\
 \n\
Your BNEP Option ? \0";


/* BNEP Connection Handles */
static APPL_BNEP_HANDLE appl_bnep_handle [BNEP_MAX_ENTITIES];

/** HCI ACL Connection List */
static HCI_CONNECTION_LIST appl_bnep_acl_list [BT_MAX_REMOTE_DEVICES];

/* To Reply to BNEP_SETUP_IND */
static UINT16 appl_bnep_setup_ind_reply = BNEP_SETUP_SUCCESSFUL;

/* Local BD_ADDR */
UCHAR appl_bnep_local_bd_addr [BT_BD_ADDR_SIZE];


#ifdef BNEP_TEST_MODE_RAW
/* Enable/Disable BNEP Raw Rx/Tx */
UCHAR appl_bnep_raw;

static UCHAR appl_packet_setup [7U+1U] =
             {
                 7U,
                 0x01U, 0x01U, 0x02U, 0x11U, 0x15U, 0x11U, 0x16U
             };

/*
static UCHAR appl_packet_setup_rsp [4+1] =
             {
                 4,
                 0x01, 0x02, 0x00, 0x00
             };

static UCHAR appl_packet_setup_rsp_neg [4+1] =
             {
                 4,
                 0x01, 0x02, 0x00, 0x01
             };
*/

static UCHAR appl_packet_nw [12U+1U] =
             {
                 12U,
                 0x01U, 0x03U, 0x00U, 0x08U, 0x44U, 0x40U, 0x44U, 0x44U,
                 0x55U, 0x50U, 0x55U, 0x55U
             };

/*
static UCHAR appl_packet_nw_rsp [4+1] =
             {
                 4,
                 0x01, 0x04, 0x00, 0x00
             };

static UCHAR appl_packet_nw_rsp_neg [4+1] =
             {
                 4,
                 0x01, 0x04, 0x00, 0x01
             };

*/

static UCHAR appl_packet_setup_nw [20U+1U] =
             {
                 20U,
                 0x81U, 0x01U, 0x02U, 0x11U, 0x15U, 0x11U, 0x16U, 0x00U,
                 0x0BU, 0x03U, 0x00U, 0x08U, 0x44U, 0x40U, 0x44U, 0x44U,
                 0x55U, 0x50U, 0x55U, 0x55U
             };

static UCHAR appl_packet_setup_rsp_nw [17U+1U] =
             {
                 17U,
                 0x81U, 0x02U, 0x00U, 0x00U, 0x00U, 0x0BU, 0x03U, 0x00U,
                 0x08U, 0x44U, 0x40U, 0x44U, 0x44U, 0x55U, 0x50U, 0x55U,
                 0x55U
             };

static UCHAR appl_packet_setup_rsp_neg_nw [17U+1U] =
             {
                 17U,
                 0x81U, 0x02U, 0x00U, 0x01U, 0x00U, 0x0BU, 0x03U, 0x00U,
                 0x08U, 0x44U, 0x40U, 0x44U, 0x44U, 0x55U, 0x50U, 0x55U,
                 0x55U
             };

static UCHAR appl_packet_setup_nw_unk_nwr [29U+1U] =
             {
                 29U,
                 0x81U, 0x01U, 0x02U, 0x11U, 0x15U, 0x11U, 0x16U, 0x80U,
                 0x0BU, 0x03U, 0x00U, 0x08U, 0x44U, 0x40U, 0x44U, 0x44U,
                 0x55U, 0x50U, 0x55U, 0x55U, 0x80U, 0x02U, 0x0BU, 0xF1U,
                 0x00U, 0x03U, 0x03U, 0x00U, 0x00U
             };

#endif /* BNEP_TEST_MODE_RAW */


/* ----------------------------------------- Functions */

void main_bnep_operations (void)
{
    int indx, choice, menu_choice;
    static UCHAR first_time = 0x0U;

    if (0x0U == first_time)
    {
        first_time = 0x1U;

        for (indx = 0U; indx < BNEP_MAX_ENTITIES; indx ++)
        {
            appl_bnep_handle[indx].allocated = 0x0U;
            appl_bnep_handle[indx].local_service = BNEP_SERVICE_NONE;
            appl_bnep_handle[indx].remote_service = BNEP_SERVICE_NONE;
            BT_mem_set (appl_bnep_handle[indx].bd_addr, 0x0, BT_BD_ADDR_SIZE);
        }

        (BT_IGNORE_RETURN_VALUE) BT_hci_get_local_bd_addr (appl_bnep_local_bd_addr);

    #ifdef BNEP_TEST_MODE_RAW
        appl_bnep_raw = 0x0U;
    #endif /* BNEP_TEST_MODE_RAW */
    }

    BT_LOOP_FOREVER()
    {
        printf("%s", bnep_options); fflush(stdout);
        scanf("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
        break;

        case 2:
            appl_bnep_display_handles();
            break;

        case 3:
            appl_bnep_registration();
        break;

        case 4:
            appl_bnep_set_policy();
            break;

        case 10:
            appl_bnep_accept();
            break;

        case 11:
            appl_bnep_connect();
        break;

        case 12:
            appl_bnep_setup();
            break;

        case 13:
            printf("[0] Set Setup IND Reply - Reject\n");
            printf("[1] Set Setup IND Reply - Accept\n");
            printf("Your Choice = "); fflush (stdout);
            scanf("%d", &choice);

            if (0U == choice)
            {
                appl_bnep_setup_ind_reply = BNEP_SETUP_NOT_ALLOWED;
            }
            else if (1U == choice)
            {
                appl_bnep_setup_ind_reply = BNEP_SETUP_SUCCESSFUL;
            }
            else
            {
                printf("*** Invalid Choice. Try again\n");
            }

            break;

        case 14:
            appl_bnep_filter();
        break;

        case 15:
            appl_bnep_disconnect();
        break;

        case 20:
            appl_bnep_write_data();
        break;

        case 21:
            appl_bnep_raw_write_data();
        break;

    #ifdef BNEP_TEST_MODE_RAW
        case 40:
            printf("Enable Raw Rx/Tx (1/0) ? "); fflush (stdout);
            scanf("%d", &choice);
            appl_bnep_raw = (1U == choice) ? 0x1U : 0x0U;
            BT_bnep_enable_raw (appl_bnep_raw);
            break;

        case 41:
            appl_bnep_write_raw();
            break;
    #endif /* BNEP_TEST_MODE_RAW */

        default:
            printf("Unrecognised User Option for BNEP Operations\n");
        break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }

        appl_bnep_wait();
    }

}


void appl_bnep_registration ( void )
{
    API_RESULT retval;
    BNEP_CALLBACKS bnep_cb;

    bnep_cb.bnep_event_ind = appl_bnep_event_ind;
    bnep_cb.bnep_read_ind = appl_bnep_read_ind;

    printf("Registering to BNEP ... "); fflush (stdout);
    retval = BT_bnep_register (&bnep_cb);
    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Reason = 0x%04x.\n", retval);
    }
    else
    {
        printf("SUCCEEDED !\n");
    }

    return;
}


void appl_bnep_set_policy ( void )
{
    int indx, choice;
    API_RESULT retval;
    UCHAR policy;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0)
    {
        printf("Invalid BNEP Handle\n");
        /* return; */
    }
    else
    {
        printf("Enter Policy (Hex) = "); fflush (stdin);
        scanf("%x", &choice);
        policy = (UCHAR) choice;

        /* BNEP Set Policy */
        printf("Starting BNEP Set Policy ... "); fflush (stdout);
        retval = BT_bnep_set_policy (appl_bnep_handle[indx].bd_addr, policy);

        if (API_SUCCESS != retval)
        {
            printf("FAILED !! Reason = 0x%04X\n", retval);
            /* return; */
        }
        else
        {
            printf("SUCCEEDED !!\n");
        }
    }

    return;
}


void appl_bnep_accept ( void )
{
    UCHAR services;
    API_RESULT retval;

    /* Initialize BNEP Services */
    services = BNEP_SERVICE_NAP | BNEP_SERVICE_GN | BNEP_SERVICE_PANU;

    /* BNEP Accept */
    printf("Starting BNEP Accept ... "); fflush (stdout);
    retval = BT_bnep_accept (services);

    if (API_SUCCESS != retval)
    {
        printf("FAILED !! Reason = 0x%04X\n", retval);
        /* return; */
    }
    else
    {
        printf("SUCCEEDED !!\n");
    }

    return;
}


void appl_bnep_connect ( void )
{
    int indx;
    UINT16 acl_indx;
    API_RESULT retval;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;
    acl_indx = 0U;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        /* Allocate Handle */
        appl_bnep_handle[indx].allocated = 0x1U;

        /* Get Remote BD_ADDR */
        retval = appl_bnep_choose_acl_connection (&acl_indx);
        if (API_SUCCESS != retval)
        {
            printf("Invalid ACL Handle \n");
            /* return; */
        }
        else
        {
            /* Copy BD_ADDR */
            BT_COPY_BD_ADDR
            (
                appl_bnep_handle[indx].bd_addr,
                appl_bnep_acl_list[acl_indx].bd_addr
            );

            /* Initiate BNEP Connect */
            printf("Starting BNEP Connect ... "); fflush (stdout);
            retval = BT_bnep_connect (appl_bnep_handle[indx].bd_addr);

            if (API_SUCCESS != retval)
            {
                printf("FAILED !! Reason = 0x%04X\n", retval);
                /* return; */
            }
            else
            {
                printf("SUCCEEDED !!\n");
                printf("Wait for BNEP_CONNECT_CNF\n");
            }
        }
    }

    return;
}


void appl_bnep_setup ( void )
{
    int indx, choice;
    API_RESULT retval;
    UCHAR local_service, remote_service;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        /* Get Local Service */
        printf("Options for Local Service: [1] PANU [2] NAP [3] GN\n");
        printf("Your Choice = "); fflush(stdout);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            local_service = BNEP_SERVICE_PANU;
            break;
        case 2:
            local_service = BNEP_SERVICE_NAP;
            break;
        case 3:
            local_service = BNEP_SERVICE_GN;
            break;
        default:
            /* Error Case */
            local_service = (UCHAR) choice;
            break;
        }

        /* Get Remote Service */
        printf("Options for Remote Service: [1] PANU [2] NAP [3] GN\n");
        printf("Your Choice = "); fflush(stdout);
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            remote_service = BNEP_SERVICE_PANU;
            break;
        case 2:
            remote_service = BNEP_SERVICE_NAP;
            break;
        case 3:
            remote_service = BNEP_SERVICE_GN;
            break;
        default:
            /* Error Case */
            remote_service = (UCHAR) choice;
            break;
        }

        /* Initiate BNEP Setup */
        printf("Starting BNEP Setup ... "); fflush (stdout);
        retval = BT_bnep_setup
                 (appl_bnep_handle[indx].bd_addr, local_service, remote_service);

        if (API_SUCCESS != retval)
        {
            printf("FAILED !! Reason = 0x%04X\n", retval);
            /* return; */
        }
        else
        {
            printf("SUCCEEDED !!\n");
            printf("Wait for BNEP_SETUP_CNF\n");
        }
    }

    return;
}


void appl_bnep_filter ( void )
{
    void * filter;
    BNEP_FILTER_NW_TYPE * nw_type_filter;
    BNEP_FILTER_MC_ADDR * mc_addr_filter;

    int indx, choice, i;
    API_RESULT retval;

    UINT16 num_range;
    UCHAR local, type;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;

    /* Init */
    retval = API_SUCCESS;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        printf("Enter Filter Type: [1] Local [2] Remote ? "); fflush (stdout);
        scanf("%d", &choice);
        local = (UCHAR) choice;

        printf("Enter Filter Type: [1] NW Type [2] MC ADDR ? "); fflush (stdout);
        scanf("%d", &choice);
        type = (UCHAR) choice;

        printf("Enter Number of Filter Range = "); fflush (stdout);
        scanf("%d", &choice);
        num_range = (UINT16) choice;

        if (num_range > 0U)
        {
            switch (type)
            {
            case BNEP_FILTER_TYPE_NW_TYPE:
                filter = BT_alloc_mem (sizeof (BNEP_FILTER_NW_TYPE) * num_range);

                if (NULL == filter)
                {
                    printf("*** Memory Allocation FAILED for NW Type Filter\n");
                    retval = API_FAILURE;
                    /* return; */
                }
                else
                {
                    nw_type_filter = filter;
                    for (i = 0U; i < num_range; i++)
                    {
                        printf("\tFilter %d: Start = ", i); fflush(stdout);
                        scanf("%x", &choice);
                        nw_type_filter[i].start = (UINT16)choice;

                        printf("\tFilter %d: End   = ", i); fflush(stdout);
                        scanf("%x", &choice);
                        nw_type_filter[i].end = (UINT16)choice;
                    }
                }

                break;

            case BNEP_FILTER_TYPE_MC_ADDR:
                filter = BT_alloc_mem (sizeof (BNEP_FILTER_MC_ADDR) * num_range);

                if (NULL == filter)
                {
                    printf("*** Memory Allocation FAILED for MC Addr Filter\n");
                    retval = API_FAILURE;
                    /* return; */
                }
                else
                {
                    mc_addr_filter = filter;
                    for (i = 0U; i < num_range; i++)
                    {
                        printf("\tFilter %d: Start = ", i); fflush(stdout);
                        appl_get_bd_addr(mc_addr_filter[i].start);

                        printf("\tFilter %d: End   = ", i); fflush(stdout);
                        appl_get_bd_addr(mc_addr_filter[i].end);
                    }
                }

                break;

            default:
                filter = NULL;
                break;
            }
        }
        else
        {
            filter = NULL;
        }

        if (API_SUCCESS == retval)
        {
            /* Initiate BNEP Setup */
            printf("Starting BNEP Filter ... "); fflush (stdout);
            retval = BT_bnep_filter
                     (
                         appl_bnep_handle[indx].bd_addr,
                         local,
                         type,
                         num_range,
                         filter
                     );

            if (API_SUCCESS != retval)
            {
                printf("FAILED !! Reason = 0x%04X\n", retval);
                /* return; */
            }
            else
            {
                printf("SUCCEEDED !!\n");
                printf("Wait for BNEP_FILTER_CNF\n");
                printf("Filter Data Pointer = %p\n", filter);
            }
        }
    }

    return;
}


void appl_bnep_disconnect ( void )
{
    int indx;
    API_RESULT retval;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    indx = 0;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        /* Initiate BNEP Disconnect */
        printf("Starting BNEP Disconnect ... "); fflush (stdout);
        retval = BT_bnep_disconnect (appl_bnep_handle[indx].bd_addr);

        if (API_SUCCESS != retval)
        {
            printf("FAILED !! Reason = 0x%04X\n", retval);
            /* return; */
        }
        else
        {
            printf("SUCCEEDED !!\n");
            printf("Wait for BNEP_DISCONNECT_CNF\n");
        }
    }

    return;
}


void appl_bnep_write_data ( void )
{
    UCHAR * eth_payload;
    int indx, read_val, i;
    API_RESULT retval;
    UINT16 eth_payload_len, offset;
    UINT16 protocol, protocol_802_1q;
    UCHAR eth_bd [BT_BD_ADDR_SIZE];
    UCHAR eth_header [BNEP_ETH_HEADER_SIZE];

    /* MISRA C-2012 Rule 9.1 */
    indx = 0;
    retval = API_SUCCESS;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        printf("Choose BNEP Packet Type:\n");
        printf("  [0] General Ethernet\n");
        printf("  [1] Compressed Ethernet Source Only\n");
        printf("  [2] Compressed Ethernet Dest Only\n");
        printf("  [3] Compressed Ethernet\n");
        printf("Your Choide = "); fflush(stdout);
        scanf("%d", &read_val);

        if (0U == read_val)
        {
            /* Get Ethernet Destination Address */
            printf("Enter Ethernet Dst Address = "); fflush(stdout);
            appl_get_bd_addr (eth_header);

            /* Get Ethernet Source Address */
            printf("Enter Ethernet Src Address = "); fflush(stdout);
            appl_get_bd_addr (&eth_header[6U]);
        }
        else if (1U == read_val)
        {
            /* Set Ethernet Destination Address */
            bnep_convert_address (eth_bd, appl_bnep_handle[indx].bd_addr);
            BT_mem_copy (eth_header, eth_bd, BT_BD_ADDR_SIZE);

            /* Get Ethernet Source Address */
            printf("Enter Ethernet Src Address = "); fflush(stdout);
            appl_get_bd_addr (&eth_header[BT_BD_ADDR_SIZE]);
        }
        else if (2U == read_val)
        {
            /* Get Ethernet Destination Address */
            printf("Enter Ethernet Dst Address = "); fflush(stdout);
            appl_get_bd_addr (eth_header);

            /* Set Ethernet Source Address */
            bnep_convert_address (eth_bd, appl_bnep_local_bd_addr);
            BT_mem_copy (&eth_header[BT_BD_ADDR_SIZE], eth_bd, BT_BD_ADDR_SIZE);
        }
        else if (3U == read_val)
        {
            /* Set Ethernet Destination Address */
            bnep_convert_address (eth_bd, appl_bnep_handle[indx].bd_addr);
            BT_mem_copy (eth_header, eth_bd, BT_BD_ADDR_SIZE);

            /* Set Ethernet Source Address */
            bnep_convert_address (eth_bd, appl_bnep_local_bd_addr);
            BT_mem_copy (&eth_header[BT_BD_ADDR_SIZE], eth_bd, BT_BD_ADDR_SIZE);
        }
        else
        {
            printf("*** Invalid Choice - %d. Try again.\n", read_val);
            retval = API_FAILURE; /* return; */
        }

        if (API_SUCCESS == retval)
        {
            printf("Enter Ethernet Protocol (in Hex) = "); fflush(stdout);
            scanf("%x", &read_val);
            protocol = (UINT16) read_val;

            eth_header[12U] = (UCHAR) (protocol >> 8U);
            eth_header[13U] = (UCHAR) protocol;

            protocol_802_1q = 0x0U;
            if (BNEP_ETH_PROTO_TYPE_802_1Q == protocol)
            {
                printf("Enter 802.1Q Ethernet Protocol (in Hex) = "); fflush(stdout);
                scanf("%x", &read_val);
                protocol_802_1q = (UINT16) read_val;
            }

            printf("Enter Ethernet Payload Length = "); fflush(stdout);
            scanf("%d", &read_val);
            eth_payload_len = (UINT16) read_val;

            if (BNEP_ETH_PROTO_TYPE_802_1Q == protocol)
            {
                eth_payload_len += 4U;
            }

            eth_payload = (UCHAR *) BT_alloc_mem (eth_payload_len);
            if (NULL == eth_payload)
            {
                printf("*** FAILED to Allocate Memory for Ethernet Payload\n");
                /* return; */
            }
            else
            {
                offset = 0U;
                if (BNEP_ETH_PROTO_TYPE_802_1Q == protocol)
                {
                    offset = 4U;
                    eth_payload[0U] = 0x0U;
                    eth_payload[1U] = 0x0U;
                    eth_payload[2U] = (UCHAR) (protocol_802_1q >> 8U);
                    eth_payload[3U] = (UCHAR) protocol_802_1q;
                }

                for (i = offset; i < eth_payload_len; i ++)
                {
                    eth_payload[i] = (UCHAR) i;
                }

                /* Write to BNEP */
                printf("Writing Data to BNEP ... "); fflush(stdout);
                retval = BT_bnep_write
                         (
                             appl_bnep_handle[indx].bd_addr,
                             eth_header,
                             NULL,
                             0U,
                             eth_payload,
                             eth_payload_len
                         );

                if (API_SUCCESS != retval)
                {
                    printf("FAILED !! Reason = 0x%04X\n", retval);
                }
                else
                {
                    printf("SUCCEEDED !!\n");
                }

                BT_free_mem(eth_payload);
            }
        }
    }

    return;
}


void appl_bnep_raw_write_data ( void )
{
    /* MISRA C - 2012 Rule 2.2 */

    /* If required, can be implemented to send raw packet over BNEP */
    printf("Empty function\n");

    return;
}


/* Callback Handler for BNEP_EVENT_IND_CB */
API_RESULT appl_bnep_event_ind
           (
               UCHAR      event_type,
               UCHAR *    bd_addr,
               UINT16     event_result,
               void *     event_data,
               UINT16     event_data_len
           )
{
    int indx;
    BNEP_SETUP * bnep_setup;
    BNEP_FILTER * bnep_filter;
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(event_data_len);

    printf("\n");

    switch (event_type)
    {
    case BNEP_CONNECT_IND:
        printf("Received BNEP_CONNECT_IND\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        /* Allocate BNEP Handle */
        retval = appl_bnep_alloc_bd_addr (&indx);
        if (API_SUCCESS != retval)
        {
            printf("*** FAILED to Allocate BNEP Entity for BD_ADDR\n");
            break;
        }
        else
        {
            /* Save BD_ADDR */
            BT_COPY_BD_ADDR (appl_bnep_handle[indx].bd_addr, bd_addr);
        }

        break;

    case BNEP_CONNECT_CNF:
        printf("Received BNEP_CONNECT_CNF\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        /* Search BNEP Handle */
        retval = appl_bnep_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            printf("*** FAILED to Get BNEP Entity for BD_ADDR\n");
            break;
        }

        break;

    case BNEP_SETUP_IND:
        printf("Received BNEP_SETUP_IND\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        /* Get BNEP_SETUP */
        bnep_setup = (BNEP_SETUP *) event_data;

        printf("    Setup: Local = 0x%02X, Remote = 0x%02X, Flag 0x%02X\n",
        bnep_setup->local_service, bnep_setup->remote_service,
        bnep_setup->flag);

        /* Search BNEP Handle */
        retval = appl_bnep_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            printf("*** FAILED to Get BNEP Entity for BD_ADDR\n");
            break;
        }

        printf("    Current: Local = 0x%02X, Remote = 0x%02X\n",
        appl_bnep_handle[indx].local_service,
        appl_bnep_handle[indx].remote_service);

        /* Is Response Required ? */
        if (0x01U == bnep_setup->flag)
        {
            /* Update Local & Remote Service */
            if (BNEP_SETUP_SUCCESSFUL == appl_bnep_setup_ind_reply)
            {
                appl_bnep_handle[indx].local_service =
                                       bnep_setup->local_service;
                appl_bnep_handle[indx].remote_service =
                                       bnep_setup->remote_service;
            }

            /* Send BNEP Setup RSP */
            printf("Starting BNEP Setup ... "); fflush (stdout);
            retval = BT_bnep_setup_rsp
                     (
                         appl_bnep_handle[indx].bd_addr,
                         appl_bnep_setup_ind_reply
                     );

            if (API_SUCCESS != retval)
            {
                printf("FAILED !! Reason = 0x%04X\n", retval);
            }
            else
            {
                printf("SUCCEEDED !!\n");
            }
        }
        else
        {
            appl_bnep_handle[indx].local_service = bnep_setup->local_service;
            appl_bnep_handle[indx].remote_service = bnep_setup->remote_service;
        }

        break;

    case BNEP_SETUP_CNF:
        printf("Received BNEP_SETUP_CNF\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        /* Get BNEP_SETUP */
        bnep_setup = (BNEP_SETUP *) event_data;

        printf("    Setup: Local = 0x%02X, Remote = 0x%02X, Flag 0x%02X\n",
        bnep_setup->local_service, bnep_setup->remote_service,
        bnep_setup->flag);

        /* Search BNEP Handle */
        retval = appl_bnep_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            printf("*** FAILED to Get BNEP Entity for BD_ADDR\n");
            break;
        }

        printf("    Current: Local = 0x%02X, Remote = 0x%02X\n",
        appl_bnep_handle[indx].local_service,
        appl_bnep_handle[indx].remote_service);

        /* Is Setup Successful ? */
        if (API_SUCCESS == event_result)
        {
            appl_bnep_handle[indx].local_service = bnep_setup->local_service;
            appl_bnep_handle[indx].remote_service = bnep_setup->remote_service;
        }

        break;

    case BNEP_DISCONNECT_IND:
        printf("Received BNEP_DISCONNECT_IND\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        /* Search BNEP Handle */
        retval = appl_bnep_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            printf("*** FAILED to Get BNEP Entity for BD_ADDR\n");
            break;
        }

        BT_mem_set (appl_bnep_handle[indx].bd_addr, 0x0, BT_BD_ADDR_SIZE);
        appl_bnep_handle[indx].local_service = BNEP_SERVICE_NONE;
        appl_bnep_handle[indx].remote_service = BNEP_SERVICE_NONE;
        appl_bnep_handle[indx].allocated = 0x0U;

        break;

    case BNEP_DISCONNECT_CNF:
        printf("Received BNEP_DISCONNECT_CNF\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        /* Search BNEP Handle */
        retval = appl_bnep_search_bd_addr (bd_addr, &indx);
        if (API_SUCCESS != retval)
        {
            printf("*** FAILED to Get BNEP Entity for BD_ADDR\n");
            break;
        }

        BT_mem_set (appl_bnep_handle[indx].bd_addr, 0x0, BT_BD_ADDR_SIZE);
        appl_bnep_handle[indx].local_service = BNEP_SERVICE_NONE;
        appl_bnep_handle[indx].remote_service = BNEP_SERVICE_NONE;
        appl_bnep_handle[indx].allocated = 0x0U;

        break;

    case BNEP_FILTER_IND:
        printf("Received BNEP_FILTER_IND\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        /* Get BNEP_FILTER */
        bnep_filter = (BNEP_FILTER *) event_data;

        printf("    Filter Type = 0x%02X\n", bnep_filter->type);
        printf("    Number of Filter Range = %d\n", bnep_filter->num_range);
        if (0U < bnep_filter->num_range)
        {
            if (BNEP_FILTER_TYPE_NW_TYPE == bnep_filter->type)
            {
                printf("    Filter (NW Type) Data: "); fflush (stdout);
                appl_dump_bytes
                (
                    (UCHAR *) bnep_filter->filter,
                    bnep_filter->num_range * 4U
                );
            }
            else if (BNEP_FILTER_TYPE_MC_ADDR == bnep_filter->type)
            {
                printf("    Filter (MC Addr) Data: "); fflush (stdout);
                appl_dump_bytes
                (
                    (UCHAR *) bnep_filter->filter,
                    bnep_filter->num_range * 12U
                );
            }
            else
            {
                printf("*** Unknown Filter Type - 0x%02X\n",
                bnep_filter->type);
            }
        }

        break;

    case BNEP_FILTER_CNF:
        printf("Received BNEP_FILTER_CNF\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        /* Get BNEP_FILTER */
        bnep_filter = (BNEP_FILTER *) event_data;

        printf("    Filter Type = 0x%02X\n", bnep_filter->type);
        printf("    Number of Filter Range = %d\n", bnep_filter->num_range);
        if (bnep_filter->num_range > 0U)
        {
            printf("    Filter Data: %p\n", bnep_filter->filter);
            if (NULL != bnep_filter->filter)
            {
                BT_free_mem (bnep_filter->filter);
            }
            else
            {
                printf("*** Expected Filter Data Pointer. NOT Returned\n");
            }
        }
        else
        {
            if (NULL != bnep_filter->filter)
            {
                printf("*** NOT Expected Filter Data Pointer. Num Range 0!\n");
            }
        }

        break;

    case BNEP_WRITE_CNF:
        printf("Received BNEP_WRITE_ERR\n");
        printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
        bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);
        printf("    Event Result = 0x%04X\n", event_result);

        break;

    default:
        printf("Received ???? Unknwon BNEP Event IND: 0x%02X\n",
        event_type);

        break; /* return API_SUCCESS; */
    }

    return API_SUCCESS;
}


/* Callback Handler for BNEP_READ_IND_CB */
API_RESULT appl_bnep_read_ind
           (
               UCHAR *    bd_addr,
               UCHAR *    eth_header,
               UCHAR *    eth_payload,
               UINT16     eth_payload_len
           )
{
#ifdef BNEP_TEST_MODE_RAW

    if (0x1U == appl_bnep_raw)
    {
        return appl_bnep_read_ind_raw
               (bd_addr, eth_payload, eth_payload_len);
    }
    else
    {
        appl_bnep_handle_data
        (bd_addr, eth_header, eth_payload, eth_payload_len);
    }

#else /* BNEP_TEST_MODE_RAW */

    (BT_IGNORE_RETURN_VALUE) appl_bnep_handle_data
    (bd_addr, eth_header, eth_payload, eth_payload_len);

#endif /* BNEP_TEST_MODE_RAW */

    return API_SUCCESS;
}


API_RESULT appl_bnep_handle_data
           (
               UCHAR *    bd_addr,
               UCHAR *    eth_header,
               UCHAR *    eth_payload,
               UINT16     eth_payload_len
           )
{
    UINT16 eth_protocol;

#ifdef APPL_BNEP_TEST_PROD_VS_PROD
    UINT16 offset, i;
#endif /* APPL_BNEP_TEST_PROD_VS_PROD */

    printf("\n");
    printf("Received BNEP_READ_IND\n");

    printf("    BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X\n",
    bd_addr[0U], bd_addr[1U], bd_addr[2U], bd_addr[3U], bd_addr[4U], bd_addr[5U]);

    printf("    Eth Dst Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
    eth_header[0U], eth_header[1U], eth_header[2U],
    eth_header[3U], eth_header[4U], eth_header[5U]);

    printf("    Eth Src Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
    eth_header[6U], eth_header[7U], eth_header[8U],
    eth_header[9U], eth_header[10U], eth_header[11U]);

    eth_protocol  = eth_header[12U];
    eth_protocol  = eth_protocol << 8U;
    eth_protocol |= eth_header[13U];

    printf("    Eth Protocol: %02X %02X (0x%04X, %d)\n",
    eth_header[12U], eth_header[13U], eth_protocol, eth_protocol);

    printf("    Payload Len : %d Octets\n", eth_payload_len);

#ifdef APPL_BNEP_TEST_PROD_VS_PROD
    offset = 0U;
#endif /* APPL_BNEP_TEST_PROD_VS_PROD */

    if (BNEP_ETH_PROTO_TYPE_802_1Q == eth_protocol)
    {
#ifdef APPL_BNEP_TEST_PROD_VS_PROD
        offset = 4U;
#endif /* APPL_BNEP_TEST_PROD_VS_PROD */

        eth_protocol  = eth_payload[2U];
        eth_protocol  = eth_protocol << 8U;
        eth_protocol |= eth_payload[3U];

        printf("    Eth 802.1Q Protocol: %02X %02X (0x%04X, %d)\n",
        eth_payload[2U], eth_payload[3U], eth_protocol, eth_protocol);
    }

#ifdef APPL_BNEP_TEST_PROD_VS_PROD
    for (i = offset; i < eth_payload_len; i ++)
    {
        if (eth_payload[i] != (UCHAR)i)
        {
            printf("\n");
            printf("*** Payload Corrupted!!!\n");
            break;
        }
    }
#endif /* APPL_BNEP_TEST_PROD_VS_PROD */

    return API_SUCCESS;
}


API_RESULT appl_bnep_alloc_bd_addr (int * indx)
{
    int count;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (count = 0U; count < BNEP_MAX_ENTITIES; count ++)
    {
        if (0x0U == appl_bnep_handle[count].allocated)
        {
            appl_bnep_handle[count].allocated = 0x1U;
            *indx = count;
            break; /* return API_SUCCESS; */
        }
    }

    if (BNEP_MAX_ENTITIES == count)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }

    return retval;
}


API_RESULT appl_bnep_search_bd_addr (UCHAR * bd_addr, int * indx)
{
    int count;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    for (count = 0U; count < BNEP_MAX_ENTITIES; count ++)
    {
        if ((0x1U == appl_bnep_handle[count].allocated) &&
            (0 == BT_mem_cmp(appl_bnep_handle[count].bd_addr, bd_addr, 6U)))
        {
            *indx = count;
            break; /* return API_SUCCESS; */
        }
    }

    if (BNEP_MAX_ENTITIES == count)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }

    return retval;
}


/* To Display & Choose a BNEP Connection Handle */
void appl_bnep_get_handle ( int * indx )
{
    /* Display Available BNEP Handles */
    appl_bnep_display_handles();

    BT_LOOP_FOREVER()
    {
        printf("Choose BNEP Handle Index = "); fflush(stdout);
        scanf("%d", indx);
        if (((*indx) >= 0) && ((*indx) < BNEP_MAX_ENTITIES))
        {
            break;
        }
    }

    return;
}


/* To Display BNEP Connection Handles */
void appl_bnep_display_handles ( void )
{
    int indx;

    printf("\n");
    printf("Index  Remote BD_ADDR     Local  Remote  State\n");
    printf("-----  -----------------  -----  ------  -----\n");

    for (indx = 0U; indx < BNEP_MAX_ENTITIES; indx ++)
    {
        if (0x0U == appl_bnep_handle[indx].allocated)
        {
            printf("%3d    NOT ALLOCATED\n", indx);
            continue;
        }

        printf("%3d    ", indx);
        printf("%02X:%02X:%02X:%02X:%02X:%02X  ",
        appl_bnep_handle[indx].bd_addr[0U], appl_bnep_handle[indx].bd_addr[1U],
        appl_bnep_handle[indx].bd_addr[2U], appl_bnep_handle[indx].bd_addr[3U],
        appl_bnep_handle[indx].bd_addr[4U], appl_bnep_handle[indx].bd_addr[5U]);
        switch (appl_bnep_handle[indx].local_service)
        {
        case BNEP_SERVICE_NONE:
            printf("N/A    ");
            break;
        case BNEP_SERVICE_PANU:
            printf("PANU   ");
            break;
        case BNEP_SERVICE_NAP:
            printf("NAP    ");
            break;
        case BNEP_SERVICE_GN:
            printf("GN     ");
            break;
        default:
            printf("?????  ");
            break;
        }

        switch (appl_bnep_handle[indx].remote_service)
        {
        case BNEP_SERVICE_NONE:
            printf("N/A     ");
            break;
        case BNEP_SERVICE_PANU:
            printf("PANU    ");
            break;
        case BNEP_SERVICE_NAP:
            printf("NAP     ");
            break;
        case BNEP_SERVICE_GN:
            printf("GN      ");
            break;
        default:
            printf("??????  ");
            break;
        }
        printf("Not Known\n");
    }

    printf("\n");
}


API_RESULT appl_bnep_choose_acl_connection (UINT16 * indx)
{
    int choice;
    API_RESULT retval;
    UCHAR i, num_list;

    /* Init */
    retval = API_SUCCESS;

    printf("Getting Connection Details from HCI ... "); fflush(stdout);
    retval = BT_hci_get_connection_details
             (appl_bnep_acl_list, BT_MAX_REMOTE_DEVICES, &num_list);

    if (API_SUCCESS != retval)
    {
         printf("FAILED !! Error Code = 0x%04X\n", retval);
         retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        printf("Succeeded.\n");

        if (0U == num_list)
        {
            retval = API_FAILURE; /*  return API_FAILURE; */
        }
        else
        {
            printf("\n");
            printf("==========================================================\n");
            printf("Active ACL Connections\n");
            printf("----------------------------------------------------------\n");
            for (i = 0U; i < num_list; i ++)
            {
                printf("  [%2d]  ", i);
                printf("BD_ADDR %02X:%02X:%02X:%02X:%02X:%02X  ",
                appl_bnep_acl_list[i].bd_addr[0U], appl_bnep_acl_list[i].bd_addr[1U],
                appl_bnep_acl_list[i].bd_addr[2U], appl_bnep_acl_list[i].bd_addr[3U],
                appl_bnep_acl_list[i].bd_addr[4U], appl_bnep_acl_list[i].bd_addr[5U]);
                printf("ACL Handle = 0x%04X\n", appl_bnep_acl_list[i].acl_handle);
            }
            printf("==========================================================\n");
            printf("\n");
        }
    }

    if (API_SUCCESS == retval)
    {
        printf("Choose ACL Connection Index = "); fflush(stdout);
        scanf("%d", &choice);
        if ((UINT16)choice >= BT_MAX_REMOTE_DEVICES)
        {
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            *indx = (UINT16)choice;
        }
    }

    return retval;
}


void appl_bnep_wait (void)
{
#if 0
    printf("\n");
    printf("Press ENTER to proceed ... ");
    fflush(stdin);

    while (getchar() != '\n')
    {
        /* MISRA C-2012 Rule 15.6 */
    }
#endif
}


#ifdef BNEP_TEST_MODE_RAW
void appl_bnep_write_raw (void)
{
    appl_bnep_test_1();
#if 0
    appl_bnep_test_2();
    appl_bnep_test_3();
    appl_bnep_test_4();
#endif


#if 0
    appl_bnep_test_5();
    appl_bnep_test_6();
    appl_bnep_test_7();
    appl_bnep_test_8();
    appl_bnep_test_9();
#endif
}

void appl_bnep_test_1 (void)
{
    int indx;

    /* MISRA C-2012 Rule 9.1 */
    indx = 0U;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0U)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        appl_bnep_wait();
        appl_bnep_send_raw_packet(indx, appl_packet_setup);
        appl_bnep_wait();
        appl_bnep_send_raw_packet(indx, appl_packet_nw);
        appl_bnep_wait();
    }

    return;
}


void appl_bnep_test_2 (void)
{
    int indx;

    /* MISRA C-2012 Rule 9.1 */
    indx = 0U;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0U)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        appl_bnep_wait();
        appl_bnep_send_raw_packet(indx, appl_packet_setup_nw);
        appl_bnep_wait();
    }

    return;
}


void appl_bnep_test_3 (void)
{
    int indx;

    /* MISRA C-2012 Rule 9.1 */
    indx = 0U;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0U)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        appl_bnep_wait();
        appl_bnep_send_raw_packet(indx, appl_packet_setup_rsp_nw);
        appl_bnep_wait();
    }

    return;
}


void appl_bnep_test_4 (void)
{
    int indx;

    /* MISRA C-2012 Rule 9.1 */
    indx = 0U;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0U)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        appl_bnep_wait();
        appl_bnep_send_raw_packet(indx, appl_packet_setup_rsp_neg_nw);
        appl_bnep_wait();
    }

    return;
}


void appl_bnep_test_5 (void)
{
    int indx;

    /* MISRA C-2012 Rule 9.1 */
    indx = 0U;

    /* Choose Handle */
    appl_bnep_get_handle (&indx);
    if (indx < 0U)
    {
        printf("Invalid BNEP Handle \n");
        /* return; */
    }
    else
    {
        appl_bnep_wait();
        appl_bnep_send_raw_packet(indx, appl_packet_setup_nw_unk_nwr);
        appl_bnep_wait();
    }

    return;
}


API_RESULT appl_bnep_read_ind_raw
           (
               UCHAR *    bd_addr,
               UCHAR *    packet,
               UINT16     packet_len
           )
{
    char stream [110U];
    UINT16 i, offset, count;

    printf("\n");
    printf("<--- BNEP Rx: %d Bytes\n", packet_len);

    count = 0U;
    offset = 0U;
    for (i = 0U; i < packet_len; i ++)
    {
        if (0U == offset)
        {
            BT_str_print(&stream[offset], "     + ");
            offset += 7U;
        }

        BT_str_print(&stream[offset], "%02X ", packet[i]);
        count ++;
        offset += 3U;

        if (16U == count)
        {
            printf("%s\n", stream);
            BT_mem_set (stream, 0, 110U);
            offset = 0U;
            count = 0U;
        }
    }

    if (0U != offset)
    {
        printf("%s\n", stream);
    }

    return API_SUCCESS;
}


void appl_bnep_send_raw_packet (int indx, UCHAR * packet)
{
    char stream [110U];
    UINT16 i, offset, count, packet_len;

    packet_len = packet[0U];
    packet = &packet[1U];

    printf("\n");
    printf("---> BNEP Tx: %d Bytes\n", packet_len);

    count = 0U;
    offset = 0U;
    for (i = 0U; i < packet_len; i ++)
    {
        if (0U == offset)
        {
            BT_str_print(&stream[offset], "     + ");
            offset += 7U;
        }

        BT_str_print(&stream[offset], "%02X ", packet[i]);
        count ++;
        offset += 3U;

        if (16U == count)
        {
            printf("%s\n", stream);
            BT_mem_set (stream, 0, 110U);
            offset = 0U;
            count = 0U;
        }
    }

    if (0U != offset)
    {
        printf("%s\n", stream);
    }

    /* Write Raw */
    BT_bnep_write_raw (appl_bnep_handle[indx].bd_addr, packet, packet_len);

    return;
}
#endif /* BNEP_TEST_MODE_RAW */
#endif /* BNEP */

