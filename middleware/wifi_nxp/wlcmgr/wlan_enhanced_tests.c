/** @file wlan_enhanced_tests.c
 *
 *  @brief  This file provides WLAN ENHANCED Test API
 *
 *  Copyright 2008-2020 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#include <wlan.h>
#include <cli.h>
#include <cli_utils.h>
#include <string.h>
#include <wm_net.h> /* for net_inet_aton */
#include <wifi.h>
#include <wlan_tests.h>

#ifdef WIFI_BT_TX_PWR_LIMITS
#include WIFI_BT_TX_PWR_LIMITS
#else
#error "Region tx power config not defined"
#endif

/*
 * NXP Test Framework (MTF) functions
 */

static void dump_wlan_set_pmfcfg_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-pmfcfg <mfpc> <mfpr> \r\n");
    (void)PRINTF("\r\n");
    (void)PRINTF("\t<mfpc>:   Management Frame Protection Capable (MFPC)\r\n");
    (void)PRINTF("\t          1: Management Frame Protection Capable\r\n");
    (void)PRINTF("\t          0: Management Frame Protection not Capable\r\n");
    (void)PRINTF("\t<mfpr>:   Management Frame Protection Required (MFPR)\r\n");
    (void)PRINTF("\t          1: Management Frame Protection Required\r\n");
    (void)PRINTF("\t          0: Management Frame Protection Optional\r\n");
    (void)PRINTF("\tDefault setting is PMF not capable.\r\n");
    (void)PRINTF("\tmfpc = 0, mfpr = 1 is an invalid combination\r\n");
}

static void wlan_pmfcfg_set(int argc, char *argv[])
{
    int ret;
    uint8_t mfpc, mfpr;

    if (argc != 3)
    {
        dump_wlan_set_pmfcfg_usage();
        return;
    }

    errno = 0;
    mfpc  = (uint8_t)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during wlan pmfcfg set arg_1 strtoul errno:%d", errno);
    }
    errno = 0;
    mfpr  = (uint8_t)strtol(argv[2], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during wlan pmfcfg set arg_2 strtoul errno:%d", errno);
    }

    ret = wlan_set_pmfcfg(mfpc, mfpr);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("PMF configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("PMF configuration failed\r\n");
        dump_wlan_set_pmfcfg_usage();
    }
}

static void dump_wlan_get_pmfcfg_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-pmfcfg \r\n");
}

static void wlan_pmfcfg_get(int argc, char *argv[])
{
    int ret;
    uint8_t mfpc, mfpr;

    if (argc != 1)
    {
        dump_wlan_get_pmfcfg_usage();
        return;
    }

    ret = wlan_get_pmfcfg(&mfpc, &mfpr);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Management Frame Protection Capability: %s\r\n", mfpc == 1U ? "Yes" : "No");
        if (mfpc != 0U)
        {
            (void)PRINTF("Management Frame Protection: %s\r\n", mfpr == 1U ? "Required" : "Optional");
        }
    }
    else
    {
        (void)PRINTF("PMF configuration read failed\r\n");
        dump_wlan_get_pmfcfg_usage();
    }
}

static void dump_wlan_set_antcfg_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-antcfg <ant mode> [evaluate_time] \r\n");
    (void)PRINTF("\r\n");
    (void)PRINTF("\t<ant mode>: \r\n");
    (void)PRINTF("\t           Bit 0   -- Tx/Rx antenna 1\r\n");
    (void)PRINTF("\t           Bit 1   -- Tx/Rx antenna 2\r\n");
    (void)PRINTF("\t           0xFFFF  -- Tx/Rx antenna diversity\r\n");
    (void)PRINTF("\t[evaluate_time]: \r\n");
    (void)PRINTF("\t           if ant mode = 0xFFFF, SAD evaluate time interval,\r\n");
    (void)PRINTF("\t           default value is 6s(0x1770)\r\n");
}

static void wlan_antcfg_set(int argc, char *argv[])
{
    int ret;
    uint32_t ant_mode;
    uint16_t evaluate_time = 0;

    if ((argc != 2) && ((argc != 3)))
    {
        dump_wlan_set_antcfg_usage();
        return;
    }

    errno    = 0;
    ant_mode = (uint32_t)strtol(argv[1], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }

    if ((argc == 3) && (ant_mode != 0xFFFFU))
    {
        dump_wlan_set_antcfg_usage();
        return;
    }

    errno = 0;
    if (argc == 3)
    {
        evaluate_time = (uint16_t)strtol(argv[2], NULL, 16);
    }
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }

    ret = wlan_set_antcfg(ant_mode, evaluate_time);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Antenna configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("Antenna configuration failed\r\n");
        dump_wlan_set_antcfg_usage();
    }
}

static void dump_wlan_get_antcfg_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-antcfg \r\n");
}

static void wlan_antcfg_get(int argc, char *argv[])
{
    int ret;
    uint32_t ant_mode;
    uint16_t evaluate_time = 0;

    if (argc != 1)
    {
        dump_wlan_get_antcfg_usage();
        return;
    }

    ret = wlan_get_antcfg(&ant_mode, &evaluate_time);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Mode of Tx/Rx path is : %x\r\n", ant_mode);
        if (ant_mode == 0XFFFFU)
        {
            (void)PRINTF("Evaluate time : %x\r\n", evaluate_time);
        }
    }
    else
    {
        (void)PRINTF("antcfg configuration read failed\r\n");
        dump_wlan_get_antcfg_usage();
    }
}

static void dump_wlan_set_ed_mac_mode_usage(void)
{
    (void)PRINTF("Usage:\r\n");
#ifdef CONFIG_5GHz_SUPPORT
    (void)PRINTF("wlan-set-ed-mac-mode <ed_ctrl_2g> <ed_offset_2g> <ed_ctrl_5g> <ed_offset_5g>\r\n");
#else
    (void)PRINTF("wlan-set-ed-mac-mode <ed_ctrl_2g> <ed_offset_2g>\r\n");
#endif
    (void)PRINTF("\r\n");
    (void)PRINTF("\ted_ctrl_2g \r\n");
    (void)PRINTF("\t    # 0       - disable EU adaptivity for 2.4GHz band\r\n");
    (void)PRINTF("\t    # 1       - enable EU adaptivity for 2.4GHz band\r\n");
    (void)PRINTF("\ted_offset_2g \r\n");
    (void)PRINTF("\t    # 0       - Default Energy Detect threshold\r\n");
    (void)PRINTF("\t    #offset value range: 0x80 to 0x7F\r\n");
#ifdef CONFIG_5GHz_SUPPORT
    (void)PRINTF("\ted_ctrl_5g \r\n");
    (void)PRINTF("\t    # 0       - disable EU adaptivity for 5GHz band\r\n");
    (void)PRINTF("\t    # 1       - enable EU adaptivity for 5GHz band\r\n");
    (void)PRINTF("\ted_offset_2g \r\n");
    (void)PRINTF("\t    # 0       - Default Energy Detect threshold\r\n");
    (void)PRINTF("\t    #offset value range: 0x80 to 0x7F\r\n");
#endif
}

static void wlan_ed_mac_mode_set(int argc, char *argv[])
{
    int ret;
    wlan_ed_mac_ctrl_t wlan_ed_mac_ctrl;

#ifdef CONFIG_5GHz_SUPPORT
    if (argc != 5)
#else
    if (argc != 3)
#endif
    {
        dump_wlan_set_ed_mac_mode_usage();
        return;
    }

    errno                       = 0;
    wlan_ed_mac_ctrl.ed_ctrl_2g = (t_u16)strtol(argv[1], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
    errno                         = 0;
    wlan_ed_mac_ctrl.ed_offset_2g = (t_s16)strtol(argv[2], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
#ifdef CONFIG_5GHz_SUPPORT
    errno                       = 0;
    wlan_ed_mac_ctrl.ed_ctrl_5g = (t_u16)strtol(argv[3], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
    errno                         = 0;
    wlan_ed_mac_ctrl.ed_offset_5g = (t_s16)strtol(argv[4], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
#endif

    if (wlan_ed_mac_ctrl.ed_ctrl_2g != 0U && wlan_ed_mac_ctrl.ed_ctrl_2g != 1U)
    {
        dump_wlan_set_ed_mac_mode_usage();
        return;
    }
#ifdef CONFIG_5GHz_SUPPORT
    if (wlan_ed_mac_ctrl.ed_ctrl_5g != 0U && wlan_ed_mac_ctrl.ed_ctrl_5g != 1U)
    {
        dump_wlan_set_ed_mac_mode_usage();
        return;
    }
#endif

    ret = wlan_set_ed_mac_mode(wlan_ed_mac_ctrl);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("ED MAC MODE settings configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("ED MAC MODE settings configuration failed\r\n");
        dump_wlan_set_ed_mac_mode_usage();
    }
}

static void dump_wlan_get_ed_mac_mode_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-ed-mac-mode \r\n");
}

static void wlan_ed_mac_mode_get(int argc, char *argv[])
{
    int ret;
    wlan_ed_mac_ctrl_t wlan_ed_mac_ctrl;

    if (argc != 1)
    {
        dump_wlan_get_ed_mac_mode_usage();
        return;
    }

    ret = wlan_get_ed_mac_mode(&wlan_ed_mac_ctrl);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("EU adaptivity for 2.4GHz band : %s\r\n",
                     wlan_ed_mac_ctrl.ed_ctrl_2g == 1U ? "Enabled" : "Disabled");
        if (wlan_ed_mac_ctrl.ed_ctrl_2g != 0U)
        {
            (void)PRINTF("Energy Detect threshold offset : 0X%x\r\n", wlan_ed_mac_ctrl.ed_offset_2g);
        }
#ifdef CONFIG_5GHz_SUPPORT
        (void)PRINTF("EU adaptivity for 5GHz band : %s\r\n",
                     wlan_ed_mac_ctrl.ed_ctrl_5g == 1U ? "Enabled" : "Disabled");
        if (wlan_ed_mac_ctrl.ed_ctrl_5g != 0U)
        {
            (void)PRINTF("Energy Detect threshold offset : 0X%x\r\n", wlan_ed_mac_ctrl.ed_offset_5g);
        }
#endif
    }
    else
    {
        (void)PRINTF("ED MAC MODE read failed\r\n");
        dump_wlan_get_ed_mac_mode_usage();
    }
}
#if 0
static int wlan_memrdwr_getset(int argc, char *argv[])
{
    uint8_t action;
    uint32_t value;
    int ret;

    if (argc != 3 && argc != 4)
    {
        return -WM_FAIL;
    }

    if (argc == 3)
    {
        action = ACTION_GET;
        value  = 0;
    }
    else
    {
        action = ACTION_SET;
        value  = a2hex_or_atoi(argv[3]);
    }

    ret = wifi_mem_access(action, a2hex_or_atoi(argv[2]), &value);

    if (ret == WM_SUCCESS)
    {
        if (action == ACTION_GET)
        {
            (void)PRINTF("At Memory 0x%x: 0x%x\r\n", a2hex_or_atoi(argv[2]), value);
        }
        else
        {
            (void)PRINTF("Set the Memory successfully\r\n");
        }
    }
    else
    {
        wlcm_e("Read/write Mem failed");
        return -WM_FAIL;
    }
    return WM_SUCCESS;
}
#endif
static void dump_wlan_set_regioncode_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-regioncode <region-code>\r\n");
    (void)PRINTF("where, region code =\r\n");
    (void)PRINTF("0xAA : World Wide Safe Mode\r\n");
    (void)PRINTF("0x10 : US FCC, Singapore\r\n");
    (void)PRINTF("0x20 : IC Canada\r\n");
    (void)PRINTF("0x30 : ETSI, Australia, Republic of Korea\r\n");
    (void)PRINTF("0x32 : France\r\n");
    (void)PRINTF("0x40 : Japan\r\n");
    (void)PRINTF("0x41 : Japan\r\n");
    (void)PRINTF("0x50 : China\r\n");
    (void)PRINTF("0xFE : Japan\r\n");
    (void)PRINTF("0xFF : Special\r\n");
}

static void test_wlan_set_regioncode(int argc, char **argv)
{
    if (argc != 2)
    {
        dump_wlan_set_regioncode_usage();
        return;
    }

    errno             = 0;
    t_u32 region_code = (t_u32)strtol(argv[1], NULL, 0);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
    int rv = wifi_set_region_code(region_code);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set region code: 0x%x\r\n", region_code);
    }
    else
    {
        (void)PRINTF("Region code: 0x%x set\r\n", region_code);
    }
}

static void test_wlan_get_regioncode(int argc, char **argv)
{
    t_u32 region_code = 0;
    int rv            = wifi_get_region_code(&region_code);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get region code: 0x%x\r\n", region_code);
    }
    else
    {
        (void)PRINTF("Region code: 0x%x\r\n", region_code);
    }
}

#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
static char *bw[]           = {"20 MHz", "40 MHz", "80 MHz", "160 MHz"};
static char *rate_format[4] = {"LG", "HT", "VHT", "HE"};
static char *lg_rate[]      = {"1 Mbps",  "2 Mbps",  "5.5 Mbps", "11 Mbps", "6 Mbps",  "9 Mbps",
                          "12 Mbps", "18 Mbps", "24 Mbps",  "36 Mbps", "48 Mbps", "54 Mbps"};
#endif

static void print_ds_rate(wlan_ds_rate ds_rate)
{
#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
    if (ds_rate.sub_command == WIFI_DS_RATE_CFG)
    {
        (void)PRINTF("Tx Rate Configuration: \r\n");
        /* format */
        if (ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_AUTO)
        {
            (void)PRINTF("    Type:       0xFF (Auto)\r\n");
        }
        else if ((unsigned int)(ds_rate.param.rate_cfg.rate_format) <= 3U)
        {
            (void)PRINTF("    Type:       %d (%s)\r\n", ds_rate.param.rate_cfg.rate_format,
                         rate_format[ds_rate.param.rate_cfg.rate_format]);
            if (ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_LG)
            {
                (void)PRINTF("    Rate Index: %d (%s)\r\n", ds_rate.param.rate_cfg.rate_index,
                             lg_rate[ds_rate.param.rate_cfg.rate_index]);
            }
            else if (ds_rate.param.rate_cfg.rate_format >= MLAN_RATE_FORMAT_HT)
            {
                (void)PRINTF("    MCS Index:  %d\r\n", (int)ds_rate.param.rate_cfg.rate_index);
            }
            else
            { /* Do Nothing */
            }
#if defined(CONFIG_11AC) || defined(CONFIG_11AX)
            if ((ds_rate.param.rate_cfg.rate_format == MLAN_RATE_FORMAT_VHT) ||
                (ds_rate.param.rate_cfg.rate_format == 3U))
            {
                (void)PRINTF("    NSS:        %d\r\n", (int)ds_rate.param.rate_cfg.nss);
            }
#endif
        }
        else
        {
            (void)PRINTF("    Unknown rate format.\r\n");
        }
    }
    else if (ds_rate.sub_command == WIFI_DS_GET_DATA_RATE)
    {
        wifi_data_rate_t *datarate = (wifi_data_rate_t *)&(ds_rate.param.data_rate);
        (void)PRINTF("Data Rate:\r\n");
#ifdef SD8801
        (void)PRINTF("  TX: \r\n");
        if (datarate->tx_data_rate < 12)
        {
            (void)PRINTF("    Type: %s\r\n", rate_format[0]);
            /* LG */
            (void)PRINTF("    Rate: %s\r\n", lg_rate[datarate->tx_data_rate]);
        }
        else
        {
            /* HT*/
            (void)PRINTF("    Type: %s\r\n", rate_format[1]);
            if (datarate->tx_ht_bw <= 2)
                (void)PRINTF("    BW:   %s\r\n", bw[datarate->tx_ht_bw]);
            if (datarate->tx_ht_gi == 0)
                (void)PRINTF("    GI:   Long\r\n");
            else
                (void)PRINTF("    GI:   Short\r\n");
            (void)PRINTF("    MCS:  MCS %d\r\n", (int)(datarate->tx_data_rate - 12));
        }

        (void)PRINTF("  RX: \n");
        if (datarate->rx_data_rate < 12)
        {
            (void)PRINTF("    Type: %s\r\n", rate_format[0]);
            /* LG */
            (void)PRINTF("    Rate: %s\r\n", lg_rate[datarate->rx_data_rate]);
        }
        else
        {
            /* HT*/
            (void)PRINTF("    Type: %s\r\n", rate_format[1]);
            if (datarate->rx_ht_bw <= 2)
            {
                (void)PRINTF("    BW:   %s\r\n", bw[datarate->rx_ht_bw]);
            }
            if (datarate->rx_ht_gi == 0)
            {
                (void)PRINTF("    GI:   Long\r\n");
            }
            else
            {
                (void)PRINTF("    GI:   Short\r\n");
            }
            (void)PRINTF("    MCS:  MCS %d\r\n", (int)(datarate->rx_data_rate - 12));
        }
#else
        (void)PRINTF("  TX: \r\n");
        if ((unsigned int)(datarate->tx_rate_format) <= 3U)
        {
            (void)PRINTF("    Type: %s\r\n", rate_format[datarate->tx_rate_format]);
            if ((datarate->tx_rate_format == MLAN_RATE_FORMAT_LG) && datarate->tx_data_rate <= 11U)
            {
                /* LG */
                (void)PRINTF("    Rate: %s\r\n", lg_rate[datarate->tx_data_rate]);
            }
            else
            {
                /* HT and VHT*/
                if (datarate->tx_ht_bw <= 3U)
                {
                    (void)PRINTF("    BW:   %s\r\n", bw[datarate->tx_ht_bw]);
                }
                if (datarate->tx_ht_gi == 0U)
                {
                    (void)PRINTF("    GI:   Long\r\n");
                }
                else
                {
                    (void)PRINTF("    GI:   Short\r\n");
                }
#if defined(CONFIG_11AC) || defined(CONFIG_11AX)
                if (((unsigned int)(datarate->tx_rate_format) == 2U) ||
                    ((unsigned int)(datarate->tx_rate_format) == 3U))
                {
                    (void)PRINTF("    NSS:  %d\r\n", datarate->tx_nss + 1U);
                }
#endif
                if (datarate->tx_mcs_index != 0xFFU)
                {
                    (void)PRINTF("    MCS:  MCS %d\r\n", (int)datarate->tx_mcs_index);
                }
                else
                {
                    (void)PRINTF("    MCS:  Auto\r\n");
                }

                (void)PRINTF("    Rate: %u Mbps\r\n", datarate->tx_data_rate / 2U);
            }
        }

        (void)PRINTF("  RX: \r\n");
        if ((unsigned int)(datarate->rx_rate_format) <= 3U)
        {
            (void)PRINTF("    Type: %s\r\n", rate_format[datarate->rx_rate_format]);
            if ((datarate->rx_rate_format == MLAN_RATE_FORMAT_LG) && datarate->rx_data_rate <= 11U)
            {
                /* LG */
                (void)PRINTF("    Rate: %s\r\n", lg_rate[datarate->rx_data_rate]);
            }
            else
            {
                /* HT and VHT*/
                if (datarate->rx_ht_bw <= 3U)
                {
                    (void)PRINTF("    BW:   %s\r\n", bw[datarate->rx_ht_bw]);
                }
                if (datarate->rx_ht_gi == 0U)
                {
                    (void)PRINTF("    GI:   Long\r\n");
                }
                else
                {
                    (void)PRINTF("    GI:   Short\r\n");
                }
#if defined(CONFIG_11AC) || defined(CONFIG_11AX)
                if (((unsigned int)(datarate->rx_rate_format) == 2U) ||
                    ((unsigned int)(datarate->rx_rate_format) == 3U))
                {
                    (void)PRINTF("    NSS:  %d\r\n", datarate->rx_nss + 1U);
                }
#endif
                if (datarate->rx_mcs_index != 0xFFU)
                {
                    (void)PRINTF("    MCS:  MCS %d\r\n", (int)datarate->rx_mcs_index);
                }
                else
                {
                    (void)PRINTF("    MCS:  Auto\n");
                }
                (void)PRINTF("    Rate: %u Mbps\r\n", datarate->rx_data_rate / 2U);
            }
        }
#endif
    }
    else
    { /* Do Nothing */
    }
#endif
}

static void dump_wlan_set_txratecfg_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-txratecfg <format> <index> ");
#ifdef CONFIG_11AC
    (void)PRINTF("<nss>\r\n");
#else
    (void)PRINTF("\r\n");
#endif
    (void)PRINTF("\tWhere\r\n");
    (void)PRINTF("\t<format> - This parameter specifies the data rate format used in this command\r\n");
    (void)PRINTF("\t        0:    LG\r\n");
    (void)PRINTF("\t        1:    HT\r\n");
#ifdef CONFIG_11AC
    (void)PRINTF("\t        2:    VHT\r\n");
#endif
#ifdef CONFIG_11AX
    (void)PRINTF("\t        3:    HE\r\n");
#endif
    (void)PRINTF("\t        0xff: Auto\r\n");
    (void)PRINTF("\t<index> - This parameter specifies the rate or MCS index\r\n");
    (void)PRINTF("\tIf <format> is 0 (LG),\r\n");
    (void)PRINTF("\t        0       1 Mbps\r\n");
    (void)PRINTF("\t        1       2 Mbps\r\n");
    (void)PRINTF("\t        2       5.5 Mbps\r\n");
    (void)PRINTF("\t        3       11 Mbps\r\n");
    (void)PRINTF("\t        4       6 Mbps\r\n");
    (void)PRINTF("\t        5       9 Mbps\r\n");
    (void)PRINTF("\t        6       12 Mbps\r\n");
    (void)PRINTF("\t        7       18 Mbps\r\n");
    (void)PRINTF("\t        8       24 Mbps\r\n");
    (void)PRINTF("\t        9       36 Mbps\r\n");
    (void)PRINTF("\t        10      48 Mbps\r\n");
    (void)PRINTF("\t        11      54 Mbps\r\n");
    (void)PRINTF("\tIf <format> is 1 (HT),\r\n");
    (void)PRINTF("\t        0       MCS0\r\n");
    (void)PRINTF("\t        1       MCS1\r\n");
    (void)PRINTF("\t        2       MCS2\r\n");
    (void)PRINTF("\t        3       MCS3\r\n");
    (void)PRINTF("\t        4       MCS4\r\n");
    (void)PRINTF("\t        5       MCS5\r\n");
    (void)PRINTF("\t        6       MCS6\r\n");
    (void)PRINTF("\t        7       MCS7\r\n");
#ifdef CONFIG_11AC
    (void)PRINTF("\tIf <format> is 2 (VHT),\r\n");
    (void)PRINTF("\t        0       MCS0\r\n");
    (void)PRINTF("\t        1       MCS1\r\n");
    (void)PRINTF("\t        2       MCS2\r\n");
    (void)PRINTF("\t        3       MCS3\r\n");
    (void)PRINTF("\t        4       MCS4\r\n");
    (void)PRINTF("\t        5       MCS5\r\n");
    (void)PRINTF("\t        6       MCS6\r\n");
    (void)PRINTF("\t        7       MCS7\r\n");
    (void)PRINTF("\t        8       MCS8\r\n");
    (void)PRINTF("\t        9       MCS9\r\n");
#endif
#ifdef CONFIG_11AX
    (void)PRINTF("\tIf <format> is 3 (HE),\r\n");
    (void)PRINTF("\t        0       MCS0\r\n");
    (void)PRINTF("\t        1       MCS1\r\n");
    (void)PRINTF("\t        2       MCS2\r\n");
    (void)PRINTF("\t        3       MCS3\r\n");
    (void)PRINTF("\t        4       MCS4\r\n");
    (void)PRINTF("\t        5       MCS5\r\n");
    (void)PRINTF("\t        6       MCS6\r\n");
    (void)PRINTF("\t        7       MCS7\r\n");
    (void)PRINTF("\t        8       MCS8\r\n");
    (void)PRINTF("\t        9       MCS9\r\n");
    (void)PRINTF("\t        10      MCS10\r\n");
    (void)PRINTF("\t        11      MCS11\r\n");
#endif
#if defined(CONFIG_11AX) || defined(CONFIG_11AC)
    (void)PRINTF("\t<nss> - This parameter specifies the NSS. It is valid only for VHT and HE\r\n");
    (void)PRINTF("\tIf <format> is 2 (VHT) or 3 (HE),\r\n");
    (void)PRINTF("\t        1       NSS1\r\n");
    (void)PRINTF("\t        2       NSS2\r\n");
#endif
}

static void test_wlan_set_txratecfg(int argc, char **argv)
{
    wlan_ds_rate ds_rate;
    int rv = WM_SUCCESS;

    if (argc < 2 ||
#if defined(CONFIG_11AC) || defined(CONFIG_11AX)
        argc > 4)
    {
#else
        argc > 3)
    {
#endif
        (void)PRINTF("Invalid arguments\r\n");
        goto done;
    }
    (void)memset(&ds_rate, 0, sizeof(wlan_ds_rate));

    ds_rate.sub_command = WIFI_DS_RATE_CFG;

    errno                              = 0;
    ds_rate.param.rate_cfg.rate_format = (mlan_rate_format)(strtol(argv[1], NULL, 0));
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
    errno                             = 0;
    ds_rate.param.rate_cfg.rate_index = (t_u32)strtol(argv[2], NULL, 0);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
#ifdef CONFIG_11AC
    errno                      = 0;
    ds_rate.param.rate_cfg.nss = (t_u32)strtol(argv[3], NULL, 0);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
    else
    {
        /*Do Nothing*/
    }
#endif

#ifdef CONFIG_11AX
    if ((unsigned int)(ds_rate.param.rate_cfg.rate_format) > 3U)
#elif defined(CONFIG_11AC)
    if ((unsigned int)(ds_rate.param.rate_cfg.rate_format) > 2U)
#else
if ((unsigned int)(ds_rate.param.rate_cfg.rate_format) > 1U)
#endif
    {
        (void)PRINTF("Invalid format selection\r\n");
        goto done;
    }

    if ((((unsigned int)(ds_rate.param.rate_cfg.rate_format) == 0U) && (ds_rate.param.rate_cfg.rate_index > 11U))
        || (((unsigned int)(ds_rate.param.rate_cfg.rate_format) == 1U) && (ds_rate.param.rate_cfg.rate_index != 32U) &&
            (ds_rate.param.rate_cfg.rate_index > 7U))
    )
    {
        (void)PRINTF("Invalid index selection\r\n");
        goto done;
    }

#if defined(CONFIG_11AC) || defined(CONFIG_11AX)
    /* NSS is supported up to 2 */
    if ((ds_rate.param.rate_cfg.nss <= 0U) || (ds_rate.param.rate_cfg.nss >= 3U))
    {
        (void)PRINTF("Invalid nss selection\r\n");
        goto done;
    }
#endif
    rv = wlan_set_txratecfg(ds_rate);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set txratecfg\r\n");
        goto done;
    }
    (void)PRINTF("Configured txratecfg as below:\r\n");
    print_ds_rate(ds_rate);
    return;

done:
    dump_wlan_set_txratecfg_usage();
}

static void test_wlan_get_txratecfg(int argc, char **argv)
{
    wlan_ds_rate ds_rate;

    (void)memset(&ds_rate, 0, sizeof(wlan_ds_rate));

    ds_rate.sub_command = WIFI_DS_RATE_CFG;

    int rv = wlan_get_txratecfg(&ds_rate);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get tx rate cfg\r\n");
        return;
    }

    print_ds_rate(ds_rate);
}

static void test_wlan_get_data_rate(int argc, char **argv)
{
    wlan_ds_rate ds_rate;

    (void)memset(&ds_rate, 0, sizeof(wlan_ds_rate));

    ds_rate.sub_command = WIFI_DS_GET_DATA_RATE;

    int rv = wlan_get_data_rate(&ds_rate);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get tx rate cfg\r\n");
        return;
    }

    print_ds_rate(ds_rate);
}

void print_txpwrlimit(wlan_txpwrlimit_t txpwrlimit)
{
    unsigned char i, j;

    (void)PRINTF("--------------------------------------------------------------------------------\r\n");
    (void)PRINTF("Get txpwrlimit: sub_band=%x \r\n", txpwrlimit.subband);
    for (i = 0; i < txpwrlimit.num_chans; i++)
    {
        (void)PRINTF("StartFreq: %d\r\n", txpwrlimit.txpwrlimit_config[i].chan_desc.start_freq);
        (void)PRINTF("ChanWidth: %d\r\n", txpwrlimit.txpwrlimit_config[i].chan_desc.chan_width);
        (void)PRINTF("ChanNum:   %d\r\n", txpwrlimit.txpwrlimit_config[i].chan_desc.chan_num);
        (void)PRINTF("Pwr:");
        for (j = 0; j < txpwrlimit.txpwrlimit_config[i].num_mod_grps; j++)
        {
            if (j == (txpwrlimit.txpwrlimit_config[i].num_mod_grps - 1U))
            {
                (void)PRINTF("%d,%d", txpwrlimit.txpwrlimit_config[i].txpwrlimit_entry[j].mod_group,
                             txpwrlimit.txpwrlimit_config[i].txpwrlimit_entry[j].tx_power);
            }
            else
            {
                (void)PRINTF("%d,%d,", txpwrlimit.txpwrlimit_config[i].txpwrlimit_entry[j].mod_group,
                             txpwrlimit.txpwrlimit_config[i].txpwrlimit_entry[j].tx_power);
            }
        }
        (void)PRINTF("\r\n");
    }
    (void)PRINTF("\r\n");
}

static void print_chanlist(wlan_chanlist_t chanlist)
{
    unsigned char i;

    (void)PRINTF("--------------------------------------------------------------------------------\r\n");
    (void)PRINTF("Number of channels configured: %d\r\n", chanlist.num_chans);
    (void)PRINTF("\r\n");
    for (i = 0; i < chanlist.num_chans; i++)
    {
        (void)PRINTF("ChanNum: %d\t", chanlist.chan_info[i].chan_num);
        (void)PRINTF("ChanFreq: %d\t", chanlist.chan_info[i].chan_freq);
        (void)PRINTF("%s", chanlist.chan_info[i].passive_scan_or_radar_detect ? "Passive" : "Active");
        (void)PRINTF("\r\n");
    }
}

static void dump_wlan_get_txpwrlimit_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-txpwrlimit <subband> \r\n");
    (void)PRINTF("\r\n");
    (void)PRINTF("\t Where subband is: \r\n");
    (void)PRINTF("\t       0x00 2G subband  (2.4G: channel 1-14)\r\n");
#ifdef CONFIG_5GHz_SUPPORT
    (void)PRINTF("\t       0x10 5G subband0 (5G: channel 36,40,44,48,\r\n");
    (void)PRINTF("\t                                     52,56,60,64)\r\n");
    (void)PRINTF("\t       0x11 5G subband1 (5G: channel 100,104,108,112,\r\n");
    (void)PRINTF("\t                                     116,120,124,128,\r\n");
    (void)PRINTF("\t                                     132,136,140,144)\r\n");
    (void)PRINTF("\t       0x12 5G subband2 (5G: channel 149,153,157,161,165,172)\r\n");
    (void)PRINTF("\t       0x13 5G subband3 (5G: channel 183,184,185,187,188,\r\n");
    (void)PRINTF("\t                                     189, 192,196;\r\n");
    (void)PRINTF("\t                         5G: channel 7,8,11,12,16,34)\r\n");
#endif
}

static void test_wlan_get_txpwrlimit(int argc, char **argv)
{
    wifi_SubBand_t subband;
    wlan_txpwrlimit_t txpwrlimit;

    if (argc != 2)
    {
        dump_wlan_get_txpwrlimit_usage();
        return;
    }

    errno   = 0;
    subband = (wifi_SubBand_t)strtol(argv[1], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }

    if (subband != SubBand_2_4_GHz
#ifdef CONFIG_5GHz_SUPPORT
        && subband != SubBand_5_GHz_0 && subband != SubBand_5_GHz_1 && subband != SubBand_5_GHz_2 &&
        subband != SubBand_5_GHz_3
#endif
    )
    {
        dump_wlan_get_txpwrlimit_usage();
        return;
    }

    int rv = wlan_get_txpwrlimit(subband, &txpwrlimit);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get TX PWR Limit configuration\r\n");
    }
    else
    {
        print_txpwrlimit(txpwrlimit);
    }
}

static void test_wlan_set_txpwrlimit(int argc, char **argv)
{
    wlan_txpwrlimit_t txpwrlimit;

    (void)memset(&txpwrlimit, 0x00, sizeof(wlan_txpwrlimit_t));

    int rv = wlan_set_txpwrlimit(&tx_pwrlimit_2g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G TX PWR Limit configuration\r\n");
    }
    else
    {
#ifdef CONFIG_5GHz_SUPPORT
        rv = wlan_set_txpwrlimit(&tx_pwrlimit_5g_cfg);
        if (rv != WM_SUCCESS)
        {
            (void)PRINTF("Unable to set 5G TX PWR Limit configuration\r\n");
        }
        else
        {
#endif
            txpwrlimit.subband = SubBand_2_4_GHz;
            rv                 = wlan_get_txpwrlimit(txpwrlimit.subband, &txpwrlimit);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get 2G TX PWR Limit configuration\r\n");
            }
            else
            {
                print_txpwrlimit(txpwrlimit);
            }
#ifdef CONFIG_5GHz_SUPPORT
            txpwrlimit.subband = SubBand_5_GHz_0;
            rv                 = wlan_get_txpwrlimit(txpwrlimit.subband, &txpwrlimit);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get 5G SubBand0 TX PWR Limit configuration\r\n");
            }
            else
            {
                print_txpwrlimit(txpwrlimit);
            }
            txpwrlimit.subband = SubBand_5_GHz_1;
            rv                 = wlan_get_txpwrlimit(txpwrlimit.subband, &txpwrlimit);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get 5G SubBand1 TX PWR Limit configuration\r\n");
            }
            else
            {
                print_txpwrlimit(txpwrlimit);
            }
            txpwrlimit.subband = SubBand_5_GHz_2;
            rv                 = wlan_get_txpwrlimit(txpwrlimit.subband, &txpwrlimit);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get 5G SubBand2 TX PWR Limit configuration\r\n");
            }
            else
            {
                print_txpwrlimit(txpwrlimit);
            }
        }
#endif
    }
}

static void test_wlan_set_chanlist_and_txpwrlimit(int argc, char **argv)
{
    wlan_txpwrlimit_t txpwrlimit;

    (void)memset(&txpwrlimit, 0x00, sizeof(wlan_txpwrlimit_t));

    int rv = wlan_set_chanlist_and_txpwrlimit(&chanlist_2g_cfg, &tx_pwrlimit_2g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G TX PWR Limit configuration\r\n");
    }
    else
    {
#ifdef CONFIG_5GHz_SUPPORT
        rv = wlan_set_chanlist_and_txpwrlimit(&chanlist_5g_cfg, &tx_pwrlimit_5g_cfg);
        if (rv != WM_SUCCESS)
        {
            (void)PRINTF("Unable to set 5G TX PWR Limit configuration\r\n");
        }
        else
        {
#endif
            txpwrlimit.subband = SubBand_2_4_GHz;
            rv                 = wlan_get_txpwrlimit(txpwrlimit.subband, &txpwrlimit);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get 2G TX PWR Limit configuration\r\n");
            }
            else
            {
                print_txpwrlimit(txpwrlimit);
            }
#ifdef CONFIG_5GHz_SUPPORT
            txpwrlimit.subband = SubBand_5_GHz_0;
            rv                 = wlan_get_txpwrlimit(txpwrlimit.subband, &txpwrlimit);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get 5G SubBand0 TX PWR Limit configuration\r\n");
            }
            else
            {
                print_txpwrlimit(txpwrlimit);
            }
            txpwrlimit.subband = SubBand_5_GHz_1;
            rv                 = wlan_get_txpwrlimit(txpwrlimit.subband, &txpwrlimit);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get 5G SubBand1 TX PWR Limit configuration\r\n");
            }
            else
            {
                print_txpwrlimit(txpwrlimit);
            }
            txpwrlimit.subband = SubBand_5_GHz_2;
            rv                 = wlan_get_txpwrlimit(txpwrlimit.subband, &txpwrlimit);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get 5G SubBand2 TX PWR Limit configuration\r\n");
            }
            else
            {
                print_txpwrlimit(txpwrlimit);
            }
        }
#endif
        wlan_chanlist_t chanlist;

        (void)memset(&chanlist, 0x00, sizeof(wlan_chanlist_t));
        rv = wlan_get_chanlist(&chanlist);
        if (rv != WM_SUCCESS)
        {
            (void)PRINTF("Unable to get channel list configuration\r\n");
        }
        else
        {
            print_chanlist(chanlist);
        }
    }
}

static void test_wlan_set_chanlist(int argc, char **argv)
{
    wlan_chanlist_t chanlist;

    (void)memset(&chanlist, 0x00, sizeof(wlan_chanlist_t));

    int rv = wlan_set_chanlist(&chanlist_2g_cfg);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to set 2G channel list configuration\r\n");
    }
    else
    {
#ifdef CONFIG_5GHz_SUPPORT
        rv = wlan_set_chanlist(&chanlist_5g_cfg);
        if (rv != WM_SUCCESS)
        {
            (void)PRINTF("Unable to set 5G channel list configuration\r\n");
        }
        else
        {
#endif
            rv = wlan_get_chanlist(&chanlist);
            if (rv != WM_SUCCESS)
            {
                (void)PRINTF("Unable to get channel list configuration\r\n");
            }
            else
            {
                print_chanlist(chanlist);
            }
#ifdef CONFIG_5GHz_SUPPORT
        }
#endif
    }
}

static void test_wlan_get_chanlist(int argc, char **argv)
{
    wlan_chanlist_t chanlist;

    (void)memset(&chanlist, 0x00, sizeof(wlan_chanlist_t));
    int rv = wlan_get_chanlist(&chanlist);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get channel list configuration\r\n");
    }
    else
    {
        print_chanlist(chanlist);
    }
}

#ifdef CONFIG_11AX
static void dump_wlan_set_txomi_usage()
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-tx-omi <tx-omi>\r\n");
    (void)PRINTF("where, tx-omi =\r\n");
    (void)PRINTF("Bit 0-2: Rx NSS\r\n");
    (void)PRINTF("Bit 3-4: Channel Width\r\n");
    (void)PRINTF("Bit 6  : Tx NSTS (applies to client mode only)\r\n");
}

static void test_wlan_set_tx_omi(int argc, char **argv)
{
    int ret;

    uint16_t tx_omi;

    if (argc != 2)
    {
        dump_wlan_set_txomi_usage();
        return;
    }

    errno  = 0;
    tx_omi = (uint16_t)strtol(argv[1], NULL, 0);

    if (errno != 0)
        (void)PRINTF("Error during strtoul errno:%d", errno);

    ret = wlan_set_11ax_tx_omi(tx_omi);

    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("TX OMI: 0x%x set\r\n", tx_omi);
    }
    else
    {
        (void)PRINTF("Unable to set TX OMI: 0x%x\r\n", tx_omi);
    }
}
#endif

static struct cli_command wlan_enhanced_commands[] = {
    {"wlan-set-regioncode", "<region-code>", test_wlan_set_regioncode},
    {"wlan-get-regioncode", NULL, test_wlan_get_regioncode},
    {"wlan-get-txpwrlimit", "<subband>", test_wlan_get_txpwrlimit},
    {"wlan-set-txpwrlimit", NULL, test_wlan_set_txpwrlimit},
    {"wlan-set-chanlist-and-txpwrlimit", NULL, test_wlan_set_chanlist_and_txpwrlimit},
    {"wlan-set-chanlist", NULL, test_wlan_set_chanlist},
    {"wlan-get-chanlist", NULL, test_wlan_get_chanlist},
#ifdef CONFIG_11AC
    {"wlan-set-txratecfg", "<format> <index> <nss>", test_wlan_set_txratecfg},
#else
    {"wlan-set-txratecfg", "<format> <index>", test_wlan_set_txratecfg},
#endif
    {"wlan-get-txratecfg", NULL, test_wlan_get_txratecfg},
    {"wlan-get-data-rate", NULL, test_wlan_get_data_rate},
    {"wlan-set-pmfcfg", "<mfpc> <mfpr>", wlan_pmfcfg_set},
    {"wlan-get-pmfcfg", NULL, wlan_pmfcfg_get},
    {"wlan-set-antcfg", "<ant mode> [evaluate_time]", wlan_antcfg_set},
    {"wlan-get-antcfg", NULL, wlan_antcfg_get},
#ifdef CONFIG_5GHz_SUPPORT
    {"wlan-set-ed-mac-mode", "<ed_ctrl_2g> <ed_offset_2g> <ed_ctrl_5g> <ed_offset_5g>", wlan_ed_mac_mode_set},
#else
    {"wlan-set-ed-mac-mode", "<ed_ctrl_2g> <ed_offset_2g>", wlan_ed_mac_mode_set},
#endif
    {"wlan-get-ed-mac-mode", NULL, wlan_ed_mac_mode_get},
#ifdef CONFIG_11AX
    {"wlan-set-tx-omi", "<tx-omi>", test_wlan_set_tx_omi},
#endif
};

int wlan_enhanced_cli_init(void)
{
    if (cli_register_commands(wlan_enhanced_commands,
                              (int)(sizeof(wlan_enhanced_commands) / sizeof(struct cli_command))) != 0)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}
