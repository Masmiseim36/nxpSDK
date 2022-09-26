
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
   50.  Receive file\n\
   51.  Stop Receiving file\n\
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
\n\
   50.  Receive file\n\
   51.  Stop Receiving file\n\
\n\
   80.  Get local server channel\n\
   81.  Set local server channel\n\
\n\
Your Choice : \0";
#endif /* SPP_VCOM_INTERFACE */

char spp_send_str_options[] = "\n\
--------- Sample Strings ---------\n\
    1.  AT+CIND=?\\r\n\
    2.  AT+CIND?\\r\n\
    3.  ATEP\\r\n\
    4.  AT+CKPD=E\\r\n\
\n\
Your Choice[1-4] -> \0";

/* Send Str Buffer */
UCHAR appl_spp_buffer[31U];

/* Active SPP Handle */
SPP_HANDLE  g_spp_handle;

/* SPP Handles for SPP and VS_SPP instances */
SPP_HANDLE  spp_handle_list[SPP_MAX_ENTITY];

/* Instance from where the vendor specific entites start */
UCHAR spp_vs_instance_start;

/* ----------------------------------------- Static Global Variables */
/* SDP handle */
DECL_STATIC SDP_HANDLE sdp_handle;

/* SPP Peer BD Address */
DECL_STATIC UCHAR spp_peer_bd_addr[BT_BD_ADDR_SIZE];

/* Remote SPP Peer server channel */
DECL_STATIC UINT8 remote_server_channel;

/* Local SPP Server Channel */
DECL_STATIC UINT8 local_server_channel;

/* SPP SDP record handle */
DECL_STATIC UINT32 spp_record_handle;

#ifdef SPP_FILE_OPERATION
DECL_STATIC char spp_recv_file_name[APPL_SPP_FILE_NAME_MAX_LEN];

DECL_STATIC BT_fops_file_handle spp_recv_fp;
DECL_STATIC UCHAR spp_recv_file_operation;
/* Global variables used for SPP Write Operation in iteration */
/* Number of bytes in each SPP write */
DECL_STATIC UINT16 appl_nbytes;
/* Number of times the SPP Write to be performed */
DECL_STATIC UINT32 appl_loop;
/* Counter to maintain the number of SPP Write/Read has completed */
DECL_STATIC UINT32 appl_count;
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
    UCHAR      server_channel;

    UINT32 i;
    API_RESULT retval;

#ifdef SPP_VCOM_INTERFACE
    int        port;
#endif /* SPP_VCOM_INTERFACE */

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
            LOG_DEBUG("Refreshing ...\n");
            break;

        case 2: /* Set BD_ADDR for SPP application */
            LOG_DEBUG("Please enter BD ADDR of Peer SPP\n");
            appl_get_bd_addr(spp_peer_bd_addr);
            break;

        case 3: /* SPP Init */
        {
            retval = BT_spp_init(spp_application_callback);
            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("SPP Init Successful!\n");

                for (i = 0U; i < SPP_MAX_ENTITY; i++)
                {
                    spp_handle_list[i] = SPP_HANDLE_INVALID;
                }

                /* Compute the application instance to start the VS SPP */
                spp_vs_instance_start = (SPP_MAX_ENTITY - 1U);
            }
            else
            {
                LOG_DEBUG("SPP Init failed. retval = 0x%04X\n", retval);
            }
        }
        break;

        case 4: /* Start SPP */
            for (i = 0U; i < spp_vs_instance_start; i++)
            {
                if (SPP_HANDLE_INVALID == spp_handle_list[i])
                {
                    break;
                }
            }

            if (spp_vs_instance_start == i)
            {
                LOG_DEBUG("No Free application entity\n");
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
                LOG_DEBUG("Failed to get SPP record handle : 0x%04X\n",
                retval);

            }
            else
            {
                LOG_DEBUG("SDP Rec Handle = 0x%08lX\n",
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

            LOG_DEBUG("Local Server Channel = %d\n",local_server_channel);

            retval = BT_spp_start(local_server_channel, &spp_handle_list[i]);
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SPP Start failed. retval = 0x%04X\n", retval);
            }
            else
            {
                g_spp_handle = spp_handle_list[i];

#ifdef SPP_VCOM_INTERFACE
                /* Initialize the VCOM */
                vcom_init_pl();
#endif /* SPP_VCOM_INTERFACE */
            }

            break;

        case 5: /* Start SPP VS */

            if (SPP_HANDLE_INVALID != spp_handle_list[spp_vs_instance_start])
            {
                LOG_DEBUG("No Free application entity\n");
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
                LOG_DEBUG("Failed to get SPP record handle : 0x%04X\n",
                retval);

            }
            else
            {
                LOG_DEBUG("SDP Rec Handle = 0x%08lX\n",
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

            LOG_DEBUG("Local Server Channel = %d\n",local_server_channel);

            retval = BT_spp_start(local_server_channel, &spp_handle_list[spp_vs_instance_start]);
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SPP Start failed. retval = 0x%04X\n", retval);
            }

            break;

        case 7: /* Stop SPP */
            retval = BT_spp_stop(g_spp_handle);

            if (API_SUCCESS == retval)
            {
                LOG_DEBUG("SPP Stop Successful\n");
            }
            else if (SPP_ERR_STOP_PENDING == retval)
            {
                LOG_DEBUG("SPP Stop Pending\n");
            }
            else if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SPP Stop failed. retval = 0x%04X\n", retval);
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
                LOG_DEBUG("HCI create connection success\n");
            }
            else
            {
                LOG_DEBUG("Error creating ACL , API RETVAL 0x%04X\n",retval);
            }
            break;

        case 11: /* Get SPP SDP record */
            /* Set the SDP Handle */
            SDP_SET_HANDLE(sdp_handle,spp_peer_bd_addr,appl_spp_sdp_cb);

            /* Call SDP open */
            retval = BT_sdp_open(&sdp_handle);

            LOG_DEBUG("API RETVAL BT_sdp_open : 0x%04X \n", retval);
            break;

        case 20: /* Connect To SPP */
            LOG_DEBUG("Enter Server Channel (0 for default from SDP): 0x");
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
                LOG_DEBUG("SPP Connect failed. retval = 0x%04X\n",retval);
            }
            break;

        case 21: /* Disconnect From Peer SPP */
            retval = BT_spp_disconnect(g_spp_handle);
            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("SPP Disconnect failed. retval = 0x%04X\n",retval);
            }
            break;

        case 30: /* Send data to SPP */
            spp_write_data();
            break;

        case 31:
            for (i = 0U; i < SPP_MAX_ENTITY; i++)
            {
                LOG_DEBUG("SPP Handle [%d] %d\n", i, spp_handle_list[i]);
            }

            LOG_DEBUG("Select SPP Handle\n");
            scanf("%d", &read_val);
            g_spp_handle = (SPP_HANDLE)read_val;
            break;

#ifdef SPP_VCOM_INTERFACE
        case 40:
            LOG_DEBUG("Enter VCOM Port number: ");
            scanf("%d", &port);
            BT_str_print(spp_port, "\\\\.\\COM%d", port);

            LOG_DEBUG("Enter VCOM Baudrate: ");
            scanf("%ld", &spp_baud);

            LOG_DEBUG("Enter VCOM flow: ");
            scanf("%d", &spp_flow);

            break;
#endif /* SPP_VCOM_INTERFACE */

        case 50:
#ifdef SPP_FILE_OPERATION
            BT_mem_set(spp_recv_file_name, 0, sizeof(spp_recv_file_name));
            BT_str_print(spp_recv_file_name, "%s%s", BT_FOPS_BASE, BT_FOPS_PATH_SEP);

            /* Read the name of the file to store */
            LOG_DEBUG("Enter File Name\n");
            scanf("%s", spp_recv_file_name + BT_str_len(spp_recv_file_name));

            retval = BT_fops_file_open
                     (
                         (UCHAR *)spp_recv_file_name,
                         (UCHAR *)"wb",
                         &spp_recv_fp
                     );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("Failed to open file: %s\n", spp_recv_file_name);
            }

            spp_recv_file_operation = BT_TRUE;

            #else /* SPP_FILE_OPERATION */
            LOG_DEBUG("SPP_FILE_OPERATION flag is disabled\n");
#endif /* SPP_FILE_OPERATION */
            break;

        case 51:
#ifdef SPP_FILE_OPERATION
            LOG_DEBUG("Aborting Receive File\n");
            spp_recv_file_operation = BT_FALSE;

            if (NULL != spp_recv_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(spp_recv_fp);
                spp_recv_fp = NULL;
            }
#else /* SPP_FILE_OPERATION */
            LOG_DEBUG("SPP_FILE_OPERATION flag is disabled\n");
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
                LOG_DEBUG("Failed to get SPP SDP record handle : 0x%04X\n",
                        retval);
            }
            else
            {
                LOG_DEBUG("SPP SDP Record Handle = 0x%08lX\n", spp_record_handle);

                /* Get the RFCOMM channel number */
                LOG_DEBUG("Gettting the Local SPP Server Channel...\n");
                retval = BT_dbase_get_server_channel
                         (
                             spp_record_handle,
                             PROTOCOL_DESC_LIST,
                             &server_channel
                         );
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG("Local SPP Server Channel = 0x%02X\n", server_channel);
                }                
            }
            
            break;

        case 81:
#ifdef SDP_SERVER_ACCESS_RFCOMM_CH
            LOG_DEBUG ("Enter local SPP server channel: ");
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
                LOG_DEBUG("Failed to get SPP SDP record handle : 0x%04X\n",
                        retval);
            }
            else
            {
                LOG_DEBUG("SPP SDP Record Handle = 0x%08lX\n", spp_record_handle);

                /* Get the RFCOMM channel number */
                LOG_DEBUG("Settting the Local SPP Server Channel...\n");
                retval = BT_dbase_set_server_channel
                         (
                             spp_record_handle,
                             PROTOCOL_DESC_LIST,
                             &server_channel
                         );
                if (API_SUCCESS == retval)
                {
                    LOG_DEBUG("Local SPP Server Channel Set = 0x%02X\n", server_channel);
                }                
            }
#else /* SDP_SERVER_ACCESS_RFCOMM_CH */
            LOG_DEBUG("SDP_SERVER_ACCESS_RFCOMM_CH not defined\n");
#endif /* SDP_SERVER_ACCESS_RFCOMM_CH */
            break;

        default:
            LOG_DEBUG("Invalid choice. Please try again\n");
            break;
        }/* switch */

        LOG_DEBUG("\n");

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
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
    LOG_DEBUG("\n"\
           "SPP HANDLE : %u\n"\
           "EVENT      : %d\n"\
           "RESULT     : 0x%04X\n",
            (unsigned int) spp_handle, spp_event, status);

    if (API_SUCCESS != status)
    {
        LOG_DEBUG("\nSPP Command failure\n");
        return API_FAILURE;
    }
#endif /* 0 */

    switch(spp_event)
    {
    case SPP_CONNECT_CNF:
        LOG_DEBUG("SPP_CONNECT_CNF -> 0x%04X\n", status);
        LOG_DEBUG("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        if (API_SUCCESS == status)
        {
            /* Set the global handle */
            /* g_spp_handle = spp_handle; */

#ifdef SPP_VCOM_INTERFACE
            /* Connect the VCOM */
            appl_vcom_handle = vcom_connect_pl(spp_port, spp_baud, spp_flow, spp_read_cb);
#endif /* SPP_VCOM_INTERFACE */
        }

        break;

    case SPP_CONNECT_IND:
        LOG_DEBUG("SPP_CONNECT_IND -> 0x%04X\n", status);
        LOG_DEBUG("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        /* Set the global handle */
        /* g_spp_handle = spp_handle; */

#ifdef SPP_VCOM_INTERFACE
        /* Connect the VCOM */
        appl_vcom_handle = vcom_connect_pl(spp_port, spp_baud, spp_flow, spp_read_cb);
#endif /* SPP_VCOM_INTERFACE */

        break;

    case SPP_DISCONNECT_CNF:
        LOG_DEBUG("SPP_DISCONNECT_CNF -> Disconnection Successful\n");
        LOG_DEBUG("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

#ifdef SPP_VCOM_INTERFACE
        if (API_SUCCESS == status)
        {
            /* Disconnect the VCOM */
            vcom_disconnect_pl(appl_vcom_handle);
        }
#endif /* SPP_VCOM_INTERFACE */

        break;

    case SPP_DISCONNECT_IND:
        LOG_DEBUG("SPP_DISCONNECT_IND -> Disconnection Successful\n");
        LOG_DEBUG("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

#ifdef SPP_VCOM_INTERFACE
        /* Disconnect the VCOM */
        vcom_disconnect_pl(appl_vcom_handle);
#endif /* SPP_VCOM_INTERFACE */

        break;

    case SPP_STOP_CNF:
        LOG_DEBUG("SPP_STOP_CNF -> Stop Successful\n");
        break;

    case SPP_SEND_CNF:
    {
        /* LOG_DEBUG("SPP_SEND_CNF -> String sent successfully\n"); */
        UCHAR * buffer;
        API_RESULT retval;

#ifdef SPP_ENABLE_DATA_CNF_LOG
        LOG_DEBUG("Received spp send cnf\n");
        LOG_DEBUG("   spp handle = %d\n", g_spp_handle);
        LOG_DEBUG("    Buffer = %p\n", l_data);
        LOG_DEBUG("    Actual Data Length = %d\n", data_length);
#else /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        LOG_DEBUG(". ");
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */

        if (API_SUCCESS != status)
        {
            LOG_DEBUG ("*** 0x%04X\n", status);
        }

        BT_free_mem(data);
        appl_count++;

        if (appl_count == appl_loop)
        {
            LOG_DEBUG("\n*** SPP Write for %d times has completed!!!\n", appl_loop);
            break;
        }

        /* Send next chunk */
        buffer = (UCHAR *)BT_alloc_mem(appl_nbytes);
        if (NULL == buffer)
        {
            LOG_DEBUG("*** Buffer Allocation FAILED. !!\n");
            break;
        }

        /* Set the appl data */
        if (2U > appl_nbytes)
        {
            BT_mem_set(buffer, appl_count, appl_nbytes);
        }
        else
        {
            BT_PACK_BE_2_BYTE_VAL(buffer, appl_count);
            BT_mem_set((&buffer[2U]), appl_count, (appl_nbytes - 2U));
        }

#ifdef SPP_ENABLE_DATA_CNF_LOG
        LOG_DEBUG("Allocated Buffer = %p\n", buffer);
        LOG_DEBUG("Initiating [%d]th spp  Write ... ", appl_count);
#endif /* RFCOMM_ENABLE_DATA_WRITE_CNF_LOG */
        retval = BT_spp_send
                 (
                     g_spp_handle,
                     buffer,
                     appl_nbytes
                 );
        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("** FAILED ** !! Reason Code = 0x%04x\n", retval);
            BT_free_mem(buffer);
        }
        else
        {
#ifdef SPP_ENABLE_DATA_CNF_LOG
            LOG_DEBUG("Succeeded !!\n");
            LOG_DEBUG("Wait for Write Complete.\n");
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

            LOG_DEBUG(". ");
        }
        else
#endif /* SPP_FILE_OPERATION */
        {
#ifdef SPP_ENABLE_DATA_CNF_LOG
            LOG_DEBUG("SPP_RECVD_DATA_IND -> Data received successfully\n");
            LOG_DEBUG("\n----------------CHAR DUMP-----------------------\n");
            for (index = 0U; index < data_length; index++)
            {
                LOG_DEBUG("%c ", l_data[index]);
            }
            LOG_DEBUG("\n------------------------------------------------\n");
            LOG_DEBUG("\n----------------HEX DUMP------------------------\n");
            for (index = 0U; index < data_length; index++)
            {
                LOG_DEBUG("%02X ", l_data[index]);
            }
            LOG_DEBUG("\n------------------------------------------------\n");

#else /* SPP_ENABLE_DATA_CNF_LOG */
            LOG_DEBUG(".");

#endif /* SPP_ENABLE_DATA_CNF_LOG */
        }

#ifdef SPP_VCOM_INTERFACE
        /* Write to the VCOM */
        vcom_write_pl(appl_vcom_handle, l_data, data_length);
#endif /* SPP_VCOM_INTERFACE */

        break;

    default:
        LOG_DEBUG("\nUnknown command type\n");
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
    API_RESULT   retval;

    UINT16       len_attrib_data;

    static UCHAR sdp_close_due2error;

    BT_IGNORE_UNUSED_PARAM(length);
    BT_IGNORE_UNUSED_PARAM(sdp_close_due2error);

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
        LOG_DEBUG("STATUS : %X \n", status);
        LOG_DEBUG("COMMAND : %X \n", command);

        /* Implies that SDP open was successful but service search failed */
        if (command == SDP_ServiceSearchAttributeResponse)
        {
            sdp_close_due2error = 2U;

            LOG_DEBUG("SDP CB : Closing SDP \n");

            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);

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
                    LOG_DEBUG("SDP CB : Alloc mem failed, closing SDP \n");
                    /* Error in SDP operation */
                    sdp_close_due2error = 2U;

                    /* Close SDP */
                    (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);
                    /* return; */
                }
                else
                {
                    len_attrib_data = 100U;

                    /* Do Service Search Request */
                    retval = BT_sdp_servicesearchattributerequest
                             (
                                 &sdp_handle,
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
                        LOG_DEBUG("SDP CB:BT_sdp_servicesearchattributerequest Failed\n");

                        BT_free_mem(attribute_data);

                        LOG_DEBUG("Closing SDP Connection\n");

                        /* Error in SDP operation */
                        sdp_close_due2error = 2U;

                        /* Close SDP */
                        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);

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

                LOG_DEBUG("SDP CB : SDP PROCEDURE DONE... \n");

                break;

            /* Service Search Attribute callback */
            case SDP_ServiceSearchAttributeResponse:
                /* From the response extract the server channel */
                retval = BT_sdp_get_channel_number
                         (
                             data,
                             &remote_server_channel
                         );

                LOG_DEBUG("Remote Server Channel 0x%02X\n",remote_server_channel);

                if (API_SUCCESS != retval)
                {
                    sdp_close_due2error = 2U;

                    BT_free_mem(data);

                    LOG_DEBUG("BT_sdp_get_channel_number failed , Closing SDP \n");

                    (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);
                    /*  return; */
                }
                else
                {
                    /* Free allocated memory for attribute data */
                    BT_free_mem(data);

                    /* Close SDP */
                    (BT_IGNORE_RETURN_VALUE)BT_sdp_close(&sdp_handle);
                }

               break;

            case SDP_ErrorResponse:
                LOG_DEBUG("SDP_ErrorResponse \n");

                /* Free allocated memory for attribute data */
                BT_free_mem(data);

                sdp_close_due2error = 2U;

                (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_handle);

                break;

            default: /* Invalid: Nothing to do */
                LOG_DEBUG("Invalid SDP Command: 0x%02X\n", command);
                break;

        }/* switch */
    }

    return;
}

void spp_write_data (void)
{
    int read;
    UINT8  read_string_ip[11];
    UINT32 input_loop_number;

    API_RESULT retval;
    UCHAR *buffer = NULL;

    /* Init */
    retval = API_SUCCESS;

    LOG_DEBUG("No of Bytes to Write = "); fflush (stdout);
    scanf("%d", &read);
    if (read < 0)
    {
        LOG_DEBUG("Invalid no. of bytes to write \n");
        /* return; */
    }
    else
    {
        appl_nbytes = (UINT16)read;
        appl_count = 0U;

        LOG_DEBUG("Loop  = "); fflush (stdout);
        scanf("%s", &read_string_ip);
        input_loop_number = appl_str_to_num((UCHAR *)read_string_ip,(UINT16)BT_str_len(read_string_ip));
        appl_loop = ((input_loop_number == 0)? (UINT32)1U: (UINT32)input_loop_number);
        LOG_DEBUG("appl_loop - %u\n",(uint32_t)appl_loop);

        /* Return if 0 loop */
        if (0U == appl_loop)
        {
            retval = API_FAILURE; /* return; */
        }

        if (API_SUCCESS == retval)
        {
            buffer = (UCHAR *) BT_alloc_mem (appl_nbytes);
            if (NULL == buffer)
            {
                LOG_DEBUG("*** Buffer Allocation FAILED. !!\n");
                /* return; */
            }
            else
            {
                /* Set the appl data */
                if (2U > appl_nbytes)
                {
                    BT_mem_set(buffer, appl_count, appl_nbytes);
                }
                else
                {
                    BT_PACK_BE_2_BYTE_VAL(buffer, appl_count);
                    BT_mem_set((&buffer[2U]), appl_count, (appl_nbytes - 2U));
                }

                LOG_DEBUG("Allocated Buffer = %p\n", buffer);

                retval = BT_spp_send
                         (
                            g_spp_handle,
                            buffer,
                            appl_nbytes
                         );
                if (API_SUCCESS != retval)
                {
                    LOG_DEBUG("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                    BT_free_mem(buffer);
                }
                else
                {
                    LOG_DEBUG("Succeeded !!\n");
                    LOG_DEBUG("Wait for Write Complete.\n");
                }
            }
        }
    }

    return;
}

#ifdef SPP_VCOM_INTERFACE
void spp_read_cb(UCHAR * data, UINT16 datalen)
{
    API_RESULT retval;

    /* LOG_DEBUG("~ "); */

    retval = BT_spp_send
             (
                 g_spp_handle,
                 data,
                 datalen
             );
    if (API_SUCCESS != retval)
    {
        LOG_DEBUG("SPP send data failed retval = 0x%04X\n", retval);
    }
}
#endif /* SPP_VCOM_INTERFACE */
#endif /* SPP */

