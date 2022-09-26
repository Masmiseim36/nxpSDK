
/**
 *  \file appl_did_client.c
 *
 *  Source File for DID Client Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_did.h"
#include "appl_utils.h"


#ifdef DID_CLIENT
/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
/* HCI ACL Connection List */
static HCI_CONNECTION_LIST did_appl_acl_list [BT_MAX_REMOTE_DEVICES];

static SDP_HANDLE          sdp_did_handle;
static UCHAR               did_attrib_data[1024U];
static UINT16              did_len_attrib_data;


static const char did_client_menu[] = "\n\
========= Device ID Menu ================ \n\
    0. Exit \n\
    1. Refresh \n\
 \n\
   ====== Device ID Client Options ====== \n\
   10. Get DID Record from Peer \n\
Your Option ? ";

/* ----------------------------------------- Functions */

static INT32 appl_debug_dump_bytes ( UCHAR *buffer, UINT32 length )
{
    char hex_stream[49U];
    char char_stream[17U];
    UINT32 i;
    UINT16 offset, count;
    UCHAR c;

    BT_debug_dump(BT_MODULE_BIT_MASK_DID, "\n");
    BT_debug_dump(BT_MODULE_BIT_MASK_DID, "-- Dumping %d Bytes --\n",
    (int)length);

    BT_debug_dump(BT_MODULE_BIT_MASK_DID,
    "-------------------------------------------------------------------\n");

    count = 0U;
    offset = 0U;
    for(i = 0U; i < length; i ++)
    {
        c =  buffer[i];
        BT_str_print(&hex_stream[offset], "%02X ", c);

        if ( (c >= 0x20U) && (c <= 0x7EU) )
        {
            char_stream[count] = c;
        }
        else
        {
            char_stream[count] = '.';
        }

        count ++;
        offset += 3U;

        if(16U == count)
        {
            char_stream[count] = '\0';
            count = 0U;
            offset = 0U;

            BT_debug_dump(BT_MODULE_BIT_MASK_DID, "%s   %s\n",
            hex_stream, char_stream);

            BT_mem_set(hex_stream, 0, 49U);
            BT_mem_set(char_stream, 0, 17U);
        }
    }

    if(offset != 0U)
    {
        char_stream[count] = '\0';

        /* Maintain the alignment */
        BT_debug_dump(BT_MODULE_BIT_MASK_DID, "%-48s   %s\n",
        hex_stream, char_stream);
    }

    BT_debug_dump(BT_MODULE_BIT_MASK_DID,
    "-------------------------------------------------------------------\n");

    BT_debug_dump(BT_MODULE_BIT_MASK_DID, "\n");

    return 0U;
}


static void appl_did_print_attributes(UCHAR *data, UINT16 length)
{
    UCHAR      *attribute_data;
    API_RESULT  retval;
    UINT16      two_byte_value;
    UINT32      record_handle;
    UINT16      attribute_len;
    UCHAR       one_byte_value;
    UCHAR       string_attr[256U];
    UCHAR       index;
    UCHAR       record_no;

    BT_IGNORE_UNUSED_PARAM(length);

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    record_handle = 0U;
    two_byte_value = 0U;
    one_byte_value = 0U;

    /* Extract Record Handle */
    record_no = 1U;

    while((attribute_data = BT_sdp_get_Pointer_to_next_AttributeList
                            (
                                data,
                                record_no
                            )) != NULL)
    {
        LOG_DEBUG("\n-------- DID Record [%d] --------\n", record_no);

        attribute_len = 4U;
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x0000U,
                     attribute_data,
                     (UCHAR *)&record_handle,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            printf("DID Record Handle : 0x%08X\n", record_handle);
        }

        attribute_len = 2U;
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x0200U,
                     attribute_data,
                     (UCHAR *)&two_byte_value,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            printf("DID Specification ID : 0x%04X\n", two_byte_value);
        }

        attribute_len = 2U;
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x0201U,
                     attribute_data,
                     (UCHAR *)&two_byte_value,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            printf("DID Vendor ID : 0x%04X\n", two_byte_value);
        }

        attribute_len = 2U;
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x0202U,
                     attribute_data,
                     (UCHAR *)&two_byte_value,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            printf("DID Product ID : 0x%04X\n", two_byte_value);
        }

        attribute_len = 2U;
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x0203U,
                     attribute_data,
                     (UCHAR *)&two_byte_value,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("DID Version : 0x%04X\n", two_byte_value);
        }

        attribute_len = 1U;
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x0204U,
                     attribute_data,
                     &one_byte_value,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("DID Primary Record : %s\n", (one_byte_value ? "TRUE" : "FALSE"));
        }

        attribute_len = 2U;
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x0205U,
                     attribute_data,
                     (UCHAR *)&two_byte_value,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("DID Vendor ID Source : 0x%04X\n", two_byte_value);
        }

        attribute_len = sizeof(string_attr);
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x000AU,
                     attribute_data,
                     string_attr,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("Length : %d\n", attribute_len);

            LOG_DEBUG("Documentation URL : ");
            for(index = 0U; index < attribute_len; index ++)
            {
                LOG_DEBUG("%c", string_attr[index]);
            }

            LOG_DEBUG("\n");
        }

        attribute_len = sizeof(string_attr);
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x000BU,
                     attribute_data,
                     string_attr,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("Length : %d\n", attribute_len);

            LOG_DEBUG("Client Executable URL : ");
            for(index = 0U; index < attribute_len; index ++)
            {
                LOG_DEBUG("%c", string_attr[index]);
            }

            LOG_DEBUG("\n");
        }

        attribute_len = sizeof(string_attr);
        retval = BT_sdp_get_DataSequence_attr_val
                 (
                     0x0101U,
                     attribute_data,
                     string_attr,
                     &attribute_len
                 );

        if (API_SUCCESS != retval)
        {
            LOG_DEBUG("[DID Application] BT_sdp_get_DataSequence_attr_val failed. Reason: 0x%04X\n", retval);
        }
        else
        {
            LOG_DEBUG("Service Description : ");
            for(index = 0U; index < attribute_len; index ++)
            {
                LOG_DEBUG("%c", string_attr[index]);
            }

            LOG_DEBUG("\n");
        }

        record_no ++;
    }
}

static API_RESULT did_appl_get_acl_connection (UCHAR * indx)
{
    int choice;
    API_RESULT retval;
    UCHAR i, num_list;

    /* Init */
    retval = API_SUCCESS;

    retval = BT_hci_get_connection_details
             (did_appl_acl_list, BT_MAX_REMOTE_DEVICES, &num_list);

    if (retval != API_SUCCESS)
    {
         LOG_DEBUG("*** HCI Get Connection Details FAILED !! "\
         "Error Code = 0x%04X\n", retval);
         retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        if (0U == num_list)
        {
            LOG_DEBUG("*** ACL Connection to a Remote Device NOT Available\n");
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            LOG_DEBUG("\n");
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("Active ACL Connections\n");
            LOG_DEBUG("----------------------------------------------------------\n");
            for (i = 0U; i < num_list; i++)
            {
                LOG_DEBUG("  [%2d]  ", i);
                LOG_DEBUG("BD_ADDR %02X:%02X:%02X:%02X:%02X:%02X  ",
                    did_appl_acl_list[i].bd_addr[0U],
                    did_appl_acl_list[i].bd_addr[1U],
                    did_appl_acl_list[i].bd_addr[2U],
                    did_appl_acl_list[i].bd_addr[3U],
                    did_appl_acl_list[i].bd_addr[4U],
                    did_appl_acl_list[i].bd_addr[5U]);

                LOG_DEBUG("ACL Handle = 0x%04X\n",
                    did_appl_acl_list[i].acl_handle);
            }
            LOG_DEBUG("==========================================================\n");
            LOG_DEBUG("\n");
        }
    }

    LOG_DEBUG("Choose ACL Connection Index = \n");
    scanf("%d", &choice);
    if ((UINT16)choice >= BT_MAX_REMOTE_DEVICES)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        *indx = (UCHAR)choice;
    }

    return retval;
}

/*
 * Callback Function Registered with SDP,
 * Using SDP_SET_HANDLE() and BT_sdp_open ()
 */
static void appl_did_sdp_callback
            (
                UCHAR command,
                UCHAR * data,
                UINT16 length,
                UINT16 status
            )
{
    S_UUID uuid;
    API_RESULT retval;
    UINT32 attribute_range;

    switch(command)
    {
    case SDP_Open :
        if(status != API_SUCCESS)
        {
            LOG_DEBUG("SDP OPEN FAILED!\n");
            break;
        }
        LOG_DEBUG("SDP OPEN\n\n");

        LOG_DEBUG("Initiating SDP Service Attribute Request ....\n");
        /*
         * Frame the SDP Search pattern with PNP_INFO_UUID
         * and the complete range of Attribute IDs 0x0000 - 0xFFFF
         */
        uuid.uuid_type = UUID_16;
        uuid.uuid_union.uuid_16 = PNP_INFO_UUID;

        attribute_range = 0x0000FFFFU;
        did_len_attrib_data = sizeof(did_attrib_data);

        retval = BT_sdp_servicesearchattributerequest
                 (
                     &sdp_did_handle,
                     &uuid,
                     1U,
                     NULL,
                     0U,
                     &attribute_range,
                     1U,
                     did_attrib_data,
                     &did_len_attrib_data
                 );

        if(API_SUCCESS != retval)
        {
            LOG_DEBUG("SDP SEARCH Failed for DID..Closing SDP\n");
            (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_did_handle);
        }

        break;

    case SDP_Close:
        LOG_DEBUG("SDP CLOSE\n\n");
        break;

    case SDP_ErrorResponse:
        LOG_DEBUG("SDP ERROR RESPONSE\n\n");
        break;

    case SDP_ServiceSearchAttributeResponse:
        /* Dump the received data */
        (BT_IGNORE_RETURN_VALUE) appl_debug_dump_bytes (data, length);

        /* Extract the attriutes and print */
        appl_did_print_attributes(data, length);

        /* Now Close the SDP Connection */
        (BT_IGNORE_RETURN_VALUE) BT_sdp_close(&sdp_did_handle);

        break;

    default:
        LOG_DEBUG("Unhandled SDP Event (0x%02X)\n", command);
        break;
    }

    return;
}

void main_did_client_operations (void)
{
    UINT32 choice, menu_choice;
    API_RESULT retval;
    UCHAR acl_indx;

    BT_LOOP_FOREVER()
    {
        LOG_DEBUG("%s", did_client_menu);
        scanf("%d", &choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            LOG_DEBUG("\nRefreshing ...\n\n");
            break;

        /*** DID Client Options */
        case 10: /* Get DID Record from Peer */

            retval = did_appl_get_acl_connection (&acl_indx);
            if (API_SUCCESS == retval)
            {
                /* Initiate SDP Operation */
                SDP_SET_HANDLE
                (
                    sdp_did_handle,
                    did_appl_acl_list[acl_indx].bd_addr,
                    appl_did_sdp_callback
                );

                retval = BT_sdp_open(&sdp_did_handle);

                if ( retval != API_SUCCESS )
                {
                    LOG_DEBUG("SDP Connect FAILED !! Error Code = 0x%04X\n", retval);
                }
                else
                {
                    LOG_DEBUG("Please Wait for SDP Operation to Complete\n");
                }
            }
            break;

        default:
            LOG_DEBUG("Invalid Option. Try again\n");
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

#endif /* DID_CLIENT */

