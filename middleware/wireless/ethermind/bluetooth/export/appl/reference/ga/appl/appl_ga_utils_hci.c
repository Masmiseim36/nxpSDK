/**
 *  \file appl_ga_utils_hci.c
 *
 *  \brief Reference application for HCI APIs and event callbacks for GA
 */

/*
 *  Copyright (C) 2020. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_ga_utils_hci.h"

#include "appl_ga_bc_ba.h"
#include "appl_ga_bc_sd.h"
#include "appl_ga_ucc.h"
#include "appl_ga_ucs.h"

#ifdef BT_GAM

/* --------------------------------------------- Global Definitions */

#define APPL_GA_UTILS_HCI_ISO_CONTROLLER_DELAY        0x00U

#define APPL_GA_UTILS_HCI_DATA_PATH_ID_HCI            0x00U

/* Count of maximum EA Reports that can be store din the DB */
#define APPL_GA_EA_DB_COUNT                           300U
/* --------------------------------------------- Data types /Structures */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* Current count of EA reports in the DB */
UINT16       ea_db_count;
/* EA DB: The scan for EA can be triggered by any module, but the control of
 * adding it into the DB is done in this module.
 */
GA_ENDPOINT  ea_db[APPL_GA_EA_DB_COUNT];
/* --------------------------------------------- APIs */

GA_RESULT appl_ga_utils_hci_set_host_feature
          (
              /* IN */ UCHAR bit_number,
              /* IN */ UCHAR bit_value
          )
{
    GA_RESULT  retval;

    APPL_DBG
    (
        "[APPL][GA][HCI]: >> appl_ga_utils_hci_set_host_feature\n"
    );

    retval = BT_hci_le_set_host_feature(bit_number, bit_value);

    if (retval != GA_SUCCESS)
    {
        retval = GA_FAILURE;
        APPL_DBG
        (
            "[APPL][GA][HCI]: >> Set Host Feature Failed 0x%04X !\n",
            retval
        );
    }
    else
    {
        APPL_DBG
        (
            "[APPL][GA][HCI]: >> Set Host Feature Success\n",
            retval
        );
    }

    APPL_DBG
    (
        "[APPL][GA][HCI]: << appl_ga_utils_hci_set_host_feature\n"
    );

    return retval;
}

GA_RESULT appl_ga_utils_hci_event_callback
           (
               UINT8   event_code,
               UINT8 * event_data,
               UINT8   event_datalen
           )
{
    GA_RESULT    retval;
    UINT8        value_1;
    UINT16       value_2;
    UINT8        marker;
    UCHAR        status;
    UINT16       acl_conn_handle;
    GA_ENDPOINT  device;
    UINT8        index;
    UINT8        role;

    /* Initialize */
    retval = GA_FAILURE;
    value_1 = 0U;
    value_2 = 0U;
    marker = 0U;
    status = 0U;
    index = 0U;
    role = 0U;

    APPL_DBG(">> appl_ga_utils_hci_event_callback\n");

    /* Switch on the Event Code */
    switch (event_code)
    {
        case HCI_LE_META_EVENT:
            /* Subevent_Code */
            hci_unpack_1_byte_param(&value_1, &event_data[marker]);
            marker += 1U;
            switch (value_1)
            {
                case HCI_LE_PERIODIC_ADVERTISING_SYNC_TRANSFER_RECEIVED_SUBEVENT:
                    appl_ga_bc_sd_hci_cb_handler
                    (
                        event_code,
                        event_data,
                        event_datalen
                    );
                    break;
                /*
                 * If the sub-event is extended advertising report sub-event,
                 * then pass it to BA
                 */
                case HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT:
                    /* Retrieve the Address and Type */
                    appl_ga_utils_hci_extract_addr_from_ea
                    (
                        event_data,
                        event_datalen,
                        &device
                    );

                    /* Validate if the EA is already in the DB */
                    if (GA_FALSE == appl_ga_utils_hci_is_ea_in_db
                                    (
                                        device.bd_addr,
                                        device.bd_type
                                    ))
                    {
                        /* Add the EA in the DB since not present */
                        appl_ga_utils_hci_add_ea_to_db
                        (
                            device.bd_addr,
                            device.bd_type
                        );
                        appl_ga_ucc_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                        appl_ga_bc_ba_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                    }
                    break;

                case HCI_LE_ENHANCED_CONNECTION_COMPLETE_SUBEVENT:
                    /* Status */
                    hci_unpack_1_byte_param(&status, &event_data[marker]);
                    marker += 1U;
                    /* Connection_Handle */
                    hci_unpack_2_byte_param(&acl_conn_handle, &event_data[marker]);
                    marker += 2U;
                    /* Role */
                    hci_unpack_1_byte_param(&role, &event_data[marker]);
                    marker += 1U;
                    /* Peer_Address_Type */
                    hci_unpack_1_byte_param(&device.bd_type, &event_data[marker]);
                    marker += 1U;
                    /* Peer_Address */
                    BT_COPY_BD_ADDR(device.bd_addr, &event_data[marker]);
                    marker += BT_BD_ADDR_SIZE;
                    retval = appl_ga_utils_handle_conn_device
                             (
                                device,
                                status,
                                acl_conn_handle
                             );
                    break;

                case HCI_LE_CIS_ESTABLISHED_SUBEVENT:
                    appl_ga_ucc_hci_cb_handler
                    (
                        event_code,
                        event_data,
                        event_datalen
                    );
                    appl_ga_ucs_hci_cb_handler
                    (
                        event_code,
                        event_data,
                        event_datalen
                    );
                    break;

                case HCI_LE_CIS_REQUEST_SUBEVENT:
                    appl_ga_ucs_hci_cb_handler
                    (
                        event_code,
                        event_data,
                        event_datalen
                    );
                    break;

                default:
                    break;
            }
            break;

        case HCI_COMMAND_COMPLETE_EVENT:
            /* Number of Command Packets */
            hci_unpack_1_byte_param(&value_1, &event_data[marker]);
            marker++;

            /* Command Opcode */
            hci_unpack_2_byte_param(&value_2, &event_data[marker]);
            marker++;

            /* Command Status */
            hci_unpack_1_byte_param(&status, &event_data[marker]);
            marker++;

            /* Command Return Parameters */
            if (event_datalen > 4U)
            {
                switch (value_2)
                {
                    case HCI_LE_SET_HOST_FEATURE_OPCODE: /* Fall - through */
                    case HCI_LE_SETUP_ISO_DATA_PATH_OPCODE: /* Fall - through */
                    case HCI_LE_REMOVE_ISO_DATA_PATH_OPCODE: /* Fall - through */
                    case HCI_LE_SET_PERIODIC_ADVERTISING_SYNC_TRANSFER_PARAMETERS_OPCODE:
                        appl_ga_ucc_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                        appl_ga_ucs_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                        appl_ga_bc_sd_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                        appl_ga_bc_ba_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                        break;

                    case HCI_LE_SET_CIG_PARAMETERS_OPCODE: /* Fall - through */
                    case HCI_LE_REMOVE_CIG_OPCODE:
                        appl_ga_ucc_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                        break;

                    case HCI_LE_SET_EXTENDED_ADV_PARAMS_OPCODE: /* Fall - through */
                    case HCI_LE_SET_EXTENDED_ADVERTISING_DATA_OPCODE: /* Fall - through */
                    case HCI_LE_SET_EXTENDED_ADVERTISE_ENABLE_OPCODE:
                        appl_ga_bc_sd_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                        break;

                    case HCI_LE_SET_EXTENDED_SCAN_PARAMETERS_OPCODE: /* Fall - through */
                    case HCI_LE_SET_EXTENDED_SCAN_ENABLE_OPCODE:
                        appl_ga_bc_ba_hci_cb_handler
                        (
                            event_code,
                            event_data,
                            event_datalen
                        );
                        break;

                    default:
                        break;
                }
            }
            break;

        case HCI_COMMAND_STATUS_EVENT:
            /* Status */
            hci_unpack_1_byte_param(&status, &event_data[marker]);
            marker++;

            /* Number of Command Packets */
            hci_unpack_1_byte_param(&value_1, &event_data[marker]);
            marker++;

            /* Command Opcode */
            hci_unpack_2_byte_param(&value_2, event_data);
            marker++;

            switch (value_2)
            {
                case HCI_LE_CREATE_CIS_OPCODE:
                    appl_ga_ucc_hci_cb_handler
                    (
                        event_code,
                        event_data,
                        event_datalen
                    );
                    break;

                default:
                    break;
            }
            break;

        case HCI_DISCONNECTION_COMPLETE_EVENT:
            /* Status */
            hci_unpack_1_byte_param(&status, &event_data[marker]);
            marker++;
            APPL_TRC("\t[APPL][GA][HCI]: Status: 0x%02X\n", status);

            /* Connection Handle - CIS or ACL */
            hci_unpack_2_byte_param(&acl_conn_handle, &event_data[marker]);
            marker += 2U;
            APPL_TRC("\t[APPL][GA][HCI]: Connection Handle: 0x%04X\n", acl_conn_handle);

            appl_ga_ucc_hci_cb_handler
            (
                event_code,
                event_data,
                event_datalen
            );
            appl_ga_ucs_hci_cb_handler
            (
                event_code,
                event_data,
                event_datalen
            );

            retval = appl_ga_utils_get_dev_index_from_handle
                     (
                         acl_conn_handle,
                         &index
                     );
            /* Got the index of the device. Remove the device from the DB */
            if (GA_SUCCESS == retval)
            {
                retval = appl_ga_utils_remove_conn_device(index);
                if (GA_SUCCESS == retval)
                {
                    APPL_TRC("[APPL][GA][HCI]: Successfully removed device from database\n");
                }
            }
            else
            {
                /* This must be CIS Handle */
            }
            break;

        default:
            break;
    }

    APPL_DBG("<< appl_ga_utils_hci_event_callback\n");

    return retval;
}

GA_RESULT appl_ga_utils_hci_register_callback(void)
{
    GA_RESULT retval;

    /* Initialize */
    retval = GA_FAILURE;

    retval = appl_hci_register_callback(&appl_ga_utils_hci_event_callback);

    return retval;
}

GA_RESULT appl_ga_utils_hci_unregister_callback(void)
{
    GA_RESULT retval;

    retval = appl_hci_unregister_callback(&appl_ga_utils_hci_event_callback);

    return retval;
}

void appl_ga_utils_hci_add_ea_to_db(UINT8* addr, UINT8 addr_type)
{
    UINT8 index;

    if (ea_db_count < APPL_GA_EA_DB_COUNT)
    {
        for (index = 0U; index < GA_BD_ADDR_SIZE; index++)
        {
            ea_db[ea_db_count].bd_addr[index] = *(addr + index);
        }
        ea_db[ea_db_count].bd_type = addr_type;
        ea_db_count++;
    }

    return;
}

void appl_ga_utils_hci_clear_ea_db(void)
{
    UINT8 index;

    for (index = 0U; index < ea_db_count; index++)
    {
        GA_INIT_ENDPOINT(&ea_db[index]);
    }
    ea_db_count = 0U;
}

UINT8 appl_ga_utils_hci_is_ea_in_db(UINT8* addr, UINT8 addr_type)
{
    UINT16 index;

    if (ea_db_count != 0U)
    {
        for (index = 0U; index < ea_db_count; index++)
        {
            if (GA_COMPARE_ENDPOINT_ADDR
                (
                    &addr[0U],
                    &ea_db[index].bd_addr[0U]
                ))
            {
                if (GA_COMPARE_ENDPOINT_TYPE(addr_type, ea_db[index].bd_type))
                {
                    return GA_TRUE;
                }
            }
        }
    }

    return GA_FALSE;
}

void appl_ga_utils_hci_extract_addr_from_ea(UINT8 *ea_report, UINT16 ea_reportlen, GA_ENDPOINT * device)
{
    UINT8 marker;
    UINT8 num_report;
    UINT8 count;
    UINT8 i;

    if ((NULL == ea_report) || (0U == ea_reportlen) || (NULL == device))
    {
        return;
    }

    marker = 0U;

    /* HCI_LE_EXTENDED_ADVERTISING_REPORT_SUBEVENT is ga_event_data[0U] */
    marker++;
    /* num_reports */
    num_report = ea_report[marker];
    marker++;

    /* Print the reports */
    for (count = 0; count < num_report; count++)
    {
        /* Event Type: 2 Octet */
        marker += 2U;

        /* Address Type: 1 Octet */
        marker++;
        device->bd_type = ea_report[marker];

        /* Address: 6 Octet */
        for (i = 0U; i < GA_BD_ADDR_SIZE; i++)
        {
            device->bd_addr[i] = ea_report[marker];
            marker++;
        }
    }
}

#endif /* BT_GAM */
