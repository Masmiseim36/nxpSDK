/** @file wlan_txpwrlimit_cfg.c
 *
 *  @brief  This file provides WLAN World Wide Safe Mode Tx Power Limit APIs.
 *
 *  Copyright 2008-2021, 2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <wlan.h>
#include <wifi.h>

#ifdef WIFI_BT_TX_PWR_LIMITS
#include WIFI_BT_TX_PWR_LIMITS
#else
#error "Region tx power config not defined"
#endif

#if   defined(CONFIG_COMPRESS_TX_PWTBL)
typedef struct _rg_power_cfg
{
    t_u16 region_code;
    t_u8 *rg_power_table;
    t_u16 rg_len;
} rg_power_cfg;

rg_power_cfg rg_power_cfg_FC[] = {
    {
        0x00,
        (t_u8 *)rg_table_fc,
        sizeof(rg_table_fc),
    },
};

int wlan_set_rg_power_cfg(t_u16 region_code)
{
    int i  = 0;
    int rv = WM_SUCCESS;

    for (i = 0; i < sizeof(rg_power_cfg_FC) / sizeof(rg_power_cfg); i++)
    {
        if (region_code == rg_power_cfg_FC[i].region_code)
        {
            rv = wlan_set_region_power_cfg(rg_power_cfg_FC[i].rg_power_table, rg_power_cfg_FC[i].rg_len);
            if (rv != WM_SUCCESS)
                (void)PRINTF("Unable to set compressed TX power table configuration\r\n");
            return rv;
        }
    }

    return -WM_FAIL;
}

#endif

#ifdef CONFIG_COMPRESS_TX_PWTBL
int wlan_set_wwsm_txpwrlimit()
{
    int rv = WM_SUCCESS;
#ifdef WLAN_REGION_CODE
    rv = wlan_set_country_code(WLAN_REGION_CODE);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set country code\r\n");
        return -WM_FAIL;
    }
#endif
    rv = wlan_set_chanlist(&chanlist_2g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G chanlist configuration\r\n");
        return -WM_FAIL;
    }
#ifdef CONFIG_5GHz_SUPPORT
    rv = wlan_set_chanlist(&chanlist_5g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 5G chanlist configuration\r\n");
        return -WM_FAIL;
    }
#endif
    rv = wlan_set_region_power_cfg(rg_table_fc, rg_table_fc_len);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set compressed TX power table configuration\r\n");
        return -WM_FAIL;
    }
#ifdef CONFIG_11AX
#ifdef CONFIG_COMPRESS_RU_TX_PWTBL
    rv = wlan_set_11ax_rutxpowerlimit(rutxpowerlimit_cfg_set, sizeof(rutxpowerlimit_cfg_set));
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set RU TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#else
    rv = wlan_set_11ax_rutxpowerlimit_legacy(&rutxpowerlimit_2g_cfg_set);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G RU TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#ifdef CONFIG_5GHz_SUPPORT
    else
    {
        rv = wlan_set_11ax_rutxpowerlimit_legacy(&rutxpowerlimit_5g_cfg_set);
        if (rv != WM_SUCCESS)
        {
            (void)PRINTF("Unable to set 5G RU TX PWR Limit configuration\r\n");
            return -WM_FAIL;
        }
    }
#endif
#endif /* CONFIG_COMPRESS_RU_TX_PWTBL */
#endif /* CONFIG_11AX */

    return rv;
}
#else
int wlan_set_wwsm_txpwrlimit(void)
{
    int rv = WM_SUCCESS;
#ifdef WLAN_REGION_CODE
    rv     = wlan_set_country_code(WLAN_REGION_CODE);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set country code\r\n");
        return -WM_FAIL;
    }
#endif
    rv = wlan_set_chanlist_and_txpwrlimit(&chanlist_2g_cfg, &tx_pwrlimit_2g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#ifdef CONFIG_5GHz_SUPPORT
    rv = wlan_set_chanlist_and_txpwrlimit(&chanlist_5g_cfg, &tx_pwrlimit_5g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 5G TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#endif

#ifdef CONFIG_11AX
#ifdef CONFIG_COMPRESS_RU_TX_PWTBL
    rv = wlan_set_11ax_rutxpowerlimit(rutxpowerlimit_cfg_set, sizeof(rutxpowerlimit_cfg_set));
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set RU TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#else
    rv = wlan_set_11ax_rutxpowerlimit_legacy(&rutxpowerlimit_2g_cfg_set);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G RU TX PWR Limit configuration\r\n");
        return -WM_FAIL;
    }
#ifdef CONFIG_5GHz_SUPPORT
    else
    {
        rv = wlan_set_11ax_rutxpowerlimit_legacy(&rutxpowerlimit_5g_cfg_set);
        if (rv != WM_SUCCESS)
        {
            (void)PRINTF("Unable to set 5G RU TX PWR Limit configuration\r\n");
            return -WM_FAIL;
        }
    }
#endif
#endif /* CONFIG_COMPRESS_RU_TX_PWTBL */
#endif /* CONFIG_11AX */

    return rv;
}
#endif /* CONFIG_COMPRESS_TX_PWTBL */

const char *wlan_get_wlan_region_code(void)
{
#ifdef WLAN_REGION_CODE
    return WLAN_REGION_CODE;
#else
#error "Please define WLAN_REGION_CODE in Region tx power config file"
#endif
}
