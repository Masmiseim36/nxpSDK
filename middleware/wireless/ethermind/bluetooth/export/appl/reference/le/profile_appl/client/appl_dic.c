
/**
 *  \file appl_dic.c
 *
 *  This is a Device Information Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"


#if (defined ATT && defined DIC)

/* ----------------------------------------- Macro Defines */

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE dis_man_nam_hdl;
static ATT_ATTR_HANDLE dis_mod_num_hdl;
static ATT_ATTR_HANDLE dis_ser_num_hdl;
static ATT_ATTR_HANDLE dis_hdw_rev_hdl;
static ATT_ATTR_HANDLE dis_fmw_rev_hdl;
static ATT_ATTR_HANDLE dis_sfw_rev_hdl;
static ATT_ATTR_HANDLE dis_sys_idn_hdl;
static ATT_ATTR_HANDLE dis_reg_crt_hdl;
static ATT_ATTR_HANDLE dis_pnp_id_hdl;

static const UCHAR dic_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\
    --- Device Information Service ---\n\
    2 - Discover Device Information Service/Characteristics\n\
    3 - Read Manufacturer Name\n\
    4 - Read Model Number\n\
    5 - Read Serial Number\n\
    6 - Read Firmware Revision\n\
    7 - Read Hardware Revision\n\
    8 - Read Software Revision\n\
    9 - Read System ID\n\
   10 - Read Reg Cert Data List\n\
   11 - Read PnP ID\n\
   Your Option?";

/* ------------------------------- Functions */

void dic_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i, j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_MANUFACTURER_NAME_CHARACTERISTIC:
                dis_man_nam_hdl = characteristic->value_handle;
                break;

            case GATT_MODEL_NUMBER_CHARACTERISTIC:
                dis_mod_num_hdl = characteristic->value_handle;
                break;

            case GATT_SERIAL_NUMBER_CHARACTERISTIC:
                dis_ser_num_hdl = characteristic->value_handle;
                break;

            case GATT_HARDWARE_REV_CHARACTERISTIC:
                dis_hdw_rev_hdl = characteristic->value_handle;
                break;

            case GATT_FIRMWARE_REV_CHARACTERISTIC:
                dis_fmw_rev_hdl = characteristic->value_handle;
                break;

            case GATT_SOFTWARE_REV_CHARACTERISTIC:
                dis_sfw_rev_hdl = characteristic->value_handle;
                break;

            case GATT_SYSTEM_ID_CHARACTERISTIC:
                dis_sys_idn_hdl = characteristic->value_handle;
                break;

            case GATT_REG_CERT_DATA_CHARACTERISTIC:
                dis_reg_crt_hdl = characteristic->value_handle;
                break;

           case GATT_PNP_ID_CHARACTERISTIC:
                dis_pnp_id_hdl = characteristic->value_handle;
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

void dic_profile_operations (void)
{
    UINT32     choice, menu_choice;
    ATT_UUID   uuid;
    API_RESULT retval;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT ("%s \n", dic_client_menu);
        LOG_DEBUG ("Enter your choice : ");
        CONSOLE_IN ("%u", &choice);

        menu_choice = choice;

        switch(choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 2:
            uuid.uuid_16 = GATT_DEVICE_INFO_SERVICE;
            retval = gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            LOG_DEBUG ("retval of discovery proc = %04x\n", retval);
            break;

        case 3:
            uuid.uuid_16 = 0x0000U;
            LOG_DEBUG ("dis_man_nam_hdl = %04x\n", dis_man_nam_hdl);
            retval = gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_man_nam_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            LOG_DEBUG ("retval = %04x\n", retval);
            break;

        case 4:
            uuid.uuid_16 = 0x0000U;
            LOG_DEBUG ("dis_mod_num_hdl = %04x\n", dis_man_nam_hdl);
            retval = gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_mod_num_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            LOG_DEBUG ("retval = %04x\n", retval);
            break;

        case 5:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_ser_num_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 6:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_fmw_rev_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 7:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_hdw_rev_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 8:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_sfw_rev_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 9:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_sys_idn_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 10:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_reg_crt_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                dis_pnp_id_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        default:
            LOG_DEBUG("Invalid Choice\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return; */
            break;
        }
    }

    return;
}

#endif /* (defined ATT && defined DIC) */
