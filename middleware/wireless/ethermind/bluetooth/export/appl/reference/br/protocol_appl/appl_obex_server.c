/**
 *  \file appl_obex_server.c
 *
 *  Source File for OBEX Server Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_obex_server.h"
#include "appl_utils.h"

#ifdef OBEX_SERVER

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static APPL_OBEX_INSTANCE   appl_obex_ser_inst[APPL_OBEX_SER_NUM_INSTANCES];
static APPL_OBEX_ENTITY     appl_obex_entities[APPL_OBEX_NUM_INSTANCES];

static UCHAR                appl_obex_ser_init;

static BT_fops_file_handle  obex_rx_fp;
static BT_fops_file_handle  obex_tx_fp;
static UINT32               obj_size;
static UINT32               remaining;
static UINT32               obex_app_xchg_size;
static UINT32               sent;
static UINT16               actual;
static UCHAR                obex_push_started;

static UCHAR               *appl_push_fp;
static UCHAR               *appl_pull_fp;

/**
 * OBEX Server Application Menu
 */
static const char appl_obex_server_menu[] = " \n\
--------------- OBEX Server Appl Menu ---------------------- \n\n\
  0. Exit. \n\
  1. Refresh. \n\
 \n\
  2. OBEX Initialize. \n\
  3. OBEX Shutdown. \n\
  4. OBEX Start. \n\
  5. OBEX Stop. \n\
  \n\
 17. Transport Disconnection.\n\
  \n\
Your Option -> ";

/* ----------------------------------------- Functions */

void main_obex_server_operations (void)
{
    int choice, menu_choice;
    API_RESULT retval;
    UCHAR *appl_obex_handle_ptr, i;

    choice = 0U;
    appl_obex_ser_init = BT_FALSE;

    BT_LOOP_FOREVER()
    {
        printf ("%s", appl_obex_server_menu);
        scanf ("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            appl_obex_server_print_instances();
            break;

        case 2:
            printf ("Initializing OBEX Server Appl...\n");
            retval = appl_obex_server_init ();
            printf ("retval - 0x%04X\n", retval);
            if (API_SUCCESS == retval)
            {
                for (i = 0U; i < APPL_OBEX_SER_NUM_INSTANCES; i++)
                {
                    APPL_OBEX_RESET_INSTANCE(&appl_obex_ser_inst[i]);
                }
            }

            /* init */
            sent        = 0U;
            remaining   = 0U;
            obj_size    = 0U;

            appl_obex_server_print_instances();
            break;

        case 3:
            printf ("Shutting down OBEX Server Appl...\n");
            retval = appl_obex_server_shutdown ();
            printf ("retval - 0x%04X\n", retval);

            appl_obex_server_print_instances();
            break;

        case 4:
            printf ("Enter OBEX Server Appl. Instance:");
            scanf ("%d", &choice);

            appl_obex_handle_ptr = &appl_obex_ser_inst[choice].obex_app_handle;

            printf ("Starting OBEX Server Appl. Instance....\n");
            retval = appl_obex_server_start
                     (
                         appl_obex_handle_ptr,
                         appl_obex_server_callback
                     );
            printf ("retval - 0x%04X\n", retval);
            if (API_SUCCESS != retval)
            {
                *appl_obex_handle_ptr = 0xFFU;
            }

            appl_obex_server_print_instances();
            break;

        case 5:
            printf ("Enter OBEX Server Appl. Instance:");
            scanf ("%d", &choice);

            appl_obex_handle_ptr = &appl_obex_ser_inst[choice].obex_app_handle;

            printf ("Starting OBEX Server Appl. Instance....\n");
            retval = appl_obex_server_stop (appl_obex_handle_ptr);
            printf ("retval - 0x%04X\n", retval);
            if (API_SUCCESS == retval)
            {
                /* Reset the pointer */
                appl_obex_ser_inst[(*appl_obex_handle_ptr)].obex_entity = NULL;

                *appl_obex_handle_ptr = 0xFFU;
            }

            appl_obex_server_print_instances();
            break;

        case 17:
            printf ("Enter OBEX Server Appl. Instance:");
            scanf ("%d", &choice);

            appl_obex_handle_ptr = &appl_obex_ser_inst[choice].obex_app_handle;

            printf ("Disconnecting from OBEX Client Transport... \n");
            retval = appl_obex_server_transport_disconnect
                     (
                         appl_obex_handle_ptr
                     );
            printf ("retval - 0x%04X\n", retval);
            break;

        default:
            printf("Invalid Option: %d\n", choice);
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

API_RESULT appl_obex_server_init (void)
{
    API_RESULT retval;
    UCHAR i;

    retval = API_SUCCESS;

    /* Check if already initialzed */
    if (BT_TRUE == appl_obex_ser_init)
    {
        printf ("[OBEX-APP]Obex Server Already Initialized\n");
        retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
    }
    else
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
            APPL_OBEX_SET_STATE(i, OBEX_SERVER_STATE_INITIALIZED);
            /* Set obex handle to invalide value */
            appl_obex_entities[i].obex_handle = OBEX_MAX_INSTANCES;
        }

        /* Set as initialized */
        appl_obex_ser_init = BT_TRUE;
    }

    return retval;
}

API_RESULT appl_obex_server_shutdown(void)
{
    API_RESULT retval;
    UCHAR i;

    /* Init */
    retval = API_SUCCESS;

    /* Check if initialzed or not */
    if (BT_FALSE == appl_obex_ser_init)
    {
        printf ("[OBEX-APP] Obex Server is not Initialized to Shutdown\n");
        retval = APPL_OBEX_INVALID_STATE; /* return APPL_OBEX_INVALID_STATE; */
    }
    else
    {

        /* Validate the state */
        for (i = 0U; i < APPL_OBEX_NUM_INSTANCES; i++)
        {
            if (OBEX_SERVER_STATE_INITIALIZED != appl_obex_entities[i].appl_state)
            {
                /* Entity i is active */
                retval = APPL_OBEX_INVALID_STATE; break; /*  return APPL_OBEX_INVALID_STATE; */
            }
        }

        if (API_SUCCESS == retval)
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
                APPL_OBEX_SET_STATE(i, OBEX_SERVER_STATE_INVALID);
            }

            /* Set as un-initialized */
            appl_obex_ser_init = BT_FALSE;
        }
    }

    return retval;
}

API_RESULT appl_obex_server_start
           (
               APPL_OBEX_HANDLE *obex_app_handle,
               OBEX_NOTIFY_CB obex_notify_cb
           )
{
    UINT8             handle;
    API_RESULT        retval;
    APPL_OBEX_ENTITY *obex_entity;
    OBEX_INIT_PARAMS  init_params;

    /* Init */
    retval = API_SUCCESS;

    /* Validate params */
    if ((NULL == obex_app_handle) ||
        (NULL == obex_notify_cb))
    {
        printf (
        "[OBEX-APP] Invalid Parameters to Start \n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        for (handle = 0U; handle < APPL_OBEX_NUM_INSTANCES; handle++)
        {
            /* Check if state is valid */
            if (OBEX_SERVER_STATE_INITIALIZED == appl_obex_entities[handle].appl_state)
            {
                obex_entity = &appl_obex_entities[handle];

                /* Initialize the entity */
                obex_entity->notify_cb  = obex_notify_cb;
                obex_entity->trans_len  = OBEX_MAX_PACKET_LENGTH;
                obex_entity->operation  = APPL_OBEX_OP_NONE;

#ifdef RFCOMM
                /* Assigning static RFCOMM channel for now. */
                init_params.channel_num = APPL_OBEX_SER_RFCOMM_CH;
#endif /* RFCOMM */

                init_params.max_rx_size = obex_entity->trans_len;
                init_params.obex_notify_cb = appl_obex_server_callback;

#ifdef OBEX_OVER_L2CAP
                init_params.l2cap_lpsm = APPL_OBEX_SER_L2CAP_PSM;
                init_params.l2cap_rpsm = APPL_OBEX_L2CAP_PSM; /* Dummy */
#endif /* OBEX_OVER_L2CAP */

                /* Register the Server and open the OBEX Session */
                retval = BT_obex_register_server
                         (
                             &obex_entity->obex_handle,
                             &init_params
                         );
                if(API_SUCCESS != retval)
                {
                    printf (
                    "[OBEX-APP: %d] OBEX Server Register Failed - 0x%04X\n",
                    handle, retval);
                }

                /* Update the state */
                APPL_OBEX_SET_STATE(handle, OBEX_SERVER_STATE_IDLE);

                /* Assign the allocated instance */
                *obex_app_handle = handle;

                appl_obex_ser_inst[handle].obex_entity  = obex_entity;

                /**
                 * Registered RFCOMM server channel & L2CAP psm
                 */
                printf ("  Registered:\n");
#ifdef RFCOMM
                printf ("    - RFCOMM ch.:0x%02x \n", init_params.channel_num);
#endif /* RFCOMM */
                printf ("    - L2CAP psm :0x%04x \n", init_params.l2cap_lpsm);

                break;
            }
        }

        if (APPL_OBEX_NUM_INSTANCES == handle)
        {
            printf ("[OBEX-APP: %d] No free OBEX Server instance to Sart\n", handle);
            retval = APPL_OBEX_ENTITY_START_FAILED;
        }
    }

    return retval;
}


API_RESULT appl_obex_server_stop
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    API_RESULT        retval;
    APPL_OBEX_HANDLE  handle;
    APPL_OBEX_ENTITY *obex_entity;

    /* Validate params */
    if ((NULL == obex_app_handle) ||
        (APPL_OBEX_NUM_INSTANCES <= (*obex_app_handle)))
    {
        printf (
        "[OBEX-APP] Invalid Parameters to Stop \n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        handle = *obex_app_handle;
        retval = API_SUCCESS;

        /* Get the entity for the given handle */
        obex_entity = &appl_obex_entities[handle];

        /* Check if the instance is Idle/Initialized */
        if ((OBEX_SERVER_STATE_INITIALIZED != obex_entity->appl_state) &&
            (OBEX_SERVER_STATE_IDLE != obex_entity->appl_state))
        {
            printf ("[OBEX-APP] Closing OBEX Transport on handle %d\n", handle);

            if (OBEX_SERVER_STATE_IN_TRANS_DISCONNECT == obex_entity->appl_state)
            {
                APPL_OBEX_SET_STATE(handle, OBEX_SERVER_STATE_STOP_PENDING);
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
                    APPL_OBEX_SET_STATE(handle, OBEX_SERVER_STATE_STOP_PENDING);

                    printf (
                    "[OBEX-APP] Transport Close Successful on Stop."
                    " Wait for confirmation.\n");
                }
            }

            printf (
            "[OBEX-APP] Obex Server Instance %d State Changed to 0x%02X\n",
            handle, obex_entity->appl_state);
        }

        if((OBEX_SERVER_STATE_STOP_PENDING != obex_entity->appl_state) &&
           (API_SUCCESS == retval))
        {
            printf (
            "[OBEX-APP] Stopping Obex Server Instance %d\n", handle);

            /* Reset the entity field if any */
            obex_entity->notify_cb = NULL;

            /* Unregister the Server */
            retval = BT_obex_unregister_server (&obex_entity->obex_handle);
            if(API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP: %d] OBEX Server Unregister Failed - 0x%04X\n",
                handle, retval);

                retval = APPL_OBEX_ENTITY_STOP_FAILED; /*  return APPL_OBEX_ENTITY_STOP_FAILED; */
            }
            else
            {
                /* Update the entity state */
                APPL_OBEX_SET_STATE(handle, OBEX_SERVER_STATE_INITIALIZED);
            }
        }
    }

    return retval;
}

API_RESULT appl_obex_server_send_rsp
           (
               APPL_OBEX_HANDLE      *obex_app_handle,
               UINT8                  event_type,
               UCHAR                  event_result,
               APPL_OBEX_RSP_STRUCT  *app_rsp_hdrs
           )
{
    API_RESULT        retval;
    APPL_OBEX_ENTITY *obex_entity;
    OBEX_OBJ_HEADER   tx_hdrs[OBEX_MAX_OBJECTS];
    UCHAR             app_handle;
    UCHAR             flag;
    UCHAR             num_tx_hdrs;
    UCHAR             skip_rsp_flag;

#ifdef OBEX_OVER_L2CAP
    UCHAR             status;
#endif /* OBEX_OVER_L2CAP */

    /* Init */
    retval      = API_SUCCESS;
    app_handle  = *obex_app_handle;
    num_tx_hdrs = 0U;
    skip_rsp_flag = 0U;

    if(APPL_OBEX_NUM_INSTANCES <= app_handle)
    {
        printf (
        "[OBEX-APP] Invalid  Parameters to responde\n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        obex_entity = &appl_obex_entities[app_handle];

        switch(event_type)
        {
        case OBEX_EVENT_PUT_IND: /* Fall Through */
        case OBEX_EVENT_PUT_FINAL_IND:
#ifdef OBEX_OVER_L2CAP
            if (OBEX_CONTINUE_RSP == event_result)
            {
                /*
                 * Check if OBEX SRM has been enabled by the peer
                 * device during this request
                 */
                retval = BT_obex_get_peer_srm_status
                         (
                             &obex_entity->obex_handle,
                             &status
                         );
                if ((API_SUCCESS == retval) &&
                    (OBEX_SRM_ENABLE == status))
                {
                    /* If peer has enabled, check if local is already enabled */
                    retval = BT_obex_get_local_srm_status
                             (
                                 &obex_entity->obex_handle,
                                 &status
                             );
                    if (API_SUCCESS != retval)
                    {
                        /* SRM is enabled in request, enable SRM in response  */
                        tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_SRM;
                        tx_hdrs[num_tx_hdrs].hv.byte = OBEX_SRM_ENABLE;
                        tx_hdrs[num_tx_hdrs].size = 1U;

                        num_tx_hdrs++;
                    }
                    else
                    {
                        /* Local has enabled too, check if the SRM is active */
                        retval = BT_obex_get_srm_status
                                 (
                                     &obex_entity->obex_handle,
                                     0x03U,
                                     &status
                                 );
                        if ((API_SUCCESS == retval) && (OBEX_SRM_ENABLE == status))
                        {
                            /* It is active, stay quiet and just receive */
                            if(OBEX_CONTINUE_RSP == event_result)
                            {
                                APPL_OBEX_SET_STATE
                                (
                                    app_handle,
                                    OBEX_SERVER_STATE_W4_REQ
                                );

                                skip_rsp_flag = 1U; /* return API_SUCCESS; */
                            }
                        }
                    }
                }
            }
#endif /* OBEX_OVER_L2CAP */

            if (0U == skip_rsp_flag)
            {
                flag = (OBEX_EVENT_PUT_IND == event_type)? 0x00U: 0x01U;

                /* Send OBEX Put response */
                retval = BT_obex_put_rsp
                         (
                             &obex_entity->obex_handle,
                             tx_hdrs,
                             num_tx_hdrs,
                             (UCHAR) event_result,
                             flag
                         );
                if (API_SUCCESS == retval)
                {
                    if(OBEX_CONTINUE_RSP == event_result)
                    {
                        APPL_OBEX_SET_STATE
                        (
                            app_handle,
                            OBEX_SERVER_STATE_W4_REQ
                        );
                    }
                    else
                    {
                        APPL_OBEX_SET_STATE
                        (
                            app_handle,
                            OBEX_SERVER_STATE_CONNECTED
                        );

                        obex_entity->operation = APPL_OBEX_OP_NONE;
                    }
                }
                else
                {
                    printf ("Failed to send PUT response \n");

                    /* Set to connected */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_SERVER_STATE_CONNECTED
                    );
                }
            }

            break;

        case OBEX_EVENT_GET_FINAL_IND:
#ifdef OBEX_OVER_L2CAP
            if (OBEX_CONTINUE_RSP == event_result)
            {
                /*
                 * Check if OBEX SRM has been enabled by the peer
                 * device during this request
                 */
                retval = BT_obex_get_peer_srm_status
                         (
                             &obex_entity->obex_handle,
                             &status
                         );
                if ((API_SUCCESS == retval) &&
                    (OBEX_SRM_ENABLE == status))
                {
                    /* If peer has enabled, check if local is already enabled */
                    retval = BT_obex_get_local_srm_status
                             (
                                 &obex_entity->obex_handle,
                                 &status
                             );
                    if (API_SUCCESS != retval)
                    {
                        /* SRM is enabled in request, enable SRM in response  */
                        tx_hdrs[num_tx_hdrs].hi = OBEX_HDR_SRM;
                        tx_hdrs[num_tx_hdrs].hv.byte = OBEX_SRM_ENABLE;
                        tx_hdrs[num_tx_hdrs].size = 1U;

                        num_tx_hdrs++;
                    }
                }
            }
#endif /* OBEX_OVER_L2CAP */

            tx_hdrs[num_tx_hdrs].hi =
            (OBEX_CONTINUE_RSP == event_result)? OBEX_HDR_BODY: OBEX_HDR_END_OF_BODY;
            tx_hdrs[num_tx_hdrs].hv.value = app_rsp_hdrs->body->value;
            tx_hdrs[num_tx_hdrs].size = app_rsp_hdrs->body->length;
            num_tx_hdrs++;

            /* Send OBEX Get Response */
            retval = BT_obex_get_rsp
                     (
                         &obex_entity->obex_handle,
                         tx_hdrs,
                         num_tx_hdrs,
                         (UCHAR) event_result,
                         0x01U
                     );
            if(API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP] Get Response in entity %d failed - 0x%04X\n",
                app_handle, retval);
            }
            else
            {
                 /* Update the state */
                if(OBEX_CONTINUE_RSP == event_result)
                {
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_SERVER_STATE_W4_REQ
                    );
                }
                else
                {
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_SERVER_STATE_CONNECTED
                    );
                    obex_entity->operation = APPL_OBEX_OP_NONE;
                }
            }

            break;

        default:
            printf("Invalid Event Type: 0x%02X\n", event_type);
            break;
        }
    }

    return retval;
}

/* OBEX application callback */
API_RESULT appl_obex_server_callback
           (
               OBEX_HANDLE * obex_handle,
               UINT16        event_result,
               UCHAR         event_type,
               void        * event_data,
               UINT16        event_datalen
           )
{
    API_RESULT retval;

    APPL_OBEX_ENTITY *obex_entity;

    UCHAR send_response;
    UINT16 max_tx_rx_size;
    UCHAR response_reject;

    OBEX_OBJ_HEADER obex_headers[OBEX_MAX_OBJECTS];
    OBEX_OBJ_HEADER tx_hdr;
    UCHAR num_tx_hdrs;
    UCHAR num_rx_hdrs;
    UCHAR num_req_hdrs;
    UCHAR i;

    APPL_OBEX_RSP_STRUCT app_rsp_hdrs;
    APPL_OBEX_HDR_STRUCT name_info, body_info;
#ifdef OBEX_ENH_SUPPORT
    APPL_OBEX_HDR_STRUCT dest_name_info;
    UINT32               permissions;
    UCHAR                action_id;
#endif /* OBEX_ENH_SUPPORT */
    UCHAR   app_handle;
    UCHAR   more;
    UCHAR  *data;
    UCHAR   tx_response;
    UCHAR   name_hdr_flag;
    UCHAR   setpath_flag;
    UINT16  length;

    retval = API_SUCCESS;
    num_rx_hdrs = 0U;
    num_tx_hdrs = 0U;
    num_req_hdrs = 0U;
    setpath_flag = 0x00U;

    tx_response = 0U; /* fix for warning: may be used uninitialized */

    more = 0U;
    name_hdr_flag = 0x00U;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    BT_mem_set(obex_headers, 0, sizeof(OBEX_OBJ_HEADER)*OBEX_MAX_OBJECTS);
    send_response = 0x00U;

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
        APPL_OBEX_INIT_HEADER_STRUCT(name_info);
        APPL_OBEX_INIT_HEADER_STRUCT(body_info);

#ifdef OBEX_ENH_SUPPORT
        APPL_OBEX_INIT_HEADER_STRUCT(dest_name_info);
        permissions = 0U;
        action_id = 0U;
#endif /* OBEX_ENH_SUPPORT */

        app_rsp_hdrs.body = &body_info;

        obex_entity = &appl_obex_entities[app_handle];

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

        /* Event processing */
        switch(event_type)
        {
        case OBEX_EVENT_TRANS_CONNECT_IND: /* Fall Through */
        case OBEX_EVENT_TRANS_CONNECT_CFM:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_TRANS_CONNECT_CFM == event_type)?
                "OBEX_EVENT_TRANS_CONNECT_CFM":
                "OBEX_EVENT_TRANS_CONNECT_IND", event_result);

            printf ("OBEX Handle - 0x%02X\n", app_handle);

            /* Is connection successful? */
            if(API_SUCCESS == event_result)
            {
                /* Update state of entity */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_SERVER_STATE_TRANS_CONNECTED
                );
            }

            /* appl_obex_server_print_instances(); */

            break;

        case OBEX_EVENT_TRANS_DISCONNECT_CFM:
        case OBEX_EVENT_TRANS_DISCONNECT_IND: /* Fall Through */
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_TRANS_DISCONNECT_CFM == event_type)?
                "OBEX_EVENT_TRANS_DISCONNECT_CFM":
                "OBEX_EVENT_TRANS_DISCONNECT_IND", event_result);

            printf ("OBEX Handle - 0x%02X\n", app_handle);

            /* Update the entity state */
            APPL_OBEX_SET_STATE
            (
                app_handle,
                OBEX_SERVER_STATE_IDLE
            );
            break;

        case OBEX_EVENT_OBEX_CONNECT_IND:

            printf ("Recvd OBEX_EVENT_OBEX_CONNECT_IND - 0x%04X\n", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if (OBEX_SERVER_STATE_TRANS_CONNECTED != obex_entity->appl_state)
            {
                printf ("Invalide state for Obex Connect Request\n");
                break;
            }

            response_reject = 0U;

            if (API_SUCCESS == event_result)
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
                    "[OBEX-APP: %d] OBEX Server Appl. Maximum Receive Length < %d\n",
                    app_handle, OBEX_MIN_PACKET_LENGTH);

                    /* Set flag to indicate Obex validation failure */
                    response_reject = 0x01U;
                }

                /* Update the entity state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_SERVER_STATE_IN_CONNECT
                );

                if (0U != response_reject)
                {
                    /* Send Obex connection reject response */
                    retval = BT_obex_connect_rsp
                             (
                                 &obex_entity->obex_handle,
                                 NULL,
                                 0U,
                                 OBEX_NOT_ACCEPTABLE_RSP
                             );
                    if(API_SUCCESS != retval)
                    {
                        printf (
                        "[OBEX-APP: %d] OBEX Connect Response failed - 0x%04X\n",
                        app_handle, retval);
                    }
                }
                else
                {
                    /*
                     *  If the Peer recv size is more than Local Recv size,
                     *  then set it to OBEX_MAX_PACKET_LENGTH. It should be always set
                     *  to the minimum of 'peer_recv_size' and 'OBEX_MAX_PACKET_LENGTH'.
                     */
                    if (max_tx_rx_size <obex_entity->trans_len)
                    {
                        obex_entity->trans_len = max_tx_rx_size;
                    }

                    /* Update Appl trans len, Assuming 64 bytes for Name Hdr */
                    /* TODO: Avoid this assumption and use BT_obex_get_available_frame_len */
                    obex_app_xchg_size = ((UINT32)obex_entity->trans_len - 64U);

                    /* Get the Remote BD address */
                    retval = BT_obex_get_remote_bd_addr
                             (
                                 obex_handle,
                                 obex_entity->bd_addr
                             );
                    if (API_SUCCESS != retval)
                    {
                        printf (
                        "[OBEX-APP: %d] Failed to get remote BD address - 0x%04X\n",
                        app_handle, retval);
                    }

                    BT_mem_copy
                    (
                        appl_obex_ser_inst[app_handle].bd_addr,
                        obex_entity->bd_addr,
                        BT_BD_ADDR_SIZE
                    );

                    retval = BT_obex_connect_rsp
                             (
                                 &obex_entity->obex_handle,
                                 NULL,
                                 0U,
                                 OBEX_SUCCESS_RSP
                             );
                    if (API_SUCCESS != retval)
                    {
                        printf (
                        "[OBEX-APP: %d] Failed to send connect response - 0x%04X\n",
                        app_handle, retval);
                    }

                    /* Update the entity state */
                    APPL_OBEX_SET_STATE
                    (
                        app_handle,
                        OBEX_SERVER_STATE_CONNECTED
                    );
                }
            }

            appl_obex_server_print_instances();
            break;

        case OBEX_EVENT_OBEX_DISCONNECT_IND:
            printf ("Recvd OBEX_EVENT_OBEX_DISCONNECT_IND - 0x%04X\n", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if (OBEX_SERVER_STATE_CONNECTED != obex_entity->appl_state)
            {
                printf (
                "[OBEX-APP:]Invalide state for Obex Disconnect Request\n");
                break;
            }

            /* Disconnect Requests are to be responded with SUCCESS */
            retval = BT_obex_disconnect_rsp
                     (
                         &obex_entity->obex_handle,
                         &tx_hdr,
                         num_tx_hdrs,
                         OBEX_SUCCESS_RSP
                     );
            if (API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP: %d] Failed to send disconnect response - 0x%04X\n",
                app_handle, retval);
            }

            /* Reset appl  bd-addr */
            BT_mem_set(&appl_obex_ser_inst[app_handle].bd_addr, 0x00, BT_BD_ADDR_SIZE);

            /* Update the entity state */
            APPL_OBEX_SET_STATE
                (app_handle, OBEX_SERVER_STATE_TRANS_CONNECTED);

            appl_obex_server_print_instances();
            break;

        case OBEX_EVENT_PUT_IND: /* Fall Through */
        case OBEX_EVENT_PUT_FINAL_IND:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_PUT_IND == event_type)?
                "OBEX_EVENT_PUT_IND":"OBEX_EVENT_PUT_FINAL_IND", event_result);

            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if ((OBEX_SERVER_STATE_CONNECTED != obex_entity->appl_state) &&
                (OBEX_SERVER_STATE_W4_REQ != obex_entity->appl_state))
            {
                printf (
                "[OBEX-APP: %d] Invalide state for PUT Request - 0x%04X\n",
                app_handle, retval);
                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else
            {
                for (i = 0U; i < num_rx_hdrs; i++)
                {
                    if(OBEX_HDR_NAME == obex_headers[i].hi)
                    {
                        name_info.value = obex_headers[i].hv.value;
                        name_info.length = obex_headers[i].size;

                        /* Update file name */
                        appl_push_fp = obex_headers[i].hv.value;

                        num_req_hdrs++;
                    }

                    /* Get the body headers, if any */
                    if((OBEX_HDR_BODY == obex_headers[i].hi) ||
                       (OBEX_HDR_END_OF_BODY == obex_headers[i].hi))
                    {
                        body_info.value = obex_headers[i].hv.value;
                        body_info.length = obex_headers[i].size;

                        num_req_hdrs++;
                    }
                }

                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_SERVER_STATE_IN_PUT
                );

                /* Enable Response */
                send_response = 0x01U;

                if (0U == obex_push_started)
                {
                    obex_push_started = 0x01U;

                    if ((NULL == name_info.value) ||
                        (0x00U == name_info.length))
                    {
                        /**
                         * object name is NULL, so rejecting
                         */
                        tx_response = OBEX_REQ_IS_BAD_RSP;
                        break;
                    }

                    /* Open file for PUT request */
                    if (NULL == obex_rx_fp)
                    {
                        retval = BT_fops_file_open
                                 (
                                     (UCHAR *)appl_push_fp,
                                     (UCHAR *)"wb",
                                     &obex_rx_fp
                                 );
                        if ((API_SUCCESS != retval) || (NULL == obex_rx_fp))
                        {
                            printf ("Failed to open PUSH Object file: %s\n", appl_push_fp);

                            /**
                             * Send Reject response
                             */
                            tx_response = OBEX_REQ_IS_BAD_RSP;

                            printf ("    Sending Bad Response\n");
                            break;
                        }
                    }
                }

                if ((NULL == body_info.value) ||
                    (0x00U == body_info.length))
                {
                    printf ("No Body content found \n");
                }
                else
                {
                    data   = body_info.value;
                    length = body_info.length;

                    /* Write to file */
                    if (NULL != obex_rx_fp)
                    {
                        (BT_IGNORE_RETURN_VALUE) BT_fops_file_write(data, length, obex_rx_fp, &actual);
                    }
                }

                if (OBEX_EVENT_PUT_FINAL_IND == event_type)
                {
                    if (NULL != obex_rx_fp)
                    {
                        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
                        (void)BT_fops_file_close (obex_rx_fp);
                        obex_rx_fp = NULL;
                    }

                    obex_push_started = 0x00U;
                }

                tx_response = (OBEX_EVENT_PUT_FINAL_IND == event_type)?
                              OBEX_SUCCESS_RSP:OBEX_CONTINUE_RSP;
            }

            break;

        case OBEX_EVENT_GET_FINAL_IND:
            printf ("Recvd OBEX_EVENT_PUT_FINAL_IND - 0x%04X\n", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if ((OBEX_SERVER_STATE_CONNECTED != obex_entity->appl_state) &&
                (OBEX_SERVER_STATE_W4_REQ != obex_entity->appl_state))
            {
                printf (
                "[OBEX-APP: %d] Invalide state for PUT Request - 0x%04X\n",
                app_handle, retval);

                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else
            {
                for (i = 0U; i < num_rx_hdrs; i++)
                {
                    if(OBEX_HDR_NAME == obex_headers[i].hi)
                    {
                        name_info.value = obex_headers[i].hv.value;
                        name_info.length = obex_headers[i].size;

                        /* update file name */
                        appl_pull_fp = obex_headers[i].hv.value;

                        num_req_hdrs++;
                    }
                }

                /* Update the state */
                APPL_OBEX_SET_STATE
                (
                    app_handle,
                    OBEX_SERVER_STATE_IN_GET
                );

                /* Enable Response */
                send_response = 0x01U;

                if (0U == sent)
                {
                    if ((NULL == name_info.value) ||
                        (0x00U == name_info.length))
                    {
                        /**
                         * object name is NULL, so rejecting
                         */
                        tx_response = OBEX_REQ_IS_BAD_RSP;
                        break;
                    }

                    /* Open file for GET request */
                    if (NULL == obex_tx_fp)
                    {
                        retval = BT_fops_file_open
                                 (
                                     (UCHAR *)appl_pull_fp,
                                     (UCHAR *)"rb",
                                     &obex_tx_fp
                                 );
                        if ((API_SUCCESS != retval) || (NULL == obex_tx_fp))
                        {
                            printf ("Failed to open PULL Object file: %s\n", appl_pull_fp);

                            /**
                             * Send Not Found Response
                             */
                            tx_response = OBEX_NOT_FOUND_RSP;

                            printf ("    Sending Not Found Response\n");

                            break;
                        }
                        else
                        {
                            /* Get the file size */
                            (BT_IGNORE_RETURN_VALUE) BT_fops_file_size(obex_tx_fp, &obj_size);
                            remaining = obj_size;
                        }
                    }
                }

                if(remaining > obex_app_xchg_size)
                {
                    body_info.length = (UINT16)obex_app_xchg_size;
                    more = 0x01U;

                    tx_response = OBEX_CONTINUE_RSP;
                }
                else if (0U != remaining)
                {
                    body_info.length = (UINT16)remaining;
                    more = 0x00U;

                    tx_response = OBEX_SUCCESS_RSP;
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

                /* Get data from file */
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
            }
            break;

        case OBEX_EVENT_SETPATH_IND:
            printf ("Recvd OBEX_EVENT_SETPATH_IND - 0x%04X\n", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if (OBEX_SERVER_STATE_CONNECTED != obex_entity->appl_state)
            {
                printf (
                "[OBEX-APP: %d] Invalide state for Setpath Request - 0x%04X\n",
                app_handle, retval);

                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else
            {
                for (i = 0U; i < num_rx_hdrs; i++)
                {
                    if(OBEX_HDR_NAME == obex_headers[i].hi)
                    {
                        name_info.value = obex_headers[i].hv.value;
                        name_info.length = obex_headers[i].size;

                        /* Name hdr is received */
                        name_hdr_flag = 0x01U;

                        num_req_hdrs++;
                    }
                }

                /* Get the Setpath flag sent by the client */
                (BT_IGNORE_RETURN_VALUE) BT_obex_get_setpath_flag
                (
                    obex_handle,
                    &setpath_flag
                );

                printf ("Setpath Flag: 0x%02x\n", setpath_flag);
                if (((0x00U == setpath_flag) ||
                     (0x02U == setpath_flag)) &&
                     ((0x01U == name_hdr_flag) &&
                      (NULL != name_info.value) &&
                      (0x00U != name_info.length)))
                {
                    /**
                     * Child or New folder
                     */
                    printf ("    Child folder: %s\n", name_info.value);
                }

                if ((0x03U == setpath_flag) &&
                    (0x00U == name_hdr_flag))
                {
                    printf ("    Parent Folder\n");
                }

                if ((0x02U == setpath_flag) &&
                    (0x01U == name_hdr_flag) &&
                    (0x00U == name_info.length))
                {
                    printf ("    Root folder\n");
                }

                /* Send Setpath response */
                retval = BT_obex_setpath_rsp
                         (
                             &obex_entity->obex_handle,
                             NULL,
                             0U,
                             OBEX_SUCCESS_RSP
                         );
                if(API_SUCCESS == retval)
                {
                    printf (
                    "[OBEX-APP: %d] Successfully sent Setpath Rsp \n", app_handle);
                }
                else
                {
                    printf (
                    "[OBEX-APP: %d] Failed to send Setpath Response. Reason 0x%04X \n",
                    app_handle, retval);
                }
            }
            break;

#ifdef OBEX_ENH_SUPPORT
        case OBEX_EVENT_ACTION_IND: /* Fall Through */
        case OBEX_EVENT_ACTION_FINAL_IND:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_ACTION_IND == event_type)?
                "OBEX_EVENT_ACTION_IND":"OBEX_EVENT_ACTION_FINAL_IND", event_result);

            printf ("OBEX Handle - 0x%02X\n", app_handle);

            if ((OBEX_SERVER_STATE_CONNECTED != obex_entity->appl_state) &&
                (OBEX_SERVER_STATE_W4_REQ != obex_entity->appl_state))
            {
                printf (
                "[OBEX-APP: %d] Invalide state for Action Request - 0x%04X\n",
                app_handle, retval);

                retval = API_FAILURE; /* return API_FAILURE; */
            }
            else
            {
                for (i = 0U; i < num_rx_hdrs; i++)
                {
                    if(OBEX_HDR_NAME == obex_headers[i].hi)
                    {
                        name_info.value = obex_headers[i].hv.value;
                        name_info.length = obex_headers[i].size;

                        num_req_hdrs++;
                    }

                    if(OBEX_HDR_DESTNAME == obex_headers[i].hi)
                    {
                        dest_name_info.value = obex_headers[i].hv.value;
                        dest_name_info.length = obex_headers[i].size;

                        num_req_hdrs++;
                    }

                    if(OBEX_HDR_ACTION_ID == obex_headers[i].hi)
                    {
                        action_id = obex_headers[i].hv.byte;
                        num_req_hdrs++;
                    }

                    if(OBEX_HDR_PERMISSIONS == obex_headers[i].hi)
                    {
                        permissions = obex_headers[i].hv.four_byte;
                    }
                }

                printf ("Action: ");
                if (0x00U == action_id)
                {
                    printf ("Copy\n");
                }
                else if (0x01U == action_id)
                {
                    printf ("Move/Rename \n");
                }
                else if (0x02U == action_id)
                {
                    printf ("Set permissions \n");
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                if (NULL != name_info.value)
                {
                    printf ("    Source Name: %s\n", name_info.value);
                }

                if ((0x00U == action_id) ||
                    (0x01U == action_id))
                {
                    if (NULL != dest_name_info.value)
                    {
                        printf ("    Destinaion Name: %s\n", dest_name_info.value);
                    }
                }

                if (0x02U == action_id)
                {
                   printf ("    Permisssions : 0x%08x\n", permissions);
                }

                /* Send Setpath response */
                retval = BT_obex_action_rsp
                         (
                             &obex_entity->obex_handle,
                             NULL,
                             0U,
                             OBEX_SUCCESS_RSP,
                             0x01U /* Final */
                         );
                if(API_SUCCESS == retval)
                {
                    printf (
                    "[OBEX-APP: %d] Successfully sent Action Rsp \n", app_handle);
                }
                else
                {
                    printf (
                    "[OBEX-APP: %d] Failed to send Action Response. Reason 0x%04X \n",
                    app_handle, retval);
                }
            }

            break;
#endif /* OBEX_ENH_SUPPORT */

        case OBEX_EVENT_GET_ABORT_IND: /* Fall Through */
        case OBEX_EVENT_PUT_ABORT_IND:
            printf ("Recvd %s - 0x%04X\n",
                (OBEX_EVENT_GET_ABORT_IND == event_type)?
                "OBEX_EVENT_GET_ABORT_IND":"OBEX_EVENT_PUT_ABORT_IND", event_result);
            printf ("OBEX Handle - 0x%02X\n", app_handle);

            /* Respond for the OBEX abort request */
            retval = BT_obex_abort_rsp
                     (
                         &obex_entity->obex_handle,
                         NULL,
                         0U,
                         OBEX_SUCCESS_RSP
                     );
            if(API_SUCCESS == retval)
            {
                printf (
                "[OBEX-APP: %d] Successfully sent Abort Rsp \n", app_handle);
            }
            else
            {
                printf (
                "[OBEX-APP: %d] Failed to send Abort Response. Reason 0x%04X \n",
                app_handle, retval);
            }

            /* Update the state */
            APPL_OBEX_SET_STATE
            (
                app_handle,
                OBEX_SERVER_STATE_CONNECTED
            );

            obex_entity->operation = APPL_OBEX_OP_NONE;
            break;

        default:
            printf("Invalid Event Type: 0x%02X\n", event_type);
            break;
        }

        if (1U == send_response)
        {
            retval = appl_obex_server_send_rsp
                     (
                         &obex_entity->obex_handle,
                         event_type,
                         tx_response,
                         &app_rsp_hdrs
                     );
            if(API_SUCCESS != retval)
            {
                printf (
                "[OBEX-APP: %d] appl_obex_client_send_req Failed\n", app_handle);
            }

            if (OBEX_EVENT_GET_FINAL_IND == event_type)
            {
                /* Update data length sent & remaining */
                sent += body_info.length;
                remaining = obj_size - sent;

                if (0U == remaining)
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

    BT_IGNORE_UNUSED_PARAM(more);
    return retval;
}

API_RESULT appl_obex_server_transport_disconnect
           (
               APPL_OBEX_HANDLE *obex_app_handle
           )
{
    API_RESULT        retval;
    APPL_OBEX_ENTITY *obex_entity;
    UCHAR             app_handle;

    retval     = API_SUCCESS;
    app_handle = *obex_app_handle;

    if (APPL_OBEX_NUM_INSTANCES <= app_handle)
    {
        printf (
        "[OBEX-APP] Invalid Parameters to Transport Disconnect \n");
        retval = APPL_OBEX_INVALID_PARAMETERS; /* return APPL_OBEX_INVALID_PARAMETERS; */
    }
    else
    {
        obex_entity = &appl_obex_entities[app_handle];

        /* Validate the state */
        if ((OBEX_SERVER_STATE_IDLE == obex_entity->appl_state) ||
            (OBEX_SERVER_STATE_INITIALIZED == obex_entity->appl_state) ||
            (OBEX_SERVER_STATE_INVALID == obex_entity->appl_state))
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
                    OBEX_SERVER_STATE_IN_TRANS_DISCONNECT
                );
            }
        }
    }

    return retval;
}


void appl_obex_server_print_instances (void)
{
    UINT8 i;

    printf ("\n\n==============================================\n");
    printf ("Instance\tBD Addr");
    printf ("\n------------------------------------------------\n\n");

    for (i = 0U; i < APPL_OBEX_SER_NUM_INSTANCES; i++)
    {
        printf ("  %d  \t\t", i);
        printf ("%02X:%02X:%02X:%02X:%02X:%02X\t ",
        appl_obex_ser_inst[i].bd_addr[0U], appl_obex_ser_inst[i].bd_addr[1U],
        appl_obex_ser_inst[i].bd_addr[2U], appl_obex_ser_inst[i].bd_addr[3U],
        appl_obex_ser_inst[i].bd_addr[4U], appl_obex_ser_inst[i].bd_addr[5U]);
    }

    printf ("\n-------------------------------------------------\n\n");
}

#endif /* OBEX_SERVER */
