
/**
 *  \file appl_hidh.c
 *
 *  This is a HID Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"

#if (defined ATT && defined HIDH)
/* ----------------------------------------- Macro Defines */
#define HIDS_INPUT_REPORT_LEN                          1U
#define HIDS_OUTPUT_REPORT_LEN                         1U
#define HIDS_FEATURE_REPORT_LEN                        1U
#define HIDS_BOOT_KB_IN_REPORT_LEN                     2U
#define HIDS_BOOT_KB_OUT_REPORT_LEN                    2U
#define HIDS_BOOT_MUS_IN_REPORT_LEN                    4U
#define HIDS_INPUT_REPORT_CCC_LEN                      2U
#define HIDS_BOOT_KB_IN_REPORT_CCC_LEN                 2U
#define HIDS_BOOT_MUS_IN_REPORT_CCC_LEN                2U

#define APPL_HID_EXT_REPORT_REF_DESC_UUID              0x2907U
#define APPL_HID_REPORT_REF_DESC_UUID                  0x2908U

/* Number of supported HID services */
#define APPL_MAX_SERVICE_INSTANCES                     2U

/* Initialize value for GATT handle */
#define APPL_HID_GATT_HANDLE_INIT_VAL                  0x0000U

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
void hidh_initialize(void);
void hidh_init_service_instance(void);
void appl_select_hid_service_instances(void);
API_RESULT hidh_find_free_hndl_range_inst
           (
               /* OUT */ UCHAR * index
           );

static ATT_ATTR_HANDLE hids_report_map_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_ext_report_ref_desc_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_input_report_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_input_report_ref_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_input_report_ccc_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_output_report_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_output_report_ref_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_feature_report_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_feature_report_ref_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_hid_info_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_boot_kb_in_report_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_boot_kb_in_report_ccc_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_boot_kb_out_report_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_boot_mus_in_report_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_boot_mus_in_report_ccc_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_proto_mode_hdl[APPL_MAX_SERVICE_INSTANCES];
static ATT_ATTR_HANDLE hids_hid_cp_hdl[APPL_MAX_SERVICE_INSTANCES];

static ATT_ATTR_HANDLE appl_hids_curr_rd_attr_hndl;

DECL_STATIC ATT_HANDLE_RANGE appl_hid_serv_range[APPL_MAX_SERVICE_INSTANCES];
DECL_STATIC UCHAR service_count;
DECL_STATIC UCHAR appl_cur_service_inst;
DECL_STATIC UCHAR appl_hidh_disc_state = BT_FALSE;

static const UCHAR hidh_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
   --- HID Service ---\n\
   10 - Discover HID Service\n\
   11 - Discover Included Services\n\
   12 - Discover HID Characteristics\n\
   13 - Read Report Map\n\
   14 - Read External Report Reference Descriptor\n\
   15 - Read Input Report\n\
   16 - Read Report Reference Descriptor for Input Report\n\
   17 - Read Client Configuration Descriptor for Input Report\n\
   18 - Read Output Report\n\
   19 - Read Report Reference Descriptor for Output Report\n\
   20 - Read Feature Report\n\
   21 - Read Report Reference Descriptor for Feature Report\n\
   22 - Read HID Information\n\
   23 - Read Boot Keyboard Input Report\n\
   24 - Read Client Configuration Descriptor for Boot Keyboard Input Report\n\
   25 - Read Boot Keyboard Output Report\n\
   26 - Read Boot Mouse Input Report\n\
   27 - Read Client Configuration Descriptor for Boot Mouse Input Report\n\
   28 - Read Protocol Mode\n\
   29 - Write Input Report\n\
   30 - Write Output Report\n\
   31 - Write Command Output Report\n\
   32 - Write Feature Report\n\
   33 - Set Protocol Mode\n\
   34 - Write Boot Keyboard Input Report\n\
   35 - Write Boot Keyboard Output Report\n\
   36 - Write Boot Mouse Input Report\n\
   37 - Configure HID Control Point\n\
   38 - Enable/Disable HID Report\n\
   39 - Enable/Disable Boot Keyboard Input Report CCD\n\
   40 - Enable/Disable Boot Mouse Input Report CCD\n\
   41 - Select the instance of the service\n\n\
   42 - DIS Options\n\n\
   43 - SPC Options\n\
Your Option?\n\
";

/* ------------------------------- Functions */

void hidh_init_service_instance(void)
{
    UINT32 k;

    /* Find the Free element in the given array */
    for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES; k++)
    {
        appl_hid_serv_range[k].start_handle = APPL_HID_GATT_HANDLE_INIT_VAL;
        appl_hid_serv_range[k].end_handle   = APPL_HID_GATT_HANDLE_INIT_VAL;
    }

    /* Reset the associated Globals */
    service_count         = 0U;
    appl_cur_service_inst = 0U;
}

void hidh_initialize(void)
{
    UINT32 k;

    for(k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
    {
        hids_proto_mode_hdl[k]              = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_input_report_hdl[k]            = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_output_report_hdl[k]           = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_feature_report_hdl[k]          = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_input_report_ref_hdl[k]        = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_output_report_ref_hdl[k]       = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_feature_report_ref_hdl[k]      = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_report_map_hdl[k]              = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_boot_kb_in_report_hdl[k]       = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_boot_mus_in_report_hdl[k]      = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_boot_kb_out_report_hdl[k]      = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_hid_info_hdl[k]                = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_hid_cp_hdl[k]                  = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_input_report_ccc_hdl[k]        = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_ext_report_ref_desc_hdl[k]     = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_boot_kb_in_report_ccc_hdl[k]   = APPL_HID_GATT_HANDLE_INIT_VAL;
        hids_boot_mus_in_report_ccc_hdl[k]  = APPL_HID_GATT_HANDLE_INIT_VAL;
    }

    /* Reset the associated Globals */
    appl_hids_curr_rd_attr_hndl = APPL_HID_GATT_HANDLE_INIT_VAL;

    /* Initialize the HID Service Instance List */
    hidh_init_service_instance();
}

void hidh_notify_gatt_conn (void)
{
    hidh_initialize();
}

void hidh_notify_gatt_disconn (void)
{
    hidh_initialize();
}

void hidh_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;
    UINT16 j;
    UINT16 k;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_HID_PROTO_MODE_CHARACTERISTIC:
                for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                {
                    /* Allocate only if an Uninitialized location is found */
                    if(hids_proto_mode_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                    {
                        hids_proto_mode_hdl[k] = characteristic->value_handle;
                        break;
                    }
                }
                break;

            case GATT_HID_RPT_CHARACTERISTIC:
                /**
                 *  Add a check to populate Input,Output or Feature Report handle with
                 *  respect to characteristic property
                 */
                for (j = 0U; j < characteristic->desc_index; j++)
                {
                    if (APPL_HID_REPORT_REF_DESC_UUID ==
                         GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                    {

                        if (GATT_DB_CHAR_NOTIFY_PROPERTY == (characteristic->cproperty & GATT_DB_CHAR_NOTIFY_PROPERTY))
                        {
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                if(hids_input_report_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                    /* To populate Input Report Characteristic handle */
                                    hids_input_report_hdl[k] =  characteristic->value_handle;
                                    break;
                                }
                            }
                            /* To populate Input Report Descriptor handle*/
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                if(hids_input_report_ref_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                    hids_input_report_ref_hdl[k] =  characteristic->descriptor[j].handle;
                                    break;
                                }

                            }
                        }
                        else if (GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY == (characteristic->cproperty & GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY))
                        {
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                if(hids_output_report_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                    /* To populate Output Report Characteristic handle*/
                                    hids_output_report_hdl[k] =  characteristic->value_handle;
                                    break;
                                }
                            }
                            /* To populate Output Report Descriptor handle */
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                if(hids_output_report_ref_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                    /* To populate Output Report Characteristic handle*/
                                    hids_output_report_ref_hdl[k] =  characteristic->descriptor[j].handle;
                                    break;
                                }

                            }
                        }
                        else
                        {
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                if(hids_feature_report_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                     /* To populate Feature Report Characteristic handle */
                                     hids_feature_report_hdl[k] =  characteristic->value_handle;
                                     break;
                                }
                            }

                            /* To populate Feature Report Descriptor handle */
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                if(hids_feature_report_ref_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                    hids_feature_report_ref_hdl[k] =  characteristic->descriptor[j].handle;
                                    break;
                                }

                            }
                        }
                    }
                }
                break;

            case GATT_HID_RPT_MAP_CHARACTERISTIC:
                for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                {
                    if(hids_report_map_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                    {
                        hids_report_map_hdl[k] = characteristic->value_handle;
                        break;
                    }

                }

                /* Check for External Report reference  Descriptor and populate it*/
                for (j = 0U; j < characteristic->desc_index; j++)
                {
                    if (APPL_HID_EXT_REPORT_REF_DESC_UUID ==
                           GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                    {
                        for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                        {
                            if(hids_ext_report_ref_desc_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                            {
                                hids_ext_report_ref_desc_hdl[k] = characteristic->descriptor[j].handle;
                                break;
                            }
                        }
                    }
                }
                break;

            case GATT_BOOT_KYBRD_IN_RPT_CHARACTERISTIC:
                for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                {
                    /* Allocate only if an Uninitialized location is found */
                    if(hids_boot_kb_in_report_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                    {
                       hids_boot_kb_in_report_hdl[k] = characteristic->value_handle;
                       break;
                    }
                }
                break;

            case GATT_BOOT_KYBRD_OP_CHARACTERISTIC:
                for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                {
                    /* Allocate only if an Uninitialized location is found */
                    if(hids_boot_kb_out_report_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                    {
                       hids_boot_kb_out_report_hdl[k] = characteristic->value_handle;
                       break;
                    }
                }
                break;

            case GATT_MOUSE_IP_CHARACTERISTIC:
                for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                {
                    /* Allocate only if an Uninitialized location is found */
                    if(hids_boot_mus_in_report_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                    {
                       hids_boot_mus_in_report_hdl[k] = characteristic->value_handle;
                       break;
                    }
                }
                break;

            case GATT_HID_INFO_CHARACTERISTIC:
                for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                {
                    /* Allocate only if an Uninitialized location is found */
                    if(hids_hid_info_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                    {
                       hids_hid_info_hdl[k] = characteristic->value_handle;
                       break;
                    }
                }
                break;

            case GATT_HID_CP_CHARACTERISTIC:
                for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                {
                    /* Allocate only if an Uninitialized location is found */
                    if(hids_hid_cp_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                    {
                       hids_hid_cp_hdl[k] = characteristic->value_handle;
                       break;
                    }
                }
                break;

            default:
                /* CONSOLE_OUT("Invalid...\n"); */
                break;
        }

        if (0U != characteristic->desc_index)
        {
            for (j = 0U; j < characteristic->desc_index; j++)
            {
                if (GATT_CLIENT_CONFIG ==
                    GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                {
                    switch (GATT_GET_CHAR_UUID(characteristic))
                    {
                        case GATT_MOUSE_IP_CHARACTERISTIC:
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                /* Allocate only if an Uninitialized location is found*/
                                if(hids_boot_mus_in_report_ccc_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                    hids_boot_mus_in_report_ccc_hdl[k] = characteristic->descriptor[j].handle;
                                    break;
                                }
                            }
                            break;

                        case GATT_BOOT_KYBRD_IN_RPT_CHARACTERISTIC:
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                /* Allocate only if an Uninitialized location is found*/
                                if(hids_boot_kb_in_report_ccc_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                    hids_boot_kb_in_report_ccc_hdl[k] = characteristic->descriptor[j].handle;
                                    break;
                                }
                            }
                            break;

                        case GATT_HID_RPT_CHARACTERISTIC:
                            for (k = 0U; k < APPL_MAX_SERVICE_INSTANCES ; k++)
                            {
                                /* Allocate only if an Uninitialized location is found*/
                                if(hids_input_report_ccc_hdl[k] == APPL_HID_GATT_HANDLE_INIT_VAL)
                                {
                                    hids_input_report_ccc_hdl[k] = characteristic->descriptor[j].handle;
                                    break;
                                }
                            }
                            break;

                        default:
                            /* CONSOLE_OUT("Invalid...\n"); */
                            break;
                    }

                    break;
                }
            }
        }

        characteristic ++;
    }
}

void appl_select_hid_service_instances(void)
{
    int   read_val;
    UINT8 index = 0U;

    APPL_TRC ("Enter the desired HID Instance:\n");

    APPL_TRC ("Instance -  Start Handle  |  End Handle\n");
    for (index = 0U; index < service_count; index++)
    {
        APPL_TRC ("   %d     -     0x%04X    |     0x%04X\n",index,
            appl_hid_serv_range[index].start_handle,
            appl_hid_serv_range[index].end_handle);
    }

    APPL_TRC ("Select the range for characteristics discovery:");
    fflush (stdout);
    CONSOLE_IN ( "%u",&read_val);
    appl_cur_service_inst = (UCHAR) read_val;

    return;
}

API_RESULT hidh_find_free_hndl_range_inst
           (
               /* OUT */ UCHAR * index
           )
{
    UINT32 i;
    API_RESULT retval;

    retval = API_FAILURE;

    /* Find the Free element in the given array */
    for (i = 0U; i < APPL_MAX_SERVICE_INSTANCES; i++)
    {
        if (appl_hid_serv_range[i].start_handle == APPL_HID_GATT_HANDLE_INIT_VAL)
        {
            *index = (UCHAR)i;
            retval = API_SUCCESS;
            break;
        }
    }

    return retval;
}

void hidh_notify_gatt_servdata (GATT_SERVICE_PARAM * service, UINT16 size)
{
    API_RESULT retval;
    UCHAR i;

    BT_IGNORE_UNUSED_PARAM(size);

    /**
     * Save the Service Discovery Handle only when Dedicated HIDH Discovery
     * was Initiated from the Menu Option here.
     */
    if (BT_TRUE == appl_hidh_disc_state)
    {
        retval = hidh_find_free_hndl_range_inst(&i);

        if ((API_SUCCESS == retval) && (GATT_HID_SERVICE == service->uuid.uuid_16))
        {
            /* Populate the incoming Service Handle Range in the free index */
            appl_hid_serv_range[i].start_handle = service->range.start_handle;
            appl_hid_serv_range[i].end_handle = service->range.end_handle;

            /* Increment the HID Service Count For each Handle Range Discovered */
            service_count++;
        }
        else
        {
            APPL_ERR ("[HIDH]: **ERR** Max Service Instances cache limit reached!\n");
        }
    }
}

void hidh_parse_read_data(UCHAR * data, UINT16 datalen)
{
    ATT_READ_BLOB_REQ_PARAM read_blob_req_param;
    API_RESULT              retval;
    UINT16                  curr_mtu;

    BT_IGNORE_UNUSED_PARAM(data);

    /* Initialize */
    retval = API_SUCCESS;

    /* Access the Local MTU for the current ATT_HANDLE */
    retval = BT_att_access_mtu
             (
                 &appl_gatt_client_handle,
                 &curr_mtu
             );

    /**
     * Check if incoming datalen is around Max ATT data for the
     * current ATT Bearer's negotiated MTU.
     * If the datalen matches the negotiated ATT MTU, then there
     * is possibility more data for the corresponding Attribute Handle.
     * Use the Read Blob Request to fetch any remaining data if present.
     */
    if ((API_SUCCESS == retval) && (curr_mtu <= (datalen + 1U)))
    {
        read_blob_req_param.handle = appl_hids_curr_rd_attr_hndl;
        read_blob_req_param.offset = datalen;

        gatt_char_read_long
        (
            &appl_gatt_client_handle,
            read_blob_req_param.handle,
            read_blob_req_param.offset
        );
    }
}

void hidh_profile_operations (void)
{
    UINT32                  choice, menu_choice;
    UCHAR                   ccc_value[10U];
    UCHAR                   cp_operation[2U];
    UCHAR                   proto_mode[2U];
    UINT16                  length;
    ATT_UUID                uuid;

    /* Set the HIDH Discovery State to True once inside HIDH Menu */
    appl_hidh_disc_state = BT_TRUE;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",hidh_client_menu);
        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_HID_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            gatt_discover_is
            (
                &appl_gatt_client_handle,
                0x0001U,
                0xFFFFU
            );
            break;

        case 12:
            /* Select the HID Instance */
            appl_select_hid_service_instances();

            gatt_discover_char
            (
                &appl_gatt_client_handle,
                appl_hid_serv_range[appl_cur_service_inst].start_handle,
                appl_hid_serv_range[appl_cur_service_inst].end_handle,
                0x0000U,
                1U
            );
            break;

        case 13:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_report_map_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_report_map_hdl[appl_cur_service_inst];
            break;

        case 14:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_ext_report_ref_desc_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_ext_report_ref_desc_hdl[appl_cur_service_inst];
            break;

        case 15:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_input_report_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_input_report_hdl[appl_cur_service_inst];
            break;

        case 16:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_input_report_ref_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_input_report_ref_hdl[appl_cur_service_inst];
            break;

        case 17:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_input_report_ccc_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_input_report_ccc_hdl[appl_cur_service_inst];
            break;

        case 18:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_output_report_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_output_report_hdl[appl_cur_service_inst];
            break;

        case 19:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_output_report_ref_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_output_report_ref_hdl[appl_cur_service_inst];
            break;

        case 20:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_feature_report_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_feature_report_hdl[appl_cur_service_inst];
            break;

        case 21:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_feature_report_ref_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_feature_report_ref_hdl[appl_cur_service_inst];
            break;

        case 22:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_hid_info_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_hid_info_hdl[appl_cur_service_inst];
            break;

        case 23:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_boot_kb_in_report_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_boot_kb_in_report_hdl[appl_cur_service_inst];
            break;

        case 24:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_boot_kb_in_report_ccc_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_boot_kb_in_report_ccc_hdl[appl_cur_service_inst];
            break;

        case 25:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_boot_kb_out_report_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_boot_kb_out_report_hdl[appl_cur_service_inst];
            break;

        case 26:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_boot_mus_in_report_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_boot_mus_in_report_hdl[appl_cur_service_inst];
            break;

        case 27:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_boot_mus_in_report_ccc_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_boot_mus_in_report_ccc_hdl[appl_cur_service_inst];
            break;

        case 28:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                hids_proto_mode_hdl[appl_cur_service_inst],
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Store the Read Attribute Handle in global */
            appl_hids_curr_rd_attr_hndl = hids_proto_mode_hdl[appl_cur_service_inst];
            break;

        case 29:
            /* Report value as expected by PTS */
            ccc_value[4U] = 0xCDU;
            ccc_value[3U] = 0xA6U;
            ccc_value[2U] = 0xF8U;
            ccc_value[1U] = 0xB3U;
            ccc_value[0U] = 0xAAU;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_input_report_hdl[appl_cur_service_inst],
                ccc_value,
                5U,
                0x01U
            );
            break;

        case 30:
            /* Report value as expected by PTS */
            ccc_value[6U] = 0x00U;
            ccc_value[5U] = 0x12U;
            ccc_value[4U] = 0x34U;
            ccc_value[3U] = 0x56U;
            ccc_value[2U] = 0x78U;
            ccc_value[1U] = 0x90U;
            ccc_value[0U] = 0xEFU;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_output_report_hdl[appl_cur_service_inst],
                ccc_value,
                7U,
                0x01U
            );
            break;

        case 31:
            /* Report value as expected by PTS */
            ccc_value[4U] = 0x87U;
            ccc_value[3U] = 0x2DU;
            ccc_value[2U] = 0x3FU;
            ccc_value[1U] = 0x45U;
            ccc_value[0U] = 0xEAU;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_feature_report_hdl[appl_cur_service_inst],
                ccc_value,
                5U,
                0x01U
            );
            break;

        case 32:
            ccc_value[0U] = 0x03U;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_feature_report_hdl[appl_cur_service_inst],
                ccc_value,
                0x0001U,
                0x01U
            );
            break;

        case 33:
            CONSOLE_OUT ("Select Protocol Mode. 0 (Boot), 1 (Report)\n");
            CONSOLE_IN ("%u", &choice);

            proto_mode[0U] = (UCHAR)choice;
            length = sizeof(proto_mode[0U]);

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_proto_mode_hdl[appl_cur_service_inst],
                proto_mode,
                length,
                0x00U
            );
            break;

        case 34:
            ccc_value[0U] = 0xFFU;
            ccc_value[1U] = 0xFFU;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_boot_kb_in_report_hdl[appl_cur_service_inst],
                ccc_value,
                HIDS_BOOT_KB_IN_REPORT_LEN,
                0x01U
            );
            break;

        case 35:
            ccc_value[0U] = 0xEEU;
            ccc_value[1U] = 0xEEU;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_boot_kb_out_report_hdl[appl_cur_service_inst],
                ccc_value,
                HIDS_BOOT_KB_OUT_REPORT_LEN,
                0x01U
            );
            break;

        case 36:
            ccc_value[0U] = 0xFFU;
            ccc_value[1U] = 0xFFU;
            ccc_value[2U] = 0xFFU;
            ccc_value[3U] = 0xFFU;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_boot_mus_in_report_hdl[appl_cur_service_inst],
                ccc_value,
                HIDS_BOOT_MUS_IN_REPORT_LEN,
                0x01U
            );
            break;

        case 37:
            CONSOLE_OUT ("Select Control Point Operation. 0 (Suspend), 1 (Exit Suspend)\n");
            CONSOLE_IN ("%u", &choice);

            cp_operation[0U] = (UCHAR)choice;
            length = sizeof(cp_operation[0U]);

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_hid_cp_hdl[appl_cur_service_inst],
                cp_operation,
                length,
                0x00U
            );
            break;

        case 38:
            CONSOLE_OUT ("1 (Enable), 0 (Disable)\n");
            CONSOLE_IN ("%u", &choice);

            ccc_value[0U] = (UCHAR)choice;
            ccc_value[1U] = 0x00U;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_input_report_ccc_hdl[appl_cur_service_inst],
                ccc_value,
                HIDS_INPUT_REPORT_CCC_LEN,
                0x01U
            );
            break;

        case 39:
            CONSOLE_OUT ("1 (Enable), 0 (Disable)\n");
            CONSOLE_IN ("%u", &choice);

            ccc_value[0U] = (UCHAR)choice;
            ccc_value[1U] = 0x00U;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_boot_kb_in_report_ccc_hdl[appl_cur_service_inst],
                ccc_value,
                HIDS_BOOT_KB_IN_REPORT_CCC_LEN,
                0x01U
            );
            break;

        case 40:
            CONSOLE_OUT ("1 (Enable), 0 (Disable)\n");
            CONSOLE_IN ("%u", &choice);

            ccc_value[0U] = (UCHAR)choice;
            ccc_value[1U] = 0x00U;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                hids_boot_mus_in_report_ccc_hdl[appl_cur_service_inst],
                ccc_value,
                HIDS_BOOT_MUS_IN_REPORT_CCC_LEN,
                0x01U
            );
            break;

        case 41:
            /* Select the HID Service Instance for Read and Write Operations */
            appl_select_hid_service_instances();
            break;

        case 42:
#ifdef DIC
            dic_profile_operations ();
#endif /* DIC */
            break;

        case 43:
#ifdef SPC
           spc_profile_operations ();
#else
           CONSOLE_OUT("\nSPC is not enabled!!!\n");
#endif /* SPC */
           break;

        default:
           CONSOLE_OUT("Invalid Choice\n");
           break;
        }

        if (0 == menu_choice)
        {
            /* Reset the HIDH Discovery State to False when out of HIDH Menu */
            appl_hidh_disc_state = BT_FALSE;

            /* return */
            break;
        }
    }

    return;
}

#endif /* (defined ATT && defined HIDH) */
