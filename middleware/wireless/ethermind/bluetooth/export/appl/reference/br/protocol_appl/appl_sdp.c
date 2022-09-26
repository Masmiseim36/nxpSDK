
/**
 *  \file appl_sdp.c
 *
 *  Source File for SDP Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_sdp.h"
#include "appl_utils.h"

#ifdef SDP
/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
static const char main_sdp_options[] = "\n\
========= SDP Menu =============== \n\
    0.  Exit. \n\
    1.  Refresh \n\
 \n\
   10. SDP Service Search Request. \n\
   11. SDP Service Attribute Request. \n\
   12. SDP Service Search Attribute Request. \n\
 \n\
  100.  SDP Open. \n\
  101.  SDP Close. \n\
 \n\
  200.  Register SDP Server Callback. \n\
Your Option ? ";


/* Application SDP Handle */
DECL_STATIC SDP_HANDLE appl_sdp_handle;

/* SDP UUIDs used in SS and SSA */
DECL_STATIC S_UUID     appl_sdp_uuids[APPL_SDP_MAX_UUIDS];
DECL_STATIC UINT128_ID appl_128_bit_uuids[APPL_SDP_MAX_UUIDS];
DECL_STATIC UINT16     appl_count_uuid_16, appl_count_uuid_32, appl_count_uuid_128;

/* SDP Record Handles used in SS */
DECL_STATIC UINT32     appl_sdp_rec_handles[APPL_SDP_MAX_REC_HANDLES];
DECL_STATIC UINT16     appl_sdp_num_handles;

/* SDP Attribute IDs and Ranges used in SA and SSA */
DECL_STATIC UINT16     appl_sdp_attr_ids[APPL_SDP_MAX_ATTR_IDS];
DECL_STATIC UINT32     appl_sdp_attr_id_ranges[APPL_SDP_MAX_ATTR_ID_RANGES];
DECL_STATIC UINT16     appl_sdp_num_attr_ids;
DECL_STATIC UINT16     appl_sdp_num_attr_id_ranges;

/* SDP Attribute Data Buffer, used in SA and SSA */
DECL_STATIC UCHAR      appl_sdp_attr_data_buf [APPL_SDP_ATTR_DATA_BUF_SIZE];
DECL_STATIC UINT16     appl_sdp_max_attr_data_len;

/* SDP Record Handle, used in SA */
DECL_STATIC UINT32     appl_sdp_record_handle;

/* Application SDP State */
DECL_STATIC UCHAR appl_sdp_state = APPL_SDP_STATE_IDLE;

/* Application Initiated Operation */
DECL_STATIC UCHAR appl_sdp_operation = APPL_SDP_OP_NONE;


/* ----------------------------------------- Functions */
void main_sdp_operations (void)
{
    int choice, menu_choice;

    BT_LOOP_FOREVER()
    {
        printf("%s", main_sdp_options);
        scanf("%d", &choice);
        menu_choice = choice;

        if (choice < 0)
        {
            printf("*** Invalid Choice. Try Again.\n");
            continue;
        }

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
            appl_ss_req();
            break;

        case 11:
            appl_sa_req();
            break;

        case 12:
            appl_ssa_req();
            break;

        case 100:
            appl_sdp_open();
            break;

        case 101:
            appl_sdp_close();
            break;

        case 200:
            appl_sdp_server_register_cb();
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


void appl_sdp_open (void)
{
    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;

    if (APPL_SDP_STATE_IDLE == appl_sdp_state)
    {
        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        BT_mem_set(bd_addr, 0, BT_BD_ADDR_SIZE);

        printf("Enter BD_ADDR: ");
        appl_get_bd_addr(bd_addr);
        SDP_SET_HANDLE(appl_sdp_handle, bd_addr, appl_sdp_callback);

        printf("Initiating SDP Open ... \n");
        APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_CONN);
        APPL_SET_SDP_OP (APPL_SDP_OP_OPEN);

        retval = BT_sdp_open (&appl_sdp_handle);
        if (API_SUCCESS != retval)
        {
            printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
            APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
        }
        else
        {
            printf("Succeeded !!\n");
            printf("Wait for SDP Open Event Indication.\n");
        }
    }
    else
    {
        printf ("SDP Busy (State 0x%02X). Wait for it to complete\n",
        appl_sdp_state);
    }
}


void appl_sdp_close (void)
{
    API_RESULT retval;

    if (APPL_SDP_STATE_IDLE == appl_sdp_state)
    {
        printf ("Please ensure SDP is connected\n");
    }
    else if (APPL_SDP_STATE_IN_CONN == appl_sdp_state)
    {
        printf ("SDP is in connection\n");
    }
    else if (APPL_SDP_STATE_IN_DISC == appl_sdp_state)
    {
        printf ("SDP is in disconnection\n");
    }
    else
    {
        printf("Initiating SDP Close ...\n");
        APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_DISC);

        retval = BT_sdp_close (&appl_sdp_handle);
        if (API_SUCCESS != retval)
        {
            printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);

            /* Failed. Reset the state to IDLE */
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
            APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
        }
        else
        {
            printf("Succeeded !!\n");
            printf("Wait for SDP Close Event Indication.\n");
        }
    }
}


void appl_ss_req(void)
{
    unsigned int read_val;

    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;

    /* Flag */
    UCHAR initiate_sdp_open;

    /* Init */
    retval = API_SUCCESS;
    initiate_sdp_open = 0x00U;

    /*
     * Application state shall be either Idle or SDP Connected.
     * If Idle then initite SDP Connection first.
     * If already SDP Connected, initiate SDP SS Request
     */
    switch (appl_sdp_state)
    {
    case APPL_SDP_STATE_IDLE: /* Fall Through */
    {
        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        BT_mem_set(bd_addr, 0, BT_BD_ADDR_SIZE);

        /* Create SDP Connection */
        printf("Enter BD_ADDR: ");
        appl_get_bd_addr(bd_addr);
        SDP_SET_HANDLE(appl_sdp_handle, bd_addr, appl_sdp_callback);
        APPL_SET_SDP_OP (APPL_SDP_OP_SS_REQ);

        initiate_sdp_open = 0x01U;
    }
    case APPL_SDP_STATE_CONNECTED:
    {
        printf("Enter the max no of Service Record Handles to be returned: \n");
        scanf("%u", &read_val);
        appl_sdp_num_handles = (UINT16)read_val;

        if (APPL_SDP_MAX_REC_HANDLES < appl_sdp_num_handles)
        {
            appl_sdp_num_handles = APPL_SDP_MAX_REC_HANDLES;
        }

        /* Get UUIDS */
        appl_sdp_get_uuids();

        APPL_SET_SDP_OP (APPL_SDP_OP_SS_REQ);
    }
    break;

    default:
    {
        printf ("Incorrect Application State 0x%02X. Returning.\n",
        appl_sdp_state);

        retval = API_FAILURE; /* return; */
    }
    }

    if (API_SUCCESS == retval)
    {
        if (0x01U == initiate_sdp_open)
        {
            printf("Initiating SDP Open ... \n");
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_CONN);

            retval = BT_sdp_open (&appl_sdp_handle);
            if (API_SUCCESS != retval)
            {
                printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
            }
            else
            {
                printf("Succeeded !!\n");
                printf("Wait for SDP Open Event Indication.\n");
            }
        }
        else
        {
            printf("Initiating SDP Service Search Request ....\n");
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_SS_REQ);

            retval = BT_sdp_servicesearchrequest
                     (
                         &appl_sdp_handle,
                         appl_sdp_uuids,
                         (UINT16) (appl_count_uuid_128 + appl_count_uuid_32 + appl_count_uuid_16),
                         appl_sdp_rec_handles,
                         &appl_sdp_num_handles
                     );
            if (API_SUCCESS != retval)
            {
                printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
            }
            else
            {
                printf("Succeeded !!\n");
                printf("Wait for SDP Service Search Event Indication.\n");
            }
        }
    }
}

void appl_sa_req(void)
{
    unsigned int read_val;

    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;

    /* Flag */
    UCHAR initiate_sdp_open;

    /* Init */
    initiate_sdp_open = 0x00U;
    retval = API_SUCCESS;

    /*
     * Application state shall be either Idle or SDP Connected.
     * If Idle then initite SDP Connection first.
     * If already SDP Connected, initiate SDP SS Request
     */
    switch (appl_sdp_state)
    {
    case APPL_SDP_STATE_IDLE: /* Fall Through */
    {
        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        BT_mem_set(bd_addr, 0, BT_BD_ADDR_SIZE);

        /* Create SDP Connection */
        printf("Enter BD_ADDR: ");
        appl_get_bd_addr(bd_addr);
        SDP_SET_HANDLE(appl_sdp_handle, bd_addr, appl_sdp_callback);
        APPL_SET_SDP_OP (APPL_SDP_OP_SA_REQ);

        initiate_sdp_open = 0x01U;
    }
    case APPL_SDP_STATE_CONNECTED:
    {
        printf("Enter the Service Record Handle : \n");
        scanf("%x", &read_val);
        appl_sdp_record_handle = (UINT32)read_val;

        /* Get Attribute IDs */
        appl_sdp_get_attr_ids();

        printf("Enter the max no of bytes to be returned : \n");
        scanf("%u", &read_val);
        appl_sdp_max_attr_data_len = (UINT16)read_val;

        if (APPL_SDP_ATTR_DATA_BUF_SIZE < appl_sdp_max_attr_data_len)
        {
            appl_sdp_max_attr_data_len = APPL_SDP_ATTR_DATA_BUF_SIZE;
        }
        APPL_SET_SDP_OP (APPL_SDP_OP_SA_REQ);
    }
    break;

    default:
    {
        printf ("Incorrect Application State 0x%02X. Returning.\n",
        appl_sdp_state);

        retval = API_FAILURE; /* return; */
    }
    }

    if (API_SUCCESS == retval)
    {
        if (0x01U == initiate_sdp_open)
        {
            printf("Initiating SDP Open ... \n");
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_CONN);

            retval = BT_sdp_open (&appl_sdp_handle);
            if (API_SUCCESS != retval)
            {
                printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
            }
            else
            {
                printf("Succeeded !!\n");
                printf("Wait for SDP Open Event Indication.\n");
            }
        }
        else
        {
            printf("Initiating SDP Service Attribute Request ....\n");
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_SA_REQ);

            retval = BT_sdp_serviceattributerequest
                     (
                         &appl_sdp_handle,
                         appl_sdp_record_handle,
                         appl_sdp_attr_ids,
                         appl_sdp_num_attr_ids,
                         appl_sdp_attr_id_ranges,
                         appl_sdp_num_attr_id_ranges,
                         appl_sdp_attr_data_buf,
                         &appl_sdp_max_attr_data_len
                     );
            if (API_SUCCESS != retval)
            {
                printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
            }
            else
            {
                printf("Succeeded !!\n");
                printf("Wait for SDP Service Search Event Indication.\n");
            }
        }
    }
}

void appl_ssa_req(void)
{
    unsigned int read_val;

    UCHAR bd_addr[BT_BD_ADDR_SIZE];
    API_RESULT retval;

    /* Flag */
    UCHAR initiate_sdp_open;

    /* Init */
    initiate_sdp_open = 0x00U;
    retval = API_SUCCESS;

    /*
     * Application state shall be either Idle or SDP Connected.
     * If Idle then initite SDP Connection first.
     * If already SDP Connected, initiate SDP SS Request
     */
    switch (appl_sdp_state)
    {
    case APPL_SDP_STATE_IDLE: /* Fall Through */
    {
        /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
        BT_mem_set(bd_addr, 0, BT_BD_ADDR_SIZE);

        /* Create SDP Connection */
        printf("Enter BD_ADDR: ");
        appl_get_bd_addr(bd_addr);
        SDP_SET_HANDLE(appl_sdp_handle, bd_addr, appl_sdp_callback);
        APPL_SET_SDP_OP (APPL_SDP_OP_SSA_REQ);

        initiate_sdp_open = 0x01U;
    }
    case APPL_SDP_STATE_CONNECTED:
    {
        /* Get UUIDs */
        appl_sdp_get_uuids();

        /* Get Attribute IDs */
        appl_sdp_get_attr_ids();

        printf("Enter the max no of bytes to be returned : \n");
        scanf("%u", &read_val);
        appl_sdp_max_attr_data_len = (UINT16)read_val;

        if (APPL_SDP_ATTR_DATA_BUF_SIZE < appl_sdp_max_attr_data_len)
        {
            appl_sdp_max_attr_data_len = APPL_SDP_ATTR_DATA_BUF_SIZE;
        }
        APPL_SET_SDP_OP (APPL_SDP_OP_SSA_REQ);
    }
    break;

    default:
    {
        printf ("Incorrect Application State 0x%02X. Returning.\n",
        appl_sdp_state);

        retval = API_FAILURE; /* return; */
    }
    }

    if (API_SUCCESS == retval)
    {
        if (0x01U == initiate_sdp_open)
        {
            printf("Initiating SDP Open ... \n");
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_CONN);

            retval = BT_sdp_open (&appl_sdp_handle);
            if (API_SUCCESS != retval)
            {
                printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
            }
            else
            {
                printf("Succeeded !!\n");
                printf("Wait for SDP Open Event Indication.\n");
            }
        }
        else
        {
            printf("Initiating SDP Service Search Attribute Request ....\n");
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_SSA_REQ);

            retval = BT_sdp_servicesearchattributerequest
                     (
                         &appl_sdp_handle,
                         appl_sdp_uuids,
                         (UINT16) (appl_count_uuid_128 + appl_count_uuid_32 + appl_count_uuid_16),
                         appl_sdp_attr_ids,
                         appl_sdp_num_attr_ids,
                         appl_sdp_attr_id_ranges,
                         appl_sdp_num_attr_id_ranges,
                         appl_sdp_attr_data_buf,
                         &appl_sdp_max_attr_data_len
                     );
            if (API_SUCCESS != retval)
            {
                printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
            }
            else
            {
                printf("Succeeded !!\n");
                printf("Wait for SDP Service Search Event Indication.\n");
            }
        }
    }
}


/* SDP Notification Callback */
void appl_sdp_callback
     (
         UCHAR     command,
         UCHAR *   data,
         UINT16    length,
         UINT16    status
     )
{
    UINT32 count;
    API_RESULT retval;

    switch(command)
    {
    case SDP_Open:
    {
        printf("SDP Open Return Value : %d\n", status);

        /* If status is success, check if further operation to be initiated. */
        if (API_SUCCESS == status)
        {
            switch (appl_sdp_operation)
            {
                case APPL_SDP_OP_OPEN:
                {
                    /* Do Nothing */
                    /* Reset State and Operation */
                    APPL_SET_SDP_STATE (APPL_SDP_STATE_CONNECTED);
                    APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
                }
                break;

                case APPL_SDP_OP_SS_REQ:
                {
                    /* Initiate Service Search Request */
                    APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_SS_REQ);

                    retval = BT_sdp_servicesearchrequest
                             (
                                 &appl_sdp_handle,
                                 appl_sdp_uuids,
                                 (UINT16) (appl_count_uuid_128 + appl_count_uuid_32 + appl_count_uuid_16),
                                 appl_sdp_rec_handles,
                                 &appl_sdp_num_handles
                             );
                    if (API_SUCCESS != retval)
                    {
                        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                        APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                        APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
                    }
                    else
                    {
                        printf("Succeeded !!\n");
                        printf("Wait for SDP Service Search Event Indication.\n");
                    }
                }
                break;

                case APPL_SDP_OP_SA_REQ:
                {
                    /* Initiate Service Attribute Request */
                    APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_SA_REQ);

                    retval = BT_sdp_serviceattributerequest
                             (
                                 &appl_sdp_handle,
                                 appl_sdp_record_handle,
                                 appl_sdp_attr_ids,
                                 appl_sdp_num_attr_ids,
                                 appl_sdp_attr_id_ranges,
                                 appl_sdp_num_attr_id_ranges,
                                 appl_sdp_attr_data_buf,
                                 &appl_sdp_max_attr_data_len
                             );
                    if (API_SUCCESS != retval)
                    {
                        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                        APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                        APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
                    }
                    else
                    {
                        printf("Succeeded !!\n");
                        printf("Wait for SDP Service Search Event Indication.\n");
                    }
                }
                break;

                case APPL_SDP_OP_SSA_REQ:
                {
                    /* Initiate Service Search Attribute Request */
                    APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_SSA_REQ);

                    retval = BT_sdp_servicesearchattributerequest
                             (
                                 &appl_sdp_handle,
                                 appl_sdp_uuids,
                                 (UINT16) (appl_count_uuid_128 + appl_count_uuid_32 + appl_count_uuid_16),
                                 appl_sdp_attr_ids,
                                 appl_sdp_num_attr_ids,
                                 appl_sdp_attr_id_ranges,
                                 appl_sdp_num_attr_id_ranges,
                                 appl_sdp_attr_data_buf,
                                 &appl_sdp_max_attr_data_len
                             );
                    if (API_SUCCESS != retval)
                    {
                        printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);
                        APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
                        APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
                    }
                    else
                    {
                        printf("Succeeded !!\n");
                        printf("Wait for SDP Service Search Event Indication.\n");
                    }
                }
                break;

            default:
                printf("Invalid SDP Opration: %d\n", appl_sdp_operation);
                break;
            }
        }
        else
        {
            /* Reset State and Operation */
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
            APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
        }
    }
    break;

    case SDP_Close:
    {
        printf("SDP Close Return Value : %d\n", status);

        /* Reset State and Operation */
        APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
        APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
    }
    break;

    case SDP_ErrorResponse:
    {
        printf("Error Response Recieved\n");

        /* Reset State and Operation */
        APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
        APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
    }
    break;

    case SDP_ServiceSearchResponse:
    {
        if (API_SUCCESS == status)
        {
            printf("Service Search Request Successful\n");
            printf("Number of Record Handles Received : %d\n", length);

            /* Print SDP Record Handles */
            appl_sdp_print_record_handles (data, length);
        }

        APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_DISC);

        retval = BT_sdp_close (&appl_sdp_handle);
        if (API_SUCCESS != retval)
        {
            printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);

            /* Failed. Reset the state to IDLE */
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
            APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
        }
        else
        {
            printf("Succeeded !!\n");
            printf("Wait for SDP Close Event Indication.\n");
        }
    }
    break;

    case SDP_ServiceAttributeResponse:
    {
        if (API_SUCCESS == status)
        {
            printf("Service Attribute Request Successful.\n");
            printf("No. of Data Bytes Received : %d\n", length);
            printf("Attribute ID List is:\n");
            for(count = 0U; count < length; count ++)
            {
                printf("0x%02X  ", data[count]);
            }
            printf("\n");
        }

        APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_DISC);

        retval = BT_sdp_close (&appl_sdp_handle);
        if (API_SUCCESS != retval)
        {
            printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);

            /* Failed. Reset the state to IDLE */
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
            APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
        }
        else
        {
            printf("Succeeded !!\n");
            printf("Wait for SDP Close Event Indication.\n");
        }
    }
    break;

    case SDP_ServiceSearchAttributeResponse:
    {
        if (API_SUCCESS == status)
        {
            printf("Service Search Attribute Request Successful.\n");
            printf("No. of Data Bytes Received : %d\n", length);
            printf("Attribute ID List is:\n");
            for(count = 0U; count < length; count ++)
            {
                printf("0x%02X  ", data[count]);
            }
            printf("\n");
        }

        APPL_SET_SDP_STATE (APPL_SDP_STATE_IN_DISC);

        retval = BT_sdp_close (&appl_sdp_handle);
        if (API_SUCCESS != retval)
        {
            printf("** FAILED ** !! Reason Code = 0x%04x\n", retval);

            /* Failed. Reset the state to IDLE */
            APPL_SET_SDP_STATE (APPL_SDP_STATE_IDLE);
            APPL_SET_SDP_OP (APPL_SDP_OP_NONE);
        }
        else
        {
            printf("Succeeded !!\n");
            printf("Wait for SDP Close Event Indication.\n");
        }
    }
    break;

    default: /* Dont Know What happens here */
        printf("*****UnIdentified Event\n");
        break;
    }/* switch */
}


/* User Inputs */
void appl_sdp_get_uuids(void)
{
    UINT32 read_val, uuid;
    UINT32 count;

    printf("Enter the no of 16 bit UUID's\n");
    scanf("%u", &read_val);

    if (APPL_SDP_MAX_UUIDS < read_val)
    {
        read_val = APPL_SDP_MAX_UUIDS;
    }

    for(appl_count_uuid_16 = 0U; appl_count_uuid_16 < read_val; appl_count_uuid_16++)
    {
         printf("Enter the 16 bit UUID (in Hex) \n");
         scanf("%x",&uuid);

         appl_sdp_uuids[appl_count_uuid_16].uuid_type = UUID_16;
         appl_sdp_uuids[appl_count_uuid_16].uuid_union.uuid_16 = (UINT16)uuid;
    }

    printf("Enter the no of 32 bit UUID's\n");
    scanf("%u", &read_val);

    if ((APPL_SDP_MAX_UUIDS - appl_count_uuid_16) < read_val)
    {
        read_val = APPL_SDP_MAX_UUIDS - (UINT32)appl_count_uuid_16;
    }

    for(appl_count_uuid_32 = 0U; appl_count_uuid_32 < read_val; appl_count_uuid_32++)
    {
         printf("Enter the 32 bit UUID (in Hex) \n");
         scanf("%u",&uuid);

         appl_sdp_uuids[appl_count_uuid_16 + appl_count_uuid_32].uuid_type = UUID_32;
         appl_sdp_uuids[appl_count_uuid_16 + appl_count_uuid_32].uuid_union.uuid_32 = (UINT32)uuid;
    }

    printf("Enter the no of 128  bit UUID's\n");
    scanf("%u",&read_val);

    if ((APPL_SDP_MAX_UUIDS - (appl_count_uuid_16 + appl_count_uuid_32)) < read_val)
    {
        read_val = APPL_SDP_MAX_UUIDS - ((UINT32)appl_count_uuid_16 + appl_count_uuid_32);
    }

    for(appl_count_uuid_128 = 0U; appl_count_uuid_128 < read_val; appl_count_uuid_128++)
    {
         printf("Enter the 128 bit UUID (in Hex) \n");
         for(count = 0U; count < 16U; count++)
         {
               scanf("%x",&uuid);
               appl_128_bit_uuids[appl_count_uuid_128].byte[count] = (UCHAR)uuid;
         }
         appl_sdp_uuids[appl_count_uuid_16 + appl_count_uuid_32 + appl_count_uuid_128].uuid_type = UUID_128;
         appl_sdp_uuids[appl_count_uuid_16 + appl_count_uuid_32 + appl_count_uuid_128].uuid_union.uuid_128 =
                                           appl_128_bit_uuids[appl_count_uuid_128];
    }
}


void appl_sdp_get_attr_ids(void)
{
    unsigned int read_val;
    UINT32 count;

    printf("Enter the no of Attribute ID's to be queried\n");
    scanf("%u",&read_val);
    appl_sdp_num_attr_ids = (UINT16)read_val;

    if (APPL_SDP_MAX_ATTR_IDS < appl_sdp_num_attr_ids)
    {
        appl_sdp_num_attr_ids = APPL_SDP_MAX_ATTR_IDS;
    }

    for(count = 0U; count < appl_sdp_num_attr_ids; count++)
    {
        printf("Enter the Attribute ID\n");
        scanf("%x",&read_val);
        appl_sdp_attr_ids[count] = (UINT16)read_val;
    }

    printf("Enter the no of Attribute Ranges to be queried\n");
    scanf("%u",&read_val);
    appl_sdp_num_attr_id_ranges = (UINT16)read_val;

    if (APPL_SDP_MAX_ATTR_ID_RANGES < appl_sdp_num_attr_id_ranges)
    {
        appl_sdp_num_attr_id_ranges = APPL_SDP_MAX_ATTR_ID_RANGES;
    }

    for(count = 0U; count < appl_sdp_num_attr_id_ranges; count++)
    {
        printf("Enter the Attribute Range\n");
        scanf("%x",&read_val);
        appl_sdp_attr_id_ranges[count] = (UINT32)read_val;
    }
}


/*
 * Print SDP Record Handles recevied in Service Search Response
 */
void appl_sdp_print_record_handles
     (
         /* IN */ UCHAR * buffer,
         /* IN */ UINT16 no_of_rec_hdl
     )
{
    UINT16 index;
    UINT32 *handle;

    /* In the SS response, EtherMind SDP Client shares already decoded SDP Record Handles */
    handle = (UINT32 *)buffer;

    for(index = 0U; index < no_of_rec_hdl ; index++)
    {
        printf("Record Handle[%d] = 0x%08X\n", index, handle[index]);
    }

    return;
}

#ifdef SDP_SERVER_HAVE_CB_IND_SUPPORT
API_RESULT appl_sdp_server_ind
           (
               UCHAR    event_type,
               UCHAR *  event_data,
               UCHAR    event_datalen
           )
{
    switch (event_type)
    {
        case SDP_SERVER_CONNECT_IND:
            printf("[APPL SDP SERVER CB] SERVER CONNECT IND. Peer Address \n");
            appl_dump_bytes(event_data, event_datalen);
            break;

        case SDP_SERVER_DISCONNECT_IND:
            printf("[APPL SDP SERVER CB] SERVER DISCONNECT IND. Peer Address \n");
            appl_dump_bytes(event_data, event_datalen);
            break;

        default:
            printf("Invalid Event Type: 0x%02X\n", event_type);
            break;
    }

    return API_SUCCESS;
}

void appl_sdp_server_register_cb(void)
{
    (BT_IGNORE_RETURN_VALUE) BT_sdp_server_register_ind_cb
    (
        appl_sdp_server_ind
    );
}
#endif /* SDP_SERVER_HAVE_CB_IND_SUPPORT */

/*
 * Update Service Handle Range in SDP record
 * and activate SDP record.
 */
API_RESULT appl_update_gatt_service_range_in_sdp_record
            (
                /* IN */ UCHAR  service_type,
                /* IN */ UINT16 start_handle,
                /* IN */ UINT16 end_handle
            )
{
    UINT32 record_handle;
    API_RESULT retval;

    /* local variable to extract the supported features */
    UCHAR protocol_desc_list[] = {
                                     0x35U, 0x13U, 0x35U, 0x06U, 0x19U,

                                     /* UUID L2CAP */
                                     0x01U, 0x00U, 0x09U,

                                     /* PSM = ATT */
                                     0x00U, 0x1FU, 0x35U, 0x09U, 0x19U,

                                     /* UUID ATT */
                                     0x00U, 0x07U, 0x09U,

                                     /* Start Handle */
                                     0x00U, 0x00U, 0x09U,

                                     /* End Handle */
                                     0x00U, 0x00U
                                 };

    /* Get Record Handle from SDP Database */
    retval = BT_dbase_get_record_handle (service_type, 0U, &record_handle);

    if (API_SUCCESS == retval)
    {
        /* Update Start Handle */
        BT_PACK_BE_2_BYTE_VAL(&protocol_desc_list[16U], start_handle);

        /* Update End Handle */
        BT_PACK_BE_2_BYTE_VAL(&protocol_desc_list[19U], end_handle);

        retval = BT_dbase_update_attr_value
                 (
                     record_handle,
                     PROTOCOL_DESC_LIST,
                     protocol_desc_list,
                     sizeof (protocol_desc_list)
                 );

        if (API_SUCCESS == retval)
        {
            retval = BT_dbase_activate_record(record_handle);
        }
    }
    return retval;
}

#endif /* SDP */
