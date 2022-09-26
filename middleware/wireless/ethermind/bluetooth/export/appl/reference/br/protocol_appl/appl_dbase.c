
/**
 *  \file appl_dbase.c
 *
 *  Source File for SDP Database Command Line Application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_sdp.h"

#ifdef SDP_SERVER

/* ----------------------------------------- External Global Variables */
#ifdef SDP_DYNAMIC_DB
extern void db_add_record(void);
#endif /* SDP_DYNAMIC_DB */

/* ----------------------------------------- Exported Global Variables */


/* ----------------------------------------- Static Global Variables */

static const char dbase_menu[] = "\n\
========= SDP Database Menu ========= \n\
    0. Exit \n\
    1. Refresh \n\
 \n\
    2. Get Record Handle \n\
 \n\
    3. Activate Record \n\
    4. Inactivate Record  \n\
 \n\
    5. Update Attibute Value \n\
    6. Change Attribute Value \n\
 \n\
    7. Get Server Channel \n\
 \n\
   10. Activate All Records \n\
 \n\
   20. Create Dynamic SDP database \n\
 \n\
Your Option ? ";


static const char service_type_menu[] = "\n\
Choose Serive Type from the list below: \n\
    0.  SDP \n\
    1.  SAP \n\
    2.  SPP \n\
    3.  CTP \n\
    4.  ICP \n\
    5.  HSU \n\
    6.  HSAG \n\
    7.  DUNP \n\
    8.  FAX \n\
    9.  LAP \n\
    10. FTP \n\
    11. HFP - Unit \n\
    12. HFP - AG \n\
    13. HID \n\
    14. PNP \n\
    15. A2DP - SINK \n\
    16. A2DP - SOURCE \n\
    17. OPP \n\
    18. SYNC \n\
    19. PAN - NAP \n\
    20. PAN - GN \n\
    21. PAN - PANU \n\
    22. UDI - MT \n\
    23. UDI - TA \n\
    24. BIP \n\
    25. RO \n\
    26. ARCHIVED OBJECTS \n\
    27. HCRP \n\
    28. HCP NOTIFICATION \n\
    29. BPP \n\
    30. BASIC PRINTING REFERENCE OBJECTS \n\
    31. PRINTER ADMINISTRATIVE INTERFACE \n\
    32. AVRCP - CONTROLLER \n\
    33. AVRCP - TARGET \n\
    34. SYNC CMD \n\
 \n\
Service Type ? ";


/* ----------------------------------------- Functions */

void main_dbase_operations (void)
{
    UINT32 choice, menu_choice ;
    UINT32 read_val;
    UINT32 rec_handle ;
    UINT16 attrib_id;
    UINT16 attrib_len;
    UCHAR *attrib_val;
    UCHAR server_channel, service_type;
    UINT16 char_count;
    API_RESULT result;
    UINT32     record_handle;
    UINT16     index;

    BT_LOOP_FOREVER()
    {
        printf("%s", dbase_menu);
        scanf("%u", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            printf("\nRefreshing ...\n\n");
            break;

        case 2:
            printf("%s", service_type_menu);
            scanf("%u", &read_val);
            if (34U < read_val)
            {
                printf("Invalid choice\n");
                break;
            }
            service_type = (UCHAR) read_val;

            rec_handle = 0U;
            result = BT_dbase_get_record_handle (service_type, 0U, &rec_handle);
            if (API_SUCCESS == result)
            {
                printf("Get Record Handle Successful\n");
                printf("Record Handle: 0x%08X\n", rec_handle);
            }
            else
            {
                printf("*** FAILED to Get Record Handle: 0x%04X\n", result);
            }

            break;

        case 3:
            printf("Enter Record Handle: ");
            scanf("%x", &read_val);
            rec_handle = (UINT32) read_val;

            result = BT_dbase_activate_record (rec_handle);
            if (API_SUCCESS == result)
            {
                printf("Record Activated\n");
            }
            else
            {
                printf("*** FAILED to Activate Record: 0x%04X\n", result);
            }

            break;

        case 4:
            printf("Enter Record Handle: ");
            scanf("%x", &read_val);
            rec_handle = (UINT32) read_val;

            result = BT_dbase_inactivate_record (rec_handle);
            if (API_SUCCESS == result)
            {
                printf("Record Inactivated\n");
            }
            else
            {
                printf("*** FAILED to Inactivate Record: 0x%04X\n", result);
            }

            break;

        case 5:
            printf("Enter Record Handle: ");
            scanf("%x", &read_val);
            rec_handle = (UINT32) read_val;

            printf("Enter Attribute ID: ");
            scanf("%x", &read_val);
            attrib_id = (UINT16) read_val;

            printf("Enter Attribute Length: ");
            scanf("%x", &read_val);
            attrib_len = (UINT16) read_val;

            attrib_val = BT_alloc_mem (sizeof(UCHAR) * attrib_len);
            if (NULL == attrib_val)
            {
                printf("*** FAILED to Allocate Memory for Attribute Value\n");
                break;
            }

            printf("Enter Attribute Value: ");
            for (char_count = 0U; char_count < attrib_len; char_count ++)
            {
                scanf("%x", &read_val);
                attrib_val[char_count] = (UINT8) read_val;
            }

            result = BT_dbase_update_attr_value
                     (
                         rec_handle,
                         attrib_id,
                         attrib_val,
                         attrib_len
                     );

            if (API_SUCCESS == result)
            {
                printf("Updated Attribute Value Successfully\n");
            }
            else
            {
                printf("*** FAILED to Update Attribute Value: 0x%04X\n",
                result);
            }

            BT_free_mem (attrib_val);
            break;

        case 6:
            printf("Enter Record handle: ");
            scanf("%x", &read_val);
            rec_handle = (UINT32) read_val;

            printf("Enter Attribute ID: ");
            scanf("%x", &read_val);
            attrib_id = (UINT16) read_val;

            printf("Enter Attribute Length: ");
            scanf("%x", &read_val);
            attrib_len = (UINT16) read_val;

            attrib_val = BT_alloc_mem (sizeof(UCHAR) * attrib_len);
            if (NULL == attrib_val)
            {
                printf("*** FAILED to Allocate Memory for Attribute Value\n");
                break;
            }

            printf("Enter Attribute Value: ");
            for (char_count = 0U; char_count < attrib_len; char_count ++)
            {
                scanf("%x", &read_val);
                attrib_val[char_count] = (UINT8) read_val;
            }

            result = BT_dbase_change_attr_value
                     (
                         rec_handle,
                         attrib_id,
                         attrib_val,
                         attrib_len
                     );

            if (API_SUCCESS == result)
            {
                printf("Change Attribute Value Successfully\n");
            }
            else
            {
                printf("*** FAILED to Change Attribute Value: 0x%04X\n",
                result);
            }

            break;

        case 7:
            printf("Record handle: ");
            scanf("%x",&read_val);
            rec_handle = (UINT32)read_val;

            printf("Attribute ID: ");
            scanf("%x",&read_val);
            attrib_id = (UINT16)read_val;

            result = BT_dbase_get_server_channel
                     (
                         rec_handle,
                         attrib_id,
                         &server_channel
                     );

            if (API_SUCCESS == result)
            {
                printf("Get Server Channel Successful\n");
                printf("Server Channel = 0x%02X\n", server_channel);
            }
            else
            {
                printf("*** FAILED to Get Server Channel: 0x%04X\n", result);
            }

            break;

        case 10:
            printf ("Activating all SDP Records ...\n");
            printf ("Total number of SDP Records = %d\n", DB_MAX_RECORDS);

            /*
             * Using informed assumption for Base SDP record handle.
             * If 'SDDB_BASE_RECORD_HANDLE' is defined differently
             * the initial value of SDP Record Handle value also
             * shall be updated accordingly.
             */
            record_handle = 0x00090000U;

            /*
             * Skipping activation of first record.
             * First is SDP record and it is always activated.
             */
            for (index = 1U; index < DB_MAX_RECORDS; index ++)
            {
                record_handle ++;

                result = BT_dbase_activate_record
                         (
                             record_handle
                         );

                if (API_SUCCESS != result)
                {
                    printf ("SDP Record 0x%08X activation failed. Reason 0x%04X\n",
                    record_handle, result);
                }
            }
            break;

        case 20:
#ifdef SDP_DYNAMIC_DB
            db_add_record();
#else /* SDP_DYNAMIC_DB */
            printf("SDP_DYNAMIC_DB is not defined\n");
#endif /* SDP_DYNAMIC_DB */
            break;

        default:
            printf("Invalid Option. Try again\n");
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

#endif /* SDP_SERVER */

