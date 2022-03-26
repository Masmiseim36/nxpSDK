/** @file wlan_txpwrlimit_cfg.c
 *
 *  @brief  This file provides WLAN World Wide Safe Mode Tx Power Limit APIs.
 *
 *  Copyright 2008-2021 NXP
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  'Software'), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sub license, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject
 *  to the following conditions:
 *
 *  The above copyright notice and this permission notice (including the
 *  next paragraph) shall be included in all copies or substantial
 *  portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 *  IN NO EVENT SHALL NXP AND/OR ITS SUPPLIERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
