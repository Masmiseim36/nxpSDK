
/**
 *  \file appl_bpc.c
 *
 *  This is a Blood Pressure Collector application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"


#if (defined ATT && defined BPC)
/* ----------------------------------------- Macro Defines */

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE bps_bp_msrmt_ccc_hdl;
static ATT_ATTR_HANDLE bps_intr_cuff_prsr_ccc_hdl;
static ATT_ATTR_HANDLE bps_bp_feature_hdl;
static ATT_ATTR_HANDLE bps_bp_msrmt_hdl;
static ATT_ATTR_HANDLE bps_intr_cuff_prsr_hdl;

static const UCHAR bpc_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\n\
   --- Blood Pressure Service ---\n\
   10 - Discover Blood Pressure Service\n\
   11 - Configure Blood Pressure Measurement for Indication\n\
   12 - Stop Blood Pressure Measurement Indication\n\
   13 - Configure Intermediate Cuff Pressure for Notification\n\
   14 - Stop Intermediate Cuff Pressure Notification\n\
   15 - Read Blood Pressure Feature\n\n\
   50 - Device Information Service Option\n\n\
   60 - Battery Service Option\n\
Your Option?\n\
";

/* ------------------------------- Functions */

void bpc_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i;
    UINT16 j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_BP_MSRMT_CHARACTERISTIC:
                bps_bp_msrmt_hdl = characteristic->value_handle;
                break;

            case GATT_INTRMDT_CUFF_PRSR_CHARACTERISTIC:
                bps_intr_cuff_prsr_hdl = characteristic->value_handle;
                break;

            case GATT_BP_FEATURE_CHARACTERISTIC:
                bps_bp_feature_hdl = characteristic->value_handle;
                break;

            default:
                /* LOG_DEBUG("Invalid...\n"); */
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
                        case GATT_BP_MSRMT_CHARACTERISTIC:
                            bps_bp_msrmt_ccc_hdl = characteristic->descriptor[j].handle;
                            break;

                        case GATT_INTRMDT_CUFF_PRSR_CHARACTERISTIC:
                            bps_intr_cuff_prsr_ccc_hdl = characteristic->descriptor[j].handle;
                            break;

                        default:
                            /* LOG_DEBUG("Invalid...\n"); */
                            break;
                    }

                    break;
                }
            }
        }

        characteristic ++;
    }

    /**
     * The below stored characteristic value handles are currently
     * not used, but these variables are retained for reference.
     */
    BT_IGNORE_UNUSED_PARAM(bps_bp_msrmt_hdl);
    BT_IGNORE_UNUSED_PARAM(bps_intr_cuff_prsr_hdl);
}


void bpc_profile_operations (void)
{
    UINT32       choice, menu_choice;
    UINT16       cli_cfg;
    UCHAR        cfg_val[GATT_CLI_CFG_VAL_LEN];
    ATT_UUID     uuid;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",bpc_client_menu);

        CONSOLE_IN ( "%u",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /*  return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_BLOOD_PRESSURE_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 11:
            cli_cfg = GATT_CLI_CNFG_INDICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                bps_bp_msrmt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 12:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                bps_bp_msrmt_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 13:
            cli_cfg = GATT_CLI_CNFG_NOTIFICATION;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                bps_intr_cuff_prsr_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 14:
            cli_cfg = GATT_CLI_CNFG_DEFAULT;
            BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);
            gatt_char_wr
            (
                &appl_gatt_client_handle,
                bps_intr_cuff_prsr_ccc_hdl,
                cfg_val,
                GATT_CLI_CFG_VAL_LEN,
                0x01U
            );
            break;

        case 15:
            uuid.uuid_16 = 0x0000U;
            gatt_char_read
            (
                &appl_gatt_client_handle,
                bps_bp_feature_hdl,
                0x0000U,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );
            break;

        case 50:
#ifdef DIC
            dic_profile_operations ();
#endif /* DIC */
            break;

        case 60:
#ifdef BAC
            bac_profile_operations ();
#endif /* BAC */
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

#endif /* (defined ATT && defined BPC) */

