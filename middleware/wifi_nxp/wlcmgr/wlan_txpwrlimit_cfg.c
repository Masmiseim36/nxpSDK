/** @file wlan_txpwrlimit_cfg.c
 *
 *  @brief  This file provides WLAN World Wide Safe Mode Tx Power Limit APIs.
 *
 *  Copyright 2008-2021 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#include <wlan.h>
#include <wifi.h>

#ifdef WIFI_BT_TX_PWR_LIMITS
#include WIFI_BT_TX_PWR_LIMITS
#else
#error "Region tx power config not defined"
#endif

int wlan_set_wwsm_txpwrlimit(void)
{
    int rv = WM_SUCCESS;

    rv = wlan_set_chanlist_and_txpwrlimit(&chanlist_2g_cfg, &tx_pwrlimit_2g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G TX PWR Limit configuration\r\n");
    }
#ifdef CONFIG_5GHz_SUPPORT
    rv = wlan_set_chanlist_and_txpwrlimit(&chanlist_5g_cfg, &tx_pwrlimit_5g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 5G TX PWR Limit configuration\r\n");
    }
#endif
    return rv;
}
