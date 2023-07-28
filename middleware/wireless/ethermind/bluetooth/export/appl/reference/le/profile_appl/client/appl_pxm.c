
/**
 *  \file appl_pxm.c
 *
 *  This is a Proximity Monitor(PXM) application.
 *  Sample applications detailed below:
 *  a. The PXM, as defined by the Specification plays the GAP Central role.
 *  b. The PXM lets discover the attributes of below listed services once
 *     connection with the Proximity Reporter(PXR) is established.
 *       - Immediate Alert Service
 *       - Link Loss Service
 *       - Transmit Power Service
 *  b. The PXM can be configured to monitor the RSSI value of the Link with
 *     the PXR and also to calculate the path loss by subtracting the
 *     RSSI from the transmit power level of the PXR as discovered using the
 *     Reading Tx Power procedure.
 *  c. If the path loss exceeds a threshold set on the PXM it writes the Alert
 *     Level characteristic of the Immediate Alert service, which configures
 *     the PXR to alert.
 *  d. The Support for Path-loss calculations and alert on Path-loss can be
 *     configured by enabling the compilation flag APPL_PXR_PATHLOSS_SUPPORT.
 *  e. The Path-loss threshold value is set to 0x98 by default. This can be
 *     configured to any desired value by application interfaces. The reference
 *     path-loss calculations depicted here are only for an "Upper Threshold".
 *  f. The Path-loss calculations in PXM take effect only when the PXR enlists
 *     TX Power Service with Read/Notify property for TX Power Level.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "appl_gatt_client.h"
#include "BT_hci_api.h"

#if (defined ATT && defined PXM)
/* ----------------------------------------- Global Defines */
/* Compilation Switch to enable PathLoss Calculations */
#define APPL_PXR_PATHLOSS_SUPPORT

/* ----------------------------------------- Constants */
#ifdef APPL_PXR_PATHLOSS_SUPPORT
/**
 * Default Threshold Value for monitoring Path Loss.
 * Could be modified to any desired value.
 */
#define APPL_PXM_PATHLOSS_THSHLD                                 0x98U
#endif /* APPL_PXR_PATHLOSS_SUPPORT */

/* Length of Alerts and Values. Fixed by Specification. */
/* Length of Alerts */
#define APPL_PXM_ALERT_LEVEL_LENGTH                              0x01U
/* Length of TX Power Level */
#define APPL_PXM_TX_POWER_LVL_LENGTH                             0x01U

/* Alert Level Defines */
/** No Alert */
#define APPL_PXM_NO_ALERT                                        0x00U
/** Mild Alert */
#define APPL_PXM_MILD_ALERT                                      0x01U
/** High Alert */
#define APPL_PXM_HIGH_ALERT                                      0x02U

/* ATT Write Procedure Identifiers */
/** Write Request Procedure Flag */
#define APPL_PXM_WR                                              0x01U
/** Write Without Response/ Write Command Procedure Flag */
#define APPL_PXM_WWR                                             0x00U

/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
static ATT_ATTR_HANDLE pxm_ias_alert_level_hdl;
static ATT_ATTR_HANDLE pxm_tx_power_level_hdl;
static ATT_ATTR_HANDLE pxm_ll_alert_level_hdl;
static ATT_ATTR_HANDLE pxm_tx_power_level_ccc_hdl;
static ATT_ATTR_HANDLE pxm_tx_power_cpf_hdl;

#ifdef APPL_PXR_PATHLOSS_SUPPORT
static UCHAR appl_pxm_tx_power_level;
/** Flag to maintain Alert State triggered due to Path Loss Detection */
static UCHAR appl_pxm_high_alert_set = BT_FALSE;
static UCHAR appl_pxm_pathloss_thshld = APPL_PXM_PATHLOSS_THSHLD;
#endif /* APPL_PXR_PATHLOSS_SUPPORT */

static UCHAR appl_pxm_rd_tx_power_lvl_status;

static const UCHAR pxm_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\n\
   --- Link Loss Service ---\n\
   10 - Discover Link Loss Service\n\
   11 - Read Link Loss Service - Alert Level\n\
   12 - Set Link Loss Service - Alert Level - \"No Alert\"\n\
   13 - Set Link Loss Service - Alert Level - \"Mild Alert\"\n\
   14 - Set Link Loss Service - Alert Level - \"High Alert\"\n\n\
   --- Immediate Alert Service ---\n\
   20 - Discover Immediate Alert Service\n\
   21 - Immediate Alert Service - Alert Level - \"No Alert\"\n\
   22 - Immediate Alert Service - Alert Level - \"Mild Alert\"\n\
   23 - Immediate Alert Service - Alert Level - \"High Alert\"\n\n\
   --- TX Power Service ---\n\
   30 - Discover TX Power Service\n\
   31 - Read TX Power Level\n\
   32 - Enable/Disable TX Power Service Notification\n\
   33 - Read TX Power CPF Value\n\
   34 - Set PATHLOSS Threshold Value\n\
Your Option?\n\
";
/* ----------------------------------------- Macro Defines */
/* Discovery related macros */
/** Discover Link Loss Service */
#define pxm_discover_lls(h)                                  \
        pxm_discover_service((h), GATT_LINK_LOSS_SERVICE)

/** Discover Immediate Service */
#define pxm_discover_ias(h)                                  \
        pxm_discover_service((h), GATT_IMMEDIATE_ALERT_SERVICE)

/** Discover TX Power Level Service */
#define pxm_discover_tps(h)                                  \
        pxm_discover_service((h), GATT_TX_POWER_SERVICE)

/* Getting Attributes related macros */
/** PXM Get Alert Level from LLS */
#define pxm_get_alert_lvl_lls(h,ah)                          \
        pxm_rd_attr((h),(ah))

/** PXM Get Alert Level from IAS */
#define pxm_get_alert_lvl_ias(h,ah)                          \
        pxm_rd_attr((h),(ah))

/** PXM Get TX Power Level from TPS */
#define pxm_get_tx_power_lvl(h,ah)                           \
        pxm_rd_attr((h),(ah))

/** PXM Get TX Power Level CPF Value from TPS */
#define pxm_get_tx_power_lvl_cpf(h,ah)                       \
        pxm_rd_attr((h),(ah))

/* Setting Attribute related macros */
/** PXM set No Alert On LLS */
#define pxm_set_no_alert_on_lls(h,ah)                        \
        pxm_wr_alert_lvl                                     \
        (                                                    \
            (h),                                             \
            (ah),                                            \
            APPL_PXM_NO_ALERT,                               \
            APPL_PXM_WR                                      \
        )

/** PXM set Mild Alert On LLS */
#define pxm_set_mild_alert_on_lls(h,ah)                      \
        pxm_wr_alert_lvl                                     \
        (                                                    \
            (h),                                             \
            (ah),                                            \
            APPL_PXM_MILD_ALERT,                             \
            APPL_PXM_WR                                      \
        )

/** PXM set High Alert On LLS */
#define pxm_set_high_alert_on_lls(h,ah)                      \
        pxm_wr_alert_lvl                                     \
        (                                                    \
            (h),                                             \
            (ah),                                            \
            APPL_PXM_HIGH_ALERT,                             \
            APPL_PXM_WR                                      \
        )

/** PXM set No Alert On IAS */
#define pxm_set_no_alert_on_ias(h,ah)                        \
        pxm_wr_alert_lvl                                     \
        (                                                    \
            (h),                                             \
            (ah),                                            \
            APPL_PXM_NO_ALERT,                               \
            APPL_PXM_WWR                                     \
        )

/** PXM set Mild Alert On IAS */
#define pxm_set_mild_alert_on_ias(h,ah)                      \
        pxm_wr_alert_lvl                                     \
        (                                                    \
            (h),                                             \
            (ah),                                            \
            APPL_PXM_MILD_ALERT,                             \
            APPL_PXM_WWR                                     \
        )

/** PXM set High Alert On IAS */
#define pxm_set_high_alert_on_ias(h,ah)                      \
        pxm_wr_alert_lvl                                     \
        (                                                    \
            (h),                                             \
            (ah),                                            \
            APPL_PXM_HIGH_ALERT,                             \
            APPL_PXM_WWR                                     \
        )
/* PXM common Alert Level Setting Macros */
/** PXM Set No Alert Level */
#define pxm_set_no_alert(s,h,ah)                             \
        pxm_set_no_alert_on_##s((h),(ah))

/** PXM Set Mild Alert Level */
#define pxm_set_mild_alert(s,h,ah)                           \
        pxm_set_mild_alert_on_##s((h),(ah))

/** PXM Set High Alert Level */
#define pxm_set_high_alert(s,h,ah)                           \
        pxm_set_high_alert_on_##s((h),(ah))

/** PXM Enable TPS Notification */
#define pxm_enable_tps_ntf(h,ah)                             \
        pxm_config_tps((h),(ah),(BT_TRUE))

/** PXM Disable TPS Notification */
#define pxm_disable_tps_ntf(h,ah)                            \
        pxm_config_tps((h),(ah),(BT_FALSE))

/* ------------------------------- Functions */
#ifdef APPL_PXR_PATHLOSS_SUPPORT
static void pxm_update_pathloss_thshld(/* IN */ UCHAR thshld);
static API_RESULT pxm_fetch_rssi(void);
#endif /* APPL_PXR_PATHLOSS_SUPPORT */

API_RESULT pxm_discover_service
           (
               /* IN */ ATT_HANDLE * att_handle,
               /* IN */ ATT_UUID16 s_uuid
           )
{
    ATT_UUID uuid;

    /* Initialize */
    uuid.uuid_16 = s_uuid;

    return gatt_discover_ps
           (
               att_handle,
               uuid,
               ATT_16_BIT_UUID_FORMAT
           );
}

API_RESULT pxm_rd_attr
           (
               /* IN */ ATT_HANDLE * att_handle,
               /* IN */ UINT16     attr_hndl
           )
{
    ATT_UUID uuid;

    /* Initialize */
    uuid.uuid_16 = 0x0000U;

    return gatt_char_read
           (
               att_handle,
               attr_hndl,
               0x0000U,
               uuid,
               ATT_16_BIT_UUID_FORMAT
           );
}

API_RESULT pxm_wr_alert_lvl
           (
               /* IN */ ATT_HANDLE * att_handle,
               /* IN */ UINT16     attr_hndl,
               /* IN */ UCHAR      alert_lvl,
               /* IN */ UCHAR      rsp
           )
{
    return gatt_char_wr
           (
               att_handle,
               attr_hndl,
               &alert_lvl,
               APPL_PXM_ALERT_LEVEL_LENGTH,
               rsp
           );
}

API_RESULT pxm_config_tps
           (
               /* IN */ ATT_HANDLE * att_handle,
               /* IN */ UINT16     attr_hndl,
               /* IN */ UCHAR      enable
           )
{
    UINT16 cli_cfg;
    UCHAR  cfg_val[GATT_CLI_CFG_VAL_LEN];

    /* Assign */
    cli_cfg = (BT_TRUE == enable) ? GATT_CLI_CNFG_NOTIFICATION :
                                    GATT_CLI_CNFG_DEFAULT;

    /* Pack to Byte Stream */
    BT_PACK_LE_2_BYTE(cfg_val, &cli_cfg);

    return gatt_char_wr
           (
               att_handle,
               attr_hndl,
               cfg_val,
               sizeof(cfg_val),
               APPL_PXM_WR
           );
}

API_RESULT pxm_get_rssi_complete
           (
               /* IN */ UCHAR  status,
               /* IN */ UINT16 conn_handle,
               /* IN */ UCHAR  rssi
           )
{
    API_RESULT retval;
#ifdef APPL_PXR_PATHLOSS_SUPPORT
    UCHAR      appl_pxm_path_loss;
    UCHAR      appl_pxm_rssi_value;

    /* Initialize */
    retval              = API_SUCCESS;
    appl_pxm_rssi_value = 0x00;

    /* Update the RSSI value the Global Variable */
    if  (0x00U == status)
    {
        /* Save Current RSSI */
        appl_pxm_rssi_value = rssi;
    }

    BT_IGNORE_UNUSED_PARAM(conn_handle);

    /**
     * NOTE:
     * abs(...) is C standard function.
     * The presence of this in Tool chain would depend on the
     * standard libraries of the Tool chain having support for this
     * function in "math.h" or "stdlib.h"
     *
     * In Future, some local implementation of abs(...) like below can
     * be considered...
     *         int xyz_abs(int n)
     *         {
     *             return (n * (((2*n)+ 1 ) % 2));
     *         }
     */
    appl_pxm_path_loss = (UCHAR)abs(appl_pxm_tx_power_level - appl_pxm_rssi_value);

    APPL_TRC (
    "[PXM]: TX Power Lvl 0x%X RSSI 0x%X PATHLOSS 0x%X THSHLD 0x%X\n",
    appl_pxm_tx_power_level, appl_pxm_rssi_value, appl_pxm_path_loss, appl_pxm_pathloss_thshld);

    if (appl_pxm_path_loss < appl_pxm_pathloss_thshld)
    {
        APPL_TRC ( "[PXM]: PATHLOSS ALERT\n");
        
        /**
         * Do not set High Alert if state is already in High Alert!
         */
        if (BT_TRUE != appl_pxm_high_alert_set)
        {
            /* High Alert on IAS */
            retval = pxm_set_high_alert
                    (
                        ias,
                        &appl_gatt_client_handle,
                        pxm_ias_alert_level_hdl
                    );

            APPL_TRC("[PXM]: Set High Alert on IAS returned 0x%04X\n", retval);
            if (API_SUCCESS == retval)
            {
                appl_pxm_high_alert_set = BT_TRUE;
            }
        }
    }
    else
    {
        /**
         * NOTE:
         * If Bottom Threshold implementation is desired.
         * This would be the suitable location to Set No Alert
         * on IAS when Path Gain i.e. PXR's desired proximity
         * is detected.
         *
         * Below reference code section toggles any previously
         * set High Alert value which was triggered due to
         * Path Loss Detection.
         */
        if (BT_TRUE == appl_pxm_high_alert_set)
        {
            /* No Alert on IAS */
            retval = pxm_set_no_alert
                     (
                         ias,
                         &appl_gatt_client_handle,
                         pxm_ias_alert_level_hdl
                     );

            APPL_TRC("[PXM]: Set No Alert on IAS returned 0x%04X\n", retval);

            if (API_SUCCESS == retval)
            {
                appl_pxm_high_alert_set = BT_FALSE;
            }
        }
    }
#else /* APPL_PXR_PATHLOSS_SUPPORT */
    BT_IGNORE_UNUSED_PARAM(status);
    BT_IGNORE_UNUSED_PARAM(conn_handle);
    BT_IGNORE_UNUSED_PARAM(rssi);

    retval = API_SUCCESS;
#endif /* APPL_PXR_PATHLOSS_SUPPORT */

    return retval;
}

#ifdef APPL_PXR_PATHLOSS_SUPPORT
static void pxm_update_pathloss_thshld(/* IN */ UCHAR thshld)
{
    appl_pxm_pathloss_thshld = thshld;
}

/**
 * RSSI value will be fetched for the device id associated with the current
 * ATT handle only.
 *
 * NOTE:
 * =====
 * When two PXR devices are connected the last connected PXR
 * device's RSSI will be fetched.
 */
static API_RESULT pxm_fetch_rssi(void)
{
    API_RESULT retval;
    BT_DEVICE_ADDR t_addr;
    UINT16         conn_handle;

    /* Get the BD address from the Current ATT Handle */
    retval = device_queue_get_remote_addr
             (
                 &appl_gatt_client_handle.device_id,
                 &t_addr
             );

    retval = BT_hci_get_le_connection_handle
             (
                 &t_addr,
                 &conn_handle
             );

    APPL_TRC (
    "[PXM]: Fetching RSSI for Conn Handle 0x%02X \n", conn_handle);

    /* Read RSSI Value */
    retval = BT_hci_read_rssi (conn_handle);

    if (API_SUCCESS != retval)
    {
        APPL_TRC (
        "[** ERR **]: Failed to Fetch RSSI, reason 0x%04X", retval);
    }

    return retval;
}
#endif /* APPL_PXR_PATHLOSS_SUPPORT */

void pxm_notify_gatt_chardata (GATT_CHARACTERISTIC_PARAM * characteristic, UINT16 size)
{
    UINT16 i,j;

    for (i = 0U; i < size; i++)
    {
        switch (GATT_GET_CHAR_UUID(characteristic))
        {
            case GATT_ALERT_LEVEL_CHARACTERISTIC:

                /**
                 *  Add a check to populate IAS or LL handle with
                 *  respect to characteristic property
                 */
                if (GATT_DB_WRITE_WITHOUT_RSP == characteristic->cproperty)
                {
                    /* To populate Immediate Alert Service handle*/
                    pxm_ias_alert_level_hdl = characteristic->value_handle;
                }
                else if (GATT_DB_WRITE == (characteristic->cproperty & GATT_DB_WRITE))
                {
                    /* To populate Link Loss Service handle*/
                    pxm_ll_alert_level_hdl = characteristic->value_handle;
                }
                else
                {
                    /* MISRA C-2012 Rule 15.7 */
                }

                break;

            case GATT_TX_POWER_LEVEL_CHARACTERISTIC:
                pxm_tx_power_level_hdl = characteristic->value_handle;
                break;

            default:
                /* CONSOLE_OUT("Invalid...\n"); */
                break;
        }

        if (0U != characteristic->desc_index)
        {
            for (j = 0U; j < characteristic->desc_index; j++)
            {
                if (GATT_TX_POWER_LEVEL_CHARACTERISTIC ==
                    GATT_GET_CHAR_UUID(characteristic))
                {
                    switch (GATT_GET_CLI_CHAR_DESC_UUID(characteristic,j))
                    {
                    case GATT_CLIENT_CONFIG:
                        pxm_tx_power_level_ccc_hdl = characteristic->descriptor[j].handle;
                        break;

                    case GATT_FORMAT:
                        pxm_tx_power_cpf_hdl = characteristic->descriptor[j].handle;
                        break;

                    default:
                        /* CONSOLE_OUT("Invalid...\n"); */
                        break;
                    }
                }
            }
        }

        characteristic ++;
    }
}

void pxm_parse_notification_data(ATT_ATTR_HANDLE handle, UCHAR * data, UINT16 datalen)
{
#ifdef APPL_PXR_PATHLOSS_SUPPORT
    if ((handle == pxm_tx_power_level_hdl) &&
        (APPL_PXM_TX_POWER_LVL_LENGTH == datalen))
    {
        BT_UNPACK_LE_1_BYTE (&appl_pxm_tx_power_level, data);

        CONSOLE_OUT ("TX Power Level 0x%x\n",appl_pxm_tx_power_level);

        /* Fetch the RSSI Reading For Path Loss Calculation */
        (BT_IGNORE_RETURN_VALUE)pxm_fetch_rssi();
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }
#else /* APPL_PXR_PATHLOSS_SUPPORT */
    BT_IGNORE_UNUSED_PARAM(handle);
    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);
#endif /* APPL_PXR_PATHLOSS_SUPPORT */
}

void pxm_parse_read_data (UCHAR * data, UINT16 datalen)
{
    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(datalen);

    if (BT_TRUE == appl_pxm_rd_tx_power_lvl_status)
    {
        /* Reset Read State */
        appl_pxm_rd_tx_power_lvl_status = BT_FALSE;

#ifdef APPL_PXR_PATHLOSS_SUPPORT
        BT_UNPACK_LE_1_BYTE (&appl_pxm_tx_power_level, data);
        CONSOLE_OUT ("TX Power Level 0x%x\n", appl_pxm_tx_power_level);

        /* Fetch the RSSI and check for PathLoss */
        (BT_IGNORE_RETURN_VALUE)pxm_fetch_rssi();
#endif /* APPL_PXR_PATHLOSS_SUPPORT */
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }
}

void pxm_profile_operations (void)
{
    int        choice, menu_choice;
    UCHAR      operation;
    API_RESULT retval;

    /* Initialize */
    retval = API_SUCCESS;

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",pxm_client_menu);

        CONSOLE_IN ( "%d",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
            retval = pxm_discover_lls
                     (
                         &appl_gatt_client_handle
                     );
            CONSOLE_OUT("[PXM]: pxm_discover_lls returned 0x%04X\n", retval);
            break;

        case 11:
            retval = pxm_get_alert_lvl_lls
                     (
                         &appl_gatt_client_handle,
                         pxm_ll_alert_level_hdl
                     );
            CONSOLE_OUT("[PXM]: pxm_get_alert_lvl_lls returned 0x%04X\n", retval);
            break;

        case 12:
            retval = pxm_set_no_alert
                     (
                         lls,
                         &appl_gatt_client_handle,
                         pxm_ll_alert_level_hdl
                     );
            CONSOLE_OUT("[PXM]: Set No Alert on LLS returned 0x%04X\n", retval);
            break;

        case 13:
            retval = pxm_set_mild_alert_on_lls
                     (
                         &appl_gatt_client_handle,
                         pxm_ll_alert_level_hdl
                     );
            CONSOLE_OUT("[PXM]: pxm_set_mild_alert_on_lls returned 0x%04X\n", retval);
            break;

        case 14:
            retval = pxm_set_high_alert
                     (
                         lls,
                         &appl_gatt_client_handle,
                         pxm_ll_alert_level_hdl
                     );
            CONSOLE_OUT("[PXM]: Set High Alert on LLS returned 0x%04X\n", retval);
            break;

         case 20:
            retval = pxm_discover_ias
                     (
                         &appl_gatt_client_handle
                     );
            CONSOLE_OUT("[PXM]: pxm_discover_ias returned 0x%04X\n", retval);
            break;

         case 21:
            retval = pxm_set_no_alert
                     (
                         ias,
                         &appl_gatt_client_handle,
                         pxm_ias_alert_level_hdl
                     );
            CONSOLE_OUT("[PXM]: Set No Alert on IAS returned 0x%04X\n", retval);
            break;

         case 22:
            retval = pxm_set_mild_alert
                     (
                         ias,
                         &appl_gatt_client_handle,
                         pxm_ias_alert_level_hdl
                     );
            CONSOLE_OUT("[PXM]: Set Mild Alert on IAS returned 0x%04X\n", retval);
            break;

         case 23:
            retval = pxm_set_high_alert
                     (
                         ias,
                         &appl_gatt_client_handle,
                         pxm_ias_alert_level_hdl
                     );
            CONSOLE_OUT("[PXM]: Set High Alert on IAS returned 0x%04X\n", retval);
            break;

         case 30:
            retval = pxm_discover_tps
                     (
                         &appl_gatt_client_handle
                     );
            CONSOLE_OUT("[PXM]: pxm_discover_tps returned 0x%04X\n", retval);
            break;

        case 31:
            if (BT_FALSE == appl_pxm_rd_tx_power_lvl_status)
            {
                /* Set the Read State */
                appl_pxm_rd_tx_power_lvl_status = BT_TRUE;

                retval = pxm_get_tx_power_lvl
                         (
                             &appl_gatt_client_handle,
                             pxm_tx_power_level_hdl
                         );

                CONSOLE_OUT("[PXM]: pxm_get_tx_power_lvl returned 0x%04X\n", retval);
            }
            else
            {
                /* MISRA C-2012 Rule 15.7 */
            }
            break;

        case 32:
            CONSOLE_OUT ("1 (Enable), 0 (Disable)\n");
            CONSOLE_IN ("%d", &choice);

            operation = (UCHAR)choice;

            if (BT_TRUE == operation)
            {
                retval = pxm_enable_tps_ntf
                         (
                             &appl_gatt_client_handle,
                             pxm_tx_power_level_ccc_hdl
                         );
                CONSOLE_OUT("[PXM]: pxm_enable_tps_ntf returned 0x%04X\n", retval);
            }
            else
            {
                retval = pxm_disable_tps_ntf
                         (
                             &appl_gatt_client_handle,
                             pxm_tx_power_level_ccc_hdl
                         );
                CONSOLE_OUT("[PXM]: pxm_disable_tps_ntf returned 0x%04X\n", retval);
            }
            break;

        case 33:
            retval = pxm_get_tx_power_lvl_cpf
                     (
                         &appl_gatt_client_handle,
                         pxm_tx_power_cpf_hdl
                     );
            CONSOLE_OUT("[PXM]: pxm_get_tx_power_lvl_cpf returned 0x%04X\n", retval);
            break;

        case 34:
#ifdef APPL_PXR_PATHLOSS_SUPPORT
            CONSOLE_OUT ("[PXM]: 0x%x is set as default Threshold\n",APPL_PXM_PATHLOSS_THSHLD);
            CONSOLE_OUT ("[PXM]: Enter path-loss Threshold [in HEX]: ");
            CONSOLE_IN ("%x\n", &choice);

            pxm_update_pathloss_thshld((UCHAR)choice);

            CONSOLE_OUT ("[PXM]: RSSI Threshold value set is to 0x%x\n",appl_pxm_pathloss_thshld);
#else /* APPL_PXR_PATHLOSS_SUPPORT */
            CONSOLE_OUT ("[PXM]: Define APPL_PXR_PATHLOSS_SUPPORT Flag\n");
#endif /* APPL_PXR_PATHLOSS_SUPPORT */
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

#endif /* (defined ATT && defined PXM) */

