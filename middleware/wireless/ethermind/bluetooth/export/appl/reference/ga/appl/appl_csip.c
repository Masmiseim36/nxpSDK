/**
 *  \file appl_csip.c
 *
 *  \brief This file defines the Coordinated Set Identification Profile:
 *  Application Data Structures and Methods.
 *  This application can be used by devices to be discovered as part of one
 *  or more Coordinated Sets.
 *  A Coordinated Set is defined as a group of devices that are configured to
 *  support a specific scenario. Examples of Coordinated Sets include
 *  a pair of hearing aids, a pair of earbuds, or a speaker set that receives
 *  multi-channel audio and that reacts to control commands in a
 *  coordinated way (e.g., volume up and volume down).
 *  The purpose of this application is to specify a mechanism to discover a
 *  Coordinated Set and its members, and to specify how a device can be
 *  discovered as part of one or more Coordinated Sets. It also specifies a
 *  way to grant exclusive access to the Coordinated Set to a client such that
 *  race conditions can be avoided when multiple clients want to access the
 *  Coordinated Set at the same time.
 *  Roles: CSIP Set Coordinator: Device that discovers a Coordinated Set and
 *                               its Set Members.
 *         CSIP Set Member: Device that is part of the Coordinated Set.
 *  GATT Requirement: CSIP Set Coordinator: GATT Client
 *                    CSIP Set Member: GATT Server
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_csip.h"
#include "appl_service.h"

#ifdef GA_CSIP

/* --------------------------------------------- Global Definitions */
/* CSIP Menu Operations - Set Coordinator */
static const char appl_csip_sc_options[] = " \n\
================ GA CSIP SET COORDINATOR MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Device Management ~ Setting ============ \n\
    2. Set the Peer Address \n\
\n\
================ Context Management ~ Setup ============ \n\
    4. CSIP Discover \n\
    5. CSIP Setup \n\
\n\
================ Context Management ~ Release ============ \n\
    6. CSIP Release/Close \n\
\n\
================ Profile Management (CSIS) ~ Operations(Read) ============ \n\
   10. Get SIRK \n\
   11. Get Size \n\
   12. Get Rank \n\
   13. Get Lock \n\
\n\
================ Profile Management (CSIS) ~ Operations(Write) ============ \n\
   20. Set Lock \n\
\n\
================ Context Management ~ Persistence ============ \n\
   80. Get Context Info. \n\
   81. Set Context Info. \n\
\n\
   87. Free Saved Context Info. \n\
\n\
================ Configuration Management ~ Notification/Indication Settings ===== \n\
   90. Configure Notification \n\
\n\
Your Option ?: ";

/* CSIP Menu Operations - Set Member */
static const char appl_csip_sm_options[] = " \n\
================ GA CSIP SET MEMBER MENU ================ \n\
    0. Exit \n\
    1. Refresh this Menu \n\
\n\
================ Device Management ~ Setting ============ \n\
    2. Set the Peer Address \n\
\n\
================ Service Management ~ Init ============ \n\
    3. CSIP Register \n\
\n\
================ Service Management ~ Operations(Notify) ============ \n\
   14. Set Read Response Status \n\
\n\
   21. Send 'Set Member Lock' Notification \n\
   22. Send 'Coordinated Set Size' Notification \n\
   23. Send 'Set Identity Resolving Key' Notification \n\
\n\
================ Service Management ~ Operations(SIRK Process) ============ \n\
   25. Enter the SIRK \n\
   26. Set Encrypt SIRK Required \n\
\n\
   30. Encrypt SIRK \n\
   31. Decrypt SIRK \n\
\n\
   40. Generate RSI \n\
   41. Verify RSI \n\
   42. Set Adv - RSI \n\
   43. Set Adv Params - Random Addr \n\
\n\
================ Service Management ~ SDP Record ============ \n\
   50. Activate SDP Record\n\
\n\
Your Option ?: ";

/* By default, Read Rsp status is Success */
DECL_STATIC UINT16  appl_csip_read_rsp_status = 0x00U;
/* By default, use Plain SIRK in Read Response */
DECL_STATIC UCHAR   appl_csip_encrypt_req = BT_FALSE;

DECL_STATIC GA_ENDPOINT         csip_device;
DECL_STATIC GA_CS_CONTEXT       appl_csip_context;
DECL_STATIC CSIP_MEMBER_HANDLE  csip_mhandle = 0xFF;
/** Service ID with the bearer */
GA_BRR_SVC_INST                 csis_id = 0xFF;
DECL_STATIC CSIP_SIRK           appl_csip_sirk;
DECL_STATIC UCHAR               rsi[6U];
DECL_STATIC CSIP_RSP_INFO       csip_response;
DECL_STATIC SMP_KEY_DIST        p_key_info;
DECL_STATIC UCHAR               appl_csip_size;
DECL_STATIC UCHAR               appl_csip_rank;
DECL_STATIC UCHAR               appl_csip_lock;

#ifdef CSIP_SUPPORT_CONTEXT_MANAGE
/* Variables to Save the Context Information */
DECL_STATIC  GA_BRR_SVC_RANGE  * appl_csip_csis_range_ptr;
DECL_STATIC  GA_CHAR_INFO      * appl_csip_csis_char_info_ptr;
DECL_STATIC  UINT16              appl_csip_csis_char_count;
DECL_STATIC  GA_BRR_SVC_RANGE    appl_csip_csis_range;
DECL_STATIC  GA_CHAR_INFO      * appl_csip_csis_char_info;
DECL_STATIC  GA_CS_CONTEXT       appl_csip_csis_handle_access;
#endif /* CSIP_SUPPORT_CONTEXT_MANAGE */

/*
 * To Validate if we are Initializing through GA_Init() in appl_ga.c
 * or appl_ga_legacy.c
 */
extern UINT8 ga_init_status;

/* --------------------------------------------- APIs */
/* Parser for Error Codes */
void appl_csip_display_ga_result(GA_RESULT status);

/*** APIs - CSIP Set Coordinator ***/

GA_RESULT appl_csip_sc_callback
          (
              UINT8    ga_event,
              UINT16   ga_status,
              void   * ga_data,
              UINT16   ga_datalen
          )
{
    GA_RESULT          retval;
    UCHAR              rsp_event;
    UINT16             rsp_status;
    UCHAR              i;
    GA_BRR_SVC_INFO  * svc;
    CSIP_SIRK        * sirk;

    GA_IGNORE_UNUSED_PARAM(rsp_event);
    GA_IGNORE_UNUSED_PARAM(rsp_status);

    /* Initialize */
    retval = GA_SUCCESS;
    rsp_event = GA_RSP_TYPE_INVALID;
    rsp_status = GA_SUCCESS;
    i = 0U;
    svc = NULL;
    sirk = NULL;

    APPL_TRC("[Event Status]: ");
    appl_csip_display_ga_result(ga_status);

    switch (ga_event)
    {
    case GA_CS_DISCOVER_CNF:
        {
            APPL_TRC
            (
                "[Event]       : GA_CS_DISCOVER_CNF (0x%02X)\n",
                ga_event
            );
            svc = (GA_BRR_SVC_INFO *)ga_data;
            if (NULL != svc)
            {
                GA_FORMAT_DISPLAY();
                appl_ga_utils_display_svc_uuid(svc->uuid);
                GA_FORMAT_DISPLAY();
                appl_ga_utils_display_svc_range(svc->range);
            }
            else
            {
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
                if (0U == ga_datalen)
                {
                    APPL_TRC("Data: NULL\n");
                }
            }
        }
        break;

    case GA_CS_SETUP_CNF:
        APPL_TRC
        (
            "[Event]       : GA_CS_SETUP_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_CS_RELEASE_CNF:
        APPL_TRC
        (
            "[Event]       : GA_CS_RELEASE_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    case GA_CS_SIRK_GET_CNF:
    case GA_CS_SIRK_NTF:
        {
            if (GA_CS_SIRK_GET_CNF == ga_event)
            {
                APPL_TRC
                (
                    "[Event]       : GA_CS_SIRK_GET_CNF (0x%02X)\n",
                    ga_event
                );
            }
            else
            {
                APPL_TRC
                (
                    "[Event]       : GA_CS_SIRK_NTF (0x%02X)\n",
                    ga_event
                );
            }

            sirk = (CSIP_SIRK *)ga_data;
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC
            (
                "Char UUID: Set Identity Resolving Key (0x%04X)\n",
                GA_CHAR_CSIS_SIRK
            );
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", ga_datalen);

            if ((CSIP_SIRK_LEN + 1U) == ga_datalen)
            {
                APPL_TRC("Data:\n");
                APPL_TRC("Len: 0x%02X,  Type: ", (UCHAR)sizeof(sirk->type));
                appl_csip_parse_and_display_sirk_type_value(sirk->type);
                APPL_TRC("Len: 0x%02X,  Value: ", (UCHAR)sizeof(sirk->value));
                for (i = 0U; i < CSIP_SIRK_LEN; i++)
                {
                    APPL_TRC("%02X ", sirk->value[i]);
                }
                APPL_TRC("\n");
            }
        }
        break;

    case GA_CS_SETSIZE_GET_CNF:
    case GA_CS_SETSIZE_NTF:

        if (GA_CS_SETSIZE_GET_CNF == ga_event)
        {
            APPL_TRC
            (
                "[Event]       : GA_CS_SETSIZE_GET_CNF (0x%02X)\n",
                ga_event
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : GA_CS_SETSIZE_NTF (0x%02X)\n",
                ga_event
            );
        }

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Coordinated Set Size (0x%04X)\n",
            GA_CHAR_CSIS_SIZE
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);

        if (1U == ga_datalen)
        {
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Coordinated Set Size: ", ga_datalen);
            if ((CSIP_SET_SIZE_MIN <= (*((UINT8*)ga_data))) &&
                (CSIP_SET_SIZE_MAX >= (*((UINT8*)ga_data))))
            {
                APPL_TRC("0x%02X\n", *((UINT8*)ga_data));
            }
            else
            {
                APPL_TRC("Prohibited (0x%02X)\n", *((UINT8*)ga_data));
            }
        }
        break;

    case GA_CS_SETMEMBER_RANK_GET_CNF:
        APPL_TRC
        (
            "[Event]       : GA_CS_SETMEMBER_RANK_GET_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Set Member Rank (0x%04X)\n",
            GA_CHAR_CSIS_RANK
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);

        if (1U == ga_datalen)
        {
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Set Member Rank: 0x%02X\n",
                ga_datalen, *((UINT8*)ga_data)
            );
        }
        break;

    case GA_CS_SETMEMBER_LOCK_STATUS_NTF: /* Fall - through */
    case GA_CS_SETMEMBER_LOCK_GET_CNF:
        if (GA_CS_SETMEMBER_LOCK_STATUS_NTF == ga_event)
        {
            APPL_TRC
            (
                "[Event]       : GA_CS_SETMEMBER_LOCK_STATUS_NTF (0x%02X)\n",
                ga_event
            );
        }
        else
        {
            APPL_TRC
            (
                "[Event]       : GA_CS_SETMEMBER_LOCK_GET_CNF (0x%02X)\n",
                ga_event
            );
        }
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Set Member Lock (0x%04X)\n",
            GA_CHAR_CSIS_LOCK
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);

        if (1U == ga_datalen)
        {
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Set Member Lock: ", ga_datalen);
            appl_csip_parse_and_display_set_mem_lock_value(*(UINT8*)ga_data);
        }
        break;

    case GA_CS_SETMEMBER_LOCK_SET_CNF:
        APPL_TRC
        (
            "[Event]       : GA_CS_SETMEMBER_LOCK_SET_CNF (0x%02X)\n",
            ga_event
        );
        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Set Member Lock (0x%04X)\n",
            GA_CHAR_CSIS_LOCK
        );
        GA_FORMAT_DISPLAY();
        APPL_TRC("Data Length: 0x%02X\n", ga_datalen);
        if (0U == ga_datalen)
        {
            APPL_TRC("Data: NULL\n");
        }
        break;

    default:
        APPL_ERR("UnHandled Event: 0x%02X !\n", ga_event);
        break;
    }

    return retval;
}

int appl_csip_sc_operations(void)
{
    GA_RESULT        retval;
    GA_BRR_SVC_INFO  svc_info;
    int              choice;
    int              menu_choice;
    int              input;
    UINT8            conn_dev_index;

    GA_IGNORE_UNUSED_PARAM(input);

    GA_LOOP_FOREVER()
    {
        APPL_TRC("\n");
        CONSOLE_OUT("%s", appl_csip_sc_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break;

        case 1:
            break;

        case 2:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &csip_device
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter peer BD Address: ");
                appl_get_bd_addr(csip_device.bd_addr);
            }
            break;

        case 4:
            retval = GA_cs_discover(&csip_device);
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 5:
            svc_info.uuid = GA_SERVICE_CSIS;
            CONSOLE_OUT("Enter the Start Handle[HEX] : ");
            CONSOLE_IN("%x", &choice);
            svc_info.range.start = (UINT16)choice;
            CONSOLE_OUT("Enter the End Handle[HEX] : ");
            CONSOLE_IN("%x", &choice);
            svc_info.range.end = (UINT16)choice;

            retval = GA_cs_setup(&csip_device, &svc_info, &appl_csip_context);
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            APPL_TRC("Context - 0x%02X\n", appl_csip_context);
            break;

        case 6:
        {
            CONSOLE_OUT("\n1. Release\n2. Close\n");
            CONSOLE_OUT("Enter your choice: ");
            CONSOLE_IN("%d", &choice);
            if (1 == choice)
            {
                APPL_TRC("GA_cs_release: \n");
                retval = GA_cs_release(&appl_csip_context);
                APPL_TRC("Retval - ");
                appl_csip_display_ga_result(retval);
            }
            else if (2 == choice)
            {
                APPL_TRC("GA_cs_close: \n");
                retval = GA_cs_close(&appl_csip_context);
                APPL_TRC("Retval - ");
                appl_csip_display_ga_result(retval);
            }
            else
            {
                APPL_ERR("Invalid choice !\n");
            }
        }
            break;

        case 10:
            retval = GA_cs_get_sirk(appl_csip_context);
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 11:
            retval = GA_cs_get_setsize(appl_csip_context);
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 12:
            retval = GA_cs_get_setmember_rank(appl_csip_context);
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 13:
            retval = GA_cs_get_setmember_lock(appl_csip_context);
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 20:
            CONSOLE_OUT("Enter lock state (1: Unlocked 2: Locked): ");
            CONSOLE_IN("%d", &choice);
            appl_csip_lock = (UCHAR)choice;
            retval = GA_cs_set_setmember_lock
                     (
                         appl_csip_context,
                         appl_csip_lock
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 80:
        {
#ifdef CSIP_SUPPORT_CONTEXT_MANAGE
            appl_csip_csis_handle_access = appl_csip_context;

            APPL_TRC("GA_cs_get_contex_info ...\n");
            retval = GA_cs_get_context_info
                     (
                         &csip_device,
                         &appl_csip_csis_handle_access,
                         &appl_csip_csis_range_ptr,
                         &appl_csip_csis_char_info_ptr,
                         &appl_csip_csis_char_count
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            if (GA_SUCCESS == retval)
            {
                APPL_TRC("appl_csip_csis_char_count:0x%02x\n", appl_csip_csis_char_count);

                appl_csip_csis_range = *appl_csip_csis_range_ptr;

                /**
                 *  Reference to allocate to save locally.
                 *  This can be written to persistent and retrieved
                 *  upon the next connection if bonded device
                 */
                appl_csip_csis_char_info = GA_alloc_mem(appl_csip_csis_char_count * sizeof(GA_CHAR_INFO));
                if (NULL != appl_csip_csis_char_info)
                {
                    GA_mem_copy
                    (
                        appl_csip_csis_char_info,
                        appl_csip_csis_char_info_ptr,
                        appl_csip_csis_char_count * sizeof(GA_CHAR_INFO)
                    );
                }
            }
#else /* CSIP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CSIP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CSIP_SUPPORT_CONTEXT_MANAGE */
        }
        break;

        case 81:
        {
#ifdef CSIP_SUPPORT_CONTEXT_MANAGE
            if (NULL == appl_csip_csis_char_info)
            {
                APPL_ERR("appl_csip_csis_char_info is NULL !\n");
                APPL_TRC("Call GA_cs_get_csis_contex_info \n");
                break;
            }

            appl_csip_csis_char_info_ptr = appl_csip_csis_char_info;
            appl_csip_csis_range_ptr = &appl_csip_csis_range;

            APPL_TRC("GA_cs_set_contex_info ...\n");
            retval = GA_cs_set_context_info
                     (
                         &csip_device,
                         &appl_csip_csis_handle_access,
                         &appl_csip_csis_range_ptr,
                         &appl_csip_csis_char_info_ptr,
                         &appl_csip_csis_char_count
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            if (GA_SUCCESS == retval)
            {
                appl_csip_context = appl_csip_csis_handle_access;

                /* Free the allocated memory */
                GA_free_mem(appl_csip_csis_char_info);
                appl_csip_csis_char_info = NULL;
            }
#else /* CSIP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CSIP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CSIP_SUPPORT_CONTEXT_MANAGE */
        }
            break;

        case 87:
#ifdef CSIP_SUPPORT_CONTEXT_MANAGE
            /**
             * This is to free up the allocated char. info. memory
             * If incase GA_cs_set_context_info() is NOT called after
             * GA_cs_get_context_info().
             */

            /* Free the allocated memory for CSIS char. info. */
            if (NULL != appl_csip_csis_char_info)
            {
                APPL_TRC("Freeing appl_csip_csis_char_info ...\n");
                GA_free_mem(appl_csip_csis_char_info);
                appl_csip_csis_char_info = NULL;
            }
#else /* CSIP_SUPPORT_CONTEXT_MANAGE */
            APPL_ERR("CSIP_SUPPORT_CONTEXT_MANAGE NOT defined !\n");
#endif /* CSIP_SUPPORT_CONTEXT_MANAGE */
            break;

        case 90:
        {
#ifdef CSIP_SUPPORT_CONFIG_SELECTION
            APPL_TRC("GA_cs_update_ntf_configuration...\n");
            retval = GA_cs_update_ntf_configuration
                     (
                        (/* CSIS_CHAR_CONFIG(CSIS_CHAR_ID_SIRK) |
                          CSIS_CHAR_CONFIG(CSIS_CHAR_ID_SET_SIZE) | */
                          CSIS_CHAR_CONFIG(CSIS_CHAR_ID_SET_MEMBER_LOCK))
                      );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
#else /* CSIP_SUPPORT_CONFIG_SELECTION */
            APPL_ERR("CSIP_SUPPORT_CONFIG_SELECTION NOT defined !\n");
#endif /* CSIP_SUPPORT_CONFIG_SELECTION */
        }
            break;

        default:
            APPL_INF("Invalid Option\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return 0;
}

/*** APIs - CSIP Set Member ***/

void appl_csip_sm_set_csis_sdp_record_status(UCHAR flag)
{
#if 0
    GA_RESULT  retval;
    UINT32      record_handle;

    /* Get the CSIS record handle */
    retval = BT_dbase_get_record_handle(DB_RECORD_CSIS, 0U, &record_handle);

    if (0U != flag)
    {
        retval = BT_dbase_activate_record(record_handle);
    }
    else
    {
        retval = BT_dbase_inactivate_record(record_handle);
    }

#else /* 0 */
    GA_IGNORE_UNUSED_PARAM(flag);
#endif /* 0 */

    return;
}

GA_RESULT appl_csip_sm_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT          retval;
    CSIP_EVENT_INFO  * csip_event;
    UCHAR              rsp_event;
    UINT16             rsp_status;
    UCHAR              i;
    DEVICE_HANDLE      bd_handle;
    BT_DEVICE_ADDR     dev;
    UCHAR              ltk;

    GA_IGNORE_UNUSED_PARAM(ga_datalen);
    GA_IGNORE_UNUSED_PARAM(rsp_event);

    /* Initialize */
    retval = GA_SUCCESS;
    rsp_event = GA_RSP_TYPE_INVALID;
    rsp_status = GA_SUCCESS;
    i = 0U;
    csip_event = NULL;

    APPL_TRC("[Event Status]: ");
    appl_csip_display_ga_result(ga_status);

    switch (ga_event)
    {
    case GA_CS_SIRK_GET_IND:
        APPL_TRC("[Event]       : GA_CS_SIRK_GET_IND (0x%02X)\n", ga_event);

        csip_event = (CSIP_EVENT_INFO *)ga_data;
        csip_response.context = csip_event->context;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Set Identity Resolving Key (0x%04X)\n",
            GA_CHAR_CSIS_SIRK
        );
        GA_FORMAT_DISPLAY();

        if (csip_mhandle == csip_event->handle)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", (UCHAR)sizeof(appl_csip_sirk));
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Type: ", (UCHAR)sizeof(appl_csip_sirk.type));
            appl_csip_parse_and_display_sirk_type_value(appl_csip_sirk.type);
            APPL_TRC("Len: 0x%02X,  Value: ", (UCHAR)sizeof(appl_csip_sirk.value));
            /* appl_dump_bytes((UCHAR *)appl_csip_sirk.value, CSIP_SIRK_LEN);*/
            for (i = 0U; i < CSIP_SIRK_LEN; i++)
            {
                APPL_TRC("%02X ", appl_csip_sirk.value[i]);
            }
            APPL_TRC("\n");

            if (appl_csip_encrypt_req)
            {
                GA_mem_copy(&dev.addr[0U], &csip_device.bd_addr[0U], 6U);
                dev.type = csip_device.bd_type;
                BT_smp_get_bd_handle(&dev, &bd_handle);

                /* TODO: Abstract it out */
                retval = BT_smp_get_device_keys
                         (
                             &bd_handle,
                             &ltk,
                             &p_key_info
                         );

                /*
                 * Encrypt SIRK using LTK, Send response for Read Req once SIRK
                 * is encrypted
                 */
                CSIP_sirk_process
                (
                    appl_csip_sirk.value,
                    p_key_info.enc_info,
                    1U,
                    appl_csip_sm_sirk_callback
                );
            }
            else
            {
                csip_response.data = &appl_csip_sirk;
                csip_response.datalen = sizeof(appl_csip_sirk);
                rsp_status = appl_csip_read_rsp_status;
            }
        }
        break;

    case GA_CS_SETSIZE_GET_IND:
        APPL_TRC("[Event]       : GA_CS_SETSIZE_GET_IND (0x%02X)\n", ga_event);

        csip_event = (CSIP_EVENT_INFO *)ga_data;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Coordinated Set Size (0x%04X)\n",
            GA_CHAR_CSIS_SIZE
        );
        GA_FORMAT_DISPLAY();

        if (csip_mhandle == csip_event->handle)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", (UCHAR)sizeof(appl_csip_size));
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Coordinated Set Size: ",
                (UCHAR)sizeof(appl_csip_size)
            );
            if ((CSIP_SET_SIZE_MIN <= appl_csip_size) &&
                (CSIP_SET_SIZE_MAX >= appl_csip_size))
            {
                APPL_TRC("0x%02X\n", appl_csip_size);
            }
            else
            {
                APPL_TRC("Prohibited (0x%02X)\n", appl_csip_size);
            }

            csip_response.context = csip_event->context;
            csip_response.data = &appl_csip_size;
            csip_response.datalen = sizeof(appl_csip_size);
        }
        break;

    case GA_CS_SETMEMBER_RANK_GET_IND:
        APPL_TRC
        (
            "[Event]       : GA_CS_SETMEMBER_RANK_GET_IND (0x%02X)\n",
            ga_event
        );

        csip_event = (CSIP_EVENT_INFO *)ga_data;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Set Member Rank (0x%04X)\n",
            GA_CHAR_CSIS_RANK
        );
        GA_FORMAT_DISPLAY();

        if (csip_mhandle == csip_event->handle)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", (UCHAR)sizeof(appl_csip_rank));
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Set Member Rank: 0x%02X\n",
                (UCHAR)sizeof(appl_csip_rank), appl_csip_rank
            );

            csip_response.context = csip_event->context;
            csip_response.data = &appl_csip_rank;
            csip_response.datalen = sizeof(appl_csip_rank);
        }
        break;

    case GA_CS_SETMEMBER_LOCK_GET_IND:
        APPL_TRC
        (
            "[Event]       : GA_CS_SETMEMBER_LOCK_GET_IND (0x%02X)\n",
            ga_event
        );

        csip_event = (CSIP_EVENT_INFO *)ga_data;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Set Member Lock (0x%04X)\n",
            GA_CHAR_CSIS_LOCK
        );
        GA_FORMAT_DISPLAY();

        if (csip_mhandle == csip_event->handle)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("FORMING RESPONSE DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", (UCHAR)sizeof(appl_csip_lock));
            APPL_TRC("Data:\n");
            APPL_TRC
            (
                "Len: 0x%02X,  Set Member Lock: ",
                (UCHAR)sizeof(appl_csip_lock)
            );
            appl_csip_parse_and_display_set_mem_lock_value(appl_csip_lock);

            csip_response.context = csip_event->context;
            csip_response.data = &appl_csip_lock;
            csip_response.datalen = sizeof(appl_csip_lock);
        }
        break;

    case GA_CS_SETMEMBER_LOCK_SET_IND:
        APPL_TRC
        (
            "[Event]       : GA_CS_SETMEMBER_LOCK_SET_IND (0x%02X)\n",
            ga_event
        );

        csip_event = (CSIP_EVENT_INFO *)ga_data;

        APPL_TRC("\n");
        GA_FORMAT_DISPLAY();
        APPL_TRC
        (
            "Char UUID: Set Member Lock (0x%04X)\n",
            GA_CHAR_CSIS_LOCK
        );
        GA_FORMAT_DISPLAY();

        if (NULL != csip_event)
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("RECEIVED DATA\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("Data Length: 0x%02X\n", csip_event->datalen);
            APPL_TRC("Data:\n");
            APPL_TRC("Len: 0x%02X,  Set Member Lock: ", csip_event->datalen);
            appl_csip_parse_and_display_set_mem_lock_value
            (
                *(UINT8 *)(csip_event->data)
            );

            if (csip_mhandle == csip_event->handle)
            {
                appl_csip_lock = *((UCHAR*)csip_event->data);
                APPL_TRC("\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("FORMING RESPONSE DATA\n");
                GA_FORMAT_DISPLAY();
                APPL_TRC("Data Length: 0x00\n");
                APPL_TRC("Data: NULL\n");

                csip_response.context = csip_event->context;
                csip_response.data = NULL;
                csip_response.datalen = 0U;
                rsp_status = (UINT16)appl_csip_read_rsp_status;
            }
        }
        break;

    default:
        APPL_ERR("UnHandled Event: 0x%02X !\n", ga_event);
        break;
    }

    if (csip_mhandle == csip_event->handle)
    {
        if ((NULL != csip_event) && (!appl_csip_encrypt_req))
        {
            APPL_TRC("\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("SENDING GA CSIP RESPONSE\n");
            GA_FORMAT_DISPLAY();
            APPL_TRC("API   : GA_cs_send_response\n");
            retval = GA_cs_send_response
                     (
                         device,
                         ga_event,
                         rsp_status,
                         &csip_response,
                         sizeof(csip_response)
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
        }
    }

    return retval;
}

void appl_csip_sm_sirk_callback(UCHAR * data, UINT16 datalen)
{
    APPL_TRC("SIRK Processed: %d", datalen);

    appl_dump_bytes(data, datalen);

    if (appl_csip_encrypt_req)
    {
        appl_csip_sirk.type = 0U;
        GA_mem_copy(&appl_csip_sirk.value[0U], data, 16U);
        csip_response.data = &appl_csip_sirk;
        csip_response.datalen = sizeof(appl_csip_sirk);

        GA_cs_send_response
        (
            &csip_device,
            GA_CS_SIRK_GET_IND,
            0U,
            &csip_response,
            sizeof(csip_response)
        );

        appl_csip_encrypt_req = BT_FALSE;
        /* Set it back to Plain */
        appl_csip_sirk.type = 1U;
    }

    return;
}

int appl_csip_sm_operations(void)
{
    GA_RESULT        retval;
    UCHAR            key[16U];
    int              choice;
    int              menu_choice;
    int              input;
    UINT8            conn_dev_index;

    GA_IGNORE_UNUSED_PARAM(input);

    GA_LOOP_FOREVER()
    {
        APPL_TRC("\n");
        CONSOLE_OUT("%s", appl_csip_sm_options);
        CONSOLE_IN("%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break;

        case 1:
            break;

        case 2:
            if (APPL_GA_INIT_STATUS_GA == ga_init_status)
            {
                conn_dev_index = appl_ga_utils_bap_get_input_conn_dev_index();
                /* Fetch Device from conn_dev_index */
                retval = appl_ga_utils_get_conn_device
                         (
                             conn_dev_index,
                             &csip_device
                         );
            }
            else if (APPL_GA_INIT_STATUS_LEGACY == ga_init_status)
            {
                CONSOLE_OUT("Enter peer BD Address: ");
                appl_get_bd_addr(csip_device.bd_addr);
            }
            break;

        case 3:
            CONSOLE_OUT("Enter SIRK Type (0 - Encrypted, 1 - Plain): ");
            CONSOLE_IN("%d", &choice);
            appl_csip_sirk.type = (UCHAR)choice;

            CONSOLE_OUT("Enter the 16 octet SIRK (Ex: 01 A3 ...): ");
            appl_input_octets(appl_csip_sirk.value, CSIP_SIRK_LEN);

            CONSOLE_OUT("Enter the Set Size: ");
            CONSOLE_IN("%d", &choice);
            appl_csip_size = (UCHAR)choice;

            CONSOLE_OUT("Enter the Rank: ");
            CONSOLE_IN("%d", &choice);
            appl_csip_rank = (UCHAR)choice;

            CONSOLE_OUT("Enter the Lock state:\n");
            CONSOLE_OUT("1. Unlocked\n");
            CONSOLE_OUT("2. Locked\n ");
            CONSOLE_IN("%d", &choice);
            appl_csip_lock = (UCHAR)choice;

            APPL_TRC("Registering CSIP Member...\n");
            retval = GA_cs_register
                     (
                         &csip_mhandle,
                         &csis_id
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            APPL_TRC
            (
                "Handle - 0x%02X\n",
                csip_mhandle
            );
            break;

        case 14:
            CONSOLE_OUT("Input the response status:\n");
            CONSOLE_OUT("Success[0x00]\n");
            CONSOLE_OUT("Lock Denied[0x80]\n");
            CONSOLE_OUT("Lock Release Not Allowed[0x81]\n");
            CONSOLE_OUT("Invalid Lock Value[0x82]\n");
            CONSOLE_OUT("Error OOB SIRK Only[0x83]\n");
            CONSOLE_OUT("Lock Already Granted[0x84]");
            CONSOLE_IN("%x", &choice);
            appl_csip_read_rsp_status = (UINT16)choice;
            break;

        case 21:
            CONSOLE_OUT("Enter the Lock state:\n");
            CONSOLE_OUT("1. Unlocked\n");
            CONSOLE_OUT("2. Locked\n ");
            CONSOLE_IN("%d", &choice);
            appl_csip_lock = (UCHAR)choice;
            APPL_TRC("GA_cs_send_setmember_lock_ntf..\n");
            retval = GA_cs_send_setmember_lock_ntf
                     (
                         &csip_device,
                         0U,
                         appl_csip_lock
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 22:
            APPL_TRC("GA_cs_send_coord_set_size_ntf..\n");
            retval = GA_cs_send_coord_set_size_ntf
                     (
                         &csip_device,
                         0U,
                         appl_csip_size
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 23:
            APPL_TRC("GA_cs_send_sirk_ntf..\n");
            retval = GA_cs_send_sirk_ntf
                     (
                         &csip_device,
                         0U,
                         &appl_csip_sirk,
                         sizeof(appl_csip_sirk)
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 25:
            CONSOLE_OUT("Enter the 16 octet SIRK (Ex: 01 A3 ...): ");
            appl_input_octets(appl_csip_sirk.value, CSIP_SIRK_LEN);
            break;

        case 26:
            CONSOLE_OUT("Enter Encrypt SIRK Required (1: Yes 0: No):\n");
            CONSOLE_IN("%d", &choice);
            appl_csip_encrypt_req = (UCHAR)choice;
            break;

        case 30:
            CONSOLE_OUT
            (
                "Enter the 16 octet Key tp Encrypt SIRK (Ex: 01 A3 ...): "
            );
            appl_input_octets(key, sizeof(key));
            retval = CSIP_sirk_process
                     (
                         appl_csip_sirk.value,
                         key,
                         1U,
                         appl_csip_sm_sirk_callback
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 31:
            CONSOLE_OUT
            (
                "Enter the 16 octet Key to decrypt SIRK (Ex: 01 A3 ...): "
            );
            appl_input_octets(key, sizeof(key));
            retval = CSIP_sirk_process
                     (
                         appl_csip_sirk.value,
                         key,
                         0U,
                         appl_csip_sm_sirk_callback
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 40:
            APPL_TRC("Generating RSI from SIRK...\n");
            retval = BT_smp_create_resolvable_pvt_addr(appl_csip_sirk.value);
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 41:
            CONSOLE_OUT("Enter the 6 octet RSI to be Verifed (Ex: 01 A3 ...): ");
            appl_input_octets(rsi, 6U);

            APPL_TRC("Verifying RSI with SIRK...\n");
            retval = BT_smp_verify_resolvable_pvt_addr
                     (
                         rsi,
                         appl_csip_sirk.value
                     );
            APPL_TRC("Retval - ");
            appl_csip_display_ga_result(retval);
            break;

        case 42:
            CONSOLE_OUT
            (
                "Enter the 6 octet RSI to be Advertised (Ex: 01 A3 ...): "
            );
            appl_input_octets(rsi, 6U);
            APPL_TRC("Set RSI as AD Type in ADV:\n");
            UCHAR adv_data[27U] = {
                          0x02, 0x01, 0x06,
                          0x05, 0x03, 0x0f, 0x18, 0x0a, 0x18,
                          0x09, 0x08, 0x01, 0x02, 0x03, 0x04, 0x01, 0x02, 0x03, 0x04,
                          0x07, 0x2e
            };

            GA_mem_copy(&adv_data[21U], rsi, 6U);
            retval = BT_hci_le_set_advertising_data
                     (
                         0x1B,
                         adv_data
                     );

            if (GA_SUCCESS != retval)
            {
                APPL_ERR("FAILED, Error code = %04X !\n", retval);
            }
            else
            {
                APPL_TRC("API returned success...\n");
            }
            break;

        case 43:
            appl_service_configure_adv(1U, HCI_ADV_IND, 1U, 0x00U, NULL, 0x00U);
            break;

        case 50:
            CONSOLE_OUT("Set SDP Record for CSIS: 0[Disable], 1[Enable]\n");
            CONSOLE_IN("%d", &input);
            appl_csip_sm_set_csis_sdp_record_status((UCHAR)input);
            break;

        default:
            APPL_INF("Invalid Option\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return 0;
}

/* CSIP Server Callback */
GA_RESULT appl_csip_server_callback
          (
              GA_ENDPOINT * device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )
{
    GA_RESULT  retval;

    /* Initialize */
    retval = GA_SUCCESS;

    if (NULL == device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("CSIP Server");

    APPL_TRC("[Profile]     : CSIS (0x%04X)\n", GA_SERVICE_CSIS);
    APPL_TRC("[SECTION]     : CSIS SET MEMBER EVENTS\n");

    switch(ga_event)
    {
        case GA_CS_SIRK_GET_IND: /* Fall - through */
        case GA_CS_SETSIZE_GET_IND: /* Fall - through */
        case GA_CS_SETMEMBER_RANK_GET_IND: /* Fall - through */
        case GA_CS_SETMEMBER_LOCK_GET_IND:
            APPL_TRC("[SUB-SECTION] : CSIS SET MEMBER-READ\n");
            APPL_TRC("[TYPE]        : READ REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_csip_sm_callback
                     (
                         device,
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;
        case GA_CS_SETMEMBER_LOCK_SET_IND:
            APPL_TRC("[SUB-SECTION] : CSIS SET MEMBER-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE REQUEST\n");
            appl_ga_utils_display_bdaddr_and_type(device);
            retval = appl_csip_sm_callback
                     (
                         device,
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        default:
            APPL_ERR("UnHandled CSIP Event: 0x%02X !\n", ga_event);
            break;
    }

    GA_CB_EXIT();

    return retval;
}

/* CSIP Client Callback */
GA_RESULT appl_csip_client_callback
          (
              GA_ENDPOINT * ga_device,
              UINT8         ga_event,
              UINT16        ga_status,
              void        * ga_data,
              UINT16        ga_datalen
          )

{
    GA_RESULT  retval;

    /* Initialize */
    retval = GA_SUCCESS;

    if (NULL == ga_device)
    {
        APPL_ERR("NULL Device Unexpected !\n");
        return GA_FAILURE;
    }

    GA_CB_ENTRY("CSIP Client");

    APPL_TRC("[Profile]     : CSIP (0x%04X)\n", GA_SERVICE_CSIS);
    APPL_TRC("[SECTION]     : CSIP SET COORDINATOR EVENTS\n");

    switch(ga_event)
    {
        case GA_CS_DISCOVER_CNF: /* Fall - through */
        case GA_CS_SETUP_CNF:
            APPL_TRC("[SUB-SECTION] : CSIP SET COORDINATOR-SETUP\n");
            if (GA_CS_DISCOVER_CNF == ga_event)
            {
                APPL_TRC("[TYPE]        : DISCOVER RESPONSE\n");
            }
            else
            {
                APPL_TRC("[TYPE]        : SETUP RESPONSE\n");
            }
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_csip_sc_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_CS_RELEASE_CNF:
            APPL_TRC("[SUB-SECTION] : CSIP SET COORDINATOR-RELEASE\n");
            APPL_TRC("[TYPE]        : RELEASE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_csip_sc_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_CS_SIRK_GET_CNF: /* Fall - through */
        case GA_CS_SETSIZE_GET_CNF: /* Fall - through */
        case GA_CS_SETMEMBER_RANK_GET_CNF: /* Fall - through */
        case GA_CS_SETMEMBER_LOCK_GET_CNF:
            APPL_TRC("[SUB-SECTION] : CSIP SET COORDINATOR-READ\n");
            APPL_TRC("[TYPE]        : READ RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_csip_sc_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_CS_SIRK_NTF: /* Fall - through */
        case GA_CS_SETSIZE_NTF: /* Fall - through */
        case GA_CS_SETMEMBER_LOCK_STATUS_NTF:
            APPL_TRC("[SUB-SECTION] : CSIP SET COORDINATOR-NOTIFICATION\n");
            APPL_TRC("[TYPE]        : NOTIFICATION\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_csip_sc_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        case GA_CS_SETMEMBER_LOCK_SET_CNF:
            APPL_TRC("[SUB-SECTION] : CSIP SET COORDINATOR-WRITE\n");
            APPL_TRC("[TYPE]        : WRITE RESPONSE\n");
            appl_ga_utils_display_bdaddr_and_type(ga_device);
            retval = appl_csip_sc_callback
                     (
                         ga_event,
                         ga_status,
                         ga_data,
                         ga_datalen
                     );
            break;

        default:
            APPL_ERR("UnHandled CSIP Event: 0x%02X !\n", ga_event);
            break;
    }

    GA_CB_EXIT();

    return retval;
}

/*** Common APIs - CSIP Set Coordinator and Set Member ***/

void appl_csip_parse_and_display_sirk_type_value(UINT8 sirk)
{
    if (CSIP_SIRK_TYPE_ENCRYPTED == sirk)
    {
        APPL_TRC("Encrypted SIRK (0x%02X)\n", CSIP_SIRK_TYPE_ENCRYPTED);
    }
    else if (CSIP_SIRK_TYPE_PLAIN_TEXT == sirk)
    {
        APPL_TRC("Plain text SIRK (0x%02X)\n",CSIP_SIRK_TYPE_PLAIN_TEXT);
    }
    else
    {
        /* Should not reach here */
        APPL_ERR("Undefined SIRK Type (0x%02X) !\n", sirk);
    }
    return;
}

void appl_csip_parse_and_display_set_mem_lock_value(UINT8 lock)
{
    if (CSIP_SET_MEMBER_LOCK_UNLOCKED == lock)
    {
        APPL_TRC("Unlocked (0x%02X)\n", CSIP_SET_MEMBER_LOCK_UNLOCKED);
    }
    else if (CSIP_SET_MEMBER_LOCK_LOCKED == lock)
    {
        APPL_TRC("Locked (0x%02X)\n", CSIP_SET_MEMBER_LOCK_LOCKED);
    }
    else
    {
        /* Should not reach here */
        APPL_ERR("Undefined Lock Value (0x%02X) !\n", lock);
    }

    return;
}

void appl_csip_display_ga_result(GA_RESULT status)
{
    GA_RESULT  retval;

    retval = GA_FAILURE;

    /* Checking for common GA_RESULT */
    retval = appl_ga_utils_display_ga_result(status);

    if (GA_FAILURE == retval)
    {
        switch (status)
        {
        /* Application error code */
        case CSIS_ERR_LOCK_DENIED:
            APPL_ERR("Lock Denied !");
            break;

        case CSIS_ERR_LOCK_RELEASE_NOT_ALLOWED:
            APPL_ERR("Lock Release Not Allowed !");
            break;

        case CSIS_ERR_INVALID_LOCK_VALUE:
            APPL_ERR("Invalid Lock Value !");
            break;

        case CSIS_ERR_OOB_SIRK_ONLY:
            APPL_ERR("OOB SIRK Only !");
            break;

        case CSIS_ERR_LOCK_ALREADY_GRANTED:
            APPL_ERR("Lock Already Granted !");
            break;

        /* Implementation related error codes */
        case GA_CSIP_INVALID_STATE:
            APPL_ERR("Invalid State !");
            break;

        case GA_CSIP_INVALID_PARAMS:
            APPL_ERR("Invalid Params !");
            break;

        case GA_CSIP_CNTX_ALLOC_FAILED:
            APPL_ERR("Context allocation failed !");
            break;

        case GA_CSIP_IN_BUSY_STATE:
            APPL_ERR("In Busy State!");
            break;

        case GA_CSIP_INVALID_HANDLE:
            APPL_ERR("Invalid Handle !");
            break;

        case GA_CSIP_OPERATION_NOT_SUPPORTED_BY_PEER_DEVICE:
            APPL_ERR("Operation not supported by Peer Device !");
            break;

        case GA_CSIP_SERVICE_ID_NOT_FOUND:
            APPL_ERR("Service ID not found !");
            break;

        default:
            APPL_ERR("Invalid Code !");
            break;
        }
        APPL_TRC(" (0x%04X)\n", status);
    }
    return;
}

#endif /* GA_CSIP */
