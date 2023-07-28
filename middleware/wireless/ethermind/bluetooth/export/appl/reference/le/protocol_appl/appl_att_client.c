
/**
 *  \file appl_att_client.c
 *
 *  This file contains the ATT client application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_att_client.h"
/* #include "appl.h" */
#include "BT_gatt_db_api.h"
#include "appl_utils.h"

#ifdef ATT

/* --------------------------------------------- External Global Variables */
#ifndef APPL_TRC
#define APPL_TRC(...) printf(__VA_ARGS__)
#endif /* APPL_TRC */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
#ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
static API_RESULT appl_att_client_handle_multi_handle_val_ntf_request(void);
#endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */

static ATT_HANDLE appl_att_client_handle;

static const CHAR *print_str = "\n------------------------------\n"\
                   "        ATT CLINET MENU\n"\
                   "------------------------------\n"\
                   "0.   Exit\n"\
                   "1.   Refresh\n\n"\
                   "2.   Register ATT Callback\n\n"\
                   "3.   Update ATT Handle\n\n"\
                   "4.   Exchange MTU Request\n\n"\
                   "10.  Find information Request\n"\
                   "11.  Find by Type Value Request\n"\
                   "20.  Read By Type Request\n"\
                   "21.  Read Request\n"\
                   "22.  Read Blob Request\n"\
                   "23.  Read Multiple request\n"\
                   "24.  Read Group Type Request\n"\
                   "25.  Read Multiple Variable Length request\n"\
                   "30.  Write Command \n"\
                   "31.  Write Request \n"\
                   "32.  Prepare Write Request\n"\
                   "33.  Execute Write Request\n"\
                   "34.  Signed Write Request\n\n"\
                   "40.  Handle value confirmation\n"\
                   "41.  Multiple Handle value confirmation\n\n";

/* --------------------------------------------- Functions */

API_RESULT att_cb
           (
               ATT_HANDLE    * handle,
               UCHAR         att_event,
               API_RESULT    event_result,
               UCHAR         * event_data,
               UINT16        event_datalen
           )
{
    int             index;
    ATT_ATTR_HANDLE attr_handle, handle_1;
    UINT16          arg_len, mtu, uuid, offset;
    UCHAR           uuid_arr[ATT_128_BIT_UUID_SIZE], uuid_sz, op_code;

    APPL_TRC
    ("[ATT]:[0x%02X]: Received ATT Event 0x%02X with result 0x%04X",
    handle->att_id, att_event, event_result);

    appl_dump_bytes (event_data, event_datalen);

    switch(att_event)
    {
    case ATT_CONNECTION_IND:
        APPL_TRC (
        "[0x%02X]:Received Connection Indication, Result 0x%04X!\n",
        handle->att_id, event_result);
        appl_att_client_handle = *handle;
        break;
    case ATT_ERROR_RSP:
        BT_UNPACK_LE_1_BYTE(&op_code,event_data+1U);
        BT_UNPACK_LE_2_BYTE(&attr_handle, event_data+2U);
        APPL_TRC (
        "Received Error Response, Response code 0x%04X for Opcode 0x%02X, "
        "Handle 0x%04X!\n", event_result, op_code, attr_handle);
        break;

    case ATT_XCHNG_MTU_RSP:
        BT_UNPACK_LE_2_BYTE(&mtu, event_data);
        APPL_TRC (
        "Received Exchange MTU Response with Result 0x%04X. MTU Size "
        "= 0x%04X!\n", event_result, mtu);
        break;

    case ATT_FIND_INFO_RSP:
        printf ("Received Read Information Response Opcode!\n");

        if(0U < event_datalen)
        {
            if(0x01U == ((UCHAR*)event_data)[0U])
            {
                APPL_TRC("\n\n16 bit UUIDs : \n");
                for(index = 1U; index < event_datalen; index += 4U)
                {

                    BT_UNPACK_LE_2_BYTE
                    (
                        &attr_handle,
                        ((UCHAR*)event_data + index)
                    );
                    BT_UNPACK_LE_2_BYTE
                    (
                       &uuid,
                       (&((UCHAR *)event_data)[index + 2U])
                    );
                    APPL_TRC ("Handle : 0x%04X -> ", attr_handle);
                    APPL_TRC ("UUID   : 0x%04X \n", uuid);
                }
            }
            else if(0x02U == ((UCHAR*)event_data)[0U])
            {
                uuid_sz = ATT_128_BIT_UUID_SIZE;

                for(index = 1U; index < event_datalen; index += 18U)
                {
                    APPL_TRC("\n\n128 bit UUIDs : \n");
                    BT_UNPACK_LE_2_BYTE(&attr_handle, (((UCHAR*)event_data) + index));
                    ATT_UNPACK_UUID(uuid_arr, (&((UCHAR *)event_data)[index + 2U]), uuid_sz);
                    APPL_TRC ("Handle : 0x%04X\n", attr_handle);
                    APPL_TRC ("UUID   : ");

                    appl_dump_bytes(uuid_arr, ATT_128_BIT_UUID_SIZE);
                }
            }
            else
            {
                APPL_TRC ("List of handles corresponding to the Req UUID:\n");

                for(index = 1U; index < event_datalen; index=index+2U)
                {

                    BT_UNPACK_LE_2_BYTE(&attr_handle, (((UCHAR*)event_data)+ index));
                    APPL_TRC ("Handle : 0x%04X -> \n", attr_handle);
                }
            }
        }
        break;
    case ATT_READ_BY_TYPE_RSP:
        printf ("Received Read Type Response Opcode!\n");

        arg_len = event_data[0U];

        event_datalen --;
        event_data++;

        for (index = 0U; index < (event_datalen/(arg_len)); index++ )
        {
            BT_UNPACK_LE_2_BYTE(&attr_handle, (event_data + (index * arg_len)));
            printf ("Handle - 0x%04X\n", attr_handle);

            printf ("Handle Value Received - \n");
            appl_dump_bytes(event_data, arg_len-2U);
        }
        printf ("\n");
        break;
    case ATT_READ_BY_GROUP_RSP:
        printf ("Received Read Group Type Response Opcode!\n");

        arg_len = event_data[0U];

        event_datalen --;
        event_data++;

        for (index = 0U; index < (event_datalen/(arg_len)); index++ )
        {
            BT_UNPACK_LE_2_BYTE(&attr_handle, (event_data + (index * arg_len)));
            printf ("Attribute handle - 0x%04X\n", attr_handle);

            BT_UNPACK_LE_2_BYTE(&attr_handle, (event_data + (index * arg_len+2U)));
            printf ("Group End handle - 0x%04X\n", attr_handle);

            printf ("Handle Value Received - \n");
            appl_dump_bytes(event_data, arg_len-4U);
        }
        printf ("\n");
        break;
    case ATT_FIND_BY_TYPE_VAL_RSP:
        printf ("Received Find by Type Value Response Opcode!\n");

        printf ("Found Handle        End Found Handle\n");
        for (index = 0U; index < event_datalen; index += 4U)
        {
            BT_UNPACK_LE_2_BYTE(&attr_handle, (event_data + index));
            BT_UNPACK_LE_2_BYTE(&handle_1, (&event_data[index + 2U]));
            printf ("%04X                  %04X\n", attr_handle, handle_1);
        }
        break;

    case ATT_READ_RSP:
        printf ("Received Read Response Opcode!\n");

        printf ("Handle Value Received - \n");
        appl_dump_bytes(event_data, event_datalen);
        printf ("\n");
        break;

    case ATT_READ_BLOB_RSP:
        printf ("Received Read Response Opcode!\n");
        printf ("Handle Value Received - \n");
        appl_dump_bytes(event_data, event_datalen);
        printf ("\n");
        break;

    case ATT_READ_MULTIPLE_RSP:
        printf ("Received read multiple response Opcode!\n");

        printf ("Byte stream of values received [not seperated by handle]");
        appl_dump_bytes(event_data, event_datalen);
        printf ("\n");
        break;

    case ATT_WRITE_RSP:
        printf ("Received Write Response Opcode!\n");
        break;

    case ATT_WRITE_CMD_TX_COMPLETE:
        printf("Received Write Command Tx Complete (Locally generated)\n");
        appl_dump_bytes(event_data, event_datalen);
        break;

    case ATT_SIGNED_WRITE_CMD_TX_COMPLETE:
        printf("Received Signed Write Command Tx Complete (Locally generated)\n");
        appl_dump_bytes(event_data, event_datalen);
        break;

    case ATT_PREPARE_WRITE_RSP:
        printf ("Prepare Write Response\n");
        BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
        printf ("Handle - 0x%04X\n", attr_handle);
        BT_UNPACK_LE_2_BYTE(&offset, event_data+2U);
        printf ("Offset - 0x%04X\n", offset);
        printf ("Handle Value Received - \n");
        appl_dump_bytes(event_data + 4U, event_datalen - 4U);
        printf ("\n");
        break;

    case ATT_EXECUTE_WRITE_RSP:
        printf ("Received Execute Write Response\n");
        break;

#ifdef ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
    case ATT_READ_MULTIPLE_VARIABLE_LENGTH_RSP:
        printf("Received read multiple variable length response Opcode!\n");

        /**
         * TODO: Add utility function to decode and print
         * (<Handle:2 Octet><Length:2 Octet><Value:Length no of Octets>) tuples
         */
        printf("Byte stream of values received [not seperated by handles]");
        appl_dump_bytes(event_data, event_datalen);
        printf("\n");
        break;
#endif /* ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT */

    case ATT_HANDLE_VALUE_NTF:
        printf ("Received HVN\n");
        BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
        printf ("Handle - 0x%04X\n", attr_handle);
        printf ("Handle Value Received - \n");
        appl_dump_bytes(event_data + 2U, event_datalen - 2U);
        printf ("\n");
        break;

    case ATT_HANDLE_VALUE_IND:
        printf ("Received HVI\n");
        BT_UNPACK_LE_2_BYTE(&attr_handle, event_data);
        printf ("Handle - 0x%04X\n", attr_handle);
        printf ("Handle Value Received - \n");
        appl_dump_bytes(event_data + 2U, event_datalen);
        break;

#ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
    case ATT_HANDLE_VALUE_MULTIPLE_NTF:
    {
        /* Handle Length Value Tuple - elements */
        UINT16 hlvt_attr_hdl, hlvt_len, hlvt_index;
        UINT8 * hlvt_val;
        UINT16  remaining;

        printf("Received HVMN\n");

        /**
         * TODO: Add utility function to decode and print
         * (<Handle:2 Octet><Length:2 Octet><Value:Length no of Octets>) tuples
         */
        printf("Byte stream of values received [not seperated by handles]");
        appl_dump_bytes(event_data, event_datalen);
        printf("\n");

        remaining = event_datalen;
        hlvt_val = event_data;
        hlvt_index = 0;

        /* Atleast the handle and length fields shall be present */
        while (remaining >= 4)
        {
            BT_UNPACK_LE_2_BYTE(&hlvt_attr_hdl, hlvt_val);
            hlvt_val += 2;
            remaining -= 2;

            BT_UNPACK_LE_2_BYTE(&hlvt_len, hlvt_val);
            hlvt_val += 2;
            remaining -= 2;

            printf("Tuple[%d]: <hdl> 0x%04X, <len> 0x%04X, \n", hlvt_index, hlvt_attr_hdl, hlvt_len);
            hlvt_index++;

            if (hlvt_len > remaining)
            {
                printf("<Partial Value>: \n");
                appl_dump_bytes(hlvt_val, remaining);
                remaining = 0;

                break;
            }
            else if (hlvt_len == remaining)
            {
                printf("<Last Value>: \n");
                appl_dump_bytes(hlvt_val, remaining);
                remaining = 0;

                break;
            }
            else
            {
                printf("<Value>: \n");
                appl_dump_bytes(hlvt_val, hlvt_len);
                remaining -= hlvt_len;
                hlvt_val += hlvt_len;
            }
        }

        if (0 != remaining)
        {
            printf("Could not parse following last %d octets\n", remaining);

            appl_dump_bytes(hlvt_val, remaining);
        }
    }
        break;
#endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */

    default:
        CONSOLE_OUT("Invalid Event Type: %d\n", att_event);
        break;
    }
    printf("\n\n");

    return API_SUCCESS;
}

void main_att_client_operations(void)
{
    ATT_FIND_INFO_REQ_PARAM find_info_req_param;
    ATT_FIND_BY_TYPE_VAL_REQ_PARAM find_by_type_val_req_param;
    ATT_READ_BY_TYPE_REQ_PARAM read_by_type_req;
    ATT_READ_REQ_PARAM read_req_param;
    ATT_WRITE_CMD_PARAM write_cmd_param;
    ATT_WRITE_REQ_PARAM write_req_param;
    ATT_PREPARE_WRITE_REQ_PARAM prepare_write_req_param;
    ATT_EXECUTE_WRITE_REQ_PARAM execute_write_req_param;
    ATT_READ_BLOB_REQ_PARAM read_blob_req_param;
    ATT_READ_MULTIPLE_REQ_PARAM read_multiple_req_param;
    ATT_READ_BY_GROUP_TYPE_REQ_PARAM read_by_group_param;

#ifdef ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
    ATT_READ_MULTIPLE_VARIABLE_LENGTH_REQ_PARAM read_multiple_var_len_req_param;
#endif /* ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT */

    int data;
    UINT32 index;
    INT32 choice, menu_choice;
    UINT16 mtu;
    UINT16 handle_list[20U];
    API_RESULT retval;
    UCHAR value[20U];
    data = 0U;

    BT_LOOP_FOREVER()
    {
        printf("%s \n", print_str);
        printf("Enter you choice : ");
        scanf("%d", &choice);
        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 2: /* Register ATT Callback */
        {
            ATT_APPLICATION att_interface;

            /* Register ATT interface */
            att_interface.cb = att_cb;
            (BT_IGNORE_RETURN_VALUE) BT_att_register(&att_interface);
        }
        break;

        case 3:
            APPL_TRC("Enter ATT Handle Device ID [in HEX]\n");
            scanf("%X", &data);
            appl_att_client_handle.device_id = (UCHAR)(data);
            APPL_TRC("Enter ATT Handle ATT Instance ID [in HEX]\n");
            scanf("%X", &data);
            appl_att_client_handle.att_id = (UCHAR)(data);
            break;

        case 4: /* Exchange MTU Request */

            printf ("Enter the MTU for the ATT connection\n");
            scanf ("%x", &data);
            mtu = (UINT16)data;

            retval = BT_att_send_mtu_xcnhg_req
                     (
                         &appl_att_client_handle,
                         &mtu
                     );

            printf ("Exchange MTU REQ retval - 0x%04X\n", retval);
            break;

        case 10: /* Find Information Request */
            printf("Enter start handle[HEX] : ");
            scanf("%x", &data);
            find_info_req_param.start_handle = (UINT16) data;
            printf("Enter end handle[HEX] : ");
            scanf("%x", &data);
            find_info_req_param.end_handle = (UINT16) data;


            retval = BT_att_send_find_info_req
                     (
                          &appl_att_client_handle,
                          &find_info_req_param
                      );

            APPL_TRC (
            "Find Information Request Sent with retval 0x%04X\n", retval);

            break;

        case 11: /* Find By Type Value Request */
            printf("Enter start handle[HEX] : ");
            scanf("%x", &data);
            find_by_type_val_req_param.range.start_handle = (UINT16) data;
            printf("Enter end handle[HEX] : ");
            scanf("%x", &data);
            find_by_type_val_req_param.range.end_handle = (UINT16) data;

            printf("Enter 16 Bit UUID[HEX] 16: ");
            scanf("%x", &data);
            find_by_type_val_req_param.uuid = (UINT16) data;

            printf ("Enter the length of the value (decimal)\n");
            scanf("%d", &data);
            find_by_type_val_req_param.value.len = (UINT16)data;

            printf ("Enter the value [byte by byte space seperated in HEX]: ");
            for (index = 0U; index < find_by_type_val_req_param.value.len;
                index++)
            {
                scanf ("%x", &data);
                value[index] = (UCHAR)data;
            }
            find_by_type_val_req_param.value.val = value;

            retval = BT_att_send_find_by_type_val_req
                     (
                         &appl_att_client_handle,
                         &find_by_type_val_req_param
                     );

            APPL_TRC
            ("ATT Find By Type Value Request sent with result 0x%04X\n",retval);
            break;

        case 20: /* Read By Type Request */
            printf("Enter start handle[HEX] : ");
            scanf("%x", &data);
            read_by_type_req.range.start_handle = (ATT_ATTR_HANDLE) data;
            printf("Enter end handle[HEX] : ");
            scanf("%x", &data);
            read_by_type_req.range.end_handle = (ATT_ATTR_HANDLE) data;
#ifdef ATT_SUPPORT_128_BIT_UUID
            printf("Enter uuid type\n\t1 - 16 Bit\t2 - 128 Bit) : ");
            scanf("%x", &data);
            read_by_type_req.uuid_format = (UCHAR) data;
            if (1U == read_by_type_req.uuid_format)
            {
                printf("Enter 16 Bit UUID[HEX] : ");
            }
            else if (2U == read_by_type_req.uuid_format)
            {
                printf("Enter 128 Bit UUID[HEX OCTETS]  : ");
                for (index = 0U; index < 16U; index ++)
                {
                    scanf("%x", &data);
                    read_by_type_req.uuid.uuid_128.value[index] = (UCHAR) data;
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }

#else /* ATT_SUPPORT_128_BIT_UUID */
            read_by_type_req.uuid_format = ATT_16_BIT_UUID_FORMAT;
            printf("Enter 16 Bit UUID[HEX] : ");
            scanf("%x", &data);
            read_by_type_req.uuid = (UINT16) data;
#endif /* ATT_SUPPORT_128_BIT_UUID */

            retval = BT_att_send_read_by_type_req
                     (
                          &appl_att_client_handle,
                          &read_by_type_req
                      );

            APPL_TRC (
            "ATT Read By Type Request sent with retval 0x%04X\n", retval);

            break;

        case 21: /* Read Request */
            printf("Enter handle[HEX] : ");
            scanf("%x", &data);
            read_req_param = (ATT_ATTR_HANDLE) data;

            retval = BT_att_send_read_req
                     (
                          &appl_att_client_handle,
                          &read_req_param
                      );

            APPL_TRC (
            "ATT Read Request sent with retval 0x%04X\n", retval);
            break;

        case 22: /* Read Blob Request */
            printf("Enter handle[HEX] : ");
            scanf("%x", &data);
            read_blob_req_param.handle = (ATT_ATTR_HANDLE) data;
            printf("Enter offset[HEX] : ");
            scanf("%x", &data);
            read_blob_req_param.offset = (ATT_ATTR_HANDLE) data;

            retval = BT_att_send_read_blob_req
                     (
                          &appl_att_client_handle,
                          &read_blob_req_param
                      );

            APPL_TRC (
            "ATT Read Blob Request Result 0x%04X\n", retval);

            break;

        case 23: /* Read Multiple Request */

            printf ("Enter the number of handles [decimal]\n");
            scanf("%x", &data);
            read_multiple_req_param.list_count = (UINT16)data;

            printf ("Enter the list of handle [HEX] \n");
            for (index = 0U; index < read_multiple_req_param.list_count; index++)
            {
                scanf ("%04X", &data);
                handle_list[index] = (ATT_ATTR_HANDLE)data;
            }
            read_multiple_req_param.handle_list = handle_list;

            retval = BT_att_send_read_multiple_req
                     (
                         &appl_att_client_handle,
                         &read_multiple_req_param
                     );

            APPL_TRC
            ("ATT Read Multiple Request sent with result0x%04X\n", retval);
            break;

        case 24: /* Read By Group Type Request */
            printf("Enter start handle[HEX] : ");
            scanf("%x", &data);
            read_by_group_param.range.start_handle = (ATT_ATTR_HANDLE) data;
            printf("Enter end handle[HEX] : ");
            scanf("%x", &data);
            read_by_group_param.range.end_handle = (ATT_ATTR_HANDLE) data;
#ifdef ATT_SUPPORT_128_BIT_UUID
            printf("Enter UUID Format type\n\t 1 - 16\n\t 2 - 128 Bit\n");
            scanf("%x", &data);
            read_by_group_param.uuid_format = (UCHAR) data;
            if (ATT_16_BIT_UUID_FORMAT == read_by_group_param.uuid_format)
            {
                printf("Enter 16 Bit UUID[HEX] : ");
                scanf("%x", &data);
                read_by_group_param.group_type.uuid_16 = (UINT16) data;
            }
            else if (ATT_128_BIT_UUID_FORMAT == read_by_group_param.uuid_format)
            {
                printf("Enter 128 Bit UUID[HEX] : ");
                for (index = 0U; index < 16U; index ++)
                {
                    scanf("%x", &data);
                    read_by_group_param.group_type.uuid_128.value[index] = (UCHAR) data;
                }
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
#else /* ATT_SUPPORT_128_BIT_UUID */
            printf("Enter 16 Bit UUID[HEX] : ");
            scanf("%x", &data);
            read_by_group_param.group_type = (UINT16) data;
#endif /* ATT_SUPPORT_128_BIT_UUID */
            retval = BT_att_send_read_by_group_req
                     (
                         &appl_att_client_handle,
                         &read_by_group_param
                     );

            APPL_TRC
            ("ATT Read By Group Type Request sent with result 0x%04X\n",
            retval);
            break;

        case 25: /* Read Multiple Variable Length Request */
#ifdef ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT
            APPL_TRC("Enter ATT Handle Device ID [in HEX]\n");
            scanf("%X", &choice);
            appl_att_client_handle.device_id = (UCHAR)(choice);
            APPL_TRC("Enter ATT Handle ATT Instance ID [in HEX]\n");
            scanf("%X", &choice);
            appl_att_client_handle.att_id = (UCHAR)(choice);

            printf ("Enter the number of handles [decimal]\n");
            scanf("%x", &data);
            read_multiple_var_len_req_param.list_count = (UINT16)data;

            printf ("Enter the list of handle [HEX] \n");
            for (index = 0; index < read_multiple_var_len_req_param.list_count; index++)
            {
                scanf ("%04X", &data);
                handle_list[index] = (ATT_ATTR_HANDLE)data;
            }
            read_multiple_var_len_req_param.handle_list = handle_list;

            retval = BT_att_send_read_multiple_variable_length_req
                     (
                         &appl_att_client_handle,
                         &read_multiple_var_len_req_param
                     );

            APPL_TRC
            ("ATT Read Multiple Variable Length Request sent with result0x%04X\n", retval);
#else
            APPL_TRC
            ("ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT is not defined\n");
#endif /* ATT_READ_MULTIPLE_VARIABLE_LENGTH_SUPPORT */
            break;

        case 30: /* Write Command */
            printf("Enter handle[HEX] : ");
            scanf("%x", &data);
            write_cmd_param.handle = (ATT_ATTR_HANDLE) data;
            printf("Enter value len[HEX] : ");
            scanf("%x", &data);
            write_cmd_param.value.len = (UINT16)data;
            printf("Enter value[HEX] : ");
            for(index = 0U; index < write_cmd_param.value.len; index++)
            {
                scanf("%x", &data);
                value [index] = (UCHAR)data;
            }

            write_cmd_param.value.val = value;
            write_cmd_param.value.actual_len = 0;

            retval = BT_att_send_write_cmd
                     (
                          &appl_att_client_handle,
                          &write_cmd_param
                      );

            APPL_TRC (
            "ATT Write Command Result - 0x%04X, Actual Value Len = 0x%04X\n",
            retval,write_cmd_param.value.actual_len);
            break;

        case 31: /* Write Request */
            printf("Enter handle[HEX] : ");
            scanf("%x", &data);
            write_req_param.handle = (ATT_ATTR_HANDLE) data;
            printf("Enter value len[HEX] : ");
            scanf("%x", &data);
            write_req_param.value.len = (UINT16)data;
            printf("Enter value[HEX] : ");
            for(index = 0U; index < write_req_param.value.len; index++)
            {
                scanf("%x", &data);
                value [index] = (UCHAR)data;
            }
            write_req_param.value.val = value;

            retval = BT_att_send_write_req
                     (
                          &appl_att_client_handle,
                          &write_req_param
                      );

            APPL_TRC (
            "ATT Write Command Result - 0x%04X, Actual Value Len = 0x%04X\n",
            retval,write_req_param.value.actual_len);
            break;

        case 32: /* Prepare Write Request */
            printf("Enter handle[HEX] : ");
            scanf("%x", &data);
            prepare_write_req_param.handle_value.handle = (UINT16) data;

            printf("Enter value offset[HEX] : ");
            scanf("%x", &data);
            prepare_write_req_param.offset = (UINT16) data;

            printf("Enter value len[HEX] : ");
            scanf("%x", &data);
            prepare_write_req_param.handle_value.value.len = (UINT16)data;
            printf("Enter value[HEX] : ");
            for(index = 0U;
            index < prepare_write_req_param.handle_value.value.len; index++)
            {
                scanf("%x", &data);
                value[index] = (UCHAR)data;
            }
            prepare_write_req_param.handle_value.value.val = value;

            retval = BT_att_send_prepare_write_req
                     (
                          &appl_att_client_handle,
                          &prepare_write_req_param
                      );

            APPL_TRC (
            "ATT Prepare Write Request Result - 0x%04X\n, Actual Value Len "
            "= 0x%04X\n", retval,
            prepare_write_req_param.handle_value.value.actual_len);
            break;

        case 33: /* Execute Write Request */
            printf("Enter flag\n\t1 - Execute\n\t0-Cancel\n");
            scanf("%x", &data);
            execute_write_req_param.flags = (UCHAR) data;

            retval = BT_att_send_execute_write_req
                     (
                         &appl_att_client_handle,
                         &execute_write_req_param
                     );
            APPL_TRC
            ("ATT Execute Write Request Result - 0x%04X\n",retval);
            break;

        case 34: /* Signed Write Request */
            break;
        case 40: /* Handle Value Confirmation */
            retval = BT_att_send_hndl_val_cnf(&appl_att_client_handle);
            APPL_TRC (
            "ATT Handle Value Confirmation sent with Result 0x%04X\n", retval);
            break;

        case 41: /* Multiple Handle Value Notification */
        #ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
            retval = appl_att_client_handle_multi_handle_val_ntf_request();

            APPL_TRC (
            "ATT Multiple Handle Value Notification sent with Result 0x%04X\n", retval);
        #else
            APPL_TRC (
            "ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT feature flag is not defined\n");
        #endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */
            break;

        default:
            CONSOLE_OUT("Invalid Choice\n");
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

#define APPL_ATT_CLIENT_MAX_MULT_HVN_COUNT     4

#ifdef ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT
static API_RESULT appl_att_client_handle_multi_handle_val_ntf_request(void)
{
    ATT_HNDL_VAL_MULTIPLE_NTF_PARAM param;
    ATT_HANDLE_VALUE_PAIR           handle_value_list[APPL_ATT_CLIENT_MAX_MULT_HVN_COUNT];
    int     choice;
    UINT32  index;
    API_RESULT retval;

    APPL_TRC("Enter ATT Handle Device ID [in HEX]\n");
    scanf("%X", &choice);
    appl_att_client_handle.device_id = (UCHAR)(choice);
    APPL_TRC("Enter ATT Handle ATT Instance ID [in HEX]\n");
    scanf("%X", &choice);
    appl_att_client_handle.att_id = (UCHAR)(choice);

    APPL_TRC ("Enter Number of ATT Handles (maximum %d): \n", APPL_ATT_CLIENT_MAX_MULT_HVN_COUNT);
    scanf("%d", &choice);

    if (choice > APPL_ATT_CLIENT_MAX_MULT_HVN_COUNT)
    {
        APPL_TRC (
        "Number of ATT Handles (%d) > APPL_ATT_CLIENT_MAX_MULT_HVN_COUNT (%d). Returning error.\n",
        choice, APPL_ATT_CLIENT_MAX_MULT_HVN_COUNT);

        return API_FAILURE;
    }

    param.handle_value_list = handle_value_list;
    param.count = (UINT16)choice;

    APPL_TRC("Enter ATT Handles\n");

    for (index = 0; index < param.count; index++)
    {
        APPL_TRC("ATT Handle [%d] (in HEX): \n", index);
        scanf("%x", &choice);

        handle_value_list[index].handle = (ATT_ATTR_HANDLE)choice;

        retval  = BT_gatt_db_access_handle
                  (
                      &handle_value_list[index],
                      &appl_att_client_handle,
                      0,
                      (GATT_DB_READ | GATT_DB_PEER_INITIATED)
                  );

        if (API_SUCCESS != retval)
        {
            APPL_TRC("Failed to read ATT value for Handle [0x%04X]: \n", handle_value_list[index].handle);

            break;
        }
    }

    retval = BT_att_send_hndl_val_multiple_ntf(&appl_att_client_handle, &param);

    return retval;
}
#endif /* ATT_HNDL_VAL_MULT_NOTIFICATION_SUPPORT */

#endif /* ATT */
