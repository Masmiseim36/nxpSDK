
/**
 *  \file appl_tic.c
 *
 *  This is a Time Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"

#if (defined ATT && defined TIC)
/* ----------------------------------------- Macro Defines */
#define APPL_TIC_TIME_INFO_ARRAY_LEN                0x0AU
#define APPL_TIC_YEAR_OFFSET                        0U
#define APPL_TIC_MONTH_OFFSET                       2U
#define APPL_TIC_DAY_OFFSET                         3U
#define APPL_TIC_HOUR_OFFSET                        4U
#define APPL_TIC_MINUTE_OFFSET                      5U
#define APPL_TIC_SECOND_OFFSET                      6U
#define APPL_TIC_DOW_OFFSET                         7U
#define APPL_TIC_F256_OFFSET                        8U
#define APPL_TIC_ADJUST_REASON_OFFSET               9U
/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE cts_current_time_hdl;
static ATT_ATTR_HANDLE cts_current_time_ccc_hdl;
static ATT_ATTR_HANDLE cts_local_time_info_hdl;
static ATT_ATTR_HANDLE cts_ref_time_info_hdl;
static ATT_ATTR_HANDLE rtus_time_update_cp_hdl;
static ATT_ATTR_HANDLE rtus_time_update_state_hdl;
static ATT_ATTR_HANDLE ndcs_time_with_dst;

static UCHAR tic_client_menu[] =
"\n\
    0.  Exit\n\
    1.  Refresh\n\n\
       === Current Time Service ===\n\
   10.  Discover Current Time Service\n\
        -- Current Time --\n\
   11.  Read Current Time\n\
   12.  Configure Current Time CCC\n\
   13.  Stop Current Time CCC\n\
   14.  Write Current Time Information\n\
        -- Local Time Information --\n\
   15.  Read Local Time Information\n\
   16.  Write Local Time Information\n\
        -- Reference Time Information --\n\
   17.  Read Reference Time Information\n\n\
       === Reference Time Service ===\n\
   20.  Discover Reference Time Update Service\n\
        -- Reference Time Update --\n\
   21.  Configure Time Update Control Point\n\
   22.  Read Time Update State\n\n\
       === Next DST Change Service ===\n\
   30.  Discover Next DST Change Service\n\
        -- Time with DST --\n\
   31.  Read Time with DST\n\
  "
"Your Option?\n\
";

/* ------------------------------- Functions */

void tic_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;
    UINT16 j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_CURRENT_TIME_CHARACTERISTIC:
                cts_current_time_hdl = characteristic->value_handle;
                break;

            case GATT_LOCAL_TIME_INFO_CHARACTERISTIC:
                cts_local_time_info_hdl = characteristic->value_handle;
                break;

            case GATT_REF_TIME_INFO_CHARACTERISTIC:
                cts_ref_time_info_hdl = characteristic->value_handle;
                break;

            case GATT_TIME_UPDATE_CONTROL_POINT:
                rtus_time_update_cp_hdl = characteristic->value_handle;
                break;

            case GATT_TIME_UPDATE_STATE_CHARACTERISTIC:
                rtus_time_update_state_hdl = characteristic->value_handle;
                break;

            case GATT_TIME_WITH_DST_CHARACTERISTIC:
                ndcs_time_with_dst = characteristic->value_handle;
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
                        case GATT_CURRENT_TIME_CHARACTERISTIC:
                            cts_current_time_ccc_hdl = characteristic->descriptor[j].handle;
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


void tic_profile_operations (void)
{
    int          choice, menu_choice;
    UINT16       cli_cfg;
    UCHAR        cfg_val[GATT_CLI_CFG_VAL_LEN];
    ATT_UUID     uuid;
    UCHAR        time_update_cp_value;
    UCHAR        curr_time_info[APPL_TIC_TIME_INFO_ARRAY_LEN];

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT ("%s", tic_client_menu);
        CONSOLE_IN ( "%d", &choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_CURRENT_TIME_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                cts_current_time_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 12:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cts_current_time_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 13:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cts_current_time_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 14:
            /* Year */
            LOG_DEBUG ("Enter year :");
            CONSOLE_IN ("%u", &choice);
            BT_PACK_LE_2_BYTE(&curr_time_info[APPL_TIC_YEAR_OFFSET],&choice);

            /* Month */
            LOG_DEBUG ("Enter month:");
            CONSOLE_IN ("%u", &choice);
            curr_time_info[APPL_TIC_MONTH_OFFSET] = (UCHAR)choice;

            /* Day */
            LOG_DEBUG ("Enter Day:");
            CONSOLE_IN ("%u", &choice);
            curr_time_info[APPL_TIC_DAY_OFFSET] = (UCHAR)choice;

            /* Hour */
            LOG_DEBUG ("Enter hour:");
            CONSOLE_IN ("%u", &choice);
            curr_time_info[APPL_TIC_HOUR_OFFSET] = (UCHAR)choice;

            /* Minute */
            LOG_DEBUG ("Enter minute:");
            CONSOLE_IN ("%u", &choice);
            curr_time_info[APPL_TIC_MINUTE_OFFSET] = (UCHAR)choice;

            /* Second */
            LOG_DEBUG ("Enter second:");
            CONSOLE_IN ("%u", &choice);
            curr_time_info[APPL_TIC_SECOND_OFFSET] = (UCHAR)choice;

            /* Day of week */
            LOG_DEBUG ("Enter Day of week:");
            CONSOLE_IN ("%u", &choice);
            curr_time_info[APPL_TIC_DOW_OFFSET] = (UCHAR)choice;

            /* enter fraction 256 */
            LOG_DEBUG ("Enter fraction 256:");
            CONSOLE_IN ("%u", &choice);
            curr_time_info[APPL_TIC_F256_OFFSET] = (UCHAR)choice;

            /* Adjust Reason */
            LOG_DEBUG ("Enter adjust reason:");
            CONSOLE_IN ("%u", &choice);
            curr_time_info[APPL_TIC_ADJUST_REASON_OFFSET] = (UCHAR)choice;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cts_current_time_hdl,
                curr_time_info,
                APPL_TIC_TIME_INFO_ARRAY_LEN,
                0x01U
            );
            break;

        case 15:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                cts_local_time_info_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 16:
            /* Timezone */
            LOG_DEBUG("Enter Timezone: 0x");
            CONSOLE_IN("%x", &choice);
            cfg_val[0U] = (UCHAR)choice;

            /* DST Offset */
            LOG_DEBUG("Enter DST Offset: 0x");
            CONSOLE_IN("%x", &choice);
            cfg_val[1U] = (UCHAR)choice;

            gatt_char_wr
            (
                &appl_gatt_client_handle,
                cts_local_time_info_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 17:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                cts_ref_time_info_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
             );
             break;

        case 20:
            uuid.uuid_16 = GATT_REF_TIME_UPDATE_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 21:
            LOG_DEBUG ("1 (Get Reference Update), 2 (Cancel Reference Update)\n");
            CONSOLE_IN ("%u", &choice);
            time_update_cp_value = (UCHAR) choice;
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                rtus_time_update_cp_hdl,
                &time_update_cp_value,
                1U,
                0x00U
            );
            break;

        case 22:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                rtus_time_update_state_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 30:
            uuid.uuid_16 = GATT_NEXT_DST_CHANGE_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 31:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                ndcs_time_with_dst,
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
            /* return */
            break;
        }
    }

    return;
}

#endif /* (defined ATT && defined TIC) */

