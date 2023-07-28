
/**
 *  \file appl_spp.c
 *
 *  Source file for SPP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_spp.h"
#include "appl_utils.h"

#ifdef SPP_VCOM_INTERFACE
#include "vcom_pl.h"
#endif /* SPP_VCOM_INTERFACE */

#define SPP_FILE_OPERATION

#ifdef SPP
/* ----------------------------------------- Exported Global Variables */
#ifdef SPP_VCOM_INTERFACE
static const char spp_options[] = "\n\
-------------- SPP Menu ---------------\n\
    0.  Exit \n\
    1.  Refresh \n\
\n\
    2.  Set BD_ADDR of Peer SPP\n\
    3.  SPP Init\n\
    4.  Start SPP\n\
    5.  Start SPP VS\n\
    7.  Stop SPP\n\
\n\
   10.  Create ACL connection\n\
   11.  Get SPP SDP Record\n\
\n\
   20.  Connect to SPP\n\
   21.  Disconnect from Peer SPP\n\
\n\
   30.  Send data over SPP\n\
   31.  Select SPP Instance\n\
\n\
   40.  Register with VCOM\n\
\n\
   50.  Receive Rx Data to file\n\
   51.  Stop Receiving Rx Data to file\n\
\n\
   80.  Get local server channel\n\
   81.  Set local server channel\n\
\n\
Your Choice : \0";
#else /* SPP_VCOM_INTERFACE */
static const char spp_options[] = "\n\
-------------- SPP Menu ---------------\n\
    0.  Exit \n\
    1.  Refresh \n\
\n\
    2.  Set BD_ADDR of Peer SPP\n\
    3.  SPP Init\n\
    4.  Start SPP\n\
    5.  Start SPP VS\n\
    7.  Stop SPP\n\
\n\
   10.  Create ACL connection\n\
   11.  Get SPP SDP Record\n\
\n\
   20.  Connect to SPP\n\
   21.  Disconnect from Peer SPP\n\
\n\
   30.  Send data over SPP\n\
   31.  Select SPP Instance\n\
   32.  Send data over all the connected Instances\n\
\n\
   50.  Receive Rx Data to file\n\
   51.  Stop Receiving Rx Data to file\n\
\n\
   80.  Get local server channel\n\
   81.  Set local server channel\n\
\n\
Your Choice : \0";
#endif /* SPP_VCOM_INTERFACE */

#if 0
static const char spp_send_str_options[] = "\n\
--------- Sample Strings ---------\n\
    1.  AT+CIND=?\\r\n\
    2.  AT+CIND?\\r\n\
    3.  ATEP\\r\n\
    4.  AT+CKPD=E\\r\n\
\n\
Your Choice[1-4] -> \0";

/* Send Str Buffer */
static UCHAR appl_spp_buffer[31U];
#endif

/* Active SPP Handle */
static SPP_HANDLE  g_spp_handle = SPP_HANDLE_INVALID;

/* Instance from where the vendor specific entites start */
static UCHAR spp_vs_instance_start;

/* Number of Instances over which data has to be sent */
static UCHAR spp_num_instances_to_send_data = 0U;

/* ----------------------------------------- Static Global Variables */

/**
 * This is to store the SPP connection status information.
 *
 * Update the connection status of each BD and its BDADDR.
 * Each time  case SPP_CONNECT_CNF or case SPP_CONNECT_IND is
 * called, the BDADDR of device is stored in an object of appl_spp_conn_status_list
 */
DECL_STATIC APPL_SPP_CONN_STATUS appl_spp_conn_status_list[SPP_MAX_ENTITY];

/* SDP handle */
DECL_STATIC SDP_HANDLE spp_sdp_handle;

/* SPP Peer BD Address */
DECL_STATIC UCHAR spp_peer_bd_addr[BT_BD_ADDR_SIZE];

/* Remote SPP Peer server channel */
DECL_STATIC UINT8 remote_server_channel;

/* Local SPP Server Channel */
DECL_STATIC UINT8 local_server_channel;

/* SPP SDP record handle */
DECL_STATIC UINT32 spp_record_handle;

#ifdef SPP_FILE_OPERATION
DECL_STATIC CHAR spp_recv_file_name[APPL_SPP_FILE_NAME_MAX_LEN];

DECL_STATIC BT_fops_file_handle spp_recv_fp;
DECL_STATIC UCHAR spp_recv_file_operation;

#endif /* SPP_FILE_OPERATION */

#ifdef SPP_VCOM_INTERFACE
/* VCOM specific variables */
VCOM_HANDLE appl_vcom_handle;
CHAR spp_port[16];
UINT32 spp_baud;
UCHAR spp_flow;

void spp_read_cb (UCHAR * data, UINT16 datalen);
#endif /* SPP_VCOM_INTERFACE */

/* ----------------------------------------- Static Function Declarations */


/* ------------------------------------------ Functions */
/* Basic SPP Operations */
void main_spp_operations (void)
{
    int        choice, menu_choice;
    int        read_val;
    UCHAR      server_channel, num_started_inst;

    UINT32 i;
    API_RESULT retval;

#ifdef SPP_VCOM_INTERFACE
    int        port;
#endif /* SPP_VCOM_INTERFACE */

    /* MISRA C-2012 Rule 9.1 */
    read_val = 0;
    retval = API_SUCCESS;

    BT_LOOP_FOREVER()
    {
        printf("%s", spp_options);
        fflush(stdin);
        scanf("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0: /* Exit */
            break; /* return; */

        case 1: /* Refresh */
            printf("Refreshing ...\n");
            break;

        case 2: /* Set BD_ADDR for SPP application */
            printf("Please enter BD ADDR of Peer SPP\n");
            (BT_IGNORE_RETURN_VALUE)appl_get_bd_addr(spp_peer_bd_addr);
            break;

        case 3: /* SPP Init */
        {
            retval = BT_spp_init(spp_application_callback);
            if (API_SUCCESS == retval)
            {
                printf("SPP Init Successful!\n");

                for (i = 0U; i < SPP_MAX_ENTITY; i++)
                {
                    appl_spp_conn_status_list[i].spp_handle = SPP_HANDLE_INVALID;
                }

                /* Compute the application instance to start the VS SPP */
                spp_vs_instance_start = (SPP_MAX_ENTITY - 1U);
            }
            else
            {
                printf("SPP Init failed. retval = 0x%04X\n", retval);
            }
        }
        break;

        case 4: /* Start SPP */
            for (i = 0U; i < spp_vs_instance_start; i++)
            {
                if (SPP_HANDLE_INVALID == appl_spp_conn_status_list[i].spp_handle)
                {
                    break;
                }
            }

            if (spp_vs_instance_start == i)
            {
                printf("No Free application entity\n");
                break;
            }

            /* Get record handle */
            retval = BT_dbase_get_record_handle
                     (
                         DB_RECORD_SPP,
                         (UCHAR)i,
                         &spp_record_handle
                     );

            if (API_SUCCESS != retval)
            {
                printf("Failed to get SPP record handle : 0x%04X\n",
                retval);

            }
            else
            {
                printf("SDP Rec Handle = 0x%08lX\n",
                spp_record_handle);

                /* Activate record */
                (BT_IGNORE_RETURN_VALUE) BT_dbase_activate_record(spp_record_handle);
            }

            (BT_IGNORE_RETURN_VALUE) BT_dbase_get_server_channel
            (
                spp_record_handle,
                PROTOCOL_DESC_LIST,
                &local_server_channel
            );

            printf("Local Server Channel = %d\n",local_server_channel);

            retval = BT_spp_start(local_server_channel, &appl_spp_conn_status_list[i].spp_handle);
            if (API_SUCCESS != retval)
            {
                printf("SPP Start failed. retval = 0x%04X\n", retval);
            }
            else
            {
                /* Set as started */
                appl_spp_conn_status_list[i].status = SPP_STARTED;
                printf("SPP started for spp Handle = %d\n", appl_spp_conn_status_list[i].spp_handle);
#ifdef SPP_VCOM_INTERFACE
                /* Initialize the VCOM */
                vcom_init_pl();
#endif /* SPP_VCOM_INTERFACE */
            }

            break;

        case 5: /* Start SPP VS */

            if (SPP_HANDLE_INVALID != appl_spp_conn_status_list[spp_vs_instance_start].spp_handle)
            {
                printf("No Free application entity\n");
                break;
            }

            /* Get record handle */
            retval = BT_dbase_get_record_handle
                     (
                         DB_RECORD_SPP_VS,
                         0U,
                         &spp_record_handle
                     );

            if (API_SUCCESS != retval)
            {
                printf("Failed to get SPP record handle : 0x%04X\n",
                retval);

            }
            else
            {
                printf("SDP Rec Handle = 0x%08lX\n",
                spp_record_handle);

                /* Activate record */
                (BT_IGNORE_RETURN_VALUE) BT_dbase_activate_record(spp_record_handle);
            }

            (BT_IGNORE_RETURN_VALUE) BT_dbase_get_server_channel
            (
                spp_record_handle,
                PROTOCOL_DESC_LIST,
                &local_server_channel
            );

            printf("Local Server Channel = 0x%x\n",local_server_channel);

            retval = BT_spp_start(local_server_channel, &appl_spp_conn_status_list[spp_vs_instance_start].spp_handle);
            if (API_SUCCESS != retval)
            {
                printf("SPP Start failed. retval = 0x%04X\n", retval);
            }
            else
            {
                /* Set as started */
                appl_spp_conn_status_list[spp_vs_instance_start].status = SPP_STARTED;
            }

            break;

        case 7: /* Stop SPP */
            if (SPP_HANDLE_INVALID == g_spp_handle)
            {
                printf("Select an SPP Instance to STOP\n");
                break;
            }
            for (i = 0U; i < SPP_MAX_ENTITY; i++)
            {
                if (g_spp_handle == appl_spp_conn_status_list[i].spp_handle)
                {
                    retval = BT_spp_stop(g_spp_handle);
                    if (API_SUCCESS == retval)
                    {
                        printf("SPP Stop of handle %d Successful\n", g_spp_handle);
                        appl_spp_conn_status_list[i].spp_handle = SPP_HANDLE_INVALID;
                        g_spp_handle = SPP_HANDLE_INVALID;
                        break;
                    }
                }
            }
            if (SPP_ERR_STOP_PENDING == retval)
            {
                printf("SPP Stop Pending\n");
            }
            else if (API_SUCCESS != retval)
            {
                printf("SPP Stop failed. retval = 0x%04X\n", retval);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

            break;

        case 10: /* Create ACL connection */
            retval =  BT_hci_create_connection
                      (
                          spp_peer_bd_addr,
                          LMP_ACL_DM1|LMP_ACL_DM3|LMP_ACL_DM5|
                          LMP_ACL_DH1|LMP_ACL_DH3|LMP_ACL_DH5,
                          0U, 0U, 0U,
                          0U
                      );

            if (API_SUCCESS ==  retval)
            {
                printf("HCI create connection success\n");
            }
            else
            {
                printf("Error creating ACL , API RETVAL 0x%04X\n",retval);
            }
            break;

        case 11: /* Get SPP SDP record */
            /* Set the SDP Handle */
            SDP_SET_HANDLE(spp_sdp_handle,spp_peer_bd_addr,appl_spp_sdp_cb);

            /* Call SDP open */
            retval = BT_sdp_open(&spp_sdp_handle);

            printf("API RETVAL BT_sdp_open : 0x%04X \n", retval);
            break;

        case 20: /* Connect To SPP */
            if (SPP_HANDLE_INVALID == g_spp_handle)
            {
                printf("Select an SPP Instance to Connect using Option 31\n");
                break;
            }
            else
            {
                printf("SPP Instance getting Connected %d\n",g_spp_handle);
            }
            printf("Enter Server Channel : 0x");
            scanf("%x", &choice);

            if (0U == choice)
            {
                choice = remote_server_channel;
            }

            retval = BT_spp_connect
                     (
                         g_spp_handle,
                         spp_peer_bd_addr,
                         (UINT8)choice
                     );
            if (API_SUCCESS != retval)
            {
                printf("SPP Connect failed. retval = 0x%04X\n",retval);
            }
            break;

        case 21: /* Disconnect From Peer SPP */
            if (SPP_HANDLE_INVALID == g_spp_handle)
            {
                printf("Select an SPP Instance to Disconnect using Option 31\n");
                break;
            }
            else
            {
                printf("SPP Instance getting disconnected %d\n",g_spp_handle);
            }
            retval = BT_spp_disconnect(g_spp_handle);
            if (API_SUCCESS != retval)
            {
                printf("SPP Disconnect failed. retval = 0x%04X\n",retval);
            }
            break;

        case 30: /* Send data to SPP */
        {
            int read = 0U;
#ifdef NXP_CODE
            uint8_t  read_string_ip[11];
            uint32_t input_loop_number;
#endif /* NXP_CODE */
            if (SPP_HANDLE_INVALID == g_spp_handle)
            {
                printf("Select an SPP Instance to Send data using Option 31\n");
            }
            else
            {
                printf("Enter details for handle = %d\n", g_spp_handle);
                printf("No of Bytes to Write = "); fflush(stdout);
                scanf("%d", &read);
                if (read < 0)
                {
                    printf("Invalid no. of bytes to write \n");
            }
                else
                {
                    appl_spp_conn_status_list[g_spp_handle].appl_nbytes = (UINT16)read;
                    appl_spp_conn_status_list[g_spp_handle].appl_count = 0U;
#ifdef NXP_CODE
                    LOG_DEBUG("Loop  = "); fflush(stdout);
                    scanf("%s", read_string_ip);
                    input_loop_number = appl_str_to_num(read_string_ip, 10);
                    appl_spp_conn_status_list[g_spp_handle].appl_loop = input_loop_number;
                    LOG_DEBUG("appl_loop - %u\n", (uint32_t)appl_spp_conn_status_list[g_spp_handle].appl_loop);
#else /* NXP_CODE */
                    printf("Loop  = "); fflush(stdout);
                    scanf("%d", &read);
                    appl_spp_conn_status_list[g_spp_handle].appl_loop = ((read < 0) ? (UINT16)1U : (UINT16)read);
#endif /* NXP_CODE */
                    spp_write_data(g_spp_handle);
                }
            }
        }
            break;

        case 31:
            num_started_inst = appl_spp_display_conn_status();

            if (0U != num_started_inst)
            {
                printf("Select SPP Handle\n");
                retval = appl_validate_params(&read_val,2U,0U,(SPP_MAX_ENTITY - 1U));
                if (API_SUCCESS == retval)
                {
                    for (i = 0U; i < SPP_MAX_ENTITY; i++)
                    {
                        if ((SPP_HANDLE)read_val == appl_spp_conn_status_list[i].spp_handle)
                        {
                            printf("SPP Handle Selected %d \n", read_val);
                            g_spp_handle = (SPP_HANDLE)read_val;
                            break;
                        }
                    }

                    if (SPP_MAX_ENTITY == i)
                    {
                        printf("SPP Handle Selected not started\n");
                    }
                }
            }
            else
            {
                printf("SPP Handles not started\n");
            }
            break;
        case 32:
        {
            int read = 0U;
#ifdef NXP_CODE
            uint8_t  read_string_ip[11];
            uint32_t input_loop_number;
#endif /* NXP_CODE */
            if (0U == spp_num_instances_to_send_data)
            {
                /**
                 * Enter SPP Tx. details.
                 */
                for (i = 0U; i < SPP_MAX_ENTITY; i++)
                {
                    if ((SPP_HANDLE_INVALID != appl_spp_conn_status_list[i].spp_handle) &&
                        (SPP_CONNECTED == appl_spp_conn_status_list[i].status))
                    {
                        printf("Enter details for handle = %d\n", i);
                        printf("No of Bytes to Write = "); fflush(stdout);
                        scanf("%d", &read);
                        if (read < 0)
                        {
                            printf("Invalid no. of bytes to write \n");
                        }
                        else
                        {
                            appl_spp_conn_status_list[i].appl_nbytes = (UINT16)read;
                            appl_spp_conn_status_list[i].appl_count = 0U;
#ifdef NXP_CODE
                            LOG_DEBUG("Loop  = "); fflush(stdout);
                            scanf("%s", read_string_ip);
                            input_loop_number = appl_str_to_num(read_string_ip, 10);
                            appl_spp_conn_status_list[i].appl_loop = input_loop_number;
                            LOG_DEBUG("appl_loop - %u\n", (uint32_t)appl_spp_conn_status_list[i].appl_loop);
#else /* NXP_CODE */
                            printf("Loop  = "); fflush(stdout);
                            scanf("%d", &read);
                            appl_spp_conn_status_list[i].appl_loop = ((read < 0U) ? (UINT16)1U : (UINT16)read);
#endif /* NXP_CODE */
                        }
                    }
                }

                /* Start Tx. for all the connected instances */
                for (i = 0U; i < SPP_MAX_ENTITY; i++)
                {
                    if ((SPP_HANDLE_INVALID != appl_spp_conn_status_list[i].spp_handle) &&
                        (SPP_CONNECTED == appl_spp_conn_status_list[i].status))
                    {
                        if (appl_spp_conn_status_list[i].appl_nbytes > 0)
                        {
                            spp_write_data(appl_spp_conn_status_list[i].spp_handle);
                            spp_num_instances_to_send_data++;
                        }
                    }
                }
            }
            }
            break;

#ifdef SPP_VCOM_INTERFACE
        case 40:
            printf("Enter VCOM Port number: ");
            scanf("%d", &port);
            BT_str_print(spp_port, "\\\\.\\COM%d", port);

            printf("Enter VCOM Baudrate: ");
            scanf("%ld", &spp_baud);

            printf("Enter VCOM flow: ");
            scanf("%d", &spp_flow);

            break;
#endif /* SPP_VCOM_INTERFACE */

        case 50:
#ifdef SPP_FILE_OPERATION
            BT_mem_set(spp_recv_file_name, 0, sizeof(spp_recv_file_name));
            BT_str_print(spp_recv_file_name, "%s%s", BT_FOPS_BASE, BT_FOPS_PATH_SEP);

            /* Read the name of the file to store */
            printf("Enter File Name to Receive Rx Data\n");
            scanf("%s", spp_recv_file_name + BT_str_len(spp_recv_file_name));

            retval = BT_fops_file_open
                     (
                         (UCHAR *)spp_recv_file_name,
                         (UCHAR *)"wb",
                         &spp_recv_fp
                     );

            if (API_SUCCESS != retval)
            {
                printf("Failed to open file: %s\n", spp_recv_file_name);
            }

            spp_recv_file_operation = BT_TRUE;

            #else /* SPP_FILE_OPERATION */
            printf("SPP_FILE_OPERATION flag is disabled\n");
#endif /* SPP_FILE_OPERATION */
            break;

        case 51:
#ifdef SPP_FILE_OPERATION
            printf("Stop Receive Rx Data to File\n");
            spp_recv_file_operation = BT_FALSE;

            if (NULL != spp_recv_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(spp_recv_fp);
                spp_recv_fp = NULL;
            }
#else /* SPP_FILE_OPERATION */
            printf("SPP_FILE_OPERATION flag is disabled\n");
#endif /* SPP_FILE_OPERATION */
            break;

        case 80:
            /* Get record handle */
            retval = BT_dbase_get_record_handle
                     (
                         DB_RECORD_SPP,
                         0,
                         &spp_record_handle
                     );
            if (API_SUCCESS != retval)
            {
                printf("Failed to get SPP SDP record handle : 0x%04X\n",
                        retval);
            }
            else
            {
                printf("SPP SDP Record Handle = 0x%08lX\n", spp_record_handle);

                /* Get the RFCOMM channel number */
                printf("Gettting the Local SPP Server Channel...\n");
                retval = BT_dbase_get_server_channel
                         (
                             spp_record_handle,
                             PROTOCOL_DESC_LIST,
                             &server_channel
                         );
                if (API_SUCCESS == retval)
                {
                    printf("Local SPP Server Channel = 0x%02X\n", server_channel);
                }
            }

            break;

        case 81:
#ifdef SDP_SERVER_ACCESS_RFCOMM_CH
            printf ("Enter local SPP server channel: ");
            scanf("%d", &read_val);
            server_channel = (UCHAR)read_val;

            /* Get record handle */
            retval = BT_dbase_get_record_handle
                     (
                         DB_RECORD_SPP,
                         0,
                         &spp_record_handle
                     );
            if (API_SUCCESS != retval)
            {
                printf("Failed to get SPP SDP record handle : 0x%04X\n",
                        retval);
            }
            else
            {
                printf("SPP SDP Record Handle = 0x%08lX\n", spp_record_handle);

                /* Get the RFCOMM channel number */
                printf("Settting the Local SPP Server Channel...\n");
                retval = BT_dbase_set_server_channel
                         (
                             spp_record_handle,
                             PROTOCOL_DESC_LIST,
                             &server_channel
                         );
                if (API_SUCCESS == retval)
                {
                    printf("Local SPP Server Channel Set = 0x%02X\n", server_channel);
                }
            }
#else /* SDP_SERVER_ACCESS_RFCOMM_CH */
            printf("SDP_SERVER_ACCESS_RFCOMM_CH not defined\n");
#endif /* SDP_SERVER_ACCESS_RFCOMM_CH */
            break;

        default:
            printf("Invalid choice. Please try again\n");
            break;
        }/* switch */

        printf("\n");

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

/* Display the status of spp_handle instances */
UCHAR appl_spp_display_conn_status(void)
{
    UINT32 i;
    UCHAR num_started_inst = 0U;

    for (i = 0U; i < SPP_MAX_ENTITY; i++)
    {
        if (SPP_HANDLE_INVALID != appl_spp_conn_status_list[i].spp_handle)
        {
            if ((SPP_MAX_ENTITY - 1U) == i)
            {
                printf("SPP VS Handle = %d\n", appl_spp_conn_status_list[i].spp_handle);
            }
            else
            {
                printf("SPP Handle = %d\n", appl_spp_conn_status_list[i].spp_handle);
            }
            num_started_inst++;
            /* If status is SPP_STARTED spp_handle is in STARTED state */
            if(SPP_STARTED == appl_spp_conn_status_list[appl_spp_conn_status_list[i].spp_handle].status)
            {
                printf("Status: STARTED\n");
            }
            /* If status is SPP_CONNECTED spp_handle is in CONNECTED state */
            else if(SPP_CONNECTED == appl_spp_conn_status_list[appl_spp_conn_status_list[i].spp_handle].status)
            {
                printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
                    BT_DEVICE_ADDR_ONLY_PRINT_STR (appl_spp_conn_status_list[appl_spp_conn_status_list[i].spp_handle].bd_addr));
                printf("Status: CONNECTED\n");
            }
            /* If status is SPP_DATA_TRANSFER spp_handle is in DATA TRANSFER state */
            else
            {
                printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
                    BT_DEVICE_ADDR_ONLY_PRINT_STR (appl_spp_conn_status_list[appl_spp_conn_status_list[i].spp_handle].bd_addr));
                printf("Status: DATA TRANSFER");
            }
                printf("\n");
        }
    }
    /* Total number of instances started */
    printf("Total Instances: %d\n", num_started_inst);
    return num_started_inst;
}

/* Callback function registered with SPP profile */
API_RESULT spp_application_callback
           (
               /* IN */  SPP_HANDLE spp_handle,
               /* IN */  SPP_EVENTS spp_event,
               /* IN */  API_RESULT      status,
               /* IN */  void          * data,
               /* IN */  UINT16          data_length
           )
{
#ifdef SPP_ENABLE_DATA_CNF_LOG
    UINT32 index;
#endif /* SPP_ENABLE_DATA_CNF_LOG */

    UCHAR * l_data;
    l_data = (UCHAR*)( data );

#if 0
    printf("\n"\
           "SPP HANDLE : %u\n"\
           "EVENT      : %d\n"\
           "RESULT     : 0x%04X\n",
            (unsigned int) spp_handle, spp_event, status);

    if (API_SUCCESS != status)
    {
        printf("\nSPP Command failure\n");
        return API_FAILURE;
    }
#endif /* 0 */

    switch(spp_event)
    {
    case SPP_CONNECT_CNF:
        printf("SPP_CONNECT_CNF -> 0x%04X\n", status);
        printf("SPP Instance Connected : %u\n",(unsigned int) spp_handle);
        printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        if (API_SUCCESS == status)
        {
            /* BD Address */
            BT_mem_copy
            (
                appl_spp_conn_status_list[spp_handle].bd_addr,
                l_data,
                BT_BD_ADDR_SIZE
            );

            /* Set the status as Connected */
            appl_spp_conn_status_list[spp_handle].status = SPP_CONNECTED;

        
            /* Set the global handle */
            /* g_spp_handle = spp_handle; */

#ifdef SPP_VCOM_INTERFACE
            /* Connect the VCOM */
            appl_vcom_handle = vcom_connect_pl(spp_port, spp_baud, spp_flow, spp_read_cb);
#endif /* SPP_VCOM_INTERFACE */
        }

        break;

    case SPP_CONNECT_IND:
        printf("SPP_CONNECT_IND -> 0x%04X\n", status);
        printf("SPP Instance Connected : %u\n",(unsigned int) spp_handle);
        printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        if (API_SUCCESS == status)
        {
            /* BD Address */
            BT_mem_copy
            (
                appl_spp_conn_status_list[spp_handle].bd_addr,
                l_data,
                BT_BD_ADDR_SIZE
            );

            /* Set the status as Connected */
            appl_spp_conn_status_list[spp_handle].status = SPP_CONNECTED;

            /* Set the global handle */
            /* g_spp_handle = spp_handle; */

#ifdef SPP_VCOM_INTERFACE
            /* Connect the VCOM */
            appl_vcom_handle = vcom_connect_pl(spp_port, spp_baud, spp_flow, spp_read_cb);
#endif /* SPP_VCOM_INTERFACE */
        }
        break;

    case SPP_DISCONNECT_CNF:
        printf("\nSPP_DISCONNECT_CNF -> 0x%04X\n", status);
        printf("SPP_DISCONNECT_CNF -> Disconnection Successful\n");
        printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        if (API_SUCCESS == status)
        {
            /**
            * Reset the BD Address
            */
            BT_mem_set(appl_spp_conn_status_list[spp_handle].bd_addr, 0x00, BT_BD_ADDR_SIZE);
            appl_spp_conn_status_list[spp_handle].status = SPP_STARTED;

#ifdef SPP_VCOM_INTERFACE
            /* Disconnect the VCOM */
            vcom_disconnect_pl(appl_vcom_handle);
#endif /* SPP_VCOM_INTERFACE */
        }
        break;

    case SPP_DISCONNECT_IND:
        printf("\nSPP_DISCONNECT_IND -> 0x%04X\n", status);
        printf("SPP_DISCONNECT_IND -> Disconnection Successful\n");
        printf("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        if (API_SUCCESS == status)
        {
            /**
            * Reset the BD Address
            */
            BT_mem_set(appl_spp_conn_status_list[spp_handle].bd_addr, 0x00, BT_BD_ADDR_SIZE);
            appl_spp_conn_status_list[spp_handle].status = SPP_STARTED;

#ifdef SPP_VCOM_INTERFACE
            /* Disconnect the VCOM */
            vcom_disconnect_pl(appl_vcom_handle);
#endif /* SPP_VCOM_INTERFACE */
        }
        break;

    case SPP_STOP_CNF:
        printf("SPP_STOP_CNF -> Stop Successful\n");
        break;

    case SPP_SEND_CNF:
    {
        /* printf("SPP_SEND_CNF -> String sent successfully\n"); */
        UCHAR * buffer;
        API_RESULT retval;

#ifdef SPP_ENABLE_DATA_CNF_LOG
        printf("Received spp send cnf\n");
        printf("   spp handle = %d\n", spp_handle);
        printf("    Buffer = %p\n", l_data);
        printf("    Actual Data Length = %d\n", data_length);
#else /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        printf(". ");
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */

        if (API_SUCCESS != status)
        {
            printf ("*** 0x%04X for spp handle %d\n", status, spp_handle);
        }

        BT_free_mem(data);
        appl_spp_conn_status_list[spp_handle].appl_count++;

        if (appl_spp_conn_status_list[spp_handle].appl_count == appl_spp_conn_status_list[spp_handle].appl_loop)
        {
            LOG_DEBUG("\n*** SPP Write for %d times has completed for SPP handle = %d!!!\n",
                    appl_spp_conn_status_list[spp_handle].appl_loop, spp_handle);

            /* Data transfer is completed so changing status back to SPP_CONNECTED */
            appl_spp_conn_status_list[spp_handle].status = SPP_CONNECTED;
            if (spp_num_instances_to_send_data)
            {
                spp_num_instances_to_send_data--;
                if (0U == spp_num_instances_to_send_data)
                {
                    printf("Data transfer to all the spp instances complete\n");
                }
            }
            break;
        }

        /* Send next chunk */
        buffer = (UCHAR *)BT_alloc_mem(appl_spp_conn_status_list[spp_handle].appl_nbytes);
        if (NULL == buffer)
        {
            printf("*** Buffer Allocation FAILED. !!\n");
            if (spp_num_instances_to_send_data)
            {
                spp_num_instances_to_send_data--;
            }
            break;
        }

        /* Set the appl data */
        if (2U > appl_spp_conn_status_list[spp_handle].appl_nbytes)
        {
            BT_mem_set(buffer, appl_spp_conn_status_list[spp_handle].appl_count,
                    appl_spp_conn_status_list[spp_handle].appl_nbytes);
        }
        else
        {
            BT_PACK_BE_2_BYTE_VAL(buffer, appl_spp_conn_status_list[spp_handle].appl_count);
            BT_mem_set((&buffer[2U]), appl_spp_conn_status_list[spp_handle].appl_count,
                    (appl_spp_conn_status_list[spp_handle].appl_nbytes - 2U));
        }

#ifdef SPP_ENABLE_DATA_CNF_LOG
        printf("Allocated Buffer = %p\n", buffer);
        printf("Initiating [%d]th spp  Write for SPP handle = %d... ", appl_spp_conn_status_list[spp_handle].appl_count, spp_handle);
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        retval = BT_spp_send
                 (
                     spp_handle,
                     buffer,
                     appl_spp_conn_status_list[spp_handle].appl_nbytes
                 );
        if (API_SUCCESS != retval)
        {
            printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
            BT_free_mem(buffer);
            if (spp_num_instances_to_send_data)
            {
                spp_num_instances_to_send_data--;
            }
        }
        else
        {
#ifdef SPP_ENABLE_DATA_CNF_LOG
            printf("Succeeded !!\n");
            printf("Wait for Write Complete.\n");
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        }
    }
    break;

    case SPP_RECVD_DATA_IND:

#ifdef SPP_FILE_OPERATION
        if (0U != spp_recv_file_operation)
        {
            UINT16 written;

            if (NULL != spp_recv_fp)
            {
                /* File will be read in a boot strapping fashion */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(l_data, data_length, spp_recv_fp, &written);
            }

            printf(". ");
        }
        else
#endif /* SPP_FILE_OPERATION */
        {
#ifdef SPP_ENABLE_DATA_CNF_LOG
            printf("SPP_RECVD_DATA_IND -> Data received successfully\n");
            printf("\n----------------CHAR DUMP-----------------------\n");
            for (index = 0U; index < data_length; index++)
            {
                printf("%c ", l_data[index]);
            }
            printf("\n------------------------------------------------\n");
            printf("\n----------------HEX DUMP------------------------\n");
            for (index = 0U; index < data_length; index++)
            {
                printf("%02X ", l_data[index]);
            }
            printf("\n------------------------------------------------\n");

#else /* SPP_ENABLE_DATA_CNF_LOG */
            printf(".");

#endif /* SPP_ENABLE_DATA_CNF_LOG */
        }

#ifdef SPP_VCOM_INTERFACE
        /* Write to the VCOM */
        vcom_write_pl(appl_vcom_handle, l_data, data_length);
#endif /* SPP_VCOM_INTERFACE */

        break;

    default:
        printf("\nUnknown command type\n");
        break;
    } /* switch */

    BT_IGNORE_UNUSED_PARAM(spp_handle);

    return API_SUCCESS;
}

/* SDP Callback for SPP */
void appl_spp_sdp_cb
     (
         UCHAR    command,
         UCHAR  * data,
         UINT16   length,
         UINT16   status
     )
{
    S_UUID       uuid;
    UINT16       num_uuids;
    UINT16       attrib_id[2U];
    UINT16       num_attribute_ids;
    UCHAR      * attribute_data;
    UCHAR      * sdp_query_result;
    UCHAR        no_server_channels;
    API_RESULT   retval;

    UINT16       len_attrib_data;

    static UCHAR sdp_close_due2error;

    BT_IGNORE_UNUSED_PARAM(sdp_close_due2error);
    BT_IGNORE_UNUSED_PARAM(length);

    retval = API_SUCCESS;
    attribute_data    = NULL;
    num_attribute_ids = 2U;

    /* Set SPP related UUID variables */
    uuid.uuid_type = UUID_16;
    uuid.uuid_union.uuid_16 = SERIALPORT_UUID;
    num_uuids = 0x01U;

    attrib_id[0U] = PROTOCOL_DESC_LIST;
    attrib_id[1U] = SUPPORTED_FEATURES_ATTR_ID;

    /* It can come only while creating connection. verify */
    if (API_SUCCESS != status)
    {
        printf("STATUS : %X \n", status);
        printf("COMMAND : %X \n", command);

        /* Implies that SDP open was successful but service search failed */
        if (command == SDP_ServiceSearchAttributeResponse)
        {
            sdp_close_due2error = 2U;

            printf("SDP CB : Closing SDP \n");

            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&spp_sdp_handle);

            retval = API_FAILURE; /* return; */
        }
    }

    if (API_SUCCESS == retval)
    {
        switch(command)
        {
            case SDP_Open : /* SDP open callback */
            {
                attribute_data = BT_alloc_mem(100U);

                if (NULL == attribute_data)
                {
                    printf("SDP CB : Alloc mem failed, closing SDP \n");
                    /* Error in SDP operation */
                    sdp_close_due2error = 2U;

                    /* Close SDP */
                    (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&spp_sdp_handle);
                    /* return; */
                }
                else
                {
                    len_attrib_data = 1024U;

                    /* Do Service Search Request */
                    retval = BT_sdp_servicesearchattributerequest
                             (
                                 &spp_sdp_handle,
                                 &uuid,
                                 num_uuids,
                                 attrib_id,
                                 num_attribute_ids,
                                 NULL,
                                 0x00U,
                                 attribute_data,
                                 &len_attrib_data
                             );

                    if (API_SUCCESS != retval)
                    {
                        printf("SDP CB:BT_sdp_servicesearchattributerequest Failed\n");

                        BT_free_mem(attribute_data);

                        printf("Closing SDP Connection\n");

                        /* Error in SDP operation */
                        sdp_close_due2error = 2U;

                        /* Close SDP */
                        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&spp_sdp_handle);

                        /* return; */
                    }
                    else
                    {
                        /* SDP operation to continue */
                        sdp_close_due2error = 1U;
                    }
                }

                break;
            }

            case SDP_Close: /* SDP Close callback */
                sdp_close_due2error = 0U;

                printf("SDP CB : SDP PROCEDURE DONE... \n");

                break;

            /* Service Search Attribute callback */
            case SDP_ServiceSearchAttributeResponse:
                sdp_query_result = data;
                no_server_channels = 1U;
                while (sdp_query_result != NULL)
                {
                    sdp_query_result =
                    BT_sdp_get_Pointer_to_next_AttributeList
                    (
                        data,
                        no_server_channels
                    );
                    if (NULL == sdp_query_result)
                    {
                        continue;
                    }
                    /* From the response extract the server channel */
                    retval = BT_sdp_get_channel_number
                    (
                        sdp_query_result,
                        &remote_server_channel
                    );
                    if (API_SUCCESS != retval)
                    {
                        printf("BT_sdp_get_channel_number failed , Closing SDP \n");
                        sdp_close_due2error = 2U;
                        break;
                    }
                    else
                    {
                        printf("\nRemote Server Channel 0x%02X",remote_server_channel);
                    }
                    no_server_channels++;
                }
                if (API_SUCCESS != retval)
                {
                    /* Free allocated memory for attribute data */
                    BT_free_mem(data);
                    /* Close SDP */
                    (BT_IGNORE_RETURN_VALUE)BT_sdp_close(&spp_sdp_handle);
                }
                printf("\nTotal Number of Server_channels %d\n",(no_server_channels - 1));
                break;

            case SDP_ErrorResponse:
                printf("SDP_ErrorResponse \n");

                /* Free allocated memory for attribute data */
                BT_free_mem(data);

                sdp_close_due2error = 2U;

                (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&spp_sdp_handle);

                break;

            default: /* Invalid: Nothing to do */
                printf("Invalid SDP Command: 0x%02X\n", command);
                break;

        }/* switch */
    }

    return;
}

void spp_write_data (SPP_HANDLE spp_handle)
{
    API_RESULT retval;
    UCHAR *buffer = NULL;

    /* Init */
    retval = API_SUCCESS;

    /* Return if 0 loop */
    if (0U == appl_spp_conn_status_list[spp_handle].appl_loop)
    {
            retval = API_FAILURE; /* return; */
        }

        if (API_SUCCESS == retval)
        {
        buffer = (UCHAR *) BT_alloc_mem (appl_spp_conn_status_list[spp_handle].appl_nbytes);
            if (NULL == buffer)
            {
                printf("*** Buffer Allocation FAILED. !!\n");
                /* return; */
            }
            else
            {
                /* Set the appl data */
            if (2U > appl_spp_conn_status_list[spp_handle].appl_nbytes)
                {
                BT_mem_set(buffer, appl_spp_conn_status_list[spp_handle].appl_count,
                        appl_spp_conn_status_list[spp_handle].appl_nbytes);
                }
                else
                {
                BT_PACK_BE_2_BYTE_VAL(buffer, appl_spp_conn_status_list[spp_handle].appl_count);
                BT_mem_set((&buffer[2U]), appl_spp_conn_status_list[spp_handle].appl_count,
                        (appl_spp_conn_status_list[spp_handle].appl_nbytes - 2U));
                }

                printf("Allocated Buffer = %p\n", buffer);

                retval = BT_spp_send
                         (
                        spp_handle,
                            buffer,
                        appl_spp_conn_status_list[spp_handle].appl_nbytes
                         );
                if (API_SUCCESS != retval)
                {
                    printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                    BT_free_mem(buffer);
                }
                else
                {
                    /**
                     * Changing the status of spp_handle (currently g_spp_handle) to SPP_DATA_TRANSFER.
                     * This is to indicate DATA TRANFER is ongoing.
                     */
                appl_spp_conn_status_list[spp_handle].status = SPP_DATA_TRANSFER;
                    printf("Succeeded !!\n");
                    printf("Wait for Write Complete.\n");
            }
        }
    }

    return;
}

#ifdef SPP_VCOM_INTERFACE
void spp_read_cb(UCHAR * data, UINT16 datalen)
{
    API_RESULT retval;

    /* printf("~ "); */

    retval = BT_spp_send
             (
                 g_spp_handle,
                 data,
                 datalen
             );
    if (API_SUCCESS != retval)
    {
        printf("SPP send data failed retval = 0x%04X\n", retval);
    }
}
#endif /* SPP_VCOM_INTERFACE */
#endif /* SPP */

