
/**
 *  \file appl_did_server.c
 *
 *  Source File for DID Server Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_did.h"
#include "appl_utils.h"

#ifdef DID_SERVER

/* ----------------------------------------- External Global Variables */


/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */
/* DID Record Handle */
static UINT32 rec_handle;

static const char did_server_menu[] = "\n\
========= Device ID Menu ================ \n\
    0. Exit \n\
    1. Refresh \n\
 \n\
   ====== Device ID Server Options ====== \n\
   10. DID Get Record Handle \n\
   11. DID Activate Record \n\
   12. DID Deactivate Record \n\
 \n\
   20. DID Set Specification ID \n\
   21. DID Set Vendor ID \n\
   22. DID Set Product ID \n\
   23. DID Set Version \n\
   24. DID Set Primary Record \n\
   25. DID Set Non-Primary Record \n\
   26. DID Set Vendor ID Source \n\
   27. DID Set Client Executable URL \n\
   28. DID Set Service Description \n\
   29. DID Set Documentation URL \n\
 \n\
Your Option ? ";

/* ----------------------------------------- Functions */

void main_did_server_operations (void)
{
    int read_val;
    UINT32 choice, menu_choice;
    UCHAR str_attr[256U];

    BT_LOOP_FOREVER()
    {
        LOG_DEBUG("%s", did_server_menu);
        scanf("%d", &choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            LOG_DEBUG("\nRefreshing ...\n\n");
            break;

        /*** DID Server Options */
        case 10: /*  DID Get Record Handle */
            LOG_DEBUG("Enter DID Instance\n");
            scanf("%d", &read_val);

            BT_did_get_record_handle((UCHAR)read_val, rec_handle);
            LOG_DEBUG("DID Record Handle : 0x%08X\n", rec_handle);
            break;

        case 11: /* DID Activate Record */
            (BT_IGNORE_RETURN_VALUE) BT_did_activate(rec_handle);
            break;

        case 12: /* DID Deactivate Record */
            (BT_IGNORE_RETURN_VALUE) BT_did_inactivate(rec_handle);
            break;

        case 20: /* DID Set Specification ID */
            LOG_DEBUG("Enter Specificaion ID (HEX)\n");
            scanf("%x", &read_val);
            BT_did_set_specification_id(rec_handle, (UINT16)read_val);
            break;

        case 21: /* DID Set Vendor ID */
            LOG_DEBUG("Enter Vendor ID (HEX)\n");
            scanf("%x", &read_val);
            BT_did_set_vendor_id(rec_handle, (UINT16)read_val);
            break;

        case 22: /* DID Set Product ID */
            LOG_DEBUG("Enter Product ID (HEX)\n");
            scanf("%x", &read_val);
            BT_did_set_product_id(rec_handle, (UINT16)read_val);
            break;

        case 23: /* DID Set Version */
            LOG_DEBUG("Enter Version (JJ.M.N as 0xJJMN)\n");
            LOG_DEBUG("JJ - Major Version Number\n");
            LOG_DEBUG("M - Minor Version Number\n");
            LOG_DEBUG("N - Sub-minor Version Number\n");
            scanf("%x", &read_val);
            BT_did_set_version(rec_handle, (UINT16)read_val);
            break;

        case 24: /* DID Set Primary Record */
            BT_did_set_primary_record_attr(rec_handle, 0x01U);
            break;

        case 25: /* DID Set Non-Primary Record */
            BT_did_set_primary_record_attr(rec_handle, 0x00U);
            break;

        case 26: /* DID Set Vendor ID Source */
            LOG_DEBUG("Select Vendor ID Source\n");
            LOG_DEBUG("0x0001 - BT SIG Provided\n");
            LOG_DEBUG("0x0002 - USB Forum Assigned\n");
            scanf("%x", &read_val);
            BT_did_set_vendor_id_source(rec_handle, (UINT16)read_val);
            break;

        case 27: /* DID Set Client Executable URL */
            LOG_DEBUG("Enter Client Executable URL\n");
            fflush(stdout);
            fflush(stdin);

            /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
            BT_mem_set(str_attr, 0, sizeof(str_attr));

            fgets((CHAR *)str_attr, sizeof(str_attr), stdin);

            /* Check if the '\n' is the last character */
            if('\n' == str_attr[BT_str_len((CHAR *)str_attr) - 1U])
            {
                str_attr[BT_str_len((CHAR *)str_attr) - 1U] = 0x00U;
            }

            BT_did_set_client_executable_url(rec_handle, str_attr);
            break;

        case 28: /* DID Set Service Description */
            LOG_DEBUG("Enter Service Description\n");
            fflush(stdout);
            fflush(stdin);

            /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
            BT_mem_set(str_attr, 0, sizeof(str_attr));

            fgets((CHAR *)str_attr, sizeof(str_attr), stdin);

            /* Check if the '\n' is the last character */
            if('\n' == str_attr[BT_str_len((CHAR *)str_attr) - 1U])
            {
                str_attr[BT_str_len((CHAR *)str_attr) - 1U] = 0x00U;
            }

            BT_did_set_service_description(rec_handle, str_attr);
            break;

        case 29: /* DID Set Documentation URL */
            LOG_DEBUG("Enter Documentation URL\n");
            fflush(stdout);
            fflush(stdin);

            /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
            BT_mem_set(str_attr, 0, sizeof(str_attr));

            fgets((CHAR *)str_attr, sizeof(str_attr), stdin);

            /* Check if the '\n' is the last character */
            if('\n' == str_attr[BT_str_len((CHAR *)str_attr) - 1U])
            {
                str_attr[BT_str_len((CHAR *)str_attr) - 1U] = 0x00U;
            }

            BT_did_set_documentation_url(rec_handle, str_attr);
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

#endif /* DID_SERVER */

