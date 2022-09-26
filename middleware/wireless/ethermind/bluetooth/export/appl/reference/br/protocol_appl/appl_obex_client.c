/**
 *  \file appl_obex_client.c
 *
 *  Source File for OBEX Client Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_obex_client.h"
#include "appl_utils.h"

#ifdef OBEX_CLIENT

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static UCHAR                  bd_addr[BT_BD_ADDR_SIZE];
static UINT16                 connection_handle;
static UCHAR                  appl_obex_cli_init;

static APPL_OBEX_INSTANCE     appl_obex_cli_inst[APPL_OBEX_CLI_NUM_INSTANCES];
static APPL_OBEX_ENTITY       appl_obex_entities[APPL_OBEX_NUM_INSTANCES];

static BT_fops_file_handle    obex_tx_fp;
static BT_fops_file_handle    obex_rx_fp;
static UINT32                 obj_size;
static UINT32                 remaining;
static UINT32                 obex_app_xchg_size;
static UINT32                 sent;
static UINT16                 actual;

static UCHAR                  appl_push_file_name[64U];
static UCHAR                  appl_pull_file_name[64U];

/**
 * OBEX Client Application Menu
 */
static const char appl_obex_client_menu[] = " \n\
--------------- OBEX Client Appl Menu ---------------------- \n\n\
  0. Exit. \n\
  1. Refresh. \n\
 \n\
  2. OBEX Initialize. \n\
  3. OBEX Shutdown. \n\
  4. OBEX Start. \n\
  5. OBEX Stop. \n\
  \n\
 10. Create ACL Connection. \n\
 11. ACL Disconnection. \n\
 \n\
 15. Connect to OBEX Server. \n\
 16. Disconnect from OBEX Server.\n\
 17. Transport Disconnection.\n\
 \n\
 20. OBEX Push File. \n\
 21. OBEX Pull File.\n\
 22. OBEX Setpath.\n\
 23. OBEX Action. \n\
 24. OBEX Abort. \n\
  \n\
Your Option -> ";

/* ----------------------------------------- Functions */

void main_obex_client_operations (void)
{
    int   choice, menu_choice, cn_handle, sc, val, handle;
    API_RESULT retval;
    APPL_OBEX_CONNECT_STRUCT connect_info;
    UCHAR i;
    UCHAR *appl_obex_handle_ptr;

    /* Init */
    retval             = API_SUCCESS;
    appl_obex_cli_init = BT_FALSE;

    BT_LOOP_FOREVER()
    {
        printf ("%s", appl_obex_client_menu);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            appl_obex_client_print_instances();
            break;

        case 2:
            printf ("Initializing OBEX Client Appl...\n");
            retval = appl_obex_client_init();
            printf ("retval - 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                for (i = 0U; i < APPL_OBEX_CLI_NUM_INSTANCES; i++)
                {
                    APPL_OBEX_RESET_INSTANCE(&appl_obex_cli_inst[i]);
                }
            }

            appl_obex_client_print_instances();
            break;

        case 3:
            printf ("Shutting down OBEX Client Appl...\n");
            retval = appl_obex_client_shutdown();
            printf ("retval - 0x%04X\n", retval);

            appl_obex_client_print_instances();
            break;

        case 4:
            printf ("Enter Obex Client Instance:");
            scanf ("%d", &choice);

            appl_obex_handle_ptr = &appl_obex_cli_inst[choice].obex_app_handle;

            printf ("Starting OBEX Client Appl. Instance.... \n");
            retval = appl_obex_client_start
                     (
                        appl_obex_handle_ptr,
                        appl_obex_client_callback
                     );

            printf ("retval - 0x%04X\n", retval);

            if (API_SUCCESS != retval)
            {
                *appl_obex_handle_ptr = 0xFFU;
            }

            appl_obex_client_print_instances();
            break;

        case 5:
            printf ("Enter Obex Client Instance:");
            scanf ("%d", &choice);

            appl_obex_handle_ptr = &appl_obex_cli_inst[choice].obex_app_handle;

            printf ("Stoping OBEX Client Appl. Instance.... \n");
            retval = appl_obex_client_stop
                     (
                         appl_obex_handle_ptr
                     );
            printf ("retval - 0x%04X\n", retval);
            if (API_SUCCESS == retval)
            {
                /* Reset the pointer */
                appl_obex_cli_inst[(*appl_obex_handle_ptr)].obex_entity = NULL;

                *appl_obex_handle_ptr = 0xFFU;
            }

            appl_obex_client_print_instances();
            break;

        case 10:
            /**
             * First Application should establish ACL Connection with
             * the peer Device
             */
            printf("Enter OBEX Server Bluetooth Device Address: ");

            /**
             * Read the BD_ADDR of Remote Device
             */
            appl_get_bd_addr(bd_addr);

            retval = BT_hci_create_connection
                     (
                         bd_addr,
                         LMP_ACL_DH5,
                         0x0U, 0x0U, 0x0U, 0x1U
                     );
            if (API_SUCCESS != retval)
            {
                printf("HCI Connect Attempt FAILED !! Error Code = 0x%04X\n",
                retval);
            }
            break;

        case 11:
            /**
             * To disconnect read the ACL Connection Handle
             */
            printf("Enter Connection Handle (in Hex): "); fflush(stdout);
            scanf("%x", &cn_handle);
            connection_handle = (UINT16)cn_handle;

            printf("\n");
            printf("Starting HCI Disconnection ... "); fflush(stdout);

            /**
             * Mention Reason for Disconnect as User Initiated Disconnection
             */
            retval = BT_hci_disconnect
                     (
                         connection_handle,
                         0x13U
                     );
            if (API_SUCCESS != retval)
            {
                printf("HCI Disconnection FAILED !! Error Code = 0x%04X\n",
                retval);
            }
            break;

        case 15:
            printf ("Select Application Instance: ");
            scanf ("%d", &handle);

            printf ("Enter peer device address: ");
            appl_get_bd_addr (bd_addr);
            connect_info.bd_addr = bd_addr;

#ifdef OBEX_OVER_L2CAP
            /**
             * Peer L2CAP PSM
             */
            printf ("Enter peer L2CAP PSM (in Hex): ");
            scanf ("%x", &val);
            connect_info.psm = (UINT16)val;
#endif /* OBEX_OVER_L2CAP */

            /**
             * RFCOMM Server Channel
             */
            printf ("Enter peer RFCOMM server channel (in Hex): ");
            scanf ("%x", &sc);
            connect_info.rfcomm_server_ch = (UCHAR)sc;

            printf ("Enter Data Exchange Size: ");
            scanf ("%d", &val);
            connect_info.max_recv_size = (UINT16 )val;

            printf ("Connecting to OBEX Server ... \n");
            retval = appl_obex_client_connect
                     (
                        &appl_obex_cli_inst[handle].obex_app_handle,
                        &connect_info
                     );
            printf ("retval - 0x%04X\n", retval);
            break;

        case 16:
            printf ("Select Application Instance: ");
            scanf ("%d", &handle);

            printf ("Disconnecting from OBEX Server... \n");
            retval =  appl_obex_client_disconnect
                      (
                          &appl_obex_cli_inst[handle].obex_app_handle
                      );
            printf ("retval - 0x%04X\n", retval);
            break;

        case 17:
            printf ("Select Application Instance: ");
            scanf ("%d", &handle);

            printf ("Disconnecting from OBEX Server Transport... \n");
            retval = appl_obex_client_transport_disconnect
                     (
                         &appl_obex_cli_inst[handle].obex_app_handle
                     );
            printf ("retval - 0x%04X\n", retval);
            break;

        case 20:
            printf ("Select Application Instance: ");
            scanf ("%d", &handle);

            printf ("Sending OBEX push request\n");
            retval =  appl_obex_client_push_file
                      (
                          &appl_obex_cli_inst[handle].obex_app_handle
                      );

            printf ("retval - 0x%04X\n", retval);
            break;

        case 21:
            printf ("Select Application Instance: ");
            scanf ("%d", &handle);
            printf ("Sending OBEX pull request...\n");
            retval =  appl_obex_client_pull_file
                      (
                          &appl_obex_cli_inst[handle].obex_app_handle
                      );
            printf ("retval - 0x%04X\n", retval);
            break;

        case 22:
            printf ("Select Application Instance: ");
            scanf ("%d", &handle);

            printf ("Sending OBEX Setpath request..n");
            retval =  appl_obex_client_setpath
                      (
                          &appl_obex_cli_inst[handle].obex_app_handle
                      );

            printf ("retval - 0x%04X\n", retval);
            break;

#ifdef OBEX_ENH_SUPPORT
        case 23:
            printf ("Select Application Instance: ");
            scanf ("%d", &handle);

            printf ("Sending OBEX Manipulate request ..\n");
            retval =  appl_obex_client_manipulate_obj
                      (
                          &appl_obex_cli_inst[handle].obex_app_handle
                      );

            printf ("retval - 0x%04X\n", retval);
            break;
#else
            printf ("Please enable OBEX_ENH_SUPPORT Feature Flag\n");
#endif /* OBEX_ENH_SUPPORT */

        case 24:
            printf ("Select Application Instance: ");
            scanf ("%d", &handle);

            printf ("Sending OBEX Abort request ..\n");
            retval =  appl_obex_client_abort
                      (
                          &appl_obex_cli_inst[handle].obex_app_handle
                      );

            printf ("retval - 0x%04X\n", retval);
            break;

        default:
            printf ("Invalide option\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}


API_RESULT appl_obex_client_init (void)
{
    API_RESULT retval;
    UCHAR i;

    /* Init */
    retval = API_SUCCESS;

    /* Check if already initialzed */
    if (BT_TRUE == appl_obex_cli_init)
    {
        printf ("[OBEX-APP]Obex Client Already Initialized  \n");
        retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
    }
    else
    {
        /* Reset OBEX Server Entities */
        BT_mem_set
        (
            appl_obex_entities,
            0x00,
            (sizeof(APPL_OBEX_ENTITY) * APPL_OBEX_NUM_INSTANCES)
        );

        /* Set the state */
        for (i = 0U; i < APPL_OBEX_NUM_INSTANCES; i++)
        {
            APPL_OBEX_SET_STATE(i, OBEX_CLIENT_STATE_INITIALIZED);

            /* Set obex handle to invalide value */
            appl_obex_entities[i].obex_handle = OBEX_MAX_INSTANCES;
        }

        /* Set as initialized */
        appl_obex_cli_init = BT_TRUE;
    }

    return retval;
}


API_RESULT appl_obex_client_shutdown(void)
{
    API_RESULT retval;
    UCHAR i;

    /* Init */
    retval = API_SUCCESS;

    /* Check if initialzed or not */
    if (BT_FALSE == appl_obex_cli_init)
    {
        printf ("[OBEX-APP] Obex Client is not Initialized to Shutdown\n");
        retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
    }
    else
    {
        /* Validate the state */
        for (i = 0U; i < APPL_OBEX_NUM_INSTANCES; i++)
        {
            if (OBEX_CLIENT_STATE_INITIALIZED != appl_obex_entities[i].appl_state)
            {
                /* Entity i is active */
                retval = APPL_OBEX_INVALID_STATE;
                break; /*  return APPL_OBEX_INVALID_STATE; */
            }
        }

        if (API_SUCCESS == retval)
        {
            /* Reset OBEX Server Entities */
            BT_mem_set
            (
                appl_obex_entities,
                0x00,
                (sizeof (APPL_OBEX_ENTITY) * APPL_OBEX_NUM_INSTANCES)
            );

            /* Set the state */
            for (i = 0U; i < APPL_OBEX_NUM_INSTANCES; i++)
            {
                APPL_OBEX_SET_STATE(i, OBEX_CLIENT_STATE_INVALID);
            }

            /* Set as un-initialized */
            appl_obex_cli_init = BT_FALSE;
        }
    }

    return retval;
}


API_RESULT appl_obex_client_start
           (
               APPL_OBEX_HANDLE *obex_app_handle,
               OBEX_NOTIFY_CB obex_notify_cb
           )
{
    UINT8 handle;
    API_RESULT retval;
    APPL_OBEX_ENTITY *obex_entity;

    /* Init */
    retval = API_SUCCESS;

    /* Validate params */
    if ((NULL == obex_app_handle) ||
        (NULL == obex_notify_cb))
    {
        printf ("[OBEX-APP] Invalid Parameters to Start \n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        for (handle = 0U; handle < APPL_OBEX_NUM_INSTANCES; handle++)
        {
            /* Check if state is valid */
            if (OBEX_CLIENT_STATE_INITIALIZED == appl_obex_entities[handle].appl_state)
            {
                obex_entity = &appl_obex_entities[handle];

                /* Initialize the entity */
                obex_entity->notify_cb = obex_notify_cb;
                obex_entity->trans_len = OBEX_MAX_PACKET_LENGTH;
                obex_entity->operation = APPL_OBEX_OP_NONE;

                /* Update the state */
                APPL_OBEX_SET_STATE(handle, OBEX_CLIENT_STATE_IDLE);

                /* Assign the allocated instance */
                *obex_app_handle = handle;

                appl_obex_cli_inst[handle].obex_entity = obex_entity;

                break;
            }
        }

        if (APPL_OBEX_NUM_INSTANCES == handle)
        {
            printf("[OBEX-APP: %d] No free OBEX Server instance to Sart\n", handle);

            retval = APPL_OBEX_ENTITY_START_FAILED;
        }
    }

    return retval;
}


API_RESULT appl_obex_client_stop
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    API_RESULT retval;
    APPL_OBEX_HANDLE handle;
    APPL_OBEX_ENTITY *obex_entity;

    /* Init */
    retval = API_SUCCESS;

    /* Validate params */
    if ((NULL == obex_app_handle)||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf ("[OBEX-APP] Invalid Parameters to Stop \n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        handle = *obex_app_handle;

        /* Get the entity for the given handle */
        obex_entity = &appl_obex_entities[handle];

        /* Check if the instance is Idle/Initialized */
        if ((OBEX_CLIENT_STATE_INITIALIZED != obex_entity->appl_state) &&
            (OBEX_CLIENT_STATE_IDLE != obex_entity->appl_state))
        {
            printf ("[OBEX-APP] Closing OBEX Transport on handle %d\n", handle);

            if (OBEX_CLIENT_STATE_IN_TRANS_DISCONNECT == obex_entity->appl_state)
            {
                APPL_OBEX_SET_STATE(handle, OBEX_CLIENT_STATE_STOP_PENDING);
            }
            else
            {
                /* Close the transport connection */
                retval = BT_obex_transport_disconnect (&obex_entity->obex_handle);
                if(API_SUCCESS != retval)
                {
                    printf ("[OBEX-APP] Transport Close Failed on Stop - 0x%04X\n", retval);
                }
                else
                {
                    APPL_OBEX_SET_STATE(handle, OBEX_CLIENT_STATE_STOP_PENDING);

                    printf ("[OBEX-APP] Transport Close Successful on Stop."
                    " Wait for confirmation.\n");
                }
            }

            printf (
            "[OBEX-APP] Obex Server Instance %d State Changed to 0x%02X\n",
            handle, obex_entity->appl_state);
        }

        if((OBEX_CLIENT_STATE_STOP_PENDING != obex_entity->appl_state) &&
           (API_SUCCESS == retval))
        {
            printf ("[OBEX-APP] Stopping Obex Server Instance %d\n", handle);

            /* Reset the entity field if any */
            obex_entity->notify_cb = NULL;

            /* Update the entity state */
            APPL_OBEX_SET_STATE(handle, OBEX_CLIENT_STATE_INITIALIZED);
        }
    }

    return retval;
}

API_RESULT appl_obex_client_connect
           (
               APPL_OBEX_HANDLE *obex_app_handle,
               APPL_OBEX_CONNECT_STRUCT * obex_connect_info
           )
{
    API_RESULT         retval;
    UCHAR              handle;
    APPL_OBEX_ENTITY  *obex_entity;
    OBEX_INIT_PARAMS   init_params;

    /* Init */
    retval = API_SUCCESS;

    /* Param Check */
    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)) ||
        (NULL == obex_connect_info))
    {
        printf ("[OBEX-APP] Invalid Parameters to Connect \n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        handle = *obex_app_handle;
        obex_entity = &appl_obex_entities[handle];

        if (OBEX_CLIENT_STATE_IDLE != obex_entity->appl_state)
        {
            printf ("[OBEX-APP] Invalid State to Connect \n");
            retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
        }
        else
        {
            /* Update the given connection info parameters */
            BT_mem_copy
            (
                obex_entity->bd_addr,
                obex_connect_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

            obex_entity->rfcomm_sever_ch = obex_connect_info->rfcomm_server_ch;
            obex_entity->trans_len = obex_connect_info->max_recv_size;

            /* Prepare for OBEX client registration */
            BT_mem_copy
            (
                init_params.bd_addr,
                obex_connect_info->bd_addr,
                BT_BD_ADDR_SIZE
            );

#ifdef RFCOMM
            init_params.channel_num = obex_connect_info->rfcomm_server_ch;
#endif /* RFCOMM */

#ifdef OBEX_OVER_L2CAP
            init_params.l2cap_lpsm = APPL_OBEX_L2CAP_PSM;
            init_params.l2cap_rpsm = obex_connect_info->psm;
#endif /* OBEX_OVER_L2CAP */

            init_params.max_rx_size = obex_connect_info->max_recv_size;
            init_params.obex_notify_cb = appl_obex_client_callback;

            /* Register for OBEX client */
            retval = BT_obex_register_client
                     (
                           &obex_entity->obex_handle,
                           &init_params
                     );
            if(API_SUCCESS != retval)
            {
                printf ("[OBEX-APP] OBEX Client Registration Failed for instance %d\n",
                handle);

                /* return retval; */
            }
            else
            {
                 /* Request for OBEX transport connection */
                retval = BT_obex_transport_connect
                         (
                             &obex_entity->obex_handle
                         );

                if(API_SUCCESS !=  retval)
                {
                    printf ("[OBEX-APP] OBEX Transport Connection Failed for instance %d\n",
                    handle);
                }
                else
                {
                    /* Update the entity state */
                    APPL_OBEX_SET_STATE
                    (
                        handle,
                        OBEX_CLIENT_STATE_IN_TRANS_CONNECT
                    );

                    /* Initialize OBEX App.  Operation ID */
                    obex_entity->operation = APPL_OBEX_OP_NONE;
                }
            }
        }
    }

    return retval;
}

API_RESULT appl_obex_client_transport_disconnect
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    API_RESULT retval;
    APPL_OBEX_ENTITY *obex_entity;
    UCHAR app_handle;

    /* Init */
    retval = API_SUCCESS;

    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid Parameters to Transport Disconnect \n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /*  return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        app_handle = *obex_app_handle;
        obex_entity = &appl_obex_entities[app_handle];

        /* Validate the state */
        if ((OBEX_CLIENT_STATE_IDLE == obex_entity->appl_state) ||
            (OBEX_CLIENT_STATE_INITIALIZED == obex_entity->appl_state) ||
            (OBEX_CLIENT_STATE_INVALID == obex_entity->appl_state))
        {
            printf (
            "[OBEX-APP] Invalid State for Transport Disconnect \n");
            retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
        }
        else
        {
            /* Disconnect Transport  */
            retval = BT_obex_transport_disconnect
                     (
                         &obex_entity->obex_handle
                     );
            if (API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP] Transport Disconnect Failed on entity: %d, Retval: 0x%04x",
                app_handle, retval);
            }
            else
            {
                /* Update the entity state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_IN_TRANS_DISCONNECT
                );
            }
        }
    }

    return retval;
}

API_RESULT appl_obex_client_disconnect
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    API_RESULT retval;
    APPL_OBEX_ENTITY *obex_entity;
    OBEX_OBJ_HEADER   tx_hdrs[OBEX_MAX_OBJECTS];
    UCHAR             app_handle;
    UINT8             num_tx_hdrs;

    /* Init */
    retval       = API_SUCCESS;
    num_tx_hdrs  = 0U;

    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid Parameters for OBEX Disconnection.\n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        app_handle = *obex_app_handle;
        obex_entity = &appl_obex_entities[app_handle];

        /* Validate the state */
        if (OBEX_CLIENT_STATE_CONNECTED != obex_entity->appl_state)
        {
            printf (
            "[OBEX-APP] Invalid State for OBEX Disconnection. \n");
            retval = APPL_OBEX_INVALID_STATE; /*  return APPL_OBEX_INVALID_STATE; */
        }
        else
        {
            /* Request for OBEX connection */
            retval = BT_obex_disconnect_req
                     (
                         &obex_entity->obex_handle,
                         tx_hdrs,
                         num_tx_hdrs
                     );
            if (API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP] OBEX Disconnection Failed on entity: %d, Retval: 0x%04x",
                app_handle, retval);
            }
            else
            {
                /* Update the entity state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_TRANS_CONNECTED
                );
            }
        }
    }

    return retval;
}

API_RESULT appl_obex_client_push_file
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    API_RESULT           retval;
    APPL_OBEX_ENTITY    *obex_entity;
    OBEX_OBJ_HEADER      tx_hdrs[OBEX_MAX_OBJECTS];
    APPL_OBEX_HDR_STRUCT name_info, body_info;
    UCHAR                num_tx_hdrs;
    UCHAR                app_handle;
    UCHAR                more;

    retval      = API_SUCCESS;
    num_tx_hdrs = 0U;
    more        = 0U;

    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid Handle for OBEX Put Operation.\n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /*  return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        app_handle = *obex_app_handle;
        obex_entity = &appl_obex_entities[app_handle];
        num_tx_hdrs = 0U;

        /* Validate the state */
        if (OBEX_CLIENT_STATE_CONNECTED != obex_entity->appl_state)
        {
            printf (
            "[OBEX-APP] Invalid State for OBEX Put Operation.\n");
            retval = APPL_OBEX_INVALID_STATE; /*  return APPL_OBEX_INVALID_STATE; */
        }
        else
        {
            APPL_OBEX_INIT_HEADER_STRUCT(name_info);
            APPL_OBEX_INIT_HEADER_STRUCT(body_info);

            printf ("Enter File Name:");
            scanf ("%s", appl_push_file_name);

            /* Name HDR info */
            name_info.value = appl_push_file_name;
            name_info.length = (UINT16 )(BT_str_n_len(appl_push_file_name, sizeof(appl_push_file_name)) + 1U);

            /* Open the file to be sent */
            retval = BT_fops_file_open (appl_push_file_name, (UCHAR *)"rb", &obex_tx_fp);
            if ((API_SUCCESS == retval) && (NULL != obex_tx_fp))
            {
                /* Get the file size */
                (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(obex_tx_fp, &obj_size);
                remaining = obj_size;
            }
            else
            {
                printf ("Failed to opent the file\n");
                retval = API_FAILURE; /* return  API_FAILURE; */
            }

            if (API_SUCCESS == retval)
            {
                /* Fill body hdr values */
                if (remaining > obex_app_xchg_size)
                {
                    body_info.length = (UINT16)obex_app_xchg_size;
                    more = 0x01U;
                }
                else if (0U != remaining)
                {
                    body_info.length = (UINT16)remaining;
                    more = 0x00U;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                /* Allocate memory */
                if (0U != body_info.length)
                {
                    body_info.value = BT_alloc_mem (body_info.length);
                }

                /* Read data */
                if (NULL != body_info.value)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                    (
                        body_info.value,
                        body_info.length,
                        obex_tx_fp,
                        &actual
                    );
                }

                /* ========= Add OBEX  Headers ========= */

#ifdef OBEX_OVER_L2CAP
                /* Enable SRM in response  */
                tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_SRM;
                tx_hdrs[num_tx_hdrs].hv.byte = OBEX_SRM_ENABLE;
                tx_hdrs[num_tx_hdrs].size = 1U;
                num_tx_hdrs++;
#endif /* OBEX_OVER_L2CAP */

                /* Add Name HDR */
                tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_NAME;
                tx_hdrs[num_tx_hdrs].hv.value = name_info.value;
                tx_hdrs[num_tx_hdrs].size = name_info.length;
                num_tx_hdrs ++;

                /* Add Body/End of Body HDR */
                tx_hdrs[num_tx_hdrs].hi =
                        (more == 0x01U)? OBEX_HDR_BODY: OBEX_HDR_END_OF_BODY;

                tx_hdrs[num_tx_hdrs].hv.value = body_info.value;
                tx_hdrs[num_tx_hdrs].size = body_info.length;
                num_tx_hdrs ++;

                /* Send OBEX Put Request */
                printf ("Pushing Object to the OBEX Server... \n");
                retval = BT_obex_put_req
                         (
                             &obex_entity->obex_handle,
                             tx_hdrs,
                             num_tx_hdrs,
                             !more
                         );
                if(API_SUCCESS != retval)
                {
                    printf (
                    "[OBEX-APP] Put Request in entity %d failed - 0x%04X\n",
                    app_handle, retval);
                }
                else
                {
                    /* Update current operation in entity */
                    obex_entity->operation = APPL_OBEX_OP_PUT;

                    /* Update the entity state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_CLIENT_STATE_W4_RSP
                    );

                    /* Update data length sent & remaining */
                    sent += body_info.length;
                    remaining =  obj_size - sent;

                    if (0x00U == remaining)
                    {
                        /* Reset the variables */
                        sent = 0U;
                        obj_size = 0U;

                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close(obex_tx_fp);
                        obex_tx_fp = NULL;
                    }

                    if (NULL != body_info.value)
                    {
                        BT_free_mem (body_info.value);
                        body_info.value = NULL;
                    }
                }
            } /* if (API_SUCCESS == retval) */
        } /* else of if (OBEX_CLIENT_STATE_CONNECTED != obex_entity->appl_state) */
    }

    BT_IGNORE_UNUSED_PARAM(name_info);
    return retval;
}

API_RESULT appl_obex_client_pull_file
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    API_RESULT           retval;
    APPL_OBEX_ENTITY    *obex_entity;
    OBEX_OBJ_HEADER      tx_hdrs[OBEX_MAX_OBJECTS];
    APPL_OBEX_HDR_STRUCT name_info;
    UCHAR                num_tx_hdrs;
    UCHAR                app_handle;

    /* Init */
    retval      = API_SUCCESS;
    num_tx_hdrs = 0U;

    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid Parameters for OBEX Put Operation.\n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        app_handle = *obex_app_handle;
        obex_entity = &appl_obex_entities[app_handle];

        /* Validate the state */
        if (OBEX_CLIENT_STATE_CONNECTED != obex_entity->appl_state)
        {
            printf (
            "[OBEX-APP] Invalid State for OBEX Put Operation.\n");
            retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
        }
        else
        {
            APPL_OBEX_INIT_HEADER_STRUCT(name_info);

            printf ("Enter The File Name:");
            scanf ("%s", appl_pull_file_name);

            /* Name HDR info */
            name_info.value  = appl_pull_file_name;
            name_info.length = (UINT16)(BT_str_n_len(appl_pull_file_name, sizeof(appl_pull_file_name)) + 1U);

           /* ========= Add OBEX  Headers ========= */

#ifdef OBEX_OVER_L2CAP
            /* Enable SRM in request  */
            tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_SRM;
            tx_hdrs[num_tx_hdrs].hv.byte = OBEX_SRM_ENABLE;
            tx_hdrs[num_tx_hdrs].size = 1U;
            num_tx_hdrs++;
#endif /* OBEX_OVER_L2CAP */

            /* Add Name HDR */
            tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_NAME;
            tx_hdrs[num_tx_hdrs].hv.value = name_info.value;
            tx_hdrs[num_tx_hdrs].size = name_info.length;
            num_tx_hdrs ++;

            /* Send OBEX Get Request */
            retval = BT_obex_get_req
                     (
                         &obex_entity->obex_handle,
                         tx_hdrs,
                         num_tx_hdrs,
                         0x01U  /* Final get */
                     );
            if(API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP] Get Request in entity %d failed - 0x%04X\n",
                app_handle, retval);
            }
            else
            {
                /* Update current operation in entity */
                obex_entity->operation = APPL_OBEX_OP_GET;

                /* Update the entity state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_W4_RSP
                );

                retval = BT_fops_file_open
                         (
                             (UCHAR *)appl_pull_file_name,
                             (UCHAR *)"wb",
                             &obex_rx_fp
                         );
                if ((API_SUCCESS != retval) || (NULL == obex_rx_fp))
                {
                    printf ("Failed to open Pull Object file\n");
                }
            }
        }
    }

    return retval;
}

API_RESULT appl_obex_client_setpath
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    int                  input;
    API_RESULT           retval;
    APPL_OBEX_ENTITY    *obex_entity;
    OBEX_OBJ_HEADER      tx_hdrs[OBEX_MAX_OBJECTS];
    APPL_OBEX_HDR_STRUCT name_info;
    UCHAR                num_tx_hdrs;
    UCHAR                setpath_flags;
    UCHAR                app_handle;

    /* Init */
    retval = API_SUCCESS;
    num_tx_hdrs = 0U;
    setpath_flags = 0U;

    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid Parameters for OBEX Setpath Operation.\n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        app_handle = *obex_app_handle;
        obex_entity = &appl_obex_entities[app_handle];

        /* Validate the state */
        if (OBEX_CLIENT_STATE_CONNECTED != obex_entity->appl_state)
        {
            printf (
            "[OBEX-APP] Invalid State for OBEX Setpath Operation.\n");

            retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
        }
        else
        {
            APPL_OBEX_INIT_HEADER_STRUCT(name_info);

            printf ("Select Folder to set: \n");
            printf ("\t1 -> Child\n");
            printf ("\t2 -> Parent\n");
            printf ("\t3 -> Root\n");
            scanf ("%d", &input);

            if (0x01U == input)
            {
                /**
                 * Child folder
                 */
                printf ("Enter The Folder Name:");
                scanf ("%s", appl_pull_file_name);

                setpath_flags = 0x02U;

                /* Name HDR info */
                name_info.value  = appl_pull_file_name;
                name_info.length = (UINT16)(BT_str_n_len(appl_pull_file_name, sizeof(appl_pull_file_name)) + 1U);
            }
            else if (0x02U == input)
            {
                /**
                 * Root folder
                 */
                setpath_flags = 0x03U;
            }
            else if (0x03U == input)
            {
                /**
                 * Parent folder
                 */
                setpath_flags = 0x02U;

                /* Name HDR info */
                name_info.value  = NULL;
                name_info.length = 0U;
            }
            else
            {
                printf ("Invalide option to set folder\n");
                retval = API_FAILURE; /* return API_FAILURE; */
            }

            if (API_SUCCESS == retval)
            {
                /* ========= Add OBEX  Headers ========= */

                if (0x03U != setpath_flags)
                {
                    /* Add Name HDR */
                    tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_NAME;
                    tx_hdrs[num_tx_hdrs].hv.value = name_info.value;
                    tx_hdrs[num_tx_hdrs].size = name_info.length;
                    num_tx_hdrs ++;
                }

                /* Send OBEX Setpath Request */
                retval = BT_obex_setpath_req
                         (
                             &obex_entity->obex_handle,
                             tx_hdrs,
                             num_tx_hdrs,
                             setpath_flags
                         );
                if(API_SUCCESS != retval)
                {
                    printf (
                    "[OBEX-APP] Setpath Request in entity %d failed - 0x%04X\n",
                    app_handle, retval);
                }
                else
                {
                    /* Update current operation in entity */
                    obex_entity->operation = APPL_OBEX_OP_SETPATH;

                    /* Update the entity state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_CLIENT_STATE_W4_RSP
                    );
                }
            } /* if (API_SUCCESS == retval) */
        }
    }

    return retval;
}

API_RESULT appl_obex_client_abort
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    API_RESULT           retval;
    APPL_OBEX_ENTITY    *obex_entity;
    OBEX_OBJ_HEADER      tx_hdrs[OBEX_MAX_OBJECTS];
    UCHAR                num_tx_hdrs;
    UCHAR                app_handle;

    /* Init */
    retval      = API_SUCCESS;
    num_tx_hdrs = 0U;

    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid Parameters for OBEX Setpath Operation.\n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        app_handle = *obex_app_handle;
        obex_entity = &appl_obex_entities[app_handle];

        /* Validate the state */
        if ((OBEX_CLIENT_STATE_W4_RSP != obex_entity->appl_state) &&
            (OBEX_CLIENT_STATE_IN_GET != obex_entity->appl_state) &&
            (OBEX_CLIENT_STATE_IN_PUT != obex_entity->appl_state))
        {
            printf (
            "[OBEX-APP] Invalid State for OBEX  Abort Operation.\n");
            retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
        }
        else
        {
            /* ========= Add OBEX  Headers ========= */
            if (OBEX_CLIENT_STATE_W4_RSP == obex_entity->appl_state)
            {
                /* Update the entity state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_FOR_ABORT
                );
            }
            else
            {
                /* Send OBEX Abort Request */
                retval = BT_obex_abort_req
                         (
                             &obex_entity->obex_handle,
                             tx_hdrs,
                             num_tx_hdrs
                         );
                if(API_SUCCESS != retval)
                {
                    printf (
                    "[OBEX-APP] Abort Request in entity %d failed - 0x%04X\n",
                    app_handle, retval);
                }
                else
                {
                    /* Update the entity state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_CLIENT_STATE_IN_ABORT
                    );
                }
            }
        }
    }

    return retval;
}

#ifdef OBEX_ENH_SUPPORT

API_RESULT appl_obex_client_manipulate_obj
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    int                  input;
    API_RESULT           retval;
    APPL_OBEX_ENTITY    *obex_entity;
    OBEX_OBJ_HEADER      tx_hdrs[OBEX_MAX_OBJECTS];
    UCHAR                num_tx_hdrs;
    UCHAR                app_handle;
    UCHAR                src_name[64U];
    UCHAR                des_name[64U];
    UINT32               permissions;

    /* Init */
    retval      = API_SUCCESS;
    num_tx_hdrs = 0U;

    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid Parameters for OBEX Setpath Operation.\n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        app_handle = *obex_app_handle;
        obex_entity = &appl_obex_entities[app_handle];

        /* Validate the state */
        if (OBEX_CLIENT_STATE_CONNECTED != obex_entity->appl_state)
        {
            printf (
            "[OBEX-APP] Invalid State for OBEX Setpath Operation.\n");
            retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
        }
        else
        {
            printf ("Select Action to Perform: \n");
            printf ("\t0 -> Copy\n");
            printf ("\t1 -> Move/Rename\n");
            printf ("\t2 -> Set Permissions\n");
            scanf ("%d", &input);

            /**
             * Source file name
             */
            printf ("Enter The Source Name:");
            scanf ("%s", src_name);

            if ((0x00U == input) ||
                (0x01U == input))
            {
                /**
                 * Destination file name
                 */
                printf ("Enter The Destination Name:");
                scanf ("%s", des_name);
            }
            else if (0x02U == input)
            {
                /**
                 * TODO: Get the permissions from the user
                 */
                permissions = 0x12U;
            }
            else
            {
                printf ("Invalide option for action operation\n");
                retval = API_FAILURE; /* return API_FAILURE; */
            }

            if (API_SUCCESS == retval)
            {
                /* ========= Add OBEX  Headers ========= */
                tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_ACTION_ID;
                tx_hdrs[num_tx_hdrs].size = 1U;
                tx_hdrs[num_tx_hdrs].hv.byte = (UCHAR )input;
                num_tx_hdrs ++;

                tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_NAME;
                tx_hdrs[num_tx_hdrs].size = (UINT16)(BT_str_len(src_name) + 1U);
                tx_hdrs[num_tx_hdrs].hv.value = src_name;
                num_tx_hdrs ++;

                if ((0x00U == input) ||
                    (0x01U == input))
                {
                    tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_DESTNAME;
                    tx_hdrs[num_tx_hdrs].size = (UINT16)(BT_str_len(des_name) + 1U);
                    tx_hdrs[num_tx_hdrs].hv.value = des_name;
                    num_tx_hdrs ++;
                }

                if (0x02U == input)
                {
                    tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_PERMISSIONS;
                    tx_hdrs[num_tx_hdrs].size = 4U;
                    tx_hdrs[num_tx_hdrs].hv.four_byte = permissions;
                    num_tx_hdrs ++;
                }

                /* Send OBEX Action Request */
                retval = BT_obex_action_req
                         (
                             &obex_entity->obex_handle,
                             tx_hdrs,
                             num_tx_hdrs,
                             0x01U /* Final Requestion */
                         );
                if(API_SUCCESS != retval)
                {
                    printf (
                    "[OBEX-APP] Action Request in entity %d failed - 0x%04X\n",
                    app_handle, retval);
                }
                else
                {
                    /* Update current operation in entity */
                    obex_entity->operation = APPL_OBEX_OP_ACTION;

                    /* Update the entity state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_CLIENT_STATE_W4_RSP
                    );
                }
            }
        }
    }

    return retval;
}
#endif /* OBEX_ENH_SUPPORT */

API_RESULT appl_obex_client_callback
           (
               OBEX_HANDLE * obex_handle,
               UINT16        event_result,
               UCHAR         event_type,
               void        * event_data,
               UINT16        event_datalen
           )
{
    API_RESULT           retval;
    APPL_OBEX_ENTITY    *obex_entity;
    OBEX_OBJ_HEADER      obex_headers[OBEX_MAX_OBJECTS];
    APPL_OBEX_REQ_STRUCT app_req_hdrs;
    APPL_OBEX_HDR_STRUCT name_info, body_info;
    UINT16               max_tx_rx_size;
    UINT16               length;
    UCHAR                num_tx_hdrs;
    UCHAR                num_rx_hdrs;
    UCHAR                num_rsp_hdrs;
    UCHAR                app_handle;
    UCHAR                send_request;
    UCHAR                more;
    UCHAR                i;
    UCHAR               *data;

    /* Init */
    retval       = API_SUCCESS;
    num_tx_hdrs  = 0U;
    num_rx_hdrs  = 0U;
    more         = 0U;
    num_rsp_hdrs = 0U;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(obex_headers, 0, sizeof(OBEX_OBJ_HEADER)*OBEX_MAX_OBJECTS);

    /* Get appl entity corresponding to OBEX handle */
    for (app_handle = 0U; app_handle < APPL_OBEX_NUM_INSTANCES; app_handle++)
    {
        if (appl_obex_entities[app_handle].obex_handle == *obex_handle)
        {
            break;
        }
    }

    if (APPL_OBEX_NUM_INSTANCES == app_handle)
    {
        printf (
        "[OBEX-APP: %d] OBEX handle matching with appl. instance not found\n", app_handle);
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Initialize the HDRs */
        APPL_OBEX_INIT_HEADER_STRUCT(body_info);
        APPL_OBEX_INIT_HEADER_STRUCT(name_info);

        app_req_hdrs.body = &body_info;

        obex_entity = &appl_obex_entities[app_handle];

        send_request = 0U;

        /* Extract OBEX headers, if any */
        if(0U != event_datalen)
        {
            (BT_IGNORE_RETURN_VALUE) BT_obex_extract_obex_headers
            (
                (UCHAR *)event_data,
                event_datalen,
                obex_headers,
                &num_rx_hdrs
            );
        }

        if(OBEX_CLIENT_STATE_FOR_ABORT == obex_entity->appl_state)
        {
            /* Send OBEX Abort Request */
            retval = BT_obex_abort_req
                     (
                         &obex_entity->obex_handle,
                         obex_headers,
                         num_tx_hdrs
                     );
            if(API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP] Abort Request in entity %d failed - 0x%04X\n",
                app_handle, retval);
            }
            else
            {
                /* Update the entity state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_IN_ABORT
                );
            }
        }

        /* Event processing */
        switch(event_type)
        {
        case OBEX_EVENT_TRANS_CONNECT_IND:
            printf ("Recvd OBEX_EVENT_TRANS_CONNECT_IND - 0x%04X\n", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);
            break;

        case OBEX_EVENT_TRANS_CONNECT_CFM:
            printf ("Recvd OBEX_EVENT_TRANS_CONNECT_CFM - 0x%04X\n", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if (API_SUCCESS == event_result)
            {
                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_TRANS_CONNECTED
                );
            }
            else
            {
                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_IDLE
                );

                break;
            }

            /* Request for OBEX connection */
            retval = BT_obex_connect_req
                     (
                         &obex_entity->obex_handle,
                         obex_headers,
                         num_rx_hdrs
                     );
            if(API_SUCCESS != retval)
            {
                /* TODO: Disconnect Transport */

                break;
            }

            /* Update state */
            APPL_OBEX_SET_STATE
            (
                app_handle,
                OBEX_CLIENT_STATE_IN_CONNECT
            );
            break;

        case OBEX_EVENT_TRANS_DISCONNECT_IND: /* Fall Through */
        case OBEX_EVENT_TRANS_DISCONNECT_CFM:
            printf ("Recvd %s - 0x%04X\n", (OBEX_EVENT_TRANS_DISCONNECT_CFM == event_type)?
                "OBEX_EVENT_TRANS_DISCONNECT_CFM":"OBEX_EVENT_TRANS_DISCONNECT_IND", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            /* Update the entity state */
            APPL_OBEX_SET_STATE
            (
                app_handle,
                OBEX_CLIENT_STATE_IDLE
            );
            break;

        case OBEX_EVENT_OBEX_CONNECT_CFM:
            printf ("Recvd OBEX_EVENT_OBEX_CONNECT_CFM - 0x%04X\n", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if (OBEX_SUCCESS_RSP == event_result)
            {
                /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
                max_tx_rx_size = 0U;

                /**
                 * Get the Maximum Rx size of the Peer device
                 * Set the Transmission Size to the minimum of the two:
                 * - Local Max Rx Size
                 * - Remote Max Rx Size
                 */
                (BT_IGNORE_RETURN_VALUE) BT_obex_get_peer_recv_size
                (
                    obex_handle,
                    &max_tx_rx_size
                );

                /**
                 *  Validate the Peer recv size. It should not be less than
                 *  'Minimum' OBEX Packet Length
                 */
                if (max_tx_rx_size < (OBEX_MIN_PACKET_LENGTH - 3U))
                {
                    printf (
                    "[OBEX-APP: %d] OBEX Client Appl. Maximum Receive Length < %d\n",
                    app_handle, OBEX_MIN_PACKET_LENGTH);
                }

                /* Update Appl trans len, Assuming 64 bytes for Name Hdr */
                /* TODO: Avoid this assumption and use BT_obex_get_available_frame_len */
                obex_app_xchg_size = ((UINT32)obex_entity->trans_len - 64U);

                /* Update the connection info parameters given */
                BT_mem_copy
                (
                    appl_obex_cli_inst[app_handle].bd_addr,
                    obex_entity->bd_addr,
                    BT_BD_ADDR_SIZE
                );

                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_CONNECTED
                );
            }

            appl_obex_client_print_instances();

            break;

        case OBEX_EVENT_OBEX_DISCONNECT_CFM:    /* Fall Through */
        case OBEX_EVENT_OBEX_DISCONNECT_IND:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_OBEX_DISCONNECT_CFM == event_type)?
                "OBEX_EVENT_OBEX_DISCONNECT_CFM":
                "OBEX_EVENT_OBEX_DISCONNECT_IND", event_result);

            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if (OBEX_SUCCESS_RSP == event_result)
            {
                /* Reset appl  bd-addr */
                BT_mem_set
                (
                    &appl_obex_cli_inst[app_handle].bd_addr,
                    0x00,
                    BT_BD_ADDR_SIZE
                );

                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_TRANS_CONNECTED
                 );

                /* Disconnect Transport  */
                retval = BT_obex_transport_disconnect
                         (
                             &obex_entity->obex_handle
                         );
                if (API_SUCCESS != retval)
                {
                    printf (
                    "[OBEX-APP] Transport Disconnect Failed on entity: %d, Retval: 0x%04x",
                    app_handle, retval);
                }
                else
                {
                    /* Update the entity state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_CLIENT_STATE_IN_TRANS_DISCONNECT
                    );
                }
            }

            appl_obex_client_print_instances();

            break;

        case OBEX_EVENT_PUT_CFM: /* Fall Through */
        case OBEX_EVENT_PUT_FINAL_CFM:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_PUT_CFM == event_type)?
                "OBEX_EVENT_PUT_CFM":"OBEX_EVENT_PUT_FINAL_CFM", event_result);

            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if (OBEX_CONTINUE_RSP == event_result)
            {
                send_request = 1U;

                /* Read data from the file */
                if(remaining > obex_app_xchg_size)
                {
                    body_info.length = (UINT16 )obex_app_xchg_size;
                    more = 0x01U;
                }
                else if (0x00U != remaining)
                {
                    body_info.length = (UINT16 )remaining;
                    more = 0x00U;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                /* Allocate memory */
                if (0U != body_info.length)
                {
                    body_info.value = BT_alloc_mem (body_info.length);
                }

                /* Read data */
                if (NULL != body_info.value)
                {
                    (BT_IGNORE_RETURN_VALUE) BT_fops_file_read
                    (
                        body_info.value,
                        body_info.length,
                        obex_tx_fp,
                        &actual
                    );
                }

                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_IN_PUT
                );
            }
            else
            {
                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_CONNECTED
                );

                if ( NULL != obex_tx_fp)
                {
                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close (obex_tx_fp);
                    obex_tx_fp =  NULL;
                }
            }

            break;

        case OBEX_EVENT_GET_FINAL_CONTINUE_CFM: /* Fall Through */
        case OBEX_EVENT_GET_FINAL_SUCCESS_CFM:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_GET_FINAL_CONTINUE_CFM == event_type)?
                "OBEX_EVENT_GET_FINAL_CONTINUE_CFM":"OBEX_EVENT_GET_FINAL_SUCCESS_CFM", event_result);

            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if ((OBEX_CLIENT_STATE_CONNECTED != obex_entity->appl_state) &&
                (OBEX_CLIENT_STATE_W4_RSP != obex_entity->appl_state))
            {
                printf (
                "[OBEX-APP: %d] Invalide state for Get Request - 0x%04X\n",
                app_handle, retval);

                retval = API_FAILURE; /* return API_FAILURE; */
            }

            if (API_SUCCESS == retval)
            {
                num_rsp_hdrs = 0U;
                for (i = 0U; i < num_rx_hdrs; i++)
                {
                    if(OBEX_HDR_NAME == obex_headers[i].hi)
                    {
                        name_info.value = obex_headers[i].hv.value;
                        name_info.length = obex_headers[i].size;

                        num_rsp_hdrs++;
                    }

                    /* Get the body headers, if any */
                    if((OBEX_HDR_BODY == obex_headers[i].hi) ||
                       (OBEX_HDR_END_OF_BODY == obex_headers[i].hi))
                    {
                        body_info.value = obex_headers[i].hv.value;
                        body_info.length = obex_headers[i].size;

                        num_rsp_hdrs++;
                    }
                }

                data   = body_info.value;
                length = body_info.length;

                if((NULL == data) ||
                   (0x00U == length))
                {
                    printf ("No content in the body header\n");
                }
                else
                {
                    if (NULL != obex_rx_fp)
                    {
                        (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, obex_rx_fp, &actual);
                    }
                }

                if (OBEX_EVENT_GET_FINAL_CONTINUE_CFM == event_type)
                {
                    send_request = 1U;

                    /* Update the state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_CLIENT_STATE_IN_GET
                    );
                }
                else
                {
                    /* Update the state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_CLIENT_STATE_CONNECTED
                    );

                    if (NULL != obex_rx_fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close (obex_rx_fp);
                        obex_rx_fp =  NULL;
                    }
                }
            }

            break;

        case OBEX_EVENT_SETPATH_CFM:
            printf ("Recvd OBEX_EVENT_SETPATH_CFM - 0x%04X\n", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            /* Update the state */
            APPL_OBEX_SET_STATE
            (
                app_handle,
                OBEX_CLIENT_STATE_CONNECTED
            );
            break;

#ifdef OBEX_ENH_SUPPORT
        case OBEX_EVENT_ACTION_CFM: /* Fall Through */
        case OBEX_EVENT_ACTION_FINAL_CFM:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_ACTION_CFM == event_type)?
                "OBEX_EVENT_ACTION_CFM":"OBEX_EVENT_ACTION_FINAL_CFM", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            /* Update the state */
            APPL_OBEX_SET_STATE
            (
                app_handle,
                OBEX_CLIENT_STATE_CONNECTED
            );
            break;
#endif /* OBEX_ENH_SUPPORT */

        case OBEX_EVENT_GET_ABORT_CFM: /* Fall Through */
        case OBEX_EVENT_PUT_ABORT_CFM:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_GET_ABORT_CFM == event_type)?
                "OBEX_EVENT_GET_ABORT_CFM":"OBEX_EVENT_PUT_ABORT_CFM", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            /* Update the state */
            APPL_OBEX_SET_STATE
            (
                app_handle,
                OBEX_CLIENT_STATE_CONNECTED
            );

            /* Reset All */
            if (NULL != obex_tx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(obex_tx_fp);
                obex_tx_fp = NULL;
            }

            if (NULL != obex_rx_fp)
            {
                /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                (void)BT_fops_file_close(obex_rx_fp);
                obex_rx_fp = NULL;
            }

            sent = 0U;
            obj_size = 0U;
            remaining = 0U;
            break;

        default:
            printf ("Recvd UNKNOWN_EVENT \n");
            printf ("OBEX Handle - 0x%02X\n", app_handle);
            break;
        }

        if ((OBEX_EVENT_TRANS_DISCONNECT_CFM == event_type) ||
            (OBEX_EVENT_TRANS_DISCONNECT_IND == event_type) ||
            ((OBEX_EVENT_TRANS_CONNECT_CFM == event_type) &&
             (API_SUCCESS != event_result)))
        {
            /* Unregister Client */
            retval = BT_obex_unregister_client
                     (
                         &obex_entity->obex_handle
                     );
            if(API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP: %d] OBEX Client UnRegister Failed - 0x%04X\n",
                app_handle, retval);
            }
        }

        if (0U != send_request)
        {
            retval = appl_obex_client_send_req
                     (
                        &obex_entity->obex_handle,
                        event_type,
                        event_result,
                        &app_req_hdrs,
                        more
                     );
            if(API_SUCCESS != retval)
            {
                 printf (
                "[OBEX-APP: %d] appl_obex_client_send_req Failed\n", app_handle);
            }

            if ((OBEX_EVENT_PUT_CFM == event_type) ||
                (OBEX_EVENT_PUT_FINAL_CFM == event_type))
            {
                /* Update data length sent & remaining */
                sent += body_info.length;
                remaining =  obj_size - sent;

                if (0x00U == remaining)
                {
                    /* Reset the variables */
                    sent = 0U;
                    obj_size = 0U;

                    /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                    (void)BT_fops_file_close(obex_tx_fp);
                    obex_tx_fp = NULL;
                }

                if (NULL != body_info.value)
                {
                    BT_free_mem (body_info.value);
                    body_info.value = NULL;
                }
            }
        }
    }

    /* To handle build warning - TODO: Check if this can be removed */
    BT_IGNORE_UNUSED_PARAM(name_info);

    return retval;
}

API_RESULT appl_obex_client_send_req
           (
               APPL_OBEX_HANDLE         *obex_app_handle,
               UINT8                     event_type,
               UINT16                    event_result,
               APPL_OBEX_REQ_STRUCT     *app_req_hdrs,
               UCHAR                     more
           )
{
    API_RESULT         retval;
    APPL_OBEX_ENTITY  *obex_entity;
    OBEX_OBJ_HEADER    tx_hdrs[OBEX_MAX_OBJECTS];
    UCHAR              num_tx_hdrs;
    UCHAR              app_handle;

#ifdef OBEX_OVER_L2CAP
    UCHAR              status;
#endif /* OBEX_OVER_L2CAP */

    /* Init */
    retval     =  API_SUCCESS;

    if((NULL == obex_app_handle) ||
       (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid  Parameters to responde\n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        app_handle = *obex_app_handle;
        obex_entity  = &appl_obex_entities[app_handle];
        num_tx_hdrs  = 0U;

        switch(event_type)
        {
        case OBEX_EVENT_PUT_CFM: /* Fall Through */
        case OBEX_EVENT_PUT_FINAL_CFM:
            tx_hdrs[num_tx_hdrs].hi =
            (more == 0x01U)? OBEX_HDR_BODY: OBEX_HDR_END_OF_BODY;
            tx_hdrs[num_tx_hdrs].hv.value = app_req_hdrs->body->value;
            tx_hdrs[num_tx_hdrs].size = app_req_hdrs->body->length;
            num_tx_hdrs++;

            /* Send OBEX Put Request */
            retval = BT_obex_put_req
                     (
                         &obex_entity->obex_handle,
                         tx_hdrs,
                         num_tx_hdrs,
                         !more
                     );
            if(API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP] Put Request in entity %d failed - 0x%04X\n",
                app_handle, retval);
            }
            else
            {
                /**
                 * Put request is successful
                 */

                /* Update the entity state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_W4_RSP
                );

            }
            break;

        case OBEX_EVENT_GET_FINAL_CONTINUE_CFM: /* Fall Through */
        case OBEX_EVENT_GET_FINAL_SUCCESS_CFM:

#ifdef OBEX_OVER_L2CAP
            /* Local has enabled srm, check if the SRM is active */
            retval = BT_obex_get_srm_status
                     (
                         &obex_entity->obex_handle,
                         0x03U,
                         &status
                     );
            if ((API_SUCCESS == retval) && (OBEX_SRM_ENABLE == status))
            {
                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_CLIENT_STATE_W4_RSP
                );

                /* return API_SUCCESS; */
            }
            else
#endif /* OBEX_OVER_L2CAP */
            {
                /* Send OBEX Get Request */
                retval = BT_obex_get_req
                         (
                             &obex_entity->obex_handle,
                             tx_hdrs,
                             num_tx_hdrs,
                             0x01U /* Get Final */
                         );
                if(API_SUCCESS != retval)
                {
                    printf (
                    "[OBEX-APP] Get Request in entity %d failed - 0x%04X\n",
                    app_handle, retval);
                }
                else
                {
                    /* Update the state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_CLIENT_STATE_W4_RSP
                    );
                }
            }
            break;

        default:
            printf("Invalid Event Type: 0x%02X\n", event_type);
            break;
        }
    }

    BT_IGNORE_UNUSED_PARAM(event_result);

    return retval;
}

void appl_obex_client_print_instances (void)
{
    UINT8 i;

    printf ("\n\n==============================================\n");
    printf ("Instance\tBD Addr");
    printf ("\n------------------------------------------------\n\n");

    for (i = 0U; i < APPL_OBEX_CLI_NUM_INSTANCES; i++)
    {
        printf ("  %d  \t\t", i);
        printf ("%02X:%02X:%02X:%02X:%02X:%02X\t ",
        appl_obex_cli_inst[i].bd_addr[0U], appl_obex_cli_inst[i].bd_addr[1U],
        appl_obex_cli_inst[i].bd_addr[2U], appl_obex_cli_inst[i].bd_addr[3U],
        appl_obex_cli_inst[i].bd_addr[4U], appl_obex_cli_inst[i].bd_addr[5U]);
    }

    printf ("\n-------------------------------------------------\n\n");

    return;
}

#endif  /* OBEX_CLIENT */
