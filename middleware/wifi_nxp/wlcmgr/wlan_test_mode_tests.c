/** @file wlan_test_mode_tests.c
 *
 *  @brief  This file provides WLAN Test Mode APIs
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

/*
 * NXP Test Framework (MTF) functions
 */

#ifdef CONFIG_RF_TEST_MODE

static bool rf_test_mode = false;

static void dump_wlan_set_rf_test_mode_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-rf-test-mode \r\n");
    (void)PRINTF("\r\n");
}

static void dump_wlan_set_rf_test_mode(void)
{
    (void)PRINTF("RF Test Mode is not set\r\n");
    dump_wlan_set_rf_test_mode_usage();
}

static void wlan_rf_test_mode_set(int argc, char *argv[])
{
    int ret;

    if (argc != 1)
    {
        dump_wlan_set_rf_test_mode_usage();
        return;
    }

    ret = wlan_set_rf_test_mode();
    if (ret == WM_SUCCESS)
    {
        rf_test_mode = true;
        (void)PRINTF("RF Test Mode configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("RF Test Mode configuration failed\r\n");
        dump_wlan_set_rf_test_mode_usage();
    }
}

static void dump_wlan_set_channel_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-rf-channel <channel> \r\n");
    (void)PRINTF("\r\n");
}

static void wlan_rf_channel_set(int argc, char *argv[])
{
    int ret;
    uint8_t channel;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 2)
    {
        dump_wlan_set_channel_usage();
        return;
    }

    channel = strtol(argv[1], NULL, 10);

    ret = wlan_set_rf_channel(channel);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Channel configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("Channel configuration failed\r\n");
        dump_wlan_set_channel_usage();
    }
}

static void dump_wlan_get_channel_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-rf-channel \r\n");
}

static void wlan_rf_channel_get(int argc, char *argv[])
{
    int ret;
    uint8_t channel;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 1)
    {
        dump_wlan_get_channel_usage();
        return;
    }

    ret = wlan_get_rf_channel(&channel);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Configured channel is: %d\r\n", channel);
    }
    else
    {
        (void)PRINTF("Channel configuration read failed\r\n");
        dump_wlan_get_channel_usage();
    }
}

static void dump_wlan_set_rf_band_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-rf-band <band> \r\n");
#ifdef CONFIG_5GHz_SUPPORT
    (void)PRINTF("band: 0=2.4G, 1=5G \r\n");
#else
    (void)PRINTF("band: 0=2.4G \r\n");
#endif
    (void)PRINTF("\r\n");
}

static void wlan_rf_band_set(int argc, char *argv[])
{
    int ret;
    uint8_t band;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 2)
    {
        dump_wlan_set_rf_band_usage();
        return;
    }

    band = strtol(argv[1], NULL, 10);

    if (band != 0U
#ifdef CONFIG_5GHz_SUPPORT
        && band != 1U
#endif
    )
    {
        dump_wlan_set_rf_band_usage();
        return;
    }

    ret = wlan_set_rf_band(band);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("RF Band configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("RF Band configuration failed\r\n");
        dump_wlan_set_rf_band_usage();
    }
}

static void dump_wlan_get_rf_band_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-rf-band \r\n");
}

static void wlan_rf_band_get(int argc, char *argv[])
{
    int ret;
    uint8_t band;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 1)
    {
        dump_wlan_get_rf_band_usage();
        return;
    }

    ret = wlan_get_rf_band(&band);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Configured RF Band is: %s\r\n", band ? "5G" : "2.4G");
    }
    else
    {
        (void)PRINTF("RF Band configuration read failed\r\n");
        dump_wlan_get_rf_band_usage();
    }
}

static void dump_wlan_set_bandwidth_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-bandwidth <bandwidth> \r\n");
    (void)PRINTF("\r\n");
    (void)PRINTF("\t<bandwidth>: \r\n");
    (void)PRINTF("\t        0: 20MHz\r\n");
#ifdef CONFIG_5GHz_SUPPORT
    (void)PRINTF("\t        1: 40MHz\r\n");
#endif
#ifdef CONFIG_11AC
    (void)PRINTF("\t        4: 80MHz\r\n");
#endif
    (void)PRINTF("\r\n");
}

static void wlan_rf_bandwidth_set(int argc, char *argv[])
{
    int ret;
    uint8_t bandwidth;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 2)
    {
        dump_wlan_set_bandwidth_usage();
        return;
    }

    bandwidth = strtol(argv[1], NULL, 10);

    ret = wlan_set_rf_bandwidth(bandwidth);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Bandwidth configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("Bandwidth configuration failed\r\n");
        dump_wlan_set_bandwidth_usage();
    }
}

static void dump_wlan_get_bandwidth_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-rf-bandwidth \r\n");
}

static void wlan_rf_bandwidth_get(int argc, char *argv[])
{
    int ret;
    uint8_t bandwidth;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 1)
    {
        dump_wlan_get_bandwidth_usage();
        return;
    }

    ret = wlan_get_rf_bandwidth(&bandwidth);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Configured bandwidth is: %s\r\n", bandwidth == 0 ? "20MHz" : bandwidth == 1U ? "40MHz" : "80MHz");
    }
    else
    {
        (void)PRINTF("Bandwidth configuration read failed\r\n");
        dump_wlan_get_bandwidth_usage();
    }
}

static void dump_wlan_get_and_reset_per_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-rf-per \r\n");
}

static void wlan_rf_per_get(int argc, char *argv[])
{
    int ret;
    uint32_t rx_tot_pkt_count, rx_mcast_bcast_count, rx_pkt_fcs_error;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 1)
    {
        dump_wlan_get_and_reset_per_usage();
        return;
    }

    ret = wlan_get_rf_per(&rx_tot_pkt_count, &rx_mcast_bcast_count, &rx_pkt_fcs_error);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("PER is as below: \r\n");
        (void)PRINTF("  Total Rx Packet Count                    : %d\r\n", rx_tot_pkt_count);
        (void)PRINTF("  Total Rx Multicast/Broadcast Packet Count: %d\r\n", rx_mcast_bcast_count);
        (void)PRINTF("  Total Rx Packets with FCS error          : %d\r\n", rx_pkt_fcs_error);
    }
    else
    {
        (void)PRINTF("PER configuration read failed\r\n");
        dump_wlan_get_and_reset_per_usage();
    }
}

static void dump_wlan_set_tx_cont_mode_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF(
        "wlan-set-rf-tx-cont-mode <enable_tx> <cw_mode> <payload_pattern> <cs_mode> <act_sub_ch> <tx_rate> \r\n");
    (void)PRINTF("Enable                (0:disable, 1:enable)\r\n");
    (void)PRINTF("Continuous Wave Mode  (0:disable, 1:enable)\r\n");
    (void)PRINTF("Payload Pattern       (0 to 0xFFFFFFFF) (Enter hexadecimal value)\r\n");
    (void)PRINTF("CS Mode               (Applicable only when continuous wave is disabled) (0:disable, 1:enable)\r\n");
    (void)PRINTF("Active SubChannel     (0:low, 1:upper, 3:both)\r\n");
    (void)PRINTF("Tx Data Rate          (Rate Index corresponding to legacy/HT/VHT rates)\r\n");
    (void)PRINTF("\r\n");
    (void)PRINTF("To Disable:\r\n");
    (void)PRINTF("wlan-set-rf-tx-cont-mode 0\r\n");
    (void)PRINTF("\r\n");
}

static void wlan_rf_tx_cont_mode_set(int argc, char *argv[])
{
    int ret;
    uint32_t enable_tx, cw_mode, payload_pattern, cs_mode, act_sub_ch, tx_rate;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc == 2 && strtol(argv[1], NULL, 10) == 0)
    {
        enable_tx       = 0;
        cw_mode         = 0;
        payload_pattern = 0;
        cs_mode         = 0;
        act_sub_ch      = 0;
        tx_rate         = 0;
        goto disable;
    }
    else if (argc != 7)
    {
        dump_wlan_set_tx_cont_mode_usage();
        return;
    }
    else
    { /*Do nothing*/
    }

    enable_tx       = strtol(argv[1], NULL, 10);
    cw_mode         = strtol(argv[2], NULL, 10);
    errno           = 0;
    payload_pattern = strtol(argv[3], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
    cs_mode    = strtol(argv[4], NULL, 10);
    act_sub_ch = strtol(argv[5], NULL, 10);
    tx_rate    = strtol(argv[6], NULL, 10);

disable:
    ret = wlan_set_rf_tx_cont_mode(enable_tx, cw_mode, payload_pattern, cs_mode, act_sub_ch, tx_rate);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Tx continuous configuration successful\r\n");
        (void)PRINTF("  Enable                : %s\r\n", enable_tx ? "enable" : "disable");
        (void)PRINTF("  Continuous Wave Mode  : %s\r\n", cw_mode ? "enable" : "disable");
        (void)PRINTF("  Payload Pattern       : 0x%08X\r\n", payload_pattern);
        (void)PRINTF("  CS Mode               : %s\r\n", cs_mode ? "enable" : "disable");
        (void)PRINTF("  Active SubChannel     : %s\r\n",
                     act_sub_ch == 0U ? "low" : act_sub_ch == 1U ? "upper" : "both");
        (void)PRINTF("  Tx Data Rate          : %d\r\n", tx_rate);
    }
    else
    {
        (void)PRINTF("Tx continuous configuration failed\r\n");
        dump_wlan_set_tx_cont_mode_usage();
    }
}

static void dump_wlan_set_tx_antenna_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-rf-tx-antenna <antenna> \r\n");
    (void)PRINTF("antenna: 1=Main, 2=Aux \r\n");
    (void)PRINTF("\r\n");
}

static void wlan_rf_tx_antenna_set(int argc, char *argv[])
{
    int ret;
    uint8_t ant;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 2)
    {
        dump_wlan_set_tx_antenna_usage();
        return;
    }

    ant = strtol(argv[1], NULL, 10);

    if (ant != 1U && ant != 2U)
    {
        dump_wlan_set_tx_antenna_usage();
        return;
    }

    ret = wlan_set_rf_tx_antenna(ant);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Tx Antenna configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("Tx Antenna configuration failed\r\n");
        dump_wlan_set_tx_antenna_usage();
    }
}

static void dump_wlan_get_tx_antenna_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-rf-tx-antenna \r\n");
}

static void wlan_rf_tx_antenna_get(int argc, char *argv[])
{
    int ret;
    uint8_t ant;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 1)
    {
        dump_wlan_get_tx_antenna_usage();
        return;
    }

    ret = wlan_get_rf_tx_antenna(&ant);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Configured Tx Antenna is: %s\r\n", ant == 1U ? "Main" : "Aux");
    }
    else
    {
        (void)PRINTF("Tx Antenna configuration read failed\r\n");
        dump_wlan_get_tx_antenna_usage();
    }
}

static void dump_wlan_set_rx_antenna_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-rf-rx-antenna <antenna> \r\n");
    (void)PRINTF("antenna: 1=Main, 2=Aux \r\n");
    (void)PRINTF("\r\n");
}

static void wlan_rf_rx_antenna_set(int argc, char *argv[])
{
    int ret;
    uint8_t ant;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 2)
    {
        dump_wlan_set_rx_antenna_usage();
        return;
    }

    ant = strtol(argv[1], NULL, 10);

    if (ant != 1U && ant != 2U)
    {
        dump_wlan_set_rx_antenna_usage();
        return;
    }

    ret = wlan_set_rf_rx_antenna(ant);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Rx Antenna configuration successful\r\n");
    }
    else
    {
        (void)PRINTF("Rx Antenna configuration failed\r\n");
        dump_wlan_set_rx_antenna_usage();
    }
}

static void dump_wlan_get_rx_antenna_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-get-rf-rx-antenna \r\n");
}

static void wlan_rf_rx_antenna_get(int argc, char *argv[])
{
    int ret;
    uint8_t ant;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 1)
    {
        dump_wlan_get_rx_antenna_usage();
        return;
    }

    ret = wlan_get_rf_rx_antenna(&ant);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Configured Rx Antenna is: %s\r\n", ant == 1U ? "Main" : "Aux");
    }
    else
    {
        (void)PRINTF("Rx Antenna configuration read failed\r\n");
        dump_wlan_get_rx_antenna_usage();
    }
}

static void dump_wlan_set_tx_power_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-set-rf-tx-power <tx_power> <modulation> <path_id> \r\n");
    (void)PRINTF("Power       (0 to 24 dBm)\r\n");
    (void)PRINTF("Modulation  (0: CCK, 1:OFDM, 2:MCS)\r\n");
    (void)PRINTF("Path ID     (0: PathA, 1:PathB, 2:PathA+B)\r\n");
    (void)PRINTF("\r\n");
}

static void wlan_rf_tx_power_set(int argc, char *argv[])
{
    int ret;
    uint8_t power;
    uint8_t mod;
    uint8_t path_id;

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc != 4)
    {
        dump_wlan_set_tx_power_usage();
        return;
    }

    power   = strtol(argv[1], NULL, 10);
    mod     = strtol(argv[2], NULL, 10);
    path_id = strtol(argv[3], NULL, 10);

    if (power > 24U)
    {
        dump_wlan_set_rx_antenna_usage();
        return;
    }

    if (mod != 0U && mod != 1U && mod != 2U)
    {
        dump_wlan_set_rx_antenna_usage();
        return;
    }

    if (path_id != 0U && path_id != 1U && path_id != 2U)
    {
        dump_wlan_set_rx_antenna_usage();
        return;
    }

    ret = wlan_set_rf_tx_power(power, mod, path_id);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Tx Power configuration successful\r\n");
        (void)PRINTF("  Power         : %d dBm\r\n", power);
        (void)PRINTF("  Modulation    : %s\r\n", mod == 0 ? "CCK" : mod == 1 ? "OFDM" : "MCS");
        (void)PRINTF("  Path ID       : %s\r\n", path_id == 0 ? "PathA" : path_id == 1 ? "PathB" : "PathA+B");
    }
    else
    {
        (void)PRINTF("Tx Power configuration failed\r\n");
        dump_wlan_set_tx_power_usage();
    }
}

static void dump_wlan_set_tx_frame_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF(
        "wlan-set-rf-tx-frame <start> <data_rate> <frame_pattern> <frame_len> <adjust_burst_sifs> <burst_sifs_in_us> "
        "<short_preamble> <act_sub_ch> <short_gi> <adv_coding> <tx_bf> <gf_mode> <stbc> <bssid>\r\n");
    (void)PRINTF("Enable                 (0:disable, 1:enable)\r\n");
    (void)PRINTF("Tx Data Rate           (Rate Index corresponding to legacy/HT/VHT rates)\r\n");
    (void)PRINTF("Payload Pattern        (0 to 0xFFFFFFFF) (Enter hexadecimal value)\r\n");
    (void)PRINTF("Payload Length         (1 to 0x400) (Enter hexadecimal value)\r\n");
    (void)PRINTF("Adjust Burst SIFS3 Gap (0:disable, 1:enable)\r\n");
    (void)PRINTF("Burst SIFS in us       (0 to 255us)\r\n");
    (void)PRINTF("Short Preamble         (0:disable, 1:enable)\r\n");
    (void)PRINTF("Active SubChannel      (0:low, 1:upper, 3:both)\r\n");
    (void)PRINTF("Short GI               (0:disable, 1:enable)\r\n");
    (void)PRINTF("Adv Coding             (0:disable, 1:enable)\r\n");
    (void)PRINTF("Beamforming            (0:disable, 1:enable)\r\n");
    (void)PRINTF("GreenField Mode        (0:disable, 1:enable)\r\n");
    (void)PRINTF("STBC                   (0:disable, 1:enable)\r\n");
    (void)PRINTF("BSSID                  (xx:xx:xx:xx:xx:xx)\r\n");
    (void)PRINTF("\r\n");
    (void)PRINTF("To Disable:\r\n");
    (void)PRINTF("wlan-set-rf-tx-frame 0\r\n");
    (void)PRINTF("\r\n");
}

static void wlan_rf_tx_frame_set(int argc, char *argv[])
{
    int ret;
    uint32_t enable;
    uint32_t data_rate;
    uint32_t frame_pattern;
    uint32_t frame_length;
    uint32_t adjust_burst_sifs;
    uint32_t burst_sifs_in_us;
    uint32_t short_preamble;
    uint32_t act_sub_ch;
    uint32_t short_gi;
    uint32_t adv_coding;
    uint32_t tx_bf;
    uint32_t gf_mode;
    uint32_t stbc;
    uint32_t bssid[MLAN_MAC_ADDR_LENGTH];

    if (!rf_test_mode)
    {
        dump_wlan_set_rf_test_mode();
        return;
    }

    if (argc == 2 && strtol(argv[1], NULL, 10) == 0)
    {
        enable            = 0;
        data_rate         = 0;
        frame_pattern     = 0;
        frame_length      = 1;
        adjust_burst_sifs = 0;
        burst_sifs_in_us  = 0;
        short_preamble    = 0;
        act_sub_ch        = 0;
        short_gi          = 0;
        adv_coding        = 0;
        tx_bf             = 0;
        gf_mode           = 0;
        stbc              = 0;
        (void)memset(bssid, 0, MLAN_MAC_ADDR_LENGTH);
        goto disable;
    }
    else if (argc != 15)
    {
        dump_wlan_set_tx_frame_usage();
        return;
    }

    enable        = strtol(argv[1], NULL, 10);
    data_rate     = strtol(argv[2], NULL, 10);
    errno         = 0;
    frame_pattern = strtol(argv[3], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
    errno        = 0;
    frame_length = strtol(argv[4], NULL, 16);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul errno:%d", errno);
    }
    adjust_burst_sifs = strtol(argv[5], NULL, 10);
    burst_sifs_in_us  = strtol(argv[6], NULL, 10);
    short_preamble    = strtol(argv[7], NULL, 10);
    act_sub_ch        = strtol(argv[8], NULL, 10);
    short_gi          = strtol(argv[9], NULL, 10);
    adv_coding        = strtol(argv[10], NULL, 10);
    tx_bf             = strtol(argv[11], NULL, 10);
    gf_mode           = strtol(argv[12], NULL, 10);
    stbc              = strtol(argv[13], NULL, 10);
    ret               = get_mac((const char *)argv[14], (char *)bssid, ':');
    if (ret != 0U)
    {
        dump_wlan_set_tx_frame_usage();
        return;
    }

    if (enable > 1U || frame_length < 1 || frame_length > 0x400U || burst_sifs_in_us > 255U || short_preamble > 1U ||
        act_sub_ch == 2 || act_sub_ch > 3 || short_gi > 1U || adv_coding > 1U || tx_bf > 1U || gf_mode > 1U ||
        stbc > 1U)
    {
        dump_wlan_set_tx_frame_usage();
        return;
    }

disable:
    ret = wlan_set_rf_tx_frame(enable, data_rate, frame_pattern, frame_length, adjust_burst_sifs, burst_sifs_in_us,
                               short_preamble, act_sub_ch, short_gi, adv_coding, tx_bf, gf_mode, stbc, bssid);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Tx Frame configuration successful\r\n");
        (void)PRINTF("  Enable                    : %s\r\n", enable ? "enable" : "disable");
        (void)PRINTF("  Tx Data Rate              : %d\r\n", data_rate);
        (void)PRINTF("  Payload Pattern           : 0x%08X\r\n", frame_pattern);
        (void)PRINTF("  Payload Length            : 0x%08X\r\n", frame_length);
        (void)PRINTF("  Adjust Burst SIFS3 Gap    : %s\r\n", adjust_burst_sifs ? "enable" : "disable");
        (void)PRINTF("  Burst SIFS in us          : %d us\r\n", burst_sifs_in_us);
        (void)PRINTF("  Short Preamble            : %s\r\n", short_preamble ? "enable" : "disable");
        (void)PRINTF("  Active SubChannel         : %s\r\n",
                     act_sub_ch == 0U ? "low" : act_sub_ch == 1U ? "upper" : "both");
        (void)PRINTF("  Short GI                  : %s\r\n", short_gi ? "enable" : "disable");
        (void)PRINTF("  Adv Coding                : %s\r\n", adv_coding ? "enable" : "disable");
        (void)PRINTF("  Beamforming               : %s\r\n", tx_bf ? "enable" : "disable");
        (void)PRINTF("  GreenField Mode           : %s\r\n", gf_mode ? "enable" : "disable");
        (void)PRINTF("  STBC                      : %s\r\n", stbc ? "enable" : "disable");
        (void)PRINTF("  BSSID                     : ");
        print_mac((const char *)bssid);
        (void)PRINTF("\r\n");
    }
    else
    {
        (void)PRINTF("Tx Frame configuration failed\r\n");
        dump_wlan_set_tx_frame_usage();
    }
}

static struct cli_command wlan_test_mode_commands[] = {
    {"wlan-set-rf-test-mode", NULL, wlan_rf_test_mode_set},
    {"wlan-set-rf-tx-antenna", "<antenna>", wlan_rf_tx_antenna_set},
    {"wlan-get-rf-tx-antenna", NULL, wlan_rf_tx_antenna_get},
    {"wlan-set-rf-rx-antenna", "<antenna>", wlan_rf_rx_antenna_set},
    {"wlan-get-rf-rx-antenna", NULL, wlan_rf_rx_antenna_get},
    {"wlan-set-rf-band", "<band>", wlan_rf_band_set},
    {"wlan-get-rf-band", NULL, wlan_rf_band_get},
    {"wlan-set-rf-bandwidth", "<bandwidth>", wlan_rf_bandwidth_set},
    {"wlan-get-rf-bandwidth", NULL, wlan_rf_bandwidth_get},
    {"wlan-set-rf-channel", "<channel>", wlan_rf_channel_set},
    {"wlan-get-rf-channel", NULL, wlan_rf_channel_get},
    {"wlan-set-rf-tx-power", "<tx_power> <modulation> <path_id>", wlan_rf_tx_power_set},
    {"wlan-set-rf-tx-cont-mode", "<enable_tx> <cw_mode> <payload_pattern> <cs_mode> <act_sub_ch> <tx_rate>",
     wlan_rf_tx_cont_mode_set},
    {"wlan-set-rf-tx-frame",
     "<start> <data_rate> <frame_pattern> <frame_len> <adjust_burst_sifs> <burst_sifs_in_us> <short_preamble> "
     "<act_sub_ch> <short_gi> <adv_coding> <tx_bf> <gf_mode> <stbc> <bssid>",
     wlan_rf_tx_frame_set},
    {"wlan-get-and-reset-rf-per", NULL, wlan_rf_per_get},
};

int wlan_test_mode_cli_init(void)
{
    if (cli_register_commands(wlan_test_mode_commands, sizeof(wlan_test_mode_commands) / sizeof(struct cli_command)) !=
        0U)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}
#endif
