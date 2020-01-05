// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// � Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// � Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// � Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================
/*
 * $FileName: wmiconfig.c$
 * $Version : $
 * $Date    : May-20-2011$
 *
 * Comments: Handles all Atheros defined wmiconfig command implementations.
 *
 *END************************************************************************/

#include <main.h>
#include "throughput.h"
#include <stdio.h>

#if ENABLE_P2P_MODE
#include "p2p.h"
char p2p_wps_pin_val[P2P_WPS_PIN_LEN], set_channel_p2p[P2P_CHANNEL_LEN];
int32_t p2p_callback(uint8_t evt_flag);
WMI_NOA_INFO_STRUCT p2p_noa;
#endif

#include "pmu.h"
#include "dset_api.h"
#include "qcom_api.h"

#if !ENABLE_STACK_OFFLOAD
#else
#include "common_stack_offload.h"
#endif

uint16_t ap_channel_hint;
QCOM_SSID ssid_str_concurrent = {0};
#define AUTOIP_BASE_ADDR 0xA9FE0100
#define AUTOIP_MAX_ADDR 0xA9FEFEFF

/* Set defaults for Base & range on AUtoIP address pool */
unsigned long dBASE_AUTO_IP_ADDRESS = AUTOIP_BASE_ADDR; /* 169.254.1.0 */
unsigned long dMAX_AUTO_IP_ADDRESS  = AUTOIP_MAX_ADDR;  /* 169.254.254.255 */

/**************************Globals ************************************************/
uint8_t pmk_flag = 0, hidden_flag = 0, wps_flag = 0;
AP_CFG_CMD ap_config_param;
wifimode_t mode_flag      = MODE_STATION;
WMI_POWER_MODE power_mode = REC_POWER;
QCOM_SSID ssid            = {0};
wps_context_t wps_context;
uint32_t wifi_connected_flag = 0, concurrent_connect_flag = 0;
bool p2pMode = FALSE;
#if ENABLE_P2P_MODE
int32_t task2_msec             = 100;
uint8_t p2p_connection_default = 0, p2p_persistent_done = 0, p2p_cancel_enable = 0, p2p_session_in_progress = 0; /*
                                       0 - Allow other P2P Device to connect(Accept connection)
                                       1 - Allow IOE Device to connect(Issue connect)
                                    */
char p2p_wps_pin[WPS_PIN_LEN + 1] = {0};
uint8_t p2p_intent                = 0;
bool autogo_newpp                 = false;
WMI_PERSISTENT_MAC_LIST p2p_pers_data[MAX_LIST_COUNT];
uint8_t invitation_index = 0, p2p_join_mac_addr[ATH_MAC_LEN] = {0}, p2p_join_session_active = 0;
WMI_P2P_FW_CONNECT_CMD_STRUCT p2p_join_profile;
uint8_t p2p_persistent_go = 0; /* This variable is used to remember persistent
                                 information while waiting for GO_NEG complete event */
#endif
uint16_t channel_array[] = {0, 2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462, 2467, 2472, 2482};

#define WOW_DEFAULT_FILTER_SIZE 8
uint8_t default_pattern[WOW_DEFAULT_FILTER_SIZE] = {0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};

QCOM_WOW_GPIO default_wow_gpio_config = {.gpio = 17, .isActiveLow = 0, .triggerMechanism = GPIO_LEVEL_TRIGGER};

QCOM_WOW_PATTERN default_wow_pattern = {
    .pattern_size  = WOW_DEFAULT_FILTER_SIZE,
    .offset        = 0,
    .pattern_index = 0,
};

static int security_mode = 0;
WLAN_CRYPT_TYPE cipher;
WLAN_AUTH_MODE wpa_ver;
wepkey_t key[MAX_NUM_WEP_KEYS];
uint32_t wep_keyindex;
char wpsPin[MAX_WPS_PIN_SIZE + 1] = {0};
QCOM_PASSPHRASE wpa_passphrase    = {0};
volatile uint8_t wifi_state       = 0;
int32_t block_on_dhcp(int32_t argc, char *argv[]);
int active_device = 0;
/************ Function Prototypes *********************************************/
uint32_t set_scan(uint32_t dev_num, char *ssid, uint32_t *pNum_ap /* [OUT] */);
void resetTarget(void);
void init_wep_keys(void);
int32_t clear_wep_keys(void);
static int32_t connect_handler(int32_t index, int32_t argc, char *argv[]);
static int32_t set_pmparams(int32_t index, int32_t argc, char *argv[]);
static int32_t get_rate(void);
static int32_t set_rate(int32_t index, int32_t argc, char *argv[]);
static int32_t set_tx_power_scale(int32_t index, int32_t argc, char *argv[]);
static int32_t set_gtx(char *gtx);
static int32_t set_lpl(char *lpl);
static int32_t deviceid_handler(int32_t index, int32_t argc, char *argv[]);
static int32_t gpio_handle(int32_t argc, char *argv[]);
static int32_t set_active_deviceid(uint16_t device_index);
int32_t get_active_device(void);
static int32_t set_wep_key(char *key_index, char *key_val);
void wifiCallback(int val, uint8_t devId, uint8_t *mac, bool bssConn);
static int32_t set_tcp_connection_timeout(int32_t argc, char *argv[]);
#if ENABLE_P2P_MODE
static int32_t p2p_handler(int32_t index, int32_t argc, char_ptr argv[]);
void P2PCallBackfn(uint8_t *ptr);
#endif

#if ENABLE_AP_MODE
static int32_t ap_handler(int32_t index, int32_t argc, char_ptr argv[]);
static int32_t set_ap_params(uint16_t param_cmd, uint8_t *data_val);
#endif

int32_t get_phy_mode(uint32_t dev_num, char *phy_mode);
static int32_t set_passphrase(char *passphrase);
int32_t dev_susp_start(char *susp_time);
int32_t get_version(void);
static int32_t ad_hoc_connect_handler(int32_t index, int32_t argc, char *argv[]);
static int32_t wps_handler(int32_t index, int32_t argc, char *argv[]);
#if ENABLE_P2P_MODE
static A_STATUS wmic_ether_aton(const char *orig, uint8_t *eth);
#endif
static int32_t scan_control(int32_t argc, char *argv[]);
static int32_t set_scan_para(int32_t argc, char *argv[]);
static int32_t start_scan(int32_t argc, char *argv[]);
static int32_t ath_assert_dump(int32_t argc, char *argv[]);
static unsigned char ascii_to_hex(char val);
A_STATUS enable_probe_req_event(int argc, char *argv[]);

static int32_t mac_store(char *argv[]);
int32_t wps_query(uint8_t block);
static int32_t test_promiscuous(int32_t argc, char *argv[]);
static int32_t test_raw(int32_t argc, char *argv[]);
static int32_t allow_aggr(int32_t argc, char *argv[]);
static int32_t set_wpa(int32_t index, int32_t argc, char *argv[]);
static int32_t set_wep(int32_t index, int32_t argc, char *argv[]);
int32_t get_last_error(void);
int user_defined_power_mode = MAX_PERF_POWER;
static int32_t get_reg_domain(void);

void SystemReset(void);
int ishexdigit(char digit);
extern void ath_tcp_rx_multi_TCP(int port);

#if ENABLE_STACK_OFFLOAD
int32_t ipconfig_auto_ipv4(int32_t argc, char *argv[], QCOM_IPCONFIG_MODE mode);
int32_t custom_ipdns(void *handle, int32_t command, char *domain_name, IP46ADDR *dnsaddr);
int32_t ipconfig_dhcp_pool(int32_t argc, char *argv[]);
int32_t ipconfig_dhcps_cb_enable(int32_t argc, char *argv[]);
int32_t ipconfig_dhcpc_cb_enable(int32_t argc, char *argv[]);
int32_t ip_set_bridgemode(int32_t argc, char *argv[]);
int32_t ip6_set_router_prefix(int32_t argc, char *argv[]);
static int32_t ip_set_tcp_exponential_backoff_retry(int32_t argc, char *argv[]);
static int32_t mac_store(char *argv[]);
static int32_t ip6_set_status(int32_t argc, char *argv[]);
static int32_t ipconfig_dhcp_release(int32_t argc, char *argv[]);
static int32_t ipconfig_tcp_rxbuf(int32_t argc, char *argv[]);
#if ENABLE_HTTP_SERVER
static int32_t ip_http_server(int32_t argc, char *argv[]);
static int32_t ip_http_server_post(int32_t argc, char *argv[]);
static int32_t ip_http_server_get(int32_t argc, char *argv[]);
#endif
#if ENABLE_DNS_CLIENT
static int32_t ip_resolve_hostname(int32_t argc, char *argv[]);
int32_t ip_get_addr_for_host_name(char *hostname, uint32_t *v4addr, IP6_ADDR_T *ip6addr, uint32_t domain);
static int32_t ip_gethostbyname(int32_t argc, char *argv[]);
static int32_t ip_gethostbyname2(int32_t argc, char *argv[]);
static int32_t ip_dns_client(int32_t argc, char *argv[]);
static int32_t ip_dns_server_addr(int32_t argc, char *argv[]);
static int32_t ip_dns_delete_server_addr(int32_t argc, char *argv[]);
static int32_t ip_dns_timeout(int32_t argc, char *argv[]);
#endif
#if ENABLE_SSL
static int32_t ssl_start(int32_t argc, char *argv[]);
static int32_t ssl_stop(int32_t argc, char *argv[]);
static int32_t ssl_config(int32_t argc, char *argv[]);
static int32_t ssl_add_cert(int32_t argc, char *argv[]);
static int32_t ssl_store_cert(int32_t argc, char *argv[]);
static int32_t ssl_delete_cert(int32_t argc, char *argv[]);
static int32_t ssl_list_cert(int32_t argc, char *argv[]);
#endif
static int32_t gpio_handle(int32_t argc, char *argv[]);
void configure_auto_ipv4_addr(void);
static int32_t iphostname(int32_t argc, char *argv[]);
#if ENABLE_DNS_SERVER
static int32_t ip_dns_local_domain(int32_t argc, char *argv[]);
static int32_t ipdns(int32_t argc, char *argv[]);
static int32_t ip_dns_server_enable(int32_t argc, char *argv[]);
#endif
#if ENABLE_SNTP_CLIENT
static int32_t ip_sntp_srvr(int32_t argc, char *argv[]);
static int32_t ip_sntp_get_time(int32_t argc, char *argv[]);
static int32_t ip_sntp_get_time_of_day(int32_t argc, char *argv[]);
static int32_t ip_sntp_modify_timezone_dse(int32_t argc, char *argv[]);
static int32_t ip_query_sntp_server_address(uint32_t argc, char *argv[]);
static int32_t ip_sntp_client_start_stop(uint32_t argc, char *argv[]);
#endif
#if ENABLE_HTTP_CLIENT
static int32_t httpc_command_parser(int32_t argc, char_ptr argv[]);
static void httpc_connect(HTTPC_PARAMS *p_httpc);
static void httpc_method(HTTPC_PARAMS *p_httpc);
#endif

int32_t ipv4_route(int32_t argc, char_ptr argv[]);
int32_t ipv6_route(int32_t argc, char_ptr argv[]);

static int32_t ota_upgrade(int32_t argc, char *argv[]);
static int32_t ota_read(int32_t argc, char *argv[]);
static int32_t ota_done(int32_t argc, char *argv[]);
static int32_t ota_cust(int32_t argc, char *argv[]);
static int32_t ota_format(int32_t argc, char *argv[]);
static int32_t ota_ftp_upgrade(int32_t argc, char *argv[]);

#endif /* ENABLE_STACK_OFFLOAD */

#if ENABLE_KF_PERFORMANCE
A_STATUS ath_setpmu(int argc, char *argv[]);
static int32_t pfm_handler(int32_t argc, char *argv[]);
static int32_t diag_handler(int32_t argc, char *argv[]);

#endif
static int32_t get_temperature(int32_t argc, char *argv[]);
static int32_t get_country_code(void);
static int32_t ath_roam_ctrl(int32_t argc, char *argv[]);
static int32_t wow_set_mode(int32_t argc, char *argv[]);
static int32_t wow_host_sleep(int32_t argc, char *argv[]);
A_STATUS ath_dset(int argc, char *argv[]);
A_STATUS ath_dset_api(int argc, char *argv[]);
A_STATUS ath_scratch_dset(int argc, char *argv[]);

#define PROBE_REQ_SRC_ADDR_OFFSET (10)

A_STATUS set_power_mode(bool pwr_mode, uint8_t pwr_module)
{
    uint32_t enet_device = get_active_device();

    if (pwr_mode == REC_POWER)
    {
        /*Allow power save mode setting from a module only if user has configured
         REC_POWER*/
        if (user_defined_power_mode != REC_POWER)
            return A_ERROR;
    }
    return qcom_power_set_mode(enet_device, pwr_mode, pwr_module);
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : mystrtobyte
 * Returned Value : NA
 * Comments       : converts a string to a bianry bytes.
 *
 *END------------------------------------------------------------------*/
static int mystrtobyte(const char *arg, unsigned char *ret_byte)
{
    int i = 0;
    unsigned char base_char[2];

    if (arg == NULL)
    {
        return -1;
    }

    for (i = 0; i < 2; i++)
    {
        if (arg[i] >= '0' && arg[i] <= '9')
        {
            base_char[i] = arg[i] - '0';
        }
        else if (arg[i] >= 'a' && arg[i] <= 'f')
        {
            base_char[i] = arg[i] - 'a' + 10;
        }
        else if (arg[i] >= 'A' && arg[i] <= 'F')
        {
            base_char[i] = arg[i] - 'A' + 10;
        }
        else
        {
            return -1; // error
        }
    }

    *ret_byte = (base_char[0] * 16 + base_char[1]);
    return 0; // success
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : mystrtomac
 * Returned Value : NA
 * Comments       : converts a string to a 6 byte MAC address.
 *
 *END------------------------------------------------------------------*/
static int mystrtomac(const char *arg, unsigned char *dst)
{
    int i           = 0;
    int j           = 0;
    int left_nibble = 1;
    char base_char;
    unsigned char base_val;

    if (dst == NULL || arg == NULL)
    {
        return -1;
    }

    A_MEMZERO(dst, 6);

    do
    {
        if (arg[i] >= '0' && arg[i] <= '9')
        {
            base_char = '0';
            base_val  = 0x00;
        }
        else if (arg[i] == ':')
        {
            base_char = ':';
        }
        else if (arg[i] >= 'a' && arg[i] <= 'f')
        {
            base_char = 'a';
            base_val  = 0x0a;
        }
        else if (arg[i] >= 'A' && arg[i] <= 'F')
        {
            base_char = 'A';
            base_val  = 0x0a;
        }
        else
        {
            return -1; // error
        }

        if (base_char != ':')
        {
            dst[j] |= (arg[i] - base_char + base_val) << (4 * left_nibble);
            left_nibble = (left_nibble) ? 0 : 1;

            if (left_nibble)
            {
                j++;

                if (j > 5)
                {
                    break;
                }
            }
        }

        i++;
    } while (1);

    return 0; // success
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : wmiconfig_handler
 * Returned Value : ERROR code on error else success
 * Comments       : 1st stop for all wmiconfig commands
 *
 *END------------------------------------------------------------------*/

int32_t wmiconfig_handler(int32_t argc, char *argv[])
{ /* Body */
    bool shorthelp      = FALSE;
    uint8_t printhelp   = 0;
    int32_t return_code = A_OK;
    uint32_t enet_device, index = 1;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;

#if ENABLE_P2P_MODE
    if (p2p_session_in_progress)
    {
        PRINTF("p2p event in progress" NL);
        return A_ERROR;
    }
#endif
    enet_device = get_active_device();

    if (argc > index)
    {
        if (ATH_STRCMP(argv[index], "--connect") == 0)
        {
            if (argc == index + 1)
            {
                PRINTF("Incomplete parameters for connect command" NL);
                return A_ERROR;
            }
            return_code = connect_handler(index + 1, argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--adhoc") == 0)
        {
            if (argc == index + 1)
            {
                PRINTF("Incomplete parameters for ad-hoc command" NL);
                return A_ERROR;
            }
            qcom_op_set_mode(enet_device, QCOM_WLAN_DEV_MODE_ADHOC);
            return_code = ad_hoc_connect_handler(index + 1, argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--device") == 0)
        {
            enet_device = atoi(argv[index + 1]);
            if ((enet_device != 0 && enet_device != 1) || (argc == index + 1))
            {
                PRINTF("usage : wmiconfig --device <id> , id is 0/1" NL);
                return A_ERROR;
            }
            return_code = deviceid_handler(index + 1, argc, argv);
            if (return_code == A_OK)
            {
                active_device = enet_device;
            }
            else
            {
                PRINTF("set device got error:" NL);
                return A_ERROR;
            }
        }
#if ENABLE_AP_MODE
        else if (ATH_STRCMP(argv[index], "--mode") == 0)
        {
            if (argc == index + 1)
            {
                PRINTF("Usage:: --mode <ap[<hidden>][<wps>]|station>" NL);
                return A_ERROR;
            }

            QCOM_SSID tmp_ssid = QCOM_SSID_FROM_STR("");
            qcom_set_ssid(enet_device, &tmp_ssid);

            if (ATH_STRCMP(argv[index + 1], "ap") == 0)
            {
                if (ATH_STRCMP(argv[index + 2], "hidden") == 0)
                {
                    hidden_flag = 1;
                }
                if (ATH_STRCMP(argv[index + 2], "wps") == 0)
                {
                    wps_flag = 1;
                }

                qcom_op_set_mode(enet_device, QCOM_WLAN_DEV_MODE_AP);
                /* Set to MAX PERF Mode */
                set_power_mode(MAX_PERF_POWER, SOFTAP);

                if (hidden_flag == 1)
                {
                    hidden_flag = 1;
                    uint8_t hidden_enable;
                    hidden_enable = 1;
                    if (qcom_ap_hidden_mode_enable(enet_device, hidden_enable) != A_OK)
                    {
                        PRINTF("not able to set hidden mode for AP" NL);
                        return A_ERROR;
                    }
                }
                if (wps_flag == 1)
                {
                    if (set_ap_params(AP_SUB_CMD_WPS_FLAG, (uint8_t *)&wps_flag) != SHELL_EXIT_SUCCESS)
                    {
                        PRINTF("not able to set wps mode for AP" NL);
                        return SHELL_EXIT_ERROR;
                    }
                }
            }
            else if (ATH_STRCMP(argv[index + 1], "station") == 0)
            {
                if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
                {
                    return A_ERROR;
                }

                if (wifimode == QCOM_WLAN_DEV_MODE_AP) /*check previous mode*/
                {
                    qcom_disconnect(enet_device);
                    /* AP Mode is always set to maxper; if we are switching mode and prev mode is REC_POWER then
                       retain the power mode for STA */

                    set_power_mode(REC_POWER, SOFTAP);
                }
                qcom_op_set_mode(enet_device, QCOM_WLAN_DEV_MODE_STATION);

                if (ATH_STRCMP(argv[index + 2], "hidden") == 0)
                {
                    PRINTF("Usage:: --mode <ap[<hidden>]|station>" NL);
                    return A_ERROR;
                }
                if (ATH_STRCMP(argv[index + 2], "wps") == 0)
                {
                    PRINTF("Usage:: --mode <ap[<wps>]|station>" NL);
                    return A_ERROR;
                }
            }
            else
            {
                PRINTF("Usage:: --mode <ap[<hidden>][<wps>]|station>" NL);
                return A_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[index], "--ap") == 0)
        {
            if (argc == index + 1)
            {
                PRINTF("usage:--ap [country <country code>]" NL "      --ap [bconint<intvl>]" NL
                       "      --ap [dtim<val>]" NL "      --ap [inact <minutes>]>" NL
                       "      --ap [psbuf <1/0-enable/disable> <buf_count for enable>]>" NL);
                return A_ERROR;
            }
            return_code = ap_handler(index + 1, argc, argv);
        }
#endif
        else if (ATH_STRCMP(argv[index], "--wps") == 0)
        {
            return_code = wps_handler(index + 1, argc, argv);
        }
#if ENABLE_P2P_MODE
        else if (ATH_STRCMP(argv[index], "--p2p") == 0)
        {
            return_code = p2p_handler(index + 1, argc, argv);
        }
#endif
        else if (ATH_STRCMP(argv[index], "--disc") == 0)
        {
            return_code = qcom_disconnect(enet_device);
        }
        else if (ATH_STRCMP(argv[index], "--settxpower") == 0)
        {
            return_code = qcom_set_tx_power(enet_device, atoi(argv[index + 1]));
        }
        else if (ATH_STRCMP(argv[index], "--pwrmode") == 0)
        {
            if (ATH_STRCMP(argv[index + 1], "0") == 0)
            {
                user_defined_power_mode = MAX_PERF_POWER;
                set_power_mode(MAX_PERF_POWER, USER);
            }
            else
            {
                if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
                {
                    return A_ERROR;
                }
                if (wifimode == QCOM_WLAN_DEV_MODE_AP && !(p2pMode))
                {
                    PRINTF("Setting REC Power is not allowed MODE_AP" NL);
                    return A_ERROR;
                }
                user_defined_power_mode = REC_POWER;
                set_power_mode(REC_POWER, USER);
            }
        }
        else if (ATH_STRCMP(argv[index], "--pmparams") == 0)
        {
            return_code = set_pmparams(index + 1, argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--channel") == 0)
        {
            return_code = qcom_set_channel(enet_device, atoi(argv[index + 1]));
        }
        else if (ATH_STRCMP(argv[index], "--countrycode") == 0)
        {
            if (argc == index + 1)
            {
                PRINTF("Usage:: --countrycode <countrycode string>" NL);
                return A_ERROR;
            }
            return_code = qcom_set_country_code(enet_device, (uint8_t *)argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--get_ctry") == 0)
        {
            return_code = get_country_code();
        }
        else if (ATH_STRCMP(argv[index], "--listen") == 0)
        {
            uint32_t listen_time;
            listen_time = atoi(argv[index + 1]);
            return_code = qcom_sta_set_listen_time(enet_device, listen_time);
        }
        else if (ATH_STRCMP(argv[index], "--wmode") == 0)
        {
            if (0 == ATH_STRCMP(argv[index + 1], "a"))
            {
                return_code = qcom_set_phy_mode(enet_device, (uint8_t)QCOM_11A_MODE);
            }
            else if (0 == ATH_STRCMP(argv[index + 1], "b"))
            {
                return_code = qcom_set_phy_mode(enet_device, (uint8_t)QCOM_11B_MODE);
            }
            else if (0 == ATH_STRCMP(argv[index + 1], "g"))
            {
                return_code = qcom_set_phy_mode(enet_device, (uint8_t)QCOM_11G_MODE);
            }
            else if (0 == ATH_STRCMP(argv[index + 1], "n"))
            {
                return_code = qcom_set_phy_mode(enet_device, (uint8_t)QCOM_11N_MODE);
            }
            else if (0 == ATH_STRCMP(argv[index + 1], "ht40"))
            {
                return_code = qcom_set_phy_mode(enet_device, (uint8_t)QCOM_HT40_MODE);
            }
            else
            {
                PRINTF("Unknown wmode, only support b/g/n/ht40" NL);
                return A_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[index], "--rssi") == 0)
        {
            uint8_t rssi = 0;
            return_code  = qcom_sta_get_rssi(enet_device, &rssi);
            PRINTF(NL "indicator = %d dB" NL, rssi);
        }
        else if (ATH_STRCMP(argv[index], "--gettxstat") == 0)
        {
            return_code = get_tx_status();
        }
        else if (ATH_STRCMP(argv[index], "--getrate") == 0)
        {
            return_code = get_rate();
        }
        else if (ATH_STRCMP(argv[index], "--setrate") == 0)
        {
            return_code = set_rate(index + 1, argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--txpwrscale") == 0)
        {
            return_code = set_tx_power_scale(index + 1, argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--wepkey") == 0)
        {
            if ((argc - index) > 2)
                return_code = set_wep_key(argv[index + 1], argv[index + 2]);
            else
            {
                PRINTF("Missing parameters" NL);
                return A_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[index], "--p") == 0)
        {
            /*We expect one parameter with this command*/
            if ((argc - index) > 1)
                return_code = set_passphrase(argv[index + 1]);
            else
            {
                PRINTF("Missing parameters" NL);
                return A_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[index], "--wpa") == 0)
        {
            return_code = set_wpa(index + 1, argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--wep") == 0)
        {
            return_code = set_wep(index + 1, argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--suspend") == 0)
        {
            return_code = qcom_suspend_enable(true);
        }
        else if (ATH_STRCMP(argv[index], "--suspstart") == 0)
        {
            return_code = dev_susp_start(argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--version") == 0)
        {
            return_code = get_version();
        }
        else if (ATH_STRCMP(argv[index], "--help") == 0)
        {
            //   return_code = A_ERROR;
            printhelp = 1;
        }

        else if (ATH_STRCMP(argv[index], "--testflash") == 0)
        {
            return_code = TEST_FLASH;
        }
        else if (ATH_STRCMP(argv[index], "--scanctrl") == 0)
        {
            return_code = scan_control(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--setscanpara") == 0)
        {
            return_code = set_scan_para(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--setscan") == 0)
        {
            return_code = start_scan(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--reg") == 0)
        {
            return_code = ATH_REG_QUERY(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--fwmem") == 0)
        {
            return_code = ATH_MEM_QUERY(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--assdump") == 0)
        {
            return_code = ath_assert_dump(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--driver") == 0)
        {
            return_code = ath_driver_state(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--raw") == 0)
        {
            return_code = test_raw(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--promisc") == 0)
        {
            return_code = test_promiscuous(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--allow_aggr") == 0)
        {
            return_code = allow_aggr(argc - (index + 1), &argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--reset") == 0)
        {
            SystemReset();
        }
        else if (ATH_STRCMP(argv[index], "--regdomain") == 0)
        {
            get_reg_domain();
        }
        else if (ATH_STRCMP(argv[index], "--getlasterr") == 0)
        {
            get_last_error();
        }
        else if (ATH_STRCMP(argv[index], "--gtx") == 0)
        {
            return_code = set_gtx(argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--lpl") == 0)
        {
            return_code = set_lpl(argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--macstore") == 0)
        {
            return_code = mac_store(&argv[index + 1]);
        }
        else if (ATH_STRCMP(argv[index], "--gpio") == 0)
        {
            return_code = gpio_handle(argc, argv);
        }

#if ENABLE_STACK_OFFLOAD
        else if (ATH_STRCMP(argv[index], "--ipconfig") == 0)
        {
            return_code = ipconfig_query(NULL, argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ipstatic") == 0)
        {
            return_code = ipconfig_static(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ipdhcp") == 0)
        {
            return_code = ipconfig_dhcp(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ipbridgemode") == 0)
        {
            return_code = ip_set_bridgemode(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ipauto") == 0)
        {
            return_code = ipconfig_auto_ipv4(argc, argv, QCOM_IPCONFIG_AUTO);
        }
        else if (ATH_STRCMP(argv[index], "--blockdhcp") == 0)
        {
            return_code = block_on_dhcp(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--udpecho") == 0)
        {
            // ath_udp_echo(argc,argv);
        }
        else if (ATH_STRCMP(argv[index], "--multi") == 0)
        {
            ath_tcp_rx_multi_TCP(atoi(argv[2]));
        }
        else if (ATH_STRCMP(argv[index], "--settcptimeout") == 0)
        {
            return_code = set_tcp_connection_timeout(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ipdhcppool") == 0)
        {
            return_code = ipconfig_dhcp_pool(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--dhcpscb") == 0)
        {
            return_code = ipconfig_dhcps_cb_enable(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--dhcpccb") == 0)
        {
            return_code = ipconfig_dhcpc_cb_enable(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip6rtprfx") == 0)
        {
            return_code = ip6_set_router_prefix(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--tcp_backoff_retry") == 0)
        {
            return_code = ip_set_tcp_exponential_backoff_retry(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ipv6") == 0)
        {
            return_code = ip6_set_status(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ipdhcp_release") == 0)
        {
            return_code = ipconfig_dhcp_release(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--tcp_rxbuf") == 0)
        {
            return_code = ipconfig_tcp_rxbuf(argc, argv);
        }
#if ENABLE_DNS_CLIENT
        else if (ATH_STRCMP(argv[index], "--ip_resolve_hostname") == 0)
        {
            return_code = ip_resolve_hostname(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_gethostbyname") == 0)
        {
            return_code = ip_gethostbyname(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_gethostbyname2") == 0)
        {
            return_code = ip_gethostbyname2(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_dns_client") == 0)
        {
            return_code = ip_dns_client(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_dns_server_addr") == 0)
        {
            return_code = ip_dns_server_addr(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_dns_delete_server_addr") == 0)
        {
            return_code = ip_dns_delete_server_addr(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_dns_timeout") == 0)
        {
            return_code = ip_dns_timeout(argc, argv);
        }
#endif /* ENABLE_DNS_CLIENT */
#if ENABLE_HTTP_SERVER
        else if (ATH_STRCMP(argv[index], "--ip_http_server") == 0)
        {
            return_code = ip_http_server(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_https_server") == 0)
        {
            return_code = ip_http_server(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_http_post") == 0)
        {
            return_code = ip_http_server_post(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_http_get") == 0)
        {
            return_code = ip_http_server_get(argc, argv);
        }
#endif /* ENABLE_HTTP_SERVER */

#if ENABLE_HTTP_CLIENT
        else if (ATH_STRCMP(argv[index], "--ip_http_client") == 0)
        {
            return_code = httpc_command_parser(argc, argv);
        }
#endif /* ENABLE_HTTP_CLIENT */
        else if (ATH_STRCMP(argv[index], "--iphostname") == 0)
        {
            return_code = iphostname(argc, argv);
        }
#if ENABLE_DNS_SERVER
        else if (ATH_STRCMP(argv[index], "--ip_dns_local_domain") == 0)
        {
            return_code = ip_dns_local_domain(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_dns") == 0)
        {
            return_code = ipdns(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_dns_server_enable") == 0)
        {
            return_code = ip_dns_server_enable(argc, argv);
        }
#endif /* ENABLE_DNS_SERVER */
#if ENABLE_SNTP_CLIENT
        else if (ATH_STRCMP(argv[index], "--ip_sntp_srvr") == 0)
        {
            return_code = ip_sntp_srvr(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_sntp_get_time") == 0)
        {
            return_code = ip_sntp_get_time(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_sntp_get_time_of_day") == 0)
        {
            return_code = ip_sntp_get_time_of_day(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_sntp_zone") == 0)
        {
            return_code = ip_sntp_modify_timezone_dse(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_show_sntpconfig") == 0)
        {
            return_code = ip_query_sntp_server_address(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ip_sntp_client") == 0)
        {
            return_code = ip_sntp_client_start_stop(argc, argv);
        }
#endif /* ENABLE_SNTP_CLIENT */
        else if (ATH_STRCMP(argv[index], "--ota_upgrade") == 0)
        {
            return_code = ota_upgrade(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ota_read") == 0)
        {
            return_code = ota_read(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ota_done") == 0)
        {
            return_code = ota_done(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ota_cust") == 0)
        {
            return_code = ota_cust(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ota_format") == 0)
        {
            return_code = ota_format(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ota_ftp") == 0)
        {
            return_code = ota_ftp_upgrade(argc, argv);
        }

#if ENABLE_SSL
        else if (ATH_STRCMP(argv[index], "--ssl_start") == 0)
        {
            return_code = ssl_start(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ssl_stop") == 0)
        {
            return_code = ssl_stop(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ssl_config") == 0)
        {
            return_code = ssl_config(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ssl_add_cert") == 0)
        {
            return_code = ssl_add_cert(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ssl_store_cert") == 0)
        {
            return_code = ssl_store_cert(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ssl_delete_cert") == 0)
        {
            return_code = ssl_delete_cert(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ssl_list_cert") == 0)
        {
            return_code = ssl_list_cert(argc, argv);
        }
#endif
#if ENABLE_ROUTING_CMDS
        else if (ATH_STRCMP(argv[index], "--ipv4_route") == 0)
        {
            return_code = ipv4_route(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--ipv6_route") == 0)
        {
            return_code = ipv6_route(argc, argv);
        }
#endif /* ENABLE_ROUTING_CMDS */
#endif /* ENABLE_STACK_OFFLOAD */

#if ENABLE_KF_PERFORMANCE
        else if (ATH_STRCMP(argv[index], "--pfm_get") == 0)
        {
            return_code = pfm_handler(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--diag") == 0)
        {
            return_code = diag_handler(argc, argv);
        }
        else if (strcmp(argv[index], "--setpmu") == 0)
        {
            return_code = ath_setpmu(argc, argv);
        }
#endif
        else if (ATH_STRCMP(argv[index], "--dset") == 0)
        {
            return_code = ath_dset(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--dset_api") == 0)
        {
            return_code = ath_dset_api(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--get_temp") == 0)
        {
            return_code = get_temperature(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--roaming") == 0)
        {
            return_code = ath_roam_ctrl(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--wow_set_mode") == 0)
        {
            return_code = wow_set_mode(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--wow_host_sleep") == 0)
        {
            return_code = wow_host_sleep(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--scratch") == 0)
        {
            return_code = ath_scratch_dset(argc, argv);
        }
        else if (ATH_STRCMP(argv[index], "--enprobe") == 0)
        {
            return_code = enable_probe_req_event(argc, argv);
        }
        else
        {
            PRINTF("ERROR: Bad wmiconfig command: %s" NL, argv[index]);
            return_code = A_ERROR;
        }
    }
    else if (argc == 1)
    {
        QCOM_SSID tmp_ssid   = {0};
        QCOM_BSSID tmp_bssid = {0};
        char *phy_mode       = NULL;
        int32_t error;
        uint16_t channel_val         = 0;
        uint32_t mode                = 0;
        QCOM_WLAN_DEV_MODE wifi_mode = QCOM_WLAN_DEV_MODE_INVALID;
        error                        = qcom_get_ssid(enet_device, &tmp_ssid);

        if (error != A_OK)
            return_code = A_ERROR;

        PRINTF("ssid         = %s" NL, tmp_ssid.ssid);

        // ATH_GET_SECTYPE (enet_device,(char*)data);
        // PRINTF("security type=   %s" NL,data);

        qcom_get_phy_mode(enet_device, &phy_mode);
        if (NULL != phy_mode)
        {
            PRINTF("Phy mode     = %s" NL, phy_mode);
        }

        if (mode == REC_POWER)
        {
            PRINTF("Power mode   = Power Save" NL);
        }
        else
        {
            PRINTF("Power mode   = Max Perf" NL);
        }

        qcom_get_bssid(enet_device, (uint8_t *)&tmp_bssid);
        PRINTF("Mac Addr     = %02x:%02x:%02x:%02x:%02x:%02x" NL, tmp_bssid.bssid[0] & 0xFF, tmp_bssid.bssid[1] & 0xFF,
               tmp_bssid.bssid[2] & 0xFF, tmp_bssid.bssid[3] & 0xFF, tmp_bssid.bssid[4] & 0xFF,
               tmp_bssid.bssid[5] & 0xFF);
        if (A_ERROR == qcom_op_get_mode(enet_device, &wifi_mode))
        {
            return A_ERROR;
        }
#if ENABLE_AP_MODE
        if (mode == QCOM_WLAN_DEV_MODE_AP)
        {
            PRINTF("mode         = softap" NL);
            if (pmk_flag)
            {
                PRINTF("passphrase   = %s" NL, wpa_passphrase);
            }
        }
        else
#endif
            if (mode == QCOM_WLAN_DEV_MODE_STATION)
        {
            PRINTF("mode         = station" NL);
        }
        else
        {
            PRINTF("mode         = adhoc" NL);
        }
        if (wifi_connected_flag == 1)
        {
            qcom_get_channel(enet_device, &channel_val);
            if (channel_val < 3000)
            {
                channel_val -= ATH_IOCTL_FREQ_1;
                if ((channel_val / 5) == 14)
                {
                    channel_val = 14;
                }
                else
                {
                    channel_val = (channel_val / 5) + 1;
                }
            }
            else
            {
                channel_val -= ATH_IOCTL_FREQ_36;
                channel_val = 36 + (channel_val / 5); // since in 11a channel 36 is the starting number
            }
            PRINTF("channel      = %d" NL, channel_val);
        }
    }

    if ((return_code != A_OK))
    {
        PRINTF("To see a list of commands: wmiconfig --help" NL);
    }

    if (printhelp)
    {
        printhelp = 0;
        if (shorthelp)
        {
            PRINTF("%s [<device>] [<command>]" NL, argv[0]);
        }
        else
        {
#if ENABLE_STACK_OFFLOAD
            PRINTF("IOT Stack Offload Feature Enabled" NL);
#else
            PRINTF("IOT Stack Offload Feature Disabled" NL);
#endif
            PRINTF("Usage: %s [<command>]" NL, argv[0]);
            PRINTF("  Commands:" NL);
            PRINTF("    --version        = Displays versions" NL);
            PRINTF("    --reset          = reset host processor" NL);
            PRINTF("    --suspend        = Enable device suspend mode(before connect command)" NL);
            PRINTF("    --regdomain      = Get Regulatory Domain" NL);
            PRINTF("    --getlasterr     = Prints last driver error" NL);
            PRINTF("    --connect <ssid>" NL);
            PRINTF(NL);
            PRINTF("   WPA Configuration" NL);
            PRINTF("      --p <passphrase>" NL);
            PRINTF("      --wpa <ver> <ucipher> <mcipher>" NL);
            PRINTF("      --connect <ssid>" NL);
            PRINTF(NL);
            PRINTF("   WEP Configuration" NL);
            PRINTF("      --wepkey <key_index> <key>" NL);
            PRINTF(" 	   --wep <def_keyix>" NL);
            PRINTF("      --connect <ssid>" NL);
            PRINTF("              where  <ssid>   :SSID of network" NL);
            PRINTF("                     <ver>    :1-WPA, 2-RSN" NL);
            PRINTF("                     <ucipher>:TKIP or CCMP" NL);
            PRINTF("                     <mcipher>:TKIP or CCMP" NL);
            PRINTF("                     <p>    :Passphrase for WPA" NL);
            // PRINTF ("                     <mode>   :<open> or <shared>" NL);
            PRINTF("                     <key_index>: Entered WEP key index [1-4]" NL);
            PRINTF("                     <def_keyix>: Default WEP key index [1-4]" NL);
            PRINTF("                     <key>    :WEP key" NL);
            PRINTF("                     *        :optional parameters" NL);
            PRINTF("          Note: key can only be composed of Hex characters of length 10 or 26" NL);
            PRINTF(NL);
            PRINTF("    --adhoc <ssid> --channel <1-14>  = Connect in ad-hoc mode" NL);
            PRINTF("    --adhoc <ssid> --wep  <def_keyix> --channel <1-14> = Connect in WEP ad-hoc mode" NL);
            PRINTF("                                    where channel is optional and ranges from 1-14" NL);
            PRINTF(NL);
            PRINTF("    --wps <connect> <mode> <*pin> <*ssid> <*mac> <*channel>" NL);
            PRINTF("              where  <connect> : 1 - Attempts to connect after wps" NL);
            PRINTF("                               : 0 - No Attempts to connect after wps" NL);
            PRINTF("                     <mode>    : <push> or <pin>" NL);
            PRINTF("                     *         :optional parameters" NL);
            PRINTF(NL);
            PRINTF("    --p2p <mode> <*>" NL);
            PRINTF("           where <mode>  :  <on/off> <setconfig> <connect> <find> <prov> <listen>" NL);
            PRINTF(
                "                            <cancel> <join> <auth> <wpspin> <autogo> <invite_auth> <passphrase>" NL);
            PRINTF("                  *      :  optional parameters that need to be entered based on mode" NL);
            PRINTF(NL);
            PRINTF("    --disc            = Disconect from current AP" NL);
            PRINTF("    --setrate <rate> , where rate can be 1,2,5,6,9,11,12,18,24,36,48,54 mbps or MCS" NL);
            PRINTF("    --txpwrscale <val> , where val is in dbm" NL);
            PRINTF("      eg:  wmiconfig --setrate 54 will set rate to 54 mbps" NL);
            PRINTF("           wmiconfig --setrate mcs 0 will set rate to mcs_0" NL);
            PRINTF("    --settxpower <>   = set transmit power" NL);
            PRINTF("    --pwrmode    <>   = set power mode 1=Power save, 0= Max Perf" NL);
            PRINTF("    --setchannel <1-14>   = set channel hint" NL);
            PRINTF("    --wmode      <>   = set mode <b,g,n>" NL);
            PRINTF("    --listen     <>   = set listen interval" NL);
            PRINTF("    --rssi       <>   = prints Link Quality (SNR)" NL);
            PRINTF("    --suspstart <time>    = Suspend device for specified time in milliseconds" NL);
            PRINTF("    --gettxstat           = get current status of TX from driver" NL);
            PRINTF("    --pmparams        = Set Power management parameters" NL);
            PRINTF("           --idle <time in ms>                        Idle period" NL);
            PRINTF("           --np   < >                                 PS Poll Number" NL);
            PRINTF("           --dp <1=Ignore 2=Normal 3=Stick 4=Auto>    DTIM Policy" NL);
            PRINTF("           --txwp <1=wake 2=do not wake>              Tx Wakeup Policy" NL);
            PRINTF("           --ntxw < >                                 Number of TX to Wakeup" NL);
            PRINTF("           --psfp <1=send fail event 2=Ignore event>  PS Fail Event Policy" NL);
            PRINTF(
                "    --scanctrl [0|1] [0|1]       = Control firmware scan behavior. Specifically, disable/enable "
                "foreground OR background scanning." NL);
            PRINTF(
                "    --setscanpara <max_act_ch_dwell_time_ms> <pas_act_chan_dwell_time_ms> [<fg_start_period(in secs)> "
                "<fg_end_period (in secs)> <bg_period (in secs)> <short_scan_ratio> <scan_ctrl_flags>  "
                "<min_active_chan_dwell_time_ms> <max_act_scan_per_ssid> <max_dfs_ch_act_time_in_ms>]" NL);
            PRINTF(
                "    --setscan <forceFgScan> <homeDwellTimeInMs> <forceScanIntervalInMs> <scanType> <numChannels> "
                "[<channel> <channel>... upto numChannels]" NL);
            PRINTF("    --reg [op] [addr] [value] [mask] = read or write specified AR600X register." NL);
            PRINTF("    --reg [op] [addr] [value] [mask] = read or write specified AR600X register." NL);
            PRINTF("    --assdump = collects assert data from AR600X and prints it to stdout." NL);
            PRINTF("    --driver [mode]" NL);
            PRINTF("					mode = down - Sets the driver State 0 unloads the driver" NL);
            PRINTF("					mode = flushdown - Waits for TX clear before unloading the driver" NL);
            PRINTF("					       up   - Reloads the driver" NL);
            PRINTF("					       raw  - Reloads the driver in raw mode" NL);
            PRINTF("					       quad - Reloads the driver in quad spi flash mode" NL);
            PRINTF("					       rawq - Reloads the driver in raw + quad mode" NL);
            PRINTF("					       normal - Reloads the driver in normal mode" NL);
            PRINTF("					       pmu - Reloads the driver in pmu enabled mode (Only MQX)" NL);
            PRINTF(
                "    --allow_aggr <tx_tid_mask> <rx_tid_mask> Enables aggregation based on the provided bit mask "
                "where" NL);
            PRINTF("		    each bit represents a TID valid TID's are 0-7" NL);
            PRINTF("    --macstore <addr> = program new mac address into wifi device." NL);
            PRINTF(
                "iwconfig scan <ssid*>  =scan channels, ssid is optional, if provided will scan only specified "
                "SSID" NL);
#if ENABLE_AP_MODE
            PRINTF("    --mode [ap <hidden><wps>|station]" NL);
            PRINTF("    --ap [country <country code>]" NL "    --ap [bconint<intvl>]" NL "    --ap [dtim<val>]" NL
                   "    --ap [inact <minutes>]>" NL "    --ap [psbuf <1/0-enable/disable> <buf_count for enable>]>" NL);
#endif
#if ENABLE_STACK_OFFLOAD
            PRINTF("    --ipbridgemode                                 = Enable the bridge mode" NL);
            PRINTF("    --ipconfig                                     = show IP parameters" NL);
            PRINTF("    --ipstatic <ip addr> <subnet mask> <Gateway>   = set static IP parameters" NL);
            PRINTF("    --ipdhcp                                       = Run DHCP client" NL);
            PRINTF("    --ipauto  <link local IPv4>(optional)          = Run Auto IPv4" NL);
            PRINTF("    --udpecho <port>                               = run UDP echo server" NL);
            PRINTF("    --ipdhcppool <start ipaddr> <End ipaddr> <Lease time> = set ip dhcp pool" NL);
            PRINTF(
                "    --ip6rtprfx <prefix> <prefixlen> <prefix_lifetime> <valid_lifetime> = set ipv6 router prefix" NL);
            PRINTF("    --tcp_backoff_retry <no of retries> = set ip tcp max exponetial backoff retries" NL);
            PRINTF("    --ipv6 <status> = set ip6 status as either enable or disable" NL);
            PRINTF("    --ipdhcp_release = Release the DHCP Address" NL);
            PRINTF("    --tcp_rxbuf = Configure the tcp rx buf space" NL);
#if ENABLE_DNS_CLIENT
            PRINTF(
                "    --ip_resolve_hostname [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2, "
                "ipv6 =3)" NL);
            PRINTF("    --ip_gethostbyname [<host name>]  = resolve hostname" NL);
            PRINTF(
                "    --ip_gethostbyname2 [<host name> <domain_type]  = resolve hostname for domain_type (ipv4 =2 ipv6 "
                "=3" NL);
            PRINTF("    --ip_dns_client [<start/stop>]         = Start/Stop the DNS Client" NL);
            PRINTF("    --ip_dns_server_addr <ip addr>         = Address of the DNS Server" NL);
            PRINTF("    --ip_dns_delete_server_addr <ip addr>         = Address of the DNS Server to be deleted" NL);
            PRINTF("    --ip_dns_timeout <timeout>         = response timeout in second" NL);
            PRINTF(
                "     --ip_dns_server_enable <enable/disable>         = Enable/Disable if DNS Server will answer "
                "client requests." NL);

#endif /* ENABLE_DNS_CLIENT */
#if ENABLE_HTTP_SERVER
            PRINTF("    --ip_http_server [<start/stop> <port>]         = Start/Stop the HTTP server" NL);
            PRINTF("    --ip_https_server [<start/stop> <port>]        = Start/Stop the HTTPs server" NL);
            PRINTF(
                "    --ip_http_post <page name>  <obj_name> <obj_type> <obj_len> <obj_value>         = Post/Update the "
                "object in HTML page" NL);
            PRINTF("               where <page name>       = page name to update" NL);
            PRINTF("                     <obj_name>        = object name to update" NL);
            PRINTF("                     <obj_type>        = <1-Bool 2-Integer 3-String>" NL);
            PRINTF("                     <obj_len>         = Length of object<1-Bool 4-Integer Length-String>" NL);
            PRINTF("                     <obj_value>       = Object value" NL);
            PRINTF("    --ip_http_get <page name>  <obj_name>          = Get the object in HTML page" NL);
            PRINTF("               where <page name>       = page name to get" NL);
            PRINTF("                     <obj_name>        = object name to get" NL);
#endif
#if ENABLE_HTTP_CLIENT
            PRINTF("    --ip_http_client [<connect/get/post/query/disc> <data1> <data2>]" NL);
            PRINTF("              where <connect> - Used to connect to HTTP server" NL);
            PRINTF("                              <data1> - Hostname or IPaddress of http server" NL);
            PRINTF("                              <data2> - port no of http server(optional)" NL);
            PRINTF("              where <get>     - Used to get a page from HTTP server" NL);
            PRINTF("                              <data1> - Page name to retrieve" NL);
            PRINTF("              where <post>    - Used to post to HTTP server" NL);
            PRINTF("                              <data1> - URL to post" NL);
            PRINTF("              where <query>   - Used to update a variable for get/post" NL);
            PRINTF("                              <data1> - Name of variable" NL);
            PRINTF("                              <data2> - value of variable" NL);
            PRINTF("              where <disc>    - Used to disconnect from HTTP server" NL);
#endif
            PRINTF("    --iphostname [<domain name>]  = configures local host name" NL);
#if ENABLE_DNS_SERVER
            PRINTF("    --ip_dns_local_domain [<domain name>]  = configures local domain" NL);
            PRINTF(
                "    --ip_dns <add/delete> <Host name> <addr>  = configures the domain name and its corresponding "
                "address" NL);
#endif
#if ENABLE_SNTP_CLIENT
            PRINTF(
                "    --ip_sntp_srvr <add/delete> <Host name>  = configures the host name or the IP address of the sntp "
                "server" NL);
            PRINTF("    --ip_sntp_get_time  = Command to Display the UTC Time" NL);
            PRINTF("    --ip_sntp_get_time_of_day  = Command to Display the seconds" NL);
            PRINTF("    --ip_sntp_zone [<UTC+hour:min>] [DSE] [enable/disable] = " NL
                   "Command to specify time zone and DSE(day light saving enable/disable)Eg:UTC+05:30 dse "
                   "enable/disable" NL);
            PRINTF("    --ip_show_sntpconfig = Displays the SNTP server address name" NL);
            PRINTF("    --ip_sntp_client [<start/stop>]         = Start/Stop the SNTP Client" NL);
#endif
#if ENABLE_SSL
            PRINTF("    --ssl_start server|client = start SSL as either server or client" NL);
            PRINTF("    --ssl_stop server|client = stop SSL server or client" NL);
            PRINTF(
                "    --ssl_config server|client [protocol <protocol>] [time 0|1] [domain 0|<name>] [cipher <cipher>] = "
                "configure SSL server or client" NL);
            PRINTF("        where protocol <protocol> = select protocol: SSL3, TLS1.0, TLS1.1, TLS1.2" NL);
            PRINTF("              time 0|1            = disable|enable certificate time validation (optional)" NL);
            PRINTF(
                "              domain 0|<name>     = disable|enable validation of peer's domain name against "
                "<name>" NL);
            PRINTF(
                "              alert 0|1           = disable|enable sending of SSL alert if certificate validation "
                "fails." NL);
            PRINTF(
                "              cipher <cipher>     = select <cipher> (enum name) suite to use, can be repeated 8 times "
                "(optional)" NL);
            PRINTF(
                "    --ssl_add_cert server|client  certificate|calist [<name>] = add a certificate or CA list to "
                "either SSL server or client." NL);
            PRINTF(
                "        where <name> = name of file to load from FLASH. A default certificate or CA list will be "
                "added if <name> is omitted." NL);
            PRINTF("    --ssl_store_cert <name> = store a certificate or CA list in FLASH with <name>" NL);
            PRINTF("    --ssl_delete_cert [<name>] = delete the certificate or CA list with <name> from FLASH." NL);
            PRINTF("    --ssl_list_cert  = list the names of the certificates and CA lists stored in FLASH." NL);
#endif
            PRINTF("    --ota_upgrade <addr> <filename> <mode> <preserve_last> <protocol>." NL);
            PRINTF("    --ota_read <offset> <size>" NL);
            PRINTF("    --ota_done <good_image>" NL);
            PRINTF("    --ota_cust <cmd_num> ....." NL);
            PRINTF("    --ota_format <partition_index>" NL);
            PRINTF("    --ota_ftp <ip> <port> <user name> <password> <filename> <flags> <partition_index>" NL);
            PRINTF("    --roaming <enable/disable>" NL);
#endif /* ENABLE_STACK_OFFLOAD */
            PRINTF("    --wow_set_mode <enable>" NL);
            PRINTF("    --wow_host_sleep <awake>" NL);
            PRINTF("    --enprobe 0|1   = disable|enable rx probe requests in host" NL);
            PRINTF("    --get_ctry" NL);
            PRINTF("    --countrycode <country code string>" NL);
        }
    }

    return return_code;
} /* Endbody */

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : wifiCallback
 * Returned Value : N/A
 * Comments       : Called from driver on a WiFI connection event
 *
 *END------------------------------------------------------------------*/
void wifiCallback(int val, uint8_t devId, uint8_t *mac, bool bssConn)
{
    wifi_state = val;
    if (val == true)
    {
        wifi_connected_flag = 1;
        PRINTF("Connected for device=%d" NL, devId);

        if (devId == 0)
        {
            PRINTF("%s connect event:" NL, bssConn ? "AP" : "CLIENT");
            concurrent_connect_flag = 0x0E;
        }
        TURN_LED_ON;
    }
    else if (val == INVALID_PROFILE) // this event is used to indicate RSNA failure
    {
        PRINTF("4 way handshake failure for device=%d n", devId);
    }
    else if (val == PEER_FIRST_NODE_JOIN_EVENT) // this event is used to RSNA success
    {
        PRINTF("4 way handshake success for device=%d" NL, devId);
    }
    else if (val == false)
    {
        wifi_connected_flag = 0;
        PRINTF("Not Connected for device=%d" NL, devId);
        if (devId == 0)
        {
            PRINTF("%s disconnect event:" NL, bssConn ? "AP" : "CLIENT");
            concurrent_connect_flag = 0x00;
        }
        TURN_LED_OFF;
    }
    else
    {
        PRINTF("last tx rate : %d kbps--for device=%d" NL, val, devId);
    }
    if (wps_context.wps_in_progress && devId == get_active_device())
    {
        wps_context.wps_in_progress = 0;
        set_power_mode(REC_POWER, WPS);
    }
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ascii_to_hex()
 * Returned Value  : hex counterpart for ascii
 * Comments	: Converts ascii character to correesponding hex value
 *
 *END*-----------------------------------------------------------------*/
static unsigned char ascii_to_hex(char val)
{
    if ('0' <= val && '9' >= val)
    {
        return (unsigned char)(val - '0');
    }
    else if ('a' <= val && 'f' >= val)
    {
        return (unsigned char)(val - 'a') + 0x0a;
    }
    else if ('A' <= val && 'F' >= val)
    {
        return (unsigned char)(val - 'A') + 0x0a;
    }

    return 0xff; /* error */
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : set_passphrase
 * Returned Value : A_OK
 * Comments       : Store WPA Passphrase for later use
 *
 *END------------------------------------------------------------------*/
static int32_t set_passphrase(char *passphrase)
{
    strncpy((char *)&wpa_passphrase, passphrase, sizeof(wpa_passphrase));
    return A_OK;
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : set_wep_key
 * Returned Value : A_ERROR on error else A_OK
 * Comments       : Store WEP key for later use. Size of Key must be 10 or 26
 *                  Hex characters
 *
 *END------------------------------------------------------------------*/
static int32_t set_wep_key(char *key_index, char *key_val)
{
    int key_idx = atoi(key_index);

    int i;
    if (strlen(key_val) != MIN_WEP_KEY_SIZE && strlen(key_val) != MAX_WEP_KEY_SIZE &&
        strlen(key_val) != (MIN_WEP_KEY_SIZE / 2) && strlen(key_val) != (MAX_WEP_KEY_SIZE / 2))
    {
        PRINTF("Invalid WEP Key length, only 10 or 26 HEX characters allowed (or) 5 or 13 ascii keys allowed" NL);
        return A_ERROR;
    }

    if (key_idx < 1 || key_idx > 4)
    {
        PRINTF("Invalid key index, Please enter between 1-4" NL);
        return A_ERROR;
    }

    if ((strlen(key_val) == MIN_WEP_KEY_SIZE) || (strlen(key_val) == MAX_WEP_KEY_SIZE))
    {
        for (i = 0; i < strlen(key_val); i++)
        {
            if (ishexdigit(key_val[i]))
            {
                continue;
            }
            else
            {
                PRINTF("for hex enter [0-9] or [A-F]" NL);
                return A_ERROR;
            }
        }
    }

    key[key_idx - 1].key_index = 1; // set flag to indicate occupancy
    memcpy(key[key_idx - 1].key, key_val, strlen(key_val));
    return A_OK;
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : clear_wep_keys
 * Returned Value : Success
 * Comments       : Clear wep keys
 *
 *END------------------------------------------------------------------*/
int32_t clear_wep_keys(void)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        key[i].key_index = 0;
        key[i].key[0]    = '\0';
    }
    return A_OK;
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : set_callback
 * Returned Value : A_ERROR on error else A_OK
 * Comments       : Sets callback function for WiFi connect/disconnect event
 *
 *END------------------------------------------------------------------*/
int32_t set_callback(void)
{
    int enet_device = get_active_device();
    return (qcom_set_connect_callback(enet_device, (void *)wifiCallback));
}

/*****************************************************
*Function- probe_req_handler- processes incoming probe requests.
* buf- points to begining of MAC header in the probe request frame.
* len- size of buffer
* freq- channel on which Probe req is received

* Structure of MAC Header-
*   uint8_t fc[2];
*   uint8_t dur[2]; //Duration in us
*   uint8_t addr1[IEEE80211_ADDR_LEN]; //Source address
*   uint8_t addr2[IEEE80211_ADDR_LEN]; //Destination address
*   uint8_t addr3[IEEE80211_ADDR_LEN];
*   uint8_t seq[2];  //Sequence number
*   uint8_t qos[2];  //QoS field
******************************************************/
void probe_req_handler(void *buf, int len, int freq)
{
    int i;
    PRINTF("RX: ProbeRequest from freq: %d MAC address: ", freq);
    for (i = 0; i < 6; i++)
    {
        PRINTF("%x ", *((uint8_t *)buf + PROBE_REQ_SRC_ADDR_OFFSET + i));
    }
    PRINTF(NL);
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : set_callback
 * Returned Value : A_ERROR on error else A_OK
 * Comments       : Sets callback function for WiFi connect/disconnect event
 *
 *END------------------------------------------------------------------*/
A_STATUS set_probe_req_callback(void)
{
    int enet_device = get_active_device();
    return (qcom_set_probe_req_callback(enet_device, (void *)probe_req_handler));
}

A_STATUS enable_probe_req_event(int argc, char *argv[])
{
    uint32_t enable = atoi(argv[2]);
    int enet_device = get_active_device();
    A_STATUS ret    = set_probe_req_callback();
    if (ret == A_OK)
    {
        return qcom_enable_probe_req_event(enet_device, enable);
    }
    return ret;
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : deviceid_handler
 * Returned Value : A_ERROR on error else A_OK
 * Comments       : Sets the Device ID in the driver
 *
 *END------------------------------------------------------------------*/
static int32_t deviceid_handler(int32_t index, int32_t argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    uint16_t deviceid = atoi(argv[index]);
    int32_t error;
    if (wps_context.wps_in_progress)
    {
        PRINTF("wps sesion in progress switching device not allowed:" NL);
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_SET_DEVICE_ID;
    inout_param.data   = (void *)&deviceid;
    inout_param.length = sizeof(uint16_t);

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    error = HANDLE_IOCTL(&inout_param);
    return error;
}

static int32_t set_active_deviceid(uint16_t device_index)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    uint16_t deviceid = device_index;
    int32_t error;
    if (wps_context.wps_in_progress)
    {
        PRINTF("wps sesion in progress switching device not allowed:" NL);
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_SET_DEVICE_ID;
    inout_param.data   = (void *)&deviceid;
    inout_param.length = sizeof(uint16_t);

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    error = HANDLE_IOCTL(&inout_param);
    return error;
}

#if ENABLE_SCC_MODE

static uint16_t freq_to_channel(uint16_t channel_val)
{
    if (channel_val < 3000)
    {
        channel_val -= ATH_IOCTL_FREQ_1;
        if ((channel_val / 5) == 14)
        {
            channel_val = 14;
        }
        else
        {
            channel_val = (channel_val / 5) + 1;
        }
    }
    else
    {
        channel_val -= ATH_IOCTL_FREQ_36;
        channel_val = 36 + (channel_val / 5); // since in 11a channel 36 is the starting number
    }
    return channel_val;
}

static int32_t handle_connect_for_concurrent(uint32_t enet_device, char *ssid)
{
    QCOM_SSID scan_ssid = {0};

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    strncpy((char *)&scan_ssid, ssid, sizeof(scan_ssid));
    strncpy((char *)&ssid_str_concurrent, ssid, sizeof(ssid_str_concurrent));
    PRINTF("remote ap ssid: %s" NL, ssid_str_concurrent);
    if (qcom_set_ssid(enet_device, &scan_ssid) != A_OK)
    {
        PRINTF("Unable to set SSID for scan" NL);
        return A_ERROR;
    }

    ap_channel_hint         = 0;
    concurrent_connect_flag = 0x0F;
    /*Do the actual scan*/
    wmi_set_scan(enet_device, NULL);
    concurrent_connect_flag = 0x0E;
    if (ap_channel_hint == 0)
    {
        PRINTF("Unable to find AP: %s....." NL, ssid);
        return A_ERROR;
    }
    PRINTF("find AP..." NL);
    A_MDELAY(50);

    if (qcom_set_channel(enet_device, ap_channel_hint) != A_OK)
    {
        PRINTF("Unable to set AP channel:%d" NL, ap_channel_hint);
        return A_ERROR;
    }
    A_MDELAY(50);
    return A_OK;
}

#endif

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : connect_handler()
 * Returned Value  : A_OK - successful completion or
 *					A_ERROR - failed.
 * Comments		  : Handles Connect commands for infrastructure mode, Open
 *                   WEP,WPA/WPA2 security is supported
 *
 *END*-----------------------------------------------------------------*/

static int32_t connect_handler(int32_t index, int32_t argc, char *argv[])
{
    int32_t error  = A_OK;
    QCOM_SSID ssid = {0};
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE temp_mode = QCOM_WLAN_DEV_MODE_INVALID;

    enet_device = get_active_device();
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    set_callback();

#if ENABLE_SCC_MODE
    if ((concurrent_connect_flag == 0x0E) && (enet_device == 1))
    {
        error = handle_connect_for_concurrent(enet_device, argv[2]);
        if (error != A_OK)
        {
            PRINTF("cannot connect for concurrent mode" NL);
            return A_ERROR;
        }
    }
#endif

    if (A_ERROR == qcom_op_get_mode(enet_device, &temp_mode))
    {
        return A_ERROR;
    }
    if (temp_mode == QCOM_WLAN_DEV_MODE_AP)
    {
        PRINTF("setting to ap mode" NL);
    }
    if ((SEC_MODE_OPEN == security_mode) && (temp_mode == QCOM_WLAN_DEV_MODE_AP))
    {
        PRINTF("AP in OPEN mode!" NL);
    }
    if (argc == 3)
    {
        int32_t tmp_len = strlen(argv[2]);
        if (tmp_len < sizeof(ssid.ssid))
        {
            strncpy(&ssid.ssid[0], argv[2], sizeof(ssid.ssid));
        }
        else
        {
            PRINTF("SSID '%s' is too long " NL, argv[2]);
            return A_ERROR;
        }
    }
    else
    {
        PRINTF("Missing ssid" NL);
        return A_ERROR;
    }

    error = qcom_set_ssid(enet_device, &ssid);
    if (error != 0)
    {
        PRINTF("Error during setting of ssid %s error=%08x!" NL, ssid.ssid, error);
        return A_ERROR;
    }

    PRINTF(NL "Setting SSID to %s" NL NL, ssid.ssid);

    if (SEC_MODE_WEP == security_mode)
    {
        cipher = WLAN_CRYPT_WEP_CRYPT;
        error  = qcom_sec_set_encrypt_mode(enet_device, cipher);

        if (error != A_OK)
        {
            return A_ERROR;
        }
    }
    else if (SEC_MODE_WPA == security_mode)
    {
        error = qcom_sec_set_encrypt_mode(enet_device, cipher);
        if (error != A_OK)
        {
            return A_ERROR;
        }
        error = qcom_sec_set_auth_mode(enet_device, wpa_ver);
        if (error != A_OK)
        {
            return A_ERROR;
        }
        if (strlen(wpa_passphrase.passphrase) != 64)
        {
            error = qcom_sec_set_passphrase(enet_device, &wpa_passphrase);
            if (error != A_OK)
            {
                return A_ERROR;
            }
            pmk_flag = 1;
        }
        else
        {
            if (qcom_sec_set_pmk(enet_device, (char *)&wpa_passphrase) != A_OK)
            {
                PRINTF("Unable to set pmk" NL);
                return A_ERROR;
            }
            pmk_flag = 1;
        }
        security_mode = SEC_MODE_OPEN;
    }

    error = qcom_commit(enet_device);
    if (error != A_OK)
    {
#if ENABLE_AP_MODE
        if (temp_mode == QCOM_WLAN_DEV_MODE_AP)
        {
            PRINTF("failed to AP mode" NL);
#if ENABLE_SCC_MODE
            PRINTF("support single concurrent channel only" NL);
#endif
            // Clear Invalid configurations
            error = qcom_sec_set_auth_mode(enet_device, WLAN_AUTH_NONE);
            if (error != A_OK)
            {
                PRINTF("Unable to clear Sec mode" NL);
                return A_ERROR;
            }

            security_mode = SEC_MODE_OPEN;
            pmk_flag      = 0;
        }
#endif
        return A_ERROR;
    }
#if ENABLE_AP_MODE
    if (temp_mode == QCOM_WLAN_DEV_MODE_AP)
    {
/* setting the AP's default IP to 192.168.1.1 */
#if ENABLE_STACK_OFFLOAD
// uint32_t ip_addr = 0xc0a80101,mask = 0xffffff00,gateway = 0xc0a80101;
// t_ipconfig((void*)handle,IPCFG_STATIC, &ip_addr, &mask, &gateway);
#else

#define AP_IPADDR IPADDR(192, 168, 1, 1)
#define AP_IPMASK IPADDR(255, 255, 255, 0)
#define AP_IPGATEWAY IPADDR(192, 168, 1, 1)

        uint32_t error;
        IPCFG_IP_ADDRESS_DATA ip_data;

        ip_data.ip      = AP_IPADDR;
        ip_data.mask    = AP_IPMASK;
        ip_data.gateway = AP_IPGATEWAY;

        error = ipcfg_bind_staticip(enet_device, &ip_data);
        if (error != 0)
        {
            PRINTF("Error during static ip bind %08x!" NL, error);
            return SHELL_EXIT_ERROR;
        }
#endif
    }
#endif

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ad_hoc_connect_handler()
 * Returned Value  : 1 - successful completion or
 *					0 - failed.
 * Comments		  : Handles Connect commands for ad-hoc mode, Open
 *                   & WEP security is supported
 *
 *END*-----------------------------------------------------------------*/

static int32_t ad_hoc_connect_handler(int32_t index, int32_t argc, char *argv[])
{
    int32_t error = A_OK, i;
    int key_idx   = 0;
    uint32_t enet_device;

    enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    set_callback();
    qcom_op_set_mode(enet_device, QCOM_WLAN_DEV_MODE_ADHOC);

    QCOM_SSID tmp_ssid = {0};
    strncpy((char *)&tmp_ssid, argv[index], sizeof(tmp_ssid));
    error = qcom_set_ssid(enet_device, &tmp_ssid);
    if (error != 0)
    {
        PRINTF("Error during setting of ssid %s error=%08x!" NL, argv[index], error);
        return A_ERROR;
    }

    PRINTF("Setting SSID to %s" NL NL, argv[index]);

    if (index + 1 < argc)
    {
        i = index + 1;
        if (ATH_STRCMP(argv[i], "--wep") == 0)
        {
            if (argc < 4) // User did not enter parameter
            {
                PRINTF("Missing parameters" NL);
                return A_ERROR;
            }

            i++;
            key_idx = atoi(argv[i]);

            if (key_idx < 1 || key_idx > 4)
            {
                PRINTF("Invalid default key index, Please enter between 1-4" NL);
                return A_ERROR;
            }

            if (key[key_idx - 1].key_index != 1)
            {
                PRINTF("This key is not present, please enter WEP key first" NL);
                return A_ERROR;
            }

            if (key[0].key[0] != '\0')
                qcom_sec_set_wepkey(enet_device, 1, key[0].key);

            if (key[1].key[0] != '\0')
                qcom_sec_set_wepkey(enet_device, 2, key[1].key);

            if (key[2].key[0] != '\0')
                qcom_sec_set_wepkey(enet_device, 3, key[2].key);

            if (key[3].key[0] != '\0')
                qcom_sec_set_wepkey(enet_device, 4, key[3].key);

            error = qcom_sec_set_wepkey_index(enet_device, key_idx);
            if (error != A_OK)
            {
                return A_ERROR;
            }
            clear_wep_keys();

            if (argc > 5)
            {
                i++;
                if (ATH_STRCMP(argv[i], "--channel") == 0)
                {
                    if (argc < 7) // User did not enter parameter
                    {
                        PRINTF("Missing Channel parameter" NL);
                        return A_ERROR;
                    }
                    i++;
                    PRINTF("Setting Channel to %d" NL, atoi(argv[i]));
                    if (qcom_set_channel(enet_device, atoi(argv[i])) == A_ERROR)
                        return A_ERROR;
                }
            }
        }
        else if (ATH_STRCMP(argv[i], "--channel") == 0)
        {
            if (argc < 4) // User did not enter parameter
            {
                PRINTF("Missing Channel parameter" NL);
                return A_ERROR;
            }
            i++;
            PRINTF("Setting Channel to %d" NL, atoi(argv[i]));
            if (qcom_set_channel(enet_device, atoi(argv[i])) == A_ERROR)
                return A_ERROR;
        }
    }
    error = qcom_commit(enet_device);
    if (error != A_OK)
    {
        return A_ERROR;
    }
    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : qcom_roaming_ctrl()
 * Parames:
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : Sample function to depict IPv6 ping functionality
 *                   Not supported on RTCS
 *END*-----------------------------------------------------------------*/
int qcom_roaming_ctrl(WMI_SET_ROAM_CTRL_CMD *roam_ctrl)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error;
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_ROAM_CTRL;
    inout_param.data   = &roam_ctrl;
    error              = HANDLE_IOCTL(&inout_param);
    if (A_OK != error)
    {
        PRINTF("roam ctrl Failed" NL);
        return error;
    }

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : qcom_get_temperature()
 * Parames:
 *       regval  ---  register value for KingFisher
 *       tempval ---  physical temperature value after caculation
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure. * Comments        : Sample function to depict IPv6 ping functionality
 *                   Not supported on RTCS
 *END*-----------------------------------------------------------------*/
int qcom_get_temperature(uint32_t *regval, float *tempval)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error;
    WMI_GET_TEMPERATURE_REPLY temperature;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_GET_TEMPERATURE;
    inout_param.data   = &temperature;
    // inout_param.length = 1;

    error = HANDLE_IOCTL(&inout_param);
    if (A_OK != error)
    {
        PRINTF("Get temperature Failed" NL);
        return error;
    }
    *regval  = temperature.tempRegVal;
    *tempval = ((float)temperature.tempDegree) / 10;

    return A_OK;
}

#if ENABLE_P2P_MODE
/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : P2PCallbackfn
 * Returned Value : N/A
 * Comments       : Called from driver on a WiFI connection event
 *
 *END------------------------------------------------------------------*/
void P2PCallBackfn(uint8_t *ptr)
{
    uint8_t *local_ptr2 = ptr, inv_response_evt_index = 0;
    union
    {
        WMI_SET_PASSPHRASE_CMD setPassPhrase;
        WMI_P2P_GRP_INIT_CMD grpInit;
        WMI_P2P_SET_CMD p2p_set_params;
    } stkP2p;
    ATH_WPS_START wps_start;
    WMI_P2P_GO_NEG_RESULT_EVENT *p2pNeg;
    uint32_t error, chnl, loop_index;
    uint32_t enet_device, temp_device_id = 0;

    enet_device = get_active_device();
    if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_PROV_DISC_REQ_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);

        uint16_t wps_method;

        PRINTF(NL "source addr : %x:%x:%x:%x:%x:%x" NL, ((WMI_P2P_PROV_DISC_REQ_EVENT *)local_ptr2)->sa[0],
               ((WMI_P2P_PROV_DISC_REQ_EVENT *)local_ptr2)->sa[1], ((WMI_P2P_PROV_DISC_REQ_EVENT *)local_ptr2)->sa[2],
               ((WMI_P2P_PROV_DISC_REQ_EVENT *)local_ptr2)->sa[3], ((WMI_P2P_PROV_DISC_REQ_EVENT *)local_ptr2)->sa[4],
               ((WMI_P2P_PROV_DISC_REQ_EVENT *)local_ptr2)->sa[5]);

        PRINTF(NL "wps_config_method : %x" NL,
               A_LE2CPU16(((WMI_P2P_PROV_DISC_REQ_EVENT *)local_ptr2)->wps_config_method));

        wps_method = A_LE2CPU16(((WMI_P2P_PROV_DISC_REQ_EVENT *)local_ptr2)->wps_config_method);

        if (WPS_CONFIG_DISPLAY == wps_method)
        {
            PRINTF("Provisional Disc Request - Display WPS PIN [%s]" NL, p2p_wps_pin);
        }
        else if (WPS_CONFIG_KEYPAD == wps_method)
        {
            PRINTF("Provisional Disc Request - Enter WPS PIN" NL);
        }
        else if (WPS_CONFIG_PUSHBUTTON == wps_method)
        {
            PRINTF("Provisional Disc Request - Push Button" NL);
        }
        else
        {
            PRINTF("Invalid Provisional Request" NL);
        }
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_REQ_TO_AUTH_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);
        PRINTF(NL "source addr : %x:%x:%x:%x:%x:%x" NL, ((WMI_P2P_REQ_TO_AUTH_EVENT *)local_ptr2)->sa[0],
               ((WMI_P2P_REQ_TO_AUTH_EVENT *)local_ptr2)->sa[1], ((WMI_P2P_REQ_TO_AUTH_EVENT *)local_ptr2)->sa[2],
               ((WMI_P2P_REQ_TO_AUTH_EVENT *)local_ptr2)->sa[3], ((WMI_P2P_REQ_TO_AUTH_EVENT *)local_ptr2)->sa[4],
               ((WMI_P2P_REQ_TO_AUTH_EVENT *)local_ptr2)->sa[5]);

        PRINTF(NL "dev_password_id : %x" NL, ((WMI_P2P_REQ_TO_AUTH_EVENT *)local_ptr2)->dev_password_id);
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_LIST_PERSISTENT_NETWORK_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);
        A_MEMZERO(p2p_pers_data, (MAX_LIST_COUNT * sizeof(WMI_PERSISTENT_MAC_LIST)));
        memcpy(p2p_pers_data, local_ptr2, (MAX_LIST_COUNT * sizeof(WMI_PERSISTENT_MAC_LIST)));
        PRINTF("" NL);
        if (p2p_cancel_enable == 0)
        {
            for (loop_index = 0; loop_index < MAX_LIST_COUNT; loop_index++)
            {
                PRINTF("mac_addr[%d] : %x:%x:%x:%x:%x:%x" NL, loop_index,
                       ((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->macaddr[0],
                       ((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->macaddr[1],
                       ((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->macaddr[2],
                       ((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->macaddr[3],
                       ((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->macaddr[4],
                       ((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->macaddr[5]);
                PRINTF("ssid[%d] : %s" NL, loop_index, ((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->ssid);
                if (((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->role == WMI_P2P_INVITE_ROLE_ACTIVE_GO)
                    PRINTF("passphrase[%d] : %s" NL, loop_index, ((WMI_PERSISTENT_MAC_LIST *)(local_ptr2))->passphrase);
                local_ptr2 += sizeof(WMI_PERSISTENT_MAC_LIST);
            }
        }
        else
        {
            p2p_cancel_enable = 0;
        }
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_SDPD_RX_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);
        PRINTF("Custom_Api_p2p_serv_disc_req event" NL);
        PRINTF("type : %d   frag id : %x" NL, ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->type,
               ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->frag_id);
        PRINTF("transaction_status : %x" NL, ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->transaction_status);
        PRINTF("freq : %d status_code : %d comeback_delay : %d tlv_length : %d update_indic : %d" NL,
               A_LE2CPU16(((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->freq),
               A_LE2CPU16(((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->status_code),
               A_LE2CPU16(((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->comeback_delay),
               A_LE2CPU16(((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->tlv_length),
               A_LE2CPU16(((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->update_indic));
        PRINTF("source addr : %x:%x:%x:%x:%x:%x" NL, ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->peer_addr[0],
               ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->peer_addr[1], ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->peer_addr[2],
               ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->peer_addr[3], ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->peer_addr[4],
               ((WMI_P2P_SDPD_RX_EVENT *)local_ptr2)->peer_addr[5]);
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_PROV_DISC_RESP_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);
        PRINTF(NL "peer addr : %x:%x:%x:%x:%x:%x" NL, ((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->peer[0],
               ((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->peer[1],
               ((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->peer[2],
               ((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->peer[3],
               ((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->peer[4],
               ((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->peer[5]);

        if (WPS_CONFIG_KEYPAD == A_LE2CPU16(((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->config_methods))
        {
            PRINTF("Provisional Disc Response Keypad - WPS PIN [%s]" NL, p2p_wps_pin);
        }
        else if (WPS_CONFIG_DISPLAY == A_LE2CPU16(((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->config_methods))
        {
            PRINTF("Provisional Disc Response Display" NL);
        }
        else if (WPS_CONFIG_PUSHBUTTON == A_LE2CPU16(((WMI_P2P_PROV_DISC_RESP_EVENT *)local_ptr2)->config_methods))
        {
            PRINTF("Provisional Disc Response Push Button" NL);
        }
        else
        {
            PRINTF("Invalid Provisional Response" NL);
        }
        // PRINTF(NL "config_methods : %x" NL,                A_LE2CPU16(((WMI_P2P_PROV_DISC_RESP_EVENT
        // *)local_ptr2)->config_methods));
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_NODE_LIST_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);
        uint8_t index, temp_val, *temp_ptr = NULL;
        // temp_val = ((WMI_P2P_NODE_LIST_EVENT *)(local_ptr2))->num_p2p_dev;
        temp_val = *local_ptr2;
        local_ptr2++;

        temp_ptr = local_ptr2;
        // local_ptr2 += sizeof(uint8_t);
        p2p_session_in_progress = 1;
        enet_device             = get_active_device();
        if (enet_device != 0)
        {
            /* P2P device should always send/receive events/commands on dev 0 if
            the app has switched to dev 1 while event is in dev 0 send command
            via dev 0 and then switch to dev 1*/
            temp_device_id = enet_device;
            enet_device    = 0;
            set_active_deviceid(enet_device);
        }
        if (temp_val > 0)
        {
            for (index = 0; index < temp_val; index++)
            {
                if (p2p_join_session_active)
                {
                    if (A_MEMCMP(((P2P_DEVICE_LITE *)(local_ptr2))->interface_addr, p2p_join_mac_addr, ATH_MAC_LEN) ==
                        0)
                    {
                        p2p_join_profile.go_oper_freq = ((P2P_DEVICE_LITE *)(local_ptr2))->oper_freq;
                        break;
                    }
                }
                else
                {
                    PRINTF(NL "\t p2p_config_method     : %x" NL, (((P2P_DEVICE_LITE *)(local_ptr2))->config_methods));
                    PRINTF("\t p2p_device_name       : %s" NL, ((P2P_DEVICE_LITE *)(local_ptr2))->device_name);

                    PRINTF("\t p2p_primary_dev_type  : %x:%x:%x:%x:%x:%x:%x:%x" NL,
                           ((P2P_DEVICE_LITE *)(local_ptr2))->pri_dev_type[0],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->pri_dev_type[1],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->pri_dev_type[2],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->pri_dev_type[3],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->pri_dev_type[4],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->pri_dev_type[5],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->pri_dev_type[6],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->pri_dev_type[7]);

                    PRINTF("\t p2p_interface_addr    : %x:%x:%x:%x:%x:%x" NL,
                           ((P2P_DEVICE_LITE *)(local_ptr2))->interface_addr[0],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->interface_addr[1],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->interface_addr[2],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->interface_addr[3],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->interface_addr[4],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->interface_addr[5]);
                    PRINTF("\t p2p_device_addr       : %x:%x:%x:%x:%x:%x" NL,
                           ((P2P_DEVICE_LITE *)(local_ptr2))->p2p_device_addr[0],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->p2p_device_addr[1],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->p2p_device_addr[2],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->p2p_device_addr[3],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->p2p_device_addr[4],
                           ((P2P_DEVICE_LITE *)(local_ptr2))->p2p_device_addr[5]);
                    PRINTF("\t p2p_device_capability : %x" NL, ((P2P_DEVICE_LITE *)(local_ptr2))->dev_capab);
                    PRINTF("\t p2p_group_capability  : %x" NL, ((P2P_DEVICE_LITE *)(local_ptr2))->group_capab);
                    PRINTF("\t p2p_wps_method        : %x" NL, (int)((P2P_DEVICE_LITE *)(local_ptr2))->wps_method);
                    // PRINTF("\t Peer Listen channel   : %d" NL, A_LE2CPU16(((struct p2p_device
                    // *)(local_ptr2->dataptr))->listen_freq));
                    PRINTF("\t Peer Oper   channel   : %d" NL,
                           A_LE2CPU16(((P2P_DEVICE_LITE *)(local_ptr2))->oper_freq));
                }
                local_ptr2 += sizeof(P2P_DEVICE_LITE);
            }
            if (p2p_join_session_active)
            {
                p2p_join_session_active                = 0;
                stkP2p.p2p_set_params.val.mode.p2pmode = P2P_CLIENT;
                if (qcom_p2p_set(enet_device, P2P_CONFIG_P2P_OPMODE, &stkP2p.p2p_set_params.val,
                                 sizeof(stkP2p.p2p_set_params.val.mode)) != A_OK)
                {
                    PRINTF(NL "StartP2P JOIN SET command did not execute properly" NL);
                    p2p_session_in_progress = 0;
                    return;
                }
                if (qcom_p2p_func_join(enet_device, (P2P_WPS_METHOD)p2p_join_profile.wps_method, &p2p_join_mac_addr[0],
                                       p2p_wps_pin, p2p_join_profile.go_oper_freq) != A_OK)
                {
                    PRINTF(NL "P2P JOIN command did not execute properly" NL);
#if ENABLE_SCC_MODE
                    PRINTF("support single concurrent channel only...." NL);
#endif
                    p2p_session_in_progress = 0;
                    return;
                }
            }
        }
        local_ptr2 = temp_ptr;
        if (temp_device_id != 0)
        {
            set_active_deviceid(temp_device_id);
        }
        p2p_session_in_progress = 0;
        if (temp_val > 0)
            PRINTF("shell> ");
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_INVITE_SENT_RESULT_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);
        PRINTF("Invitation Result %d" NL, ((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->status);

        if (((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->status == 0)
        {
            PRINTF("SSID %x:%x:%x:%x:%x:%x" NL, ((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->bssid[0],
                   ((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->bssid[1],
                   ((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->bssid[2],
                   ((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->bssid[3],
                   ((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->bssid[4],
                   ((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->bssid[5]);
        }

        if ((p2p_pers_data[invitation_index].role == WMI_P2P_INVITE_ROLE_ACTIVE_GO) && (p2p_persistent_done == 0) &&
            (((WMI_P2P_INVITE_SENT_RESULT_EVENT *)(local_ptr2))->status == 0))
        {
            p2p_session_in_progress = 1;
            enet_device             = get_active_device();
            if (enet_device != 0)
            {
                /* P2P device should always send/receive events/commands on dev 0 if
            the app has switched to dev 1 while event is in dev 0 send command
            via dev 0 and then switch to dev 1*/
                temp_device_id = enet_device;
                enet_device    = 0;
                set_active_deviceid(enet_device);
            }
            wps_flag = 0x01;

            PRINTF("Starting Autonomous GO" NL);
            if (qcom_p2p_func_start_go(enet_device, NULL, NULL, P2P_AUTO_CHANNEL, 1) != A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return;
            }

            p2p_persistent_done = 1;

            if (temp_device_id != 0)
            {
                set_active_deviceid(temp_device_id);
            }
            p2p_session_in_progress = 0;
        }
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_INVITE_RCVD_RESULT_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);
        int i;
        PRINTF("Invite Result Status: %x" NL, ((WMI_P2P_INVITE_RCVD_RESULT_EVENT *)(local_ptr2))->status);
        if (((WMI_P2P_INVITE_RCVD_RESULT_EVENT *)(local_ptr2))->status == 0)
        {
            for (i = 0; i < ATH_MAC_LEN; i++)
            {
                PRINTF(" [%x] ", ((WMI_P2P_INVITE_RCVD_RESULT_EVENT *)(local_ptr2))->sa[i]);
            }
        }
        else
        {
            qcom_p2p_func_stop_find(enet_device);
        }
        PRINTF("" NL);
        if ((p2p_pers_data[inv_response_evt_index].role == WMI_P2P_INVITE_ROLE_ACTIVE_GO) &&
            (p2p_persistent_done == 0) && (((WMI_P2P_INVITE_RCVD_RESULT_EVENT *)(local_ptr2))->status == 0))
        {
            //
            // make AP Mode and WPS default settings for P2P GO
            //
            p2p_session_in_progress = 1;
            enet_device             = get_active_device();
            if (enet_device != 0)
            {
                /* P2P device should always send/receive events/commands on dev 0 if
                the app has switched to dev 1 while event is in dev 0 send command
                via dev 0 and then switch to dev 1*/
                temp_device_id = enet_device;
                enet_device    = 0;
                set_active_deviceid(enet_device);
            }

            wps_flag = 0x01;
            PRINTF("Starting Autonomous GO" NL);

            QCOM_PASSPHRASE tmp_pass = {0};
            QCOM_SSID tmp_ssid       = {0};
            strncpy((char *)&tmp_pass, (char const *)p2p_pers_data[inv_response_evt_index].passphrase,
                    sizeof(tmp_pass));
            strncpy((char *)&tmp_ssid, (char const *)p2p_pers_data[inv_response_evt_index].ssid, sizeof(tmp_ssid));

            if (qcom_p2p_func_start_go(enet_device, &tmp_ssid, &tmp_pass, P2P_AUTO_CHANNEL, 1) != A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return;
            }

            p2p_persistent_done    = 1;
            inv_response_evt_index = 0;

            if (temp_device_id != 0)
            {
                set_active_deviceid(temp_device_id);
            }
            p2p_session_in_progress = 0;
        }
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_INVITE_REQ_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);

        int i;
        WMI_P2P_FW_INVITE_REQ_RSP_CMD invite_rsp_cmd;

        PRINTF("Invitation Req Received From:");
        for (i = 0; i < ATH_MAC_LEN; i++)
        {
            PRINTF(" %x: ", ((WMI_P2P_FW_INVITE_REQ_EVENT *)(local_ptr2))->sa[i]);
        }
        PRINTF("" NL);

        A_MEMZERO(&invite_rsp_cmd, sizeof(WMI_P2P_FW_INVITE_REQ_RSP_CMD));

        if (((WMI_P2P_FW_INVITE_REQ_EVENT *)(local_ptr2))->is_persistent)
        {
            for (i = 0; i < MAX_LIST_COUNT; i++)
            {
                if (A_MEMCMP(((WMI_P2P_FW_INVITE_REQ_EVENT *)(local_ptr2))->sa, p2p_pers_data[i].macaddr,
                             ATH_MAC_LEN) == 0)
                {
                    invite_rsp_cmd.status  = 0;
                    inv_response_evt_index = i;
                    A_MEMCPY(invite_rsp_cmd.group_bssid, p2p_pers_data[i].macaddr, ATH_MAC_LEN);
                    break;
                }
            }

            if (i == MAX_LIST_COUNT)
            {
                invite_rsp_cmd.status = 1;
                i                     = 0;
            }
        }
        else
        {
            invite_rsp_cmd.status = 0;
            A_MEMCPY(invite_rsp_cmd.group_bssid, ((WMI_P2P_FW_INVITE_REQ_EVENT *)(local_ptr2))->sa, ATH_MAC_LEN);
        }
        p2p_session_in_progress = 1;
        enet_device             = get_active_device();
        if (enet_device != 0)
        {
            /* P2P device should always send/receive events/commands on dev 0 if
            the app has switched to dev 1 while event is in dev 0 send command
            via dev 0 and then switch to dev 1*/
            temp_device_id = enet_device;
            enet_device    = 0;
            set_active_deviceid(enet_device);
        }
        /* send invite auth event */
        if (qcom_p2p_func_invite_auth(enet_device, (uint8_t *)&invite_rsp_cmd) != A_OK)
        {
            PRINTF(NL "StartP2P (P2P invite auth persistent)command did not execute properly" NL);
            return;
        }

        if (temp_device_id != 0)
        {
            set_active_deviceid(temp_device_id);
        }
        p2p_session_in_progress = 0;
    }
    else if (((WMI_EVT_TO_APP *)ptr)->EVENT_ID == WMI_P2P_GO_NEG_RESULT_EVENTID)
    {
        local_ptr2 += sizeof(uint32_t);
        p2pNeg     = (WMI_P2P_GO_NEG_RESULT_EVENT *)local_ptr2;
        task2_msec = 100;
        PRINTF("P2P GO Negotion Result :" NL);
        PRINTF("Status       :%s" NL, (p2pNeg->status) ? "FAILURE" : "SUCCESS");
        PRINTF("P2P Role     :%s" NL, (p2pNeg->role_go) ? "P2P GO" : "P2P Client");
        PRINTF("SSID         :%s" NL, p2pNeg->ssid);
        PRINTF("Channel      :%d" NL, p2pNeg->freq);
        PRINTF("WPS Method   :%s" NL, (p2pNeg->wps_method == WPS_PBC) ? "PBC" : "PIN");
        p2p_session_in_progress = 1;
        enet_device             = get_active_device();
        if (enet_device != 0)
        {
            /* P2P device should always send/receive events/commands on dev 0 if
         the app has switched to dev 1 while event is in dev 0 send command
         via dev 0 and then switch to dev 1*/
            temp_device_id = enet_device;
            enet_device    = 0;
            set_active_deviceid(enet_device);
        }
        if (p2pNeg->status != 0)
        {
            p2p_persistent_go = 0;
            qcom_p2p_func_stop_find(enet_device);
        }
        if ((p2pNeg->status == 0) && (p2pNeg->role_go == 1))
        {
            wps_flag                        = 0x01;
            chnl                            = (p2pNeg->freq - 2412) / 5 + 1;
            stkP2p.grpInit.persistent_group = p2p_persistent_go;

            /* Reset global p2p_persistent_go variable */
            p2p_persistent_go        = 0;
            QCOM_SSID tmp_ssid       = {0};
            QCOM_PASSPHRASE tmp_pass = {0};
            strncpy((char *)&tmp_ssid, (const char *)p2pNeg->ssid, sizeof(tmp_ssid));
            strncpy((char *)&tmp_pass, (const char *)p2pNeg->pass_phrase, sizeof(tmp_pass));
            if (qcom_p2p_func_start_go(enet_device, &tmp_ssid, &tmp_pass, chnl, stkP2p.grpInit.persistent_group) !=
                A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return;
            }

/* the application should block here waiting for the connect event before issuing wps_start */
#if ENABLE_FPGA_BUILD
            A_MDELAY(5000);
#else
            A_MDELAY(2000);
#endif
        }
        else if ((p2pNeg->status == 0) && (p2pNeg->role_go == 0))
        {
            QCOM_SSID tmp_ssid = {0};
            strncpy((char *)&tmp_ssid, (char const *)p2pNeg->ssid, sizeof(tmp_ssid));
            error = qcom_set_ssid(enet_device, &tmp_ssid);
            if (error != A_OK)
            {
                PRINTF("Unable to set SSID" NL);
            }
        }
        /* do WPS */
        if (p2pNeg->status == 0)
        {
            qcom_wps_credentials_t wpsCreden;
            A_MEMZERO(&wps_start, sizeof(ATH_WPS_START));

            if ((p2pNeg->role_go == 0))
            {
                qcom_op_set_mode(enet_device, QCOM_WLAN_DEV_MODE_STATION);
            }
            else if ((p2pNeg->role_go == 1))
            {
                qcom_op_set_mode(enet_device, QCOM_WLAN_DEV_MODE_AP);
            }

            wps_context.connect_flag = (p2pNeg->role_go) ? 0 : 8;

            if (p2pNeg->wps_method != WPS_PBC)
            {
                wps_start.wps_mode   = 0;
                wps_start.pin_length = 8;
                // FIXME: This hardcoded pin value needs to be changed
                // for production to reflect what is on a sticker/label
                memcpy(wps_start.pin, p2p_wps_pin, wps_start.pin_length);
                wps_start.pin[wps_start.pin_length] = '\0';
            }
            else
            {
                wps_start.wps_mode = 1;
            }

            strncpy((char *)&wpsCreden.ssid, (char *)p2pNeg->ssid, sizeof(wpsCreden.ssid));
            // memcpy(wpsCreden.mac_addr,p2pNeg->peer_device_addr,ATH_MAC_LEN);
            memcpy(wpsCreden.mac_addr, p2pNeg->peer_interface_addr, ATH_MAC_LEN);
            wpsCreden.ap_channel = p2pNeg->freq;
            wpsCreden.ssid_len   = p2pNeg->ssid_len;

            qcom_wps_set_credentials(enet_device, &wpsCreden);
            /* this starts WPS on the Aheros wifi */
            if (qcom_wps_start(enet_device, wps_context.connect_flag, wps_start.wps_mode, (char *)wps_start.pin) !=
                A_OK)
            {
                return;
            }
            wps_context.wps_in_progress = 1;
        }
        if (temp_device_id != 0)
        {
            set_active_deviceid(temp_device_id);
        }
        p2p_session_in_progress = 0;
    }
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : p2p_callback
 * Returned Value : A_ERROR on error else A_OK
 * Comments       : Sets callback function for WiFi P2P events
 *
 *END------------------------------------------------------------------*/
int32_t p2p_callback(uint8_t evt_flag)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error;
    WMI_EVT_TO_APP p2p_evt;
    if (evt_flag == 1)
    {
        inout_param.cmd_id = ATH_P2P_EVENT_CALLBACK;
    }
    else
    {
        inout_param.cmd_id = ATH_SET_P2P_CALLBACK;
    }
    A_MEMZERO(&p2p_evt, sizeof(WMI_EVT_TO_APP));
    inout_param.data   = (void *)&p2p_evt;
    inout_param.length = sizeof(WMI_EVT_TO_APP);
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    error = HANDLE_IOCTL(&inout_param);
    if (error == A_OK)
    {
        P2PCallBackfn((uint8_t *)&p2p_evt);
    }

    return error;
}

#endif

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : set_wpa()
 * Returned Value  : A_OK - successful completion or
 *		    A_ERROR - failed.
 * Comments	: Sets WPA mode
 *
 *END*-----------------------------------------------------------------*/
static int32_t set_wpa(int32_t index, int32_t argc, char *argv[])
{
    int i = index;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE temp_mode = QCOM_WLAN_DEV_MODE_INVALID;

    if (5 != argc) // User did not enter parameter
    {
        PRINTF("Missing parameters" NL);
        return A_ERROR;
    }

    enet_device = get_active_device();

    /********** wpa version ****************/
    if (ATH_STRCMP(argv[i], "1") == 0)
    {
        wpa_ver = WLAN_AUTH_WPA_PSK;

        // this is a hack we currently only support wpa2 AES in AP mode
        if (A_ERROR == qcom_op_get_mode(enet_device, &temp_mode))
        {
            return A_ERROR;
        }
        if (temp_mode == QCOM_WLAN_DEV_MODE_AP)
        {
            wpa_ver = WLAN_AUTH_WPA2_PSK;
        }
    }
    else if (ATH_STRCMP(argv[i], "2") == 0)
    {
        wpa_ver = WLAN_AUTH_WPA2_PSK;
    }
    else
    {
        PRINTF("Invalid version" NL);
        return A_ERROR;
    }

    i++;
    /**************** cipher **********/
    if (!ATH_STRCMP(argv[i], argv[i + 1]))
    {
        if (!ATH_STRCMP(argv[i], "TKIP"))
        {
            cipher = WLAN_CRYPT_TKIP_CRYPT;
        }
        else if (!ATH_STRCMP(argv[i], "CCMP"))
        {
            cipher = WLAN_CRYPT_AES_CRYPT;
        }
        else
        {
            PRINTF("invaid uchipher mcipher, should be TKIP or CCMP" NL);
            return A_ERROR;
        }
    }
    else
    {
        PRINTF("invaid uchipher mcipher, should be same" NL);
        return A_ERROR;
    }

    security_mode = SEC_MODE_WPA;
    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : set_wep()
 * Returned Value  : A_OK - successful completion or
 *		    A_ERROR - failed.
 * Comments	: Sets WEP parameters
 *
 *END*-----------------------------------------------------------------*/
static int32_t set_wep(int32_t index, int32_t argc, char *argv[])
{
    int i         = index;
    int key_idx   = 0;
    int32_t error = A_OK;
    uint32_t enet_device;

    enet_device = get_active_device();

    if (argc < 3) // User did not enter parameter
    {
        PRINTF("Missing parameters" NL);
        return A_ERROR;
    }

    key_idx = atoi(argv[i]);

    if (key_idx < 1 || key_idx > 4)
    {
        PRINTF("Invalid default key index, Please enter between 1-4" NL);
        return A_ERROR;
    }

    if (key[key_idx - 1].key_index != 1)
    {
        PRINTF("This key is not present, please enter WEP key first" NL);
        return A_ERROR;
    }

    if (key[0].key[0] != '\0')
        qcom_sec_set_wepkey(enet_device, 1, key[0].key);

    if (key[1].key[0] != '\0')
        qcom_sec_set_wepkey(enet_device, 2, key[1].key);

    if (key[2].key[0] != '\0')
        qcom_sec_set_wepkey(enet_device, 3, key[2].key);

    if (key[3].key[0] != '\0')
        qcom_sec_set_wepkey(enet_device, 4, key[3].key);

    error = qcom_sec_set_wepkey_index(enet_device, key_idx);

    security_mode = SEC_MODE_WEP;
    clear_wep_keys();

    return error;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : wps_query()
 * Returned Value  : 0 - success, 1 - failure
 * Comments	: Queries WPS status
 *
 *END*-----------------------------------------------------------------*/
int32_t wps_query(uint8_t block)
{
    ATH_NETPARAMS netparams = {0};
    int32_t status          = 1;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;

    enet_device = get_active_device();
    if (wps_context.wps_in_progress)
    {
        if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
        {
            return A_ERROR;
        }
        if (CompleteWPS(&netparams, block, (wifimode_t)wifimode))
        {
            A_MEMCPY(&wps_context.netparams, &netparams, sizeof(ATH_NETPARAMS));

            if (netparams.error)
            {
                wps_context.connect_flag = 0;
            }

            if (wps_context.connect_flag)
            {
#if ENABLE_SCC_MODE
                do
                {
                    if ((concurrent_connect_flag == 0x0E) && (enet_device == 1))
                    {
                        PRINTF("ap_ch:%d" NL, netparams.ap_channel);
                        if (qcom_set_channel(enet_device, freq_to_channel(netparams.ap_channel)) != A_OK)
                        {
                            PRINTF("Unable to set to AP channel:%d" NL, netparams.ap_channel);
                            break;
                        }
                        A_MDELAY(50);
                    }
                    if (ATH_NET_CONNECT(&netparams))
                    {
                        PRINTF("wps connection failed" NL);
                    }
                } while (0);
#else
                if (ATH_NET_CONNECT(&netparams))
                {
                    PRINTF("connection failed" NL);
                }
#endif
            }
            status = 0;
            PRINTF("Complete WPS Case %d" NL, wps_context.wps_in_progress);
            if (wps_context.wps_in_progress == 1)
            {
                set_power_mode(REC_POWER, WPS);
            }
            wps_context.wps_in_progress = 0;
        }
        else
        {
            if (A_EBUSY != netparams.error)
            {
                wps_context.wps_in_progress = 0;
                set_power_mode(REC_POWER, WPS);
                A_MEMCPY(&wps_context.netparams, &netparams, sizeof(ATH_NETPARAMS));
                status = 0;
            }
        }
    }

    return status;
}

static int32_t allow_aggr(int32_t argc, char *argv[])
{
    ATH_SET_AGGREGATION_PARAM param;
    uint32_t enet_device;
    enet_device = get_active_device();

    do
    {
        if (argc != 2)
        {
            break;
        }

        param.txTIDMask = mystrtoul(argv[0], NULL, 16);
        param.rxTIDMask = mystrtoul(argv[1], NULL, 16);

        if (param.txTIDMask > 0xff || param.rxTIDMask > 0xff)
        {
            break;
        }

        if (A_OK != qcom_allow_aggr_set_tid(enet_device, param.txTIDMask, param.rxTIDMask))
        {
            PRINTF("ERROR: driver command failed" NL);
            return A_ERROR;
        }

        return A_OK;
    } while (0);

    PRINTF(
        "wmiconfig --allow_aggr <tx_tid_mask> <rx_tid_mask> Enables aggregation based on the provided bit mask "
        "where" NL);
    PRINTF("each bit represents a TID valid TID's are 0-7" NL);

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : test_promiscuous()
 * Returned Value  : A_OK - success
 * Comments	: Tests promiscuous mode
 *
 *END*-----------------------------------------------------------------*/
static int32_t test_promiscuous(int32_t argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT param;
    ATH_PROMISCUOUS_MODE prom_mode_details;

    param.cmd_id = ATH_SET_PROMISCUOUS_MODE;
    param.data   = &prom_mode_details;
    param.length = sizeof(prom_mode_details);

    A_MEMZERO(&prom_mode_details, sizeof(ATH_PROMISCUOUS_MODE));

    prom_mode_details.enable = atoi(argv[0]);
    /* Set to MAX PERF Mode */
    set_power_mode(MAX_PERF_POWER, PWR_MAX);

    if (argc > 1)
    {
        prom_mode_details.filter_flags = atoi(argv[1]);

        if (argc > 2)
        {
            if (mystrtomac(argv[2], &(prom_mode_details.src_mac[0])))
            {
                PRINTF("ERROR: MAC address translation failed." NL);
                return A_OK;
            }

            if (argc > 3)
            {
                if (mystrtomac(argv[3], &(prom_mode_details.dst_mac[0])))
                {
                    PRINTF("ERROR: MAC address translation failed." NL);
                    return A_OK;
                }
            }
        }
    }

    prom_mode_details.cb = application_frame_cb;
    /* call the driver */
    if (A_OK != HANDLE_IOCTL(&param))
    {
        PRINTF("ERROR: promiscuous command failed" NL);
    }

    return A_OK;
}

/* test_raw -- makes use of special API's to send a raw MAC frame irregardless of connection state
 */
static int32_t test_raw(int32_t argc, char *argv[])
{
    uint8_t rate_index;
    uint8_t tries;
    uint8_t header_type;
    uint32_t i;
    uint32_t chan, size;
    int32_t status = A_ERROR;
    uint8_t addr[4][6];
    QCOM_RAW_MODE_PARAM_t para;

    /* collect user inputs if any */

    if ((argc < 5) || (argc > 9))
    {
        goto raw_usage;
    }

    rate_index  = atoi(argv[0]);
    tries       = atoi(argv[1]);
    size        = atoi(argv[2]);
    chan        = atoi(argv[3]);
    header_type = atoi(argv[4]);
    A_MEMZERO(&addr[0][0], sizeof(addr));

    for (i = 0; i < (argc - 5); i++)
    {
        if (mystrtomac(argv[5 + i], &(addr[i][0])))
        {
            PRINTF("ERROR: MAC address translation failed." NL);
            return status;
        }
    }

    if (chan > 13)
    {
        goto raw_usage;
    }

    if (argc == 5)
    {
        addr[0][0] = 0xff;
        addr[0][1] = 0xff;
        addr[0][2] = 0xff;
        addr[0][3] = 0xff;
        addr[0][4] = 0xff;
        addr[0][5] = 0xff;
        addr[1][0] = 0x00;
        addr[1][1] = 0x03;
        addr[1][2] = 0x7f;
        addr[1][3] = 0xdd;
        addr[1][4] = 0xdd;
        addr[1][5] = 0xdd;
        addr[2][0] = 0x00;
        addr[2][1] = 0x03;
        addr[2][2] = 0x7f;
        addr[2][3] = 0xdd;
        addr[2][4] = 0xdd;
        addr[2][5] = 0xdd;
        addr[3][0] = 0x00;
        addr[3][1] = 0x03;
        addr[3][2] = 0x7f;
        addr[3][3] = 0xee;
        addr[3][4] = 0xee;
        addr[3][5] = 0xee;
        if (header_type == 1)
        {
            A_MEMCPY(&addr[0][0], &addr[1][0], ATH_MAC_LEN);
            // change destination address
            addr[2][3] = 0xaa;
            addr[2][4] = 0xaa;
            addr[2][5] = 0xaa;
        }
    }

    para.rate_index  = rate_index;
    para.tries       = tries;
    para.size        = size;
    para.chan        = chan;
    para.header_type = header_type;
    para.seq         = 0;
    A_MEMCPY(&para.addr1.addr[0], addr[0], ATH_MAC_LEN);
    A_MEMCPY(&para.addr2.addr[0], addr[1], ATH_MAC_LEN);
    A_MEMCPY(&para.addr3.addr[0], addr[2], ATH_MAC_LEN);
    A_MEMCPY(&para.addr4.addr[0], addr[3], ATH_MAC_LEN);
    para.pdatabuf = NULL;
    para.buflen   = 0;

    status = qcom_raw_mode_send_pkt(&para);
    if (status != A_OK)
    {
    raw_usage:
        PRINTF("raw input error" NL);
        PRINTF(
            "usage = wmiconfig --raw rate num_tries num_bytes channel header_type [addr1 [addr2 [addr3 [addr4]]]]" NL);
        PRINTF("rate = rate index where 0==1mbps; 1==2mbps; 2==5.5mbps etc" NL);
        PRINTF("num_tries = number of transmits 1 - 14" NL);
        PRINTF("num_bytes = payload size 0 to 1400" NL);
        PRINTF("channel = 0 - 11, 0: send on current channel" NL);
        PRINTF("header_type = 0==beacon frame; 1==QOS data frame; 2==4 address data frame" NL);
        PRINTF("addr1 = mac address xx:xx:xx:xx:xx:xx" NL);
        PRINTF("addr2 = mac address xx:xx:xx:xx:xx:xx" NL);
        PRINTF("addr3 = mac address xx:xx:xx:xx:xx:xx" NL);
        PRINTF("addr4 = mac address xx:xx:xx:xx:xx:xx" NL);
    }

    return status;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : wps_handler()
 * Returned Value  : 1 - successful completion or
 *					0 - failed.
 * Comments		  : Handles WPS related functionality
 *
 *END*-----------------------------------------------------------------*/
static int32_t wps_handler(int32_t index, int32_t argc, char *argv[])
{
    uint32_t i;
    uint8_t val;
    uint8_t wps_mode = 0;
    ATH_NETPARAMS netparams;
    char delimter[] = "-#";
    char *result    = NULL;
    qcom_wps_credentials_t wpsScan, *wpsScan_p = NULL;
    int j;
    uint32_t enet_device;
    enet_device = get_active_device();

    // init context
    wps_context.wps_in_progress = 0;
    wps_context.connect_flag    = 0;
    A_MEMZERO(&netparams, sizeof(ATH_NETPARAMS));
    A_MEMZERO(wpsPin, sizeof(wpsPin));
    if (index + 1 < argc)
    {
        i                        = index;
        wps_context.connect_flag = atoi(argv[i]);

        i++;
        if (ATH_STRCMP(argv[i], "push") == 0)
        {
            wps_mode = 1;
        }
        else if (ATH_STRCMP(argv[i], "pin") == 0)
        {
            wps_mode = 0;
            if (argc < 4) // User did not enter parameter
            {
                PRINTF("Missing parameters: please enter pin" NL);
                return A_ERROR;
            }

            i++;

            result = strtok(argv[i], delimter);
            if (result == NULL)
            {
                strncpy(wpsPin, argv[i], sizeof(wpsPin) - 1);
            }
            else
            {
                while (result != NULL)
                {
                    strcat(wpsPin, result);
                    result = strtok(NULL, delimter);
                }
            }
        }

        i++;

        if (argc > i)
        {
            if ((argc - i) < 3)
            {
                PRINTF("missing parameters" NL);
                return A_ERROR;
            }
            else
            {
                strncpy((char *)(&wpsScan.ssid), argv[i], sizeof(wpsScan.ssid));
                wpsScan.ssid_len = strlen(wpsScan.ssid.ssid);
                i++;

                if (strlen(argv[i]) != 12)
                {
                    PRINTF("Invalid MAC address length" NL);
                    return A_ERROR;
                }

                for (j = 0; j < 6; j++)
                {
                    wpsScan.mac_addr[j] = 0;
                }
                for (j = 0; j < strlen(argv[i]); j++)
                {
                    val = ascii_to_hex(argv[i][j]);
                    if (val == 0xff)
                    {
                        PRINTF("Invalid character" NL);
                        return A_ERROR;
                    }
                    else
                    {
                        if ((j & 1) == 0)
                            val <<= 4;

                        wpsScan.mac_addr[j >> 1] |= val;
                    }
                }

                i++;
                wpsScan.ap_channel = channel_array[atoi(argv[i])];
            }
            wpsScan_p = &wpsScan;
        }
        else
        {
            wpsScan_p = NULL;
        }
    }
    set_power_mode(MAX_PERF_POWER, WPS);

    qcom_wps_set_credentials(enet_device, wpsScan_p);
    if (qcom_wps_start(enet_device, wps_context.connect_flag, wps_mode, wpsPin) != A_OK)
    {
        PRINTF("WPS connect error" NL);
        set_power_mode(REC_POWER, WPS);
        return A_ERROR;
    }

    wps_context.wps_in_progress = 1;
    return A_OK;
}
#if ENABLE_P2P_MODE
#define isdigit(c) ((((c) >= '0') && ((c) <= '9')) ? (1) : (0))
/*
 * Input an Ethernet address and convert to binary.
 */

static A_STATUS wmic_ether_aton(const char *orig, uint8_t *eth)
{
    const char *bufp;
    int i;

    i = 0;
    for (bufp = orig; *bufp != '\0'; ++bufp)
    {
        unsigned int val;
        unsigned char c = *bufp++;
        if (isdigit(c))
            val = c - '0';
        else if (c >= 'a' && c <= 'f')
            val = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            val = c - 'A' + 10;
        else
            break;

        val <<= 4;
        c = *bufp++;
        if (isdigit(c))
            val |= c - '0';
        else if (c >= 'a' && c <= 'f')
            val |= c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            val |= c - 'A' + 10;
        else
            break;

        eth[i] = (unsigned char)(val & 0377);
        if (++i == ATH_MAC_LEN)
        {
            /* That's it.  Any trailing junk? */
            if (*bufp != '\0')
            {
                PRINTF("iw_ether_aton(%s): trailing junk!" NL, orig);
                return (A_EINVAL);
            }
            return (A_OK);
        }
        if (*bufp != ':')
            break;
    }
    return (A_EINVAL);
}

#endif

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : ath_assert_dump()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : causes assert information to be collected from chip and
 *				    printed to stdout.
 *
 *END*------------------------------------------------------------------------*/
static int32_t ath_assert_dump(int32_t argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT ath_param;
    int32_t error;

    ath_param.cmd_id = ATH_ASSERT_DUMP;
    ath_param.data   = NULL;
    ath_param.length = 0;

    error = HANDLE_IOCTL(&ath_param);
    return error;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : scan_control()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : Disables/Enables foreground and background scan operations
 *					in the Atheros device.  Both params must be provided where
 *					foreground param is first followed by background param. a
 *					'0' param disables the scan type while a '1' enables the
 *					scan type.  Background scan -- firmware occasionally scans
 *					while connected to a network. Foreground scan -- firmware
 *					occasionally scans while disconnected to a network.
 *
 *END*------------------------------------------------------------------------*/
static int32_t scan_control(int32_t argc, char *argv[])
{
    int32_t error;
    int32_t fg, bg;
    qcom_scan_params_t scanParam;
    uint32_t enet_device;

    do
    {
        if (argc != 2)
            break;
        fg = atoi(argv[0]);
        if (fg != 0 && fg != 1)
            break;
        bg = atoi(argv[1]);
        if (bg != 0 && bg != 1)
            break;

        if (fg == 1)
        {
            scanParam.fgStartPeriod = 0;
            scanParam.fgEndPeriod   = 0;
        }
        else
        {
            scanParam.fgStartPeriod = 0xffff;
            scanParam.fgEndPeriod   = 0xffff;
        }

        if (bg == 1)
        {
            scanParam.bgPeriod = 0;
        }
        else
        {
            scanParam.bgPeriod = 0xffff;
        }

        scanParam.maxActChDwellTimeInMs = 0;
        scanParam.pasChDwellTimeInMs    = 0;
        scanParam.shortScanRatio        = WMI_SHORTSCANRATIO_DEFAULT;
        scanParam.scanCtrlFlags         = DEFAULT_SCAN_CTRL_FLAGS;
        scanParam.minActChDwellTimeInMs = 0;
        scanParam.maxActScanPerSsid     = 0;
        scanParam.maxDfsChActTimeInMs   = 0;

        enet_device = get_active_device();
        error       = qcom_scan_params_set(enet_device, &scanParam);
        if (error == A_ERROR)
        {
            PRINTF("driver ioctl error" NL);
            return A_ERROR;
        }
        return A_OK;
    } while (0);

    PRINTF("param error: scan control requires 2 inputs [0|1] [0|1]" NL);
    return A_ERROR;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : set_scan_para()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : set scan parameters
 *
 *END*------------------------------------------------------------------------*/
static int32_t set_scan_para(int32_t argc, char *argv[])
{
    int32_t error;
    int32_t data;
    qcom_scan_params_t scan_params;
    uint32_t enet_device;

    do
    {
        if ((argc != 2) && (argc != 10))
            break;
        data                              = atoi(argv[0]);
        scan_params.maxActChDwellTimeInMs = (uint16_t)data;
        data                              = atoi(argv[1]);
        scan_params.pasChDwellTimeInMs    = (uint16_t)data;

        if (argc == 2)
        {
            scan_params.fgStartPeriod         = 0;
            scan_params.fgEndPeriod           = 0;
            scan_params.bgPeriod              = 0;
            scan_params.shortScanRatio        = WMI_SHORTSCANRATIO_DEFAULT;
            scan_params.scanCtrlFlags         = DEFAULT_SCAN_CTRL_FLAGS;
            scan_params.minActChDwellTimeInMs = 0;
            scan_params.maxActScanPerSsid     = 0;
            scan_params.maxDfsChActTimeInMs   = 0;
        }
        else
        {
            data                              = atoi(argv[2]);
            scan_params.fgStartPeriod         = (uint16_t)data;
            data                              = atoi(argv[3]);
            scan_params.fgEndPeriod           = (uint16_t)data;
            data                              = atoi(argv[4]);
            scan_params.bgPeriod              = (uint16_t)data;
            data                              = atoi(argv[5]);
            scan_params.shortScanRatio        = (uint8_t)data;
            data                              = atoi(argv[6]);
            scan_params.scanCtrlFlags         = (uint8_t)data;
            data                              = atoi(argv[7]);
            scan_params.minActChDwellTimeInMs = (uint16_t)data;
            data                              = atoi(argv[8]);
            scan_params.maxActScanPerSsid     = (uint16_t)data;
            data                              = atoi(argv[9]);
            scan_params.maxDfsChActTimeInMs   = (uint32_t)data;
        }

        enet_device = get_active_device();
        error       = qcom_scan_params_set(enet_device, &scan_params);
        if (error == A_ERROR)
        {
            PRINTF("driver ioctl error" NL);
            return A_ERROR;
        }
        return A_OK;
    } while (0);

    PRINTF(
        "wmiconfig --setscanpara <max_act_ch_dwell_time_ms> <pas_act_chan_dwell_time_ms> [<fg_start_period(in secs)> "
        "<fg_end_period (in secs)> <bg_period (in secs)> <short_scan_ratio> <scan_ctrl_flags>  "
        "<min_active_chan_dwell_time_ms> <max_act_scan_per_ssid> <max_dfs_ch_act_time_in_ms>]" NL);
    return A_ERROR;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : start_scan()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : set scan --- do real scan
 *
 *END*------------------------------------------------------------------------*/
static int32_t start_scan(int32_t argc, char *argv[])
{
    int32_t error;
    int32_t data;
    qcom_start_scan_params_t scan_params;
    uint16_t maxArgCount = 0, argI = 0;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;
    QCOM_SSID orig_ssid         = {0};

#if ENABLE_P2P_MODE
    if (p2p_session_in_progress)
    {
        PRINTF("p2p event in progress" NL);
        return A_ERROR;
    }
#endif

    enet_device = get_active_device();
    qcom_get_ssid(enet_device, &orig_ssid);
    if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
    {
        return A_ERROR;
    }

    if (wifimode == QCOM_WLAN_DEV_MODE_AP)
    {
        PRINTF("AP mode cannot scan" NL);
        return A_ERROR;
    }

    do
    {
        if (argc < 5)
            break;
        data                              = atoi(argv[0]);
        scan_params.forceFgScan           = (bool)data;
        data                              = atoi(argv[1]);
        scan_params.homeDwellTimeInMs     = (uint32_t)data;
        data                              = atoi(argv[2]);
        scan_params.forceScanIntervalInMs = (uint32_t)data;
        data                              = atoi(argv[3]);
        scan_params.scanType              = (uint8_t)data;
        data                              = atoi(argv[4]);
        scan_params.numChannels           = (uint8_t)data;
        if (scan_params.numChannels > QCOM_START_SCAN_PARAMS_CHANNEL_LIST_MAX)
        {
            PRINTF("cannot set more than 12 channels to scan" NL);
            break;
        }
        maxArgCount = scan_params.numChannels + 5;
        argI        = 5;
        if (argc != maxArgCount)
        {
            PRINTF(
                "wmiconfig --setscan <forceFgScan> <homeDwellTimeInMs> <forceScanIntervalInMs> <scanType> "
                "<numChannels> [<channelInMhz 1> <channelInMhz 2>...<channelInMhz N>]" NL);
            return A_ERROR;
        }
        while (argI < maxArgCount)
        {
            data                              = atoi(argv[argI]);
            scan_params.channelList[argI - 5] = (uint16_t)data;
            argI++;
        }

        QCOM_SSID tmp_ssid = QCOM_SSID_FROM_STR("");
        error              = qcom_set_ssid(enet_device, &tmp_ssid);
        if (error != A_OK)
        {
            PRINTF("Unable to set SSID" NL);
            return error;
        }

        /*Do the actual scan*/
        wmi_set_scan(enet_device, &scan_params);

        /*Revert to original SSID*/
        error = qcom_set_ssid(enet_device, &orig_ssid);
        if (error != A_OK)
        {
            PRINTF("Unable to set SSID" NL);
            return A_ERROR;
        }
        return A_OK;
    } while (0);
    PRINTF(
        "wmiconfig --setscan <forceFgScan> <homeDwellTimeInMs> <forceScanIntervalInMs> <scanType> <numChannels> "
        "[<channel> <channel>... upto numChannels]" NL);
    return A_ERROR;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : get_rate()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : gets TX rate from chip
 *
 *END*------------------------------------------------------------------------*/
int32_t get_rate(void)
{
    ATH_IOCTL_PARAM_STRUCT ath_param;
    int32_t error = A_ERROR;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    ath_param.cmd_id = ATH_GET_RATE;
    ath_param.data   = NULL;
    ath_param.length = 0;
    error            = HANDLE_IOCTL(&ath_param);

    if (error != A_OK)
    {
        PRINTF("Command Failed" NL);
    }
    return error;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : set_tx_power_scale()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : sets TX power scale
 *
 *END*------------------------------------------------------------------------*/
static int32_t set_tx_power_scale(int32_t index, int32_t argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT ath_param;
    int32_t error = A_ERROR;
    WMI_SET_TX_POWER_SCALE_CMD pwr_scale;

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if ((argc > 3) || (argc < 2))
    {
        PRINTF("incorrect arguments refer help" NL);
        return error;
    }

    if ((atoi(argv[index]) < -15) || (atoi(argv[index]) > 15))
    {
        PRINTF("power value to be from -15dbm to 15dbm" NL);
        return error;
    }

    pwr_scale.param_val = atoi(argv[index]);

    ath_param.cmd_id = ATH_SET_TX_PWR_SCALE;
    ath_param.data   = &pwr_scale;
    ath_param.length = sizeof(pwr_scale);
    error            = HANDLE_IOCTL(&ath_param);

    if (error != A_OK)
    {
        PRINTF("Command Failed" NL);
    }
    return error;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : set_rate()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : sets TX data rate
 *
 *END*------------------------------------------------------------------------*/
static int32_t set_rate(int32_t index, int32_t argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT ath_param;
    int32_t error = A_ERROR;
    WMI_BIT_RATE_CMD bitrate;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc > 4)
    {
        PRINTF("too many arguments refer help" NL);
        return error;
    }

    if ((argc == 3) && (atoi(argv[index]) > 0) && (atoi(argv[index]) <= 54))
    {
        switch (atoi(argv[index]))
        {
            case 1:
                bitrate.rateIndex = RATE_1Mb;
                break;
            case 2:
                bitrate.rateIndex = RATE_2Mb;
                break;
            case 5:
                bitrate.rateIndex = RATE_5_5Mb;
                break;
            case 11:
                bitrate.rateIndex = RATE_11Mb;
                break;
            case 6:
                bitrate.rateIndex = RATE_6Mb;
                break;
            case 9:
                bitrate.rateIndex = RATE_9Mb;
                break;
            case 12:
                bitrate.rateIndex = RATE_12Mb;
                break;
            case 18:
                bitrate.rateIndex = RATE_18Mb;
                break;
            case 24:
                bitrate.rateIndex = RATE_24Mb;
                break;
            case 36:
                bitrate.rateIndex = RATE_36Mb;
                break;
            case 48:
                bitrate.rateIndex = RATE_48Mb;
                break;
            case 54:
                bitrate.rateIndex = RATE_54Mb;
                break;
            default:
                PRINTF("undefined rate" NL);
                return error;
                // break;
        }
    }
    else
    {
        if ((argc == 3))
        {
            PRINTF("invalid command see help" NL);
            return error;
        }
    }

    if ((argc == 4) && (strcmp(argv[index], "mcs") == 0))
    {
        switch (atoi(argv[index + 1]))
        {
            case 0:
                bitrate.rateIndex = RATE_MCS_0_20;
                break;
            case 1:
                bitrate.rateIndex = RATE_MCS_1_20;
                break;
            case 2:
                bitrate.rateIndex = RATE_MCS_2_20;
                break;
            case 3:
                bitrate.rateIndex = RATE_MCS_3_20;
                break;
            case 4:
                bitrate.rateIndex = RATE_MCS_4_20;
                break;
            case 5:
                bitrate.rateIndex = RATE_MCS_5_20;
                break;
            case 6:
                bitrate.rateIndex = RATE_MCS_6_20;
                break;
            case 7:
                bitrate.rateIndex = RATE_MCS_7_20;
                break;
            default:
                PRINTF("undefined mcs rate" NL);
                return error;
                // break;
        }
    }
    else
    {
        if ((argc == 4))
        {
            PRINTF("invalid command see help" NL);
            return error;
        }
    }

    ath_param.cmd_id = ATH_SET_RATE;
    ath_param.data   = &bitrate;
    ath_param.length = sizeof(bitrate);
    error            = HANDLE_IOCTL(&ath_param);

    if (error != A_OK)
    {
        PRINTF("Command Failed" NL);
    }
    return error;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : get_tx_status()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : gets TX status from driver
 *
 *END*------------------------------------------------------------------------*/
int32_t get_tx_status(void)
{
    ATH_IOCTL_PARAM_STRUCT ath_param;
    int32_t error = A_ERROR;
    ATH_TX_STATUS result;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    ath_param.cmd_id = ATH_GET_TX_STATUS;
    ath_param.data   = &result;
    ath_param.length = 4;

    error = HANDLE_IOCTL(&ath_param);

    if (error == A_OK)
    {
        switch (result.status)
        {
            case ATH_TX_STATUS_IDLE:
                PRINTF("TX Idle" NL);
                break;
            case ATH_TX_STATUS_WIFI_PENDING:
                PRINTF("TX status WIFI Pending" NL);
                break;
            case ATH_TX_STATUS_HOST_PENDING:
                PRINTF("TX status Host Pending" NL);
                break;
            default:
                PRINTF("Invalid result" NL);
                return result.status;
        }
        return A_OK;
    }
    else
    {
        PRINTF("Command Failed" NL);
        return A_ERROR;
    }
}

#if ENABLE_P2P_MODE
/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : p2p_handler()
 * Returned Value  : 1 - successful completion or
 *                   0 - failed.
 * Comments        : Handles P2P related functionality
 *
 *END*-----------------------------------------------------------------*/
static int32_t p2p_handler(int32_t index, int32_t argc, char_ptr argv[])
{
#define P2P_STANDARD_TIMEOUT (300)
    union
    {
        WMI_P2P_SET_CMD p2p_set_params;
        WMI_P2P_FIND_CMD find_params;
        WMI_P2P_FW_CONNECT_CMD_STRUCT p2p_connect;
        WMI_P2P_FW_PROV_DISC_REQ_CMD p2p_prov_disc;
        WMI_P2P_PROV_INFO p2p_info;
        WMI_SET_PASSPHRASE_CMD setPassPhrase;
        WMI_P2P_GRP_INIT_CMD grpInit;
        WMI_P2P_SET_PROFILE_CMD p2p;
        WMI_P2P_FW_SET_CONFIG_CMD p2p_set_config;
        WMI_P2P_INVITE_CMD p2pInvite;
        WMI_P2P_FW_INVITE_REQ_RSP_CMD invite_req_rsp;
        P2P_NOA_DESCRIPTOR p2p_desc;
        WMI_OPPPS_INFO_STRUCT p2p_oppps;
    } stack_p2p;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;

    enet_device          = get_active_device();
    uint32_t timeout_val = 0;
    uint8_t p2p_invite_role;
    uint32_t error = SHELL_EXIT_SUCCESS, i, k, len = 0; //, val;
    i = index + 1;
    set_callback();

    if (enet_device != 0)
    {
        PRINTF("dev: %d for p2p mode switch to dev 0" NL, enet_device);
        return SHELL_EXIT_ERROR;
    }
    if (index <= argc)
    {
        if (ATH_STRCMP(argv[index], "join") == 0)
        {
            if (argc < 5)
            {
                PRINTF(NL "Incomplete parameter list for p2p join" NL);
                PRINTF(NL "Usage : wmiconfig --p2p join <intf_mac_addr> <push | keypad | display> <* wpspin>" NL);
                PRINTF("           * Only for PIN Mode" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
#if 0
                /* Set P2P Mode to P2P Client */
                stack_p2p.p2p_set_params.val.mode.p2pmode = P2P_CLIENT;
                if( qcom_p2p_set(enet_device, P2P_CONFIG_P2P_OPMODE, &stack_p2p.p2p_set_params.val, sizof(stack_p2p.p2p_set_params.val.mode)) != A_OK )
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
#endif
                /* Send the join profile to fw */
                if ((wmic_ether_aton(argv[i], p2p_join_mac_addr)) != A_OK)
                {
                    PRINTF(NL "Invalid PEER MAC Address" NL);
                    return SHELL_EXIT_ERROR;
                }
                if (qcom_p2p_func_join_profile(enet_device, p2p_join_mac_addr) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }

                PRINTF(NL "Interface MAC addr : %x:%x:%x:%x:%x:%x" NL, p2p_join_mac_addr[0], p2p_join_mac_addr[1],
                       p2p_join_mac_addr[2], p2p_join_mac_addr[3], p2p_join_mac_addr[4], p2p_join_mac_addr[5]);

                /* Send Join request */
                A_MEMZERO(&p2p_join_profile, sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT));
                if (qcom_get_bssid(enet_device, (uint8_t *)p2p_join_profile.own_interface_addr) != SHELL_EXIT_SUCCESS)
                {
                    PRINTF(NL "unable to obtain device mac address" NL);
                    return SHELL_EXIT_ERROR;
                }

                PRINTF(NL "own MAC addr : %x:%x:%x:%x:%x:%x" NL, p2p_join_profile.own_interface_addr[0],
                       p2p_join_profile.own_interface_addr[1], p2p_join_profile.own_interface_addr[2],
                       p2p_join_profile.own_interface_addr[3], p2p_join_profile.own_interface_addr[4],
                       p2p_join_profile.own_interface_addr[5]);

                if (ATH_STRCMP(argv[i + 1], "push") == 0)
                {
                    p2p_join_profile.wps_method = WPS_PBC;
                }
                else if (ATH_STRCMP(argv[i + 1], "display") == 0)
                {
                    p2p_join_profile.wps_method = WPS_PIN_DISPLAY;
                }
                else if (ATH_STRCMP(argv[i + 1], "keypad") == 0)
                {
                    p2p_join_profile.wps_method = WPS_PIN_KEYPAD;
                }
#if 1
                if (p2p_join_profile.wps_method == WPS_PIN_DISPLAY || p2p_join_profile.wps_method == WPS_PIN_KEYPAD)
                {
                    strncpy(p2p_wps_pin, argv[i + 2], sizeof(p2p_wps_pin) - 1);
                    memcpy(stack_p2p.p2p_info.wps_pin, p2p_wps_pin, sizeof(stack_p2p.p2p_info.wps_pin));
                    wmi_save_key_info(&stack_p2p.p2p_info);
                }
#endif
                p2p_join_session_active = 1;
                if (qcom_p2p_func_get_node_list(enet_device, NULL, 0) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
                A_MDELAY(100);
                p2p_callback(0);
            }
        }
        else if (ATH_STRCMP(argv[index], "invite") == 0)
        {
            if (argc < 6)
            {
                PRINTF("Incomplete parameter list" NL);
                PRINTF("Usage: wmiconfig --p2p invite <SSID> <MACADDR> <WPS_METHOD> <persistent>" NL);
                return SHELL_EXIT_ERROR;
            }

            A_MEMZERO(&stack_p2p.p2pInvite, sizeof(WMI_P2P_INVITE_CMD));

            for (k = 0; k < MAX_LIST_COUNT; k++)
            {
                if (strncmp((char *)p2p_pers_data[k].ssid, argv[i], strlen(argv[i])) == 0)
                {
                    break;
                }
            }
            if (k == MAX_LIST_COUNT)
            {
                // PRINTF("the SSID and MAC is not a persistent one" NL);
                if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
                {
                    return A_ERROR;
                }
                if ((wifimode == QCOM_WLAN_DEV_MODE_STATION) && (wifi_connected_flag == 1))
                {
                    p2p_invite_role                   = WMI_P2P_INVITE_ROLE_CLIENT;
                    stack_p2p.p2pInvite.is_persistent = 0;
                }
                k = 0;
            }
            else
            {
                invitation_index                  = k;
                p2p_invite_role                   = p2p_pers_data[k].role;
                stack_p2p.p2pInvite.is_persistent = 1;
            }

            if ((wmic_ether_aton(argv[i + 1], stack_p2p.p2pInvite.peer_addr)) != A_OK)
            {
                PRINTF(NL "Invalid Invitation MAC Address" NL);
                return SHELL_EXIT_ERROR;
            }

            if (ATH_STRCMP(argv[i + 2], "push") == 0)
            {
                stack_p2p.p2pInvite.wps_method = WPS_PBC;
            }
            else if (ATH_STRCMP(argv[i + 2], "display") == 0)
            {
                stack_p2p.p2pInvite.wps_method = WPS_PIN_KEYPAD;
            }
            else if (ATH_STRCMP(argv[i + 2], "keypad") == 0)
            {
                stack_p2p.p2pInvite.wps_method = WPS_PIN_DISPLAY;
            }
            else
            {
                PRINTF("wps method not proper P2P Invite" NL);
                return SHELL_EXIT_ERROR;
            }

            if (qcom_p2p_func_invite(enet_device, argv[i], (P2P_WPS_METHOD)stack_p2p.p2pInvite.wps_method,
                                     stack_p2p.p2pInvite.peer_addr, stack_p2p.p2pInvite.is_persistent,
                                     (P2P_INV_ROLE)p2p_invite_role) != A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return SHELL_EXIT_ERROR;
            }
#if 1
            if (p2p_invite_role == WMI_P2P_INVITE_ROLE_ACTIVE_GO)
            {
                error = qcom_sec_set_encrypt_mode(enet_device, WLAN_CRYPT_AES_CRYPT);

                if (error != A_OK)
                {
                    return A_ERROR;
                }

                error = qcom_sec_set_auth_mode(enet_device, WLAN_AUTH_WPA2_PSK);

                if (error != A_OK)
                {
                    return A_ERROR;
                }

                QCOM_PASSPHRASE tmp_pass = {0};
                QCOM_SSID tmp_ssid       = {0};
                strncpy((char *)&tmp_pass, (char *)p2p_pers_data[k].passphrase, sizeof(tmp_pass));
                strncpy((char *)&tmp_ssid, (char *)p2p_pers_data[k].ssid, sizeof(tmp_ssid));
                if (qcom_p2p_func_set_pass_ssid(enet_device, &tmp_pass, &tmp_ssid) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
#endif
        }
        else if (ATH_STRCMP(argv[index], "find") == 0)
        {
            if (ATH_STRCMP(argv[i], "help") == 0)
            {
                PRINTF(NL "Usage : wmiconfig --p2p find <* type> <* timeout>" NL);
                PRINTF(NL "* - optional" NL);
                return SHELL_EXIT_ERROR;
            }
            A_MEMZERO(&stack_p2p.find_params, sizeof(WMI_P2P_FIND_CMD));
            if (argc == 4)
            {
                if (ATH_STRCMP(argv[i], "1") == 0)
                {
                    stack_p2p.find_params.type    = (WMI_P2P_FIND_START_WITH_FULL);
                    stack_p2p.find_params.timeout = (P2P_STANDARD_TIMEOUT);
                }
                else if (ATH_STRCMP(argv[i], "2") == 0)
                {
                    stack_p2p.find_params.type    = (WMI_P2P_FIND_ONLY_SOCIAL);
                    stack_p2p.find_params.timeout = (P2P_STANDARD_TIMEOUT);
                }
                else if (ATH_STRCMP(argv[i], "3") == 0)
                {
                    stack_p2p.find_params.type    = (WMI_P2P_FIND_PROGRESSIVE);
                    stack_p2p.find_params.timeout = (P2P_STANDARD_TIMEOUT);
                }
                else
                {
                    PRINTF(NL "wrong option enter option 1,2 or 3" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
            else if (argc == 5)
            {
                if (ATH_STRCMP(argv[i], "1") == 0)
                {
                    stack_p2p.find_params.type    = (WMI_P2P_FIND_START_WITH_FULL);
                    stack_p2p.find_params.timeout = (atoi(argv[i + 1]));
                }
                else if (ATH_STRCMP(argv[i], "2") == 0)
                {
                    stack_p2p.find_params.type    = (WMI_P2P_FIND_ONLY_SOCIAL);
                    stack_p2p.find_params.timeout = (atoi(argv[i + 1]));
                }
                else if (ATH_STRCMP(argv[i], "3") == 0)
                {
                    stack_p2p.find_params.type    = (WMI_P2P_FIND_PROGRESSIVE);
                    stack_p2p.find_params.timeout = (atoi(argv[i + 1]));
                }
                else
                {
                    PRINTF(NL "wrong option enter option 1,2 or 3" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
            else
            {
                stack_p2p.find_params.type    = (WMI_P2P_FIND_ONLY_SOCIAL);
                stack_p2p.find_params.timeout = (P2P_STANDARD_TIMEOUT);
            }

            if (qcom_p2p_func_find(enet_device, NULL, stack_p2p.find_params.type, stack_p2p.find_params.timeout) !=
                A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return SHELL_EXIT_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[index], "list_network") == 0)
        {
            if (qcom_p2p_func_get_network_list(enet_device, NULL, sizeof(WPS_CREDENTIAL)) != A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return SHELL_EXIT_ERROR;
            }

            p2p_callback(0);
        }
        else if (ATH_STRCMP(argv[index], "serv_disc") == 0)
        {
            p2p_callback(1);
        }
        else if (ATH_STRCMP(argv[index], "set") == 0)
        {
            A_MEMZERO(&stack_p2p.p2p_set_params, sizeof(WMI_P2P_SET_CMD));
            if (ATH_STRCMP(argv[i], "p2pmode") == 0)
            {
                stack_p2p.p2p_set_params.config_id = P2P_CONFIG_P2P_OPMODE;
                if (ATH_STRCMP(argv[i + 1], "p2pdev") == 0)
                {
                    stack_p2p.p2p_set_params.val.mode.p2pmode = P2P_DEV;
                }
                else if (ATH_STRCMP(argv[i + 1], "p2pclient") == 0)
                {
                    stack_p2p.p2p_set_params.val.mode.p2pmode = P2P_CLIENT;
                }
                else if (ATH_STRCMP(argv[i + 1], "p2pgo") == 0)
                {
                    stack_p2p.p2p_set_params.val.mode.p2pmode = P2P_GO;
                }
                else
                {
                    PRINTF("Input can be \"p2pdev/p2pclient/p2pgo\"");
                }
                len = sizeof(stack_p2p.p2p_set_params.val.mode);
                PRINTF("p2p mode :%x, Config Id = %d" NL, stack_p2p.p2p_set_params.val.mode.p2pmode,
                       stack_p2p.p2p_set_params.config_id);
            }
            else if (ATH_STRCMP(argv[i], "postfix") == 0)
            {
                stack_p2p.p2p_set_params.config_id = P2P_CONFIG_SSID_POSTFIX;
                if (strlen(argv[i + 1]))
                {
                    A_MEMCPY(stack_p2p.p2p_set_params.val.ssid_postfix.ssid_postfix, argv[i + 1], strlen(argv[i + 1]));
                    stack_p2p.p2p_set_params.val.ssid_postfix.ssid_postfix_len = strlen(argv[i + 1]);
                    len = sizeof(stack_p2p.p2p_set_params.val.ssid_postfix);
                    PRINTF("PostFix string %s, Len %d" NL, stack_p2p.p2p_set_params.val.ssid_postfix.ssid_postfix,
                           stack_p2p.p2p_set_params.val.ssid_postfix.ssid_postfix_len);
                }
            }
            else if (ATH_STRCMP(argv[i], "intrabss") == 0)
            {
                stack_p2p.p2p_set_params.config_id          = P2P_CONFIG_INTRA_BSS;
                stack_p2p.p2p_set_params.val.intra_bss.flag = atoi(argv[i + 1]);
                len                                         = sizeof(stack_p2p.p2p_set_params.val.intra_bss);
            }
            else if (ATH_STRCMP(argv[i], "gointent") == 0)
            {
                stack_p2p.p2p_set_params.config_id           = P2P_CONFIG_GO_INTENT;
                stack_p2p.p2p_set_params.val.go_intent.value = atoi(argv[i + 1]);
                len                                          = sizeof(stack_p2p.p2p_set_params.val.go_intent);
            }
            else if (ATH_STRCMP(argv[i], "cckrates") == 0)
            {
                stack_p2p.p2p_set_params.config_id            = P2P_CONFIG_CCK_RATES;
                stack_p2p.p2p_set_params.val.cck_rates.enable = atoi(argv[i + 1]);
                len                                           = sizeof(stack_p2p.p2p_set_params.val.cck_rates);
            }
            if (qcom_p2p_set(enet_device, (P2P_CONF_ID)stack_p2p.p2p_set_params.config_id,
                             &stack_p2p.p2p_set_params.val, len) != A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return SHELL_EXIT_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[index], "cancel") == 0)
        {
            if (argc > 3)
            {
                PRINTF(NL "Usage : wmiconfig --p2p cancel" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
                if (qcom_p2p_func_cancel(enet_device) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
                autogo_newpp = false;

                p2p_cancel_enable = 1;

                if (qcom_p2p_func_get_network_list(enet_device, NULL, sizeof(WPS_CREDENTIAL)) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
                p2p_callback(0);
            }
            /* Reset to mode station */
            qcom_op_set_mode(enet_device, QCOM_WLAN_DEV_MODE_STATION);
            p2p_persistent_done = 0;
            set_power_mode(REC_POWER, P2P);
        }
        else if (ATH_STRCMP(argv[index], "prov") == 0)
        {
            if (argc < 4 || (ATH_STRCMP(argv[i], "help") == 0))
            {
                PRINTF(NL "Incomplete parameter list for p2p prov discovery" NL);
                PRINTF(NL "Usage : wmiconfig --p2p prov <peer_mac_addr> <wps_method>" NL);
                return SHELL_EXIT_ERROR;
            }
            else // if((argc >= 5) && (argc < 7))
            {
                if (ATH_STRCMP(argv[i + 1], "push") == 0)
                {
                    rand();
                    stack_p2p.p2p_prov_disc.wps_method = A_CPU2LE16((uint16_t)WPS_CONFIG_PUSHBUTTON);
                }
                else if (ATH_STRCMP(argv[i + 1], "display") == 0)
                {
                    stack_p2p.p2p_prov_disc.wps_method = A_CPU2LE16((uint16_t)WPS_CONFIG_DISPLAY);
                }
                else if (ATH_STRCMP(argv[i + 1], "keypad") == 0)
                {
                    stack_p2p.p2p_prov_disc.wps_method = A_CPU2LE16((uint16_t)WPS_CONFIG_KEYPAD);
                }
                stack_p2p.p2p_prov_disc.dialog_token = 1;

                if ((wmic_ether_aton(argv[i], stack_p2p.p2p_prov_disc.peer)) != A_OK)
                {
                    PRINTF(NL "Invalid PEER MAC Address" NL);
                    return SHELL_EXIT_ERROR;
                }

                if (qcom_p2p_func_prov(enet_device, (P2P_WPS_METHOD)stack_p2p.p2p_prov_disc.wps_method,
                                       stack_p2p.p2p_prov_disc.peer) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
                p2p_callback(0);

                /* Authorize P2P Device */
                A_MEMZERO(&stack_p2p.p2p_connect, sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT));
                if (ATH_STRCMP(argv[i + 1], "push") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = P2P_WPS_PBC;
                }
                else if (ATH_STRCMP(argv[i + 1], "display") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = P2P_WPS_NOT_READY;
                }
                else if (ATH_STRCMP(argv[i + 1], "keypad") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = P2P_WPS_PIN_DISPLAY;
                }

                if ((wmic_ether_aton(argv[i], stack_p2p.p2p_connect.peer_addr)) != A_OK)
                {
                    PRINTF(NL "Invalid PEER MAC Address" NL);
                    return SHELL_EXIT_ERROR;
                }

                stack_p2p.p2p_connect.go_intent = p2p_intent;

                if (stack_p2p.p2p_connect.wps_method != P2P_WPS_NOT_READY)
                {
                    if (qcom_p2p_func_auth(enet_device, stack_p2p.p2p_connect.dev_auth,
                                           (P2P_WPS_METHOD)stack_p2p.p2p_connect.wps_method,
                                           stack_p2p.p2p_connect.peer_addr,
                                           stack_p2p.p2p_connect.dev_capab & P2P_PERSISTENT_FLAG ? 1 : 0) != A_OK)
                    {
                        PRINTF(NL "StartP2P command did not execute properly" NL);
                        return SHELL_EXIT_ERROR;
                    }
                }
            }
        }
        else if (ATH_STRCMP(argv[index], "auth") == 0)
        {
            A_MEMZERO(&stack_p2p.p2p_connect, sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT));
            if (argc < 5)
            {
                PRINTF(NL "Incomplete parameter list for p2p auth" NL);
                PRINTF(NL "Usage : wmiconfig --p2p auth <peer_mac_addr> <wps_method> <* wps_pin> [persistent]" NL);
                return SHELL_EXIT_ERROR;
            }
            else // if((argc >= 5) && (argc < 7))
            {
                if (strlen(argv[i + 2]) == 8)
                {
                    strncpy(p2p_wps_pin, argv[i + 2], sizeof(p2p_wps_pin) - 1);
                    memcpy(stack_p2p.p2p_info.wps_pin, p2p_wps_pin, sizeof(stack_p2p.p2p_info.wps_pin));
                    wmi_save_key_info(&stack_p2p.p2p_info);
                    PRINTF("WPS Pin %s" NL, p2p_wps_pin);
                    if (ATH_STRCMP(argv[i + 3], "persistent") == 0)
                    {
                        stack_p2p.p2p_connect.dev_capab |= P2P_PERSISTENT_FLAG;
                    }
                }
                if (ATH_STRCMP(argv[i + 1], "deauth") == 0)
                {
                    stack_p2p.p2p_connect.dev_auth = 1;
                }
                if (ATH_STRCMP(argv[i + 1], "push") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = WPS_PBC;
                    if (ATH_STRCMP(argv[i + 2], "persistent") == 0)
                    {
                        PRINTF("persistent" NL);
                        stack_p2p.p2p_connect.dev_capab |= P2P_PERSISTENT_FLAG;
                    }
                }
                else if (ATH_STRCMP(argv[i + 1], "display") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = WPS_PIN_DISPLAY;
                    PRINTF("WPS PIN %s" NL, p2p_wps_pin);
                }
                else if (ATH_STRCMP(argv[i + 1], "keypad") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = WPS_PIN_KEYPAD;
                }

                if ((wmic_ether_aton(argv[i], stack_p2p.p2p_connect.peer_addr)) != A_OK)
                {
                    PRINTF(NL "Invalid PEER MAC Address" NL);
                    return SHELL_EXIT_ERROR;
                }

                /* Save P2P persistent flag as it will be needed while starting
               the group capability in beacons if role is P2P GO  */
                if (stack_p2p.p2p_connect.dev_capab & P2P_PERSISTENT_FLAG)
                {
                    p2p_persistent_go = 1;
                }

                if (qcom_p2p_func_auth(enet_device, stack_p2p.p2p_connect.dev_auth,
                                       (P2P_WPS_METHOD)stack_p2p.p2p_connect.wps_method,
                                       stack_p2p.p2p_connect.peer_addr,
                                       stack_p2p.p2p_connect.dev_capab & P2P_PERSISTENT_FLAG ? 1 : 0) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    p2p_persistent_go = 0;
                    return SHELL_EXIT_ERROR;
                }
            }

            task2_msec = 50;
        }
        else if (ATH_STRCMP(argv[index], "stop") == 0)
        {
            if (qcom_p2p_func_stop_find(enet_device) != A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return SHELL_EXIT_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[index], "nodelist") == 0)
        {
            if (qcom_p2p_func_get_node_list(enet_device, NULL, 0) != A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return SHELL_EXIT_ERROR;
            }
            A_MDELAY(100);
            p2p_callback(0);
        }
        else if (ATH_STRCMP(argv[index], "listen") == 0)
        {
            if (argc == 4)
            {
                timeout_val = atoi(argv[i]);
            }
            else
            {
                timeout_val = P2P_STANDARD_TIMEOUT;
            }
            if (qcom_p2p_func_listen(enet_device, timeout_val) != A_OK)
            {
                PRINTF(NL "StartP2P command did not execute properly" NL);
                return SHELL_EXIT_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[index], "connect") == 0)
        {
            if (argc < 5)
            {
                PRINTF(NL "Incomplete parameter list for p2p connect" NL);
                PRINTF(NL "Usage : wmiconfig --p2p connect <peer_mac_addr> <wps_method> [persistent]" NL);
                return SHELL_EXIT_ERROR;
            }
            else // if ((argc >= 6) && (argc < 8))
            {
                A_MEMZERO(&stack_p2p.p2p_info, sizeof(WMI_P2P_PROV_INFO));
                A_MEMZERO(&stack_p2p.p2p_connect, sizeof(WMI_P2P_FW_CONNECT_CMD_STRUCT));

                if (strlen(argv[i + 2]) == 8)
                {
                    strncpy(p2p_wps_pin, argv[i + 2], sizeof(p2p_wps_pin) - 1);
                    memcpy(stack_p2p.p2p_info.wps_pin, p2p_wps_pin, sizeof(stack_p2p.p2p_info.wps_pin));
                    wmi_save_key_info(&stack_p2p.p2p_info);
                    PRINTF("WPS Pin %s" NL, p2p_wps_pin);

                    if (ATH_STRCMP(argv[i + 3], "persistent") == 0)
                    {
                        stack_p2p.p2p_connect.dev_capab |= P2P_PERSISTENT_FLAG;
                    }
                }

                if (qcom_get_bssid(enet_device, (uint8_t *)stack_p2p.p2p_connect.own_interface_addr) != A_OK)
                {
                    PRINTF(NL "unable to obtain device mac address" NL);
                    return SHELL_EXIT_ERROR;
                }
                PRINTF(NL "own MAC addr : %x:%x:%x:%x:%x:%x" NL, stack_p2p.p2p_connect.own_interface_addr[0],
                       stack_p2p.p2p_connect.own_interface_addr[1], stack_p2p.p2p_connect.own_interface_addr[2],
                       stack_p2p.p2p_connect.own_interface_addr[3], stack_p2p.p2p_connect.own_interface_addr[4],
                       stack_p2p.p2p_connect.own_interface_addr[5]);

                if ((wmic_ether_aton(argv[i], stack_p2p.p2p_connect.peer_addr)) != A_OK)
                {
                    PRINTF(NL "Invalid PEER MAC Address" NL);
                    return SHELL_EXIT_ERROR;
                }
                PRINTF(NL "Peer MAC addr : %x:%x:%x:%x:%x:%x" NL, stack_p2p.p2p_connect.peer_addr[0],
                       stack_p2p.p2p_connect.peer_addr[1], stack_p2p.p2p_connect.peer_addr[2],
                       stack_p2p.p2p_connect.peer_addr[3], stack_p2p.p2p_connect.peer_addr[4],
                       stack_p2p.p2p_connect.peer_addr[5]);
                if (ATH_STRCMP(argv[i + 1], "push") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = P2P_WPS_PBC;
                    if (ATH_STRCMP(argv[i + 2], "persistent") == 0)
                    {
                        PRINTF("persistent" NL);
                        stack_p2p.p2p_connect.dev_capab |= P2P_PERSISTENT_FLAG;
                    }
                }
                else if (ATH_STRCMP(argv[i + 1], "display") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = P2P_WPS_PIN_DISPLAY;
                    PRINTF("WPS PIN %s" NL, p2p_wps_pin);
                }
                else if (ATH_STRCMP(argv[i + 1], "keypad") == 0)
                {
                    stack_p2p.p2p_connect.wps_method = P2P_WPS_PIN_KEYPAD;
                    // strcpy(p2p_wps_pin_val, argv[i+2]);
                }
#if 0
                PRINTF(NL "go_intent : %d" NL, atoi(argv[i+2]));
#endif

                /* Save P2P persistent flag as it will be needed while starting
               the group capability in beacons if role is P2P GO  */
                if (stack_p2p.p2p_connect.dev_capab & P2P_PERSISTENT_FLAG)
                {
                    p2p_persistent_go = 1;
                }

                if (qcom_p2p_func_connect(enet_device, (P2P_WPS_METHOD)stack_p2p.p2p_connect.wps_method,
                                          stack_p2p.p2p_connect.peer_addr,
                                          stack_p2p.p2p_connect.dev_capab & P2P_PERSISTENT_FLAG ? 1 : 0) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    p2p_persistent_go = 0;
                    return SHELL_EXIT_ERROR;
                }
                p2p_callback(0);
            }
        }
        else if (ATH_STRCMP(argv[index], "autogo") == 0)
        {
            if (ATH_STRCMP(argv[i], "help") == 0)
            {
                PRINTF(NL "Usage : wmiconfig --p2p autogo [persistent]" NL);
                return SHELL_EXIT_ERROR;
            }
            p2pMode  = TRUE;
            wps_flag = 0x01;

            PRINTF("Starting Autonomous GO" NL);
            if (ATH_STRCMP(argv[i], "persistent") == 0)
            {
                stack_p2p.grpInit.persistent_group = 1;
            }
            else
            {
                stack_p2p.grpInit.persistent_group = 0;
            }

            if (false == autogo_newpp)
            {
                QCOM_SSID tmp_ssid = QCOM_SSID_FROM_STR("DIRECT-iO");
                if (qcom_p2p_func_start_go(enet_device, &tmp_ssid, &wpa_passphrase, P2P_DEFAULT_CHAN,
                                           stack_p2p.grpInit.persistent_group) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
            else
            {
                if (qcom_p2p_func_start_go(enet_device, NULL, NULL, P2P_DEFAULT_CHAN,
                                           stack_p2p.grpInit.persistent_group) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
        }
        else if ((ATH_STRCMP(argv[index], "on") == 0))
        {
            if (argc > 3)
            {
                PRINTF(NL "Usage : wmiconfig --p2p <on / off>" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
                if (qcom_p2p_func_init(enet_device, TRUE) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }

                p2p_intent = 0;
                A_MEMCPY(set_channel_p2p, "6", 1);
                A_MEMZERO(&stack_p2p.p2p_info, sizeof(WMI_P2P_PROV_INFO));
                strncpy(p2p_wps_pin, "12345670", sizeof(p2p_wps_pin) - 1);
                memcpy(stack_p2p.p2p_info.wps_pin, p2p_wps_pin, sizeof(stack_p2p.p2p_info.wps_pin));
                wmi_save_key_info(&stack_p2p.p2p_info);

                /* Autonomous GO configurations */
                set_passphrase("1234567890");

                p2pMode = TRUE;
            }
        }
        else if ((ATH_STRCMP(argv[index], "off") == 0))
        {
            if (argc > 3)
            {
                PRINTF(NL "Usage : wmiconfig --p2p <on / off>" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
                if (qcom_p2p_func_init(enet_device, FALSE) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
            qcom_op_set_mode(enet_device, QCOM_WLAN_DEV_MODE_STATION);
            p2pMode = FALSE;
        }
        else if (ATH_STRCMP(argv[index], "setopclass") == 0)
        {
            if (argc < 6)
            {
                PRINTF(NL "Usage : wmiconfig --p2p setopclass <go_intent> <oper_regclass> <oper_regchannel>" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
                p2p_intent = atoi(argv[i]);
                A_MEMCPY(set_channel_p2p, argv[i + 2], sizeof(set_channel_p2p)); // for autogo
                if (qcom_p2p_func_set_config(enet_device, p2p_intent, 6, atoi(argv[i + 2]), 3000, 81, atoi(argv[i + 1]),
                                             4) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
        }
        else if (ATH_STRCMP(argv[index], "setconfig") == 0)
        {
            if (argc < 8)
            {
                PRINTF(NL "Incomplete parameter list for p2p set config" NL);
                PRINTF(NL "Usage : wmiconfig --p2p setconfig <go_intent> <listen_channel>" NL);
                PRINTF("                                  <oper_channel> <country> <node_age_timeout>" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
                PRINTF(NL "Note: Cannot set country code. Use board data file or tuneables instead" NL);
                p2p_intent = atoi(argv[i]);
                A_MEMCPY(set_channel_p2p, argv[i + 2], sizeof(set_channel_p2p)); // for autogo

                if (qcom_p2p_func_set_config(enet_device, p2p_intent, atoi(argv[i + 1]), atoi(argv[i + 2]),
                                             atoi(argv[i + 4]), 81, 81, 5) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
        }
        else if (ATH_STRCMP(argv[index], "passphrase") == 0)
        {
            if (argc < 5)
            {
                PRINTF(NL "Incomplete parameter list for p2p set config" NL);
                PRINTF(NL "Usage : wmiconfig --p2p passphrase <passphrase> <SSID>" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
                error = qcom_sec_set_encrypt_mode(enet_device, WLAN_CRYPT_AES_CRYPT);
                if (error != A_OK)
                {
                    return A_ERROR;
                }

                error = qcom_sec_set_auth_mode(enet_device, WLAN_AUTH_WPA2_PSK);
                if (error != A_OK)
                {
                    return A_ERROR;
                }

                autogo_newpp             = true;
                QCOM_PASSPHRASE tmp_pass = {0};
                QCOM_SSID tmp_ssid       = {0};
                strncpy((char *)&tmp_pass, argv[index + 1], sizeof(tmp_pass));
                strncpy((char *)&tmp_ssid, argv[index + 2], sizeof(tmp_ssid));
                if (qcom_p2p_func_set_pass_ssid(enet_device, &tmp_pass, &tmp_ssid) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
        }
        else if (ATH_STRCMP(argv[index], "setoppps") == 0)
        {
            if (argc < 5)
            {
                PRINTF(NL "Incomplete parameter list for p2p set oppps" NL);
                PRINTF(NL "Usage : wmiconfig --p2p setoppps <enable> <ctwin>" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
                if (qcom_p2p_func_set_oppps(enet_device, atoi(argv[i]), atoi(argv[i + 1])) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
        }
        else if (ATH_STRCMP(argv[index], "setnoa") == 0)
        {
            if (argc < 4)
            {
                PRINTF(NL "Incomplete parameter list for p2p set noa" NL);
                PRINTF(NL "Usage : wmiconfig --p2p setnoa  <count [1 - 255]> <start in ms>" NL);
                PRINTF("                                <duration in ms> <interval in ms>" NL);
                return SHELL_EXIT_ERROR;
            }
            else
            {
                stack_p2p.p2p_desc.count_or_type   = atoi(argv[i++]);
                stack_p2p.p2p_desc.start_or_offset = A_CPU2LE32(atoi(argv[i++]) * 1000);
                stack_p2p.p2p_desc.duration        = A_CPU2LE32(atoi(argv[i++]) * 1000);
                stack_p2p.p2p_desc.interval        = A_CPU2LE32(atoi(argv[i++]) * 1000);
                if (qcom_p2p_func_set_noa(enet_device, stack_p2p.p2p_desc.count_or_type,
                                          stack_p2p.p2p_desc.start_or_offset, stack_p2p.p2p_desc.duration,
                                          stack_p2p.p2p_desc.interval) != A_OK)
                {
                    PRINTF(NL "StartP2P command did not execute properly" NL);
                    return SHELL_EXIT_ERROR;
                }
            }
        }
        else
        {
            PRINTF(NL "This option is currently not supported" NL);
            return SHELL_EXIT_ERROR;
        }
    }
    else
    {
        PRINTF(NL "Incorrect Incomplete parameter list" NL);
        return SHELL_EXIT_ERROR;
    }
    return SHELL_EXIT_SUCCESS;
}
#endif

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : get_reg_domain()
 * Returned Value  : A_OK - on successful completion
 *		    A_ERROR - on any failure.
 * Comments        : gets Regulatory domain from driver
 *
 *END*------------------------------------------------------------------------*/
static int32_t get_reg_domain(void)
{
    ATH_IOCTL_PARAM_STRUCT ath_param;
    int32_t error = A_ERROR;
    ATH_REG_DOMAIN result;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    ath_param.cmd_id = ATH_GET_REG_DOMAIN;
    ath_param.data   = &result;
    ath_param.length = 0;

    error = HANDLE_IOCTL(&ath_param);

    if (error == A_OK)
    {
        PRINTF("Regulatory Domain 0x%x" NL, result.domain);
    }
    else
    {
        PRINTF("Command Failed" NL);
        return error;
    }
    return A_OK;
}

static int32_t set_pmparams(int32_t index, int32_t argc, char *argv[])
{
    uint32_t enet_device, i;
    ATH_WMI_POWER_PARAMS_CMD pm;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    enet_device = get_active_device();
    /*Fill in default values */
    pm.idle_period          = 0;
    pm.pspoll_number        = 10;
    pm.dtim_policy          = 3;
    pm.tx_wakeup_policy     = 2;
    pm.num_tx_to_wakeup     = 1;
    pm.ps_fail_event_policy = 2;

    for (i = index; i < argc; i++)
    {
        if (ATH_STRCMP(argv[i], "--idle") == 0)
        {
            if (i + 1 == argc) // User did not enter parameter
            {
                PRINTF("Missing parameter" NL);
                return A_ERROR;
            }
            pm.idle_period = atoi(argv[i + 1]);
            i++;
        }
        else if (ATH_STRCMP(argv[i], "--np") == 0)
        {
            if (i + 1 == argc) // User did not enter parameter
            {
                PRINTF("Missing parameter" NL);
                return A_ERROR;
            }

            pm.pspoll_number = atoi(argv[i + 1]);
            i++;
        }
        else if (ATH_STRCMP(argv[i], "--dp") == 0)
        {
            if (i + 1 == argc) // User did not enter parameter
            {
                PRINTF("Missing parameter" NL);
                return A_ERROR;
            }

            pm.dtim_policy = atoi(argv[i + 1]);
            i++;
        }
        else if (ATH_STRCMP(argv[i], "--txwp") == 0)
        {
            if (i + 1 == argc) // User did not enter parameter
            {
                PRINTF("Missing parameter" NL);
                return A_ERROR;
            }

            pm.tx_wakeup_policy = atoi(argv[i + 1]);
            i++;
        }
        else if (ATH_STRCMP(argv[i], "--ntxw") == 0)
        {
            if (i + 1 == argc) // User did not enter parameter
            {
                PRINTF("Missing parameter" NL);
                return A_ERROR;
            }

            pm.num_tx_to_wakeup = atoi(argv[i + 1]);
            i++;
        }
        else if (ATH_STRCMP(argv[i], "--psfp") == 0)
        {
            if (i + 1 == argc) // User did not enter parameter
            {
                PRINTF("Missing parameter" NL);
                return A_ERROR;
            }
            pm.ps_fail_event_policy = atoi(argv[i + 1]);
            i++;
        }
        else
        {
            PRINTF("Invalid parameter" NL);
            return A_ERROR;
        }
    }

    return (qcom_power_set_parameters(enet_device, pm.idle_period, pm.pspoll_number, pm.dtim_policy,
                                      pm.tx_wakeup_policy, pm.num_tx_to_wakeup, pm.ps_fail_event_policy));
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : get_phy_mode()
 * Returned Value  : A_OK if phy_mode set successfully else ERROR CODE
 * Comments        : Gets the phy_mode.
 *
 *END*-----------------------------------------------------------------*/

int32_t get_phy_mode(uint32_t dev_num, char *phy_mode)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error;
    uint32_t value = 0;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_GET_PHY_MODE;
    inout_param.data   = &value;

    error = HANDLE_IOCTL(&inout_param);
    if (A_OK != error)
    {
        return error;
    }

    if (*(uint32_t *)(inout_param.data) == 0x01)
    {
        strcpy(phy_mode, "a");
    }
    else if (*(uint32_t *)(inout_param.data) == 0x02)
    {
        strcpy(phy_mode, "g");
    }
    else if (*(uint32_t *)(inout_param.data) == 0x04)
    {
        strcpy(phy_mode, "b");
    }
    else
    {
        strcpy(phy_mode, "mixed");
    }
    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : get_version()
 * Returned Value  : A_OK version is retrieved successfully else ERROR CODE
 * Comments        : gets driver,firmware version.
 *
 *END*-----------------------------------------------------------------*/
int32_t get_version(void)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error;
    versionstr_t versionstr;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_GET_VERSION_STR;
    inout_param.data   = &versionstr;
    inout_param.length = sizeof(versionstr);

    error = HANDLE_IOCTL(&inout_param);
    if (A_OK != error)
    {
        return error;
    }

    PRINTF("Host version :  %s" NL, versionstr.host_ver);
    PRINTF("Target version   :  %s" NL, versionstr.target_ver);
    PRINTF("Firmware version :  %s" NL, versionstr.wlan_ver);
    PRINTF("Interface version:  %s" NL, versionstr.abi_ver);

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : get_last_error()
 * Returned Value  : A_OK if phy_mode set successfully else ERROR CODE
 * Comments        : Gets the last error in the host driver
 *
 *END*-----------------------------------------------------------------*/

int32_t get_last_error(void)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error = A_ERROR;
    int32_t result;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_GET_LAST_ERROR;
    inout_param.data   = &result;
    inout_param.length = 0;

    error = HANDLE_IOCTL(&inout_param);
    if (A_OK != error)
    {
        return error;
    }

    PRINTF("Last Driver error %d" NL, (*(uint32_t *)(inout_param.data)));
    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : dev_susp_start()
 * Returned Value  : A_OK if device suspend is started
 *                   successfully else ERROR CODE
 * Comments        : Suspends device for requested time period
 *
 *END*-----------------------------------------------------------------*/
int32_t dev_susp_start(char *susp_time)
{
    int suspend_time;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    enet_device = get_active_device();
    if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
    {
        return A_ERROR;
    }
    if (wifimode == QCOM_WLAN_DEV_MODE_AP)
    {
        PRINTF("Store-Recall Mechanism NOT supported in 'MODE_AP'" NL);
        return A_ERROR;
    }

#if ENABLE_STACK_OFFLOAD
#if ENABLE_SSL
    if (ssl_inst[SSL_CLIENT_INST].sslCtx != NULL)
    {
        PRINTF("Please disable SSL and start Store-Recall" NL);
        return A_ERROR;
    }
#endif
#endif
    /*Check if no packets are queued, if TX is pending, then wait*/
    while (get_tx_status() != ATH_TX_STATUS_IDLE)
    {
        A_MDELAY(500);
    }

    suspend_time = atoi(susp_time);
    return qcom_suspend_start(suspend_time);
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : wmi_iwconfig()
 * Returned Value  : A_OK if success else ERROR
 *
 * Comments        : Setup for scan command
 *
 *END*-----------------------------------------------------------------*/
int32_t wmi_iwconfig(int32_t argc, char *argv[])
{
    uint8_t scan_ssid_flag = 0; // Flag to decide whether to scan all ssid/channels
    int32_t error          = A_OK;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;
    QCOM_SSID scan_ssid         = {0};
    QCOM_SSID orig_ssid         = {0};

#if ENABLE_P2P_MODE
    if (p2p_session_in_progress)
    {
        PRINTF("p2p event in progress" NL);
        return A_ERROR;
    }
#endif
    enet_device = get_active_device();

    qcom_get_ssid(enet_device, &orig_ssid);
    if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
    {
        return A_ERROR;
    }

    if (argc < 2)
    {
        PRINTF("Missing Parameters" NL);
        return A_ERROR;
    }
    else if (wifimode == QCOM_WLAN_DEV_MODE_AP)
    {
        PRINTF("AP mode cannot scan" NL);
        return A_ERROR;
    }
    else if (argc > 1)
    {
        if (ATH_STRCMP(argv[1], "scan") != 0)
        {
            PRINTF("Incorrect Parameter" NL);
            return A_ERROR;
        }

        if (argc == 3)
        {
            if (strlen(argv[2]) > MAX_SSID_LENGTH)
            {
                PRINTF("SSID length exceeds Maximum value" NL);
                return A_ERROR;
            }
            /*Scan specified SSID*/
            scan_ssid_flag = 1;
            strncpy((char *)&scan_ssid, argv[2], sizeof(scan_ssid));
        }
    }
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    /*Set SSID for scan*/
    if (scan_ssid_flag)
    {
        error = qcom_set_ssid(enet_device, &scan_ssid);
        if (error != A_OK)
        {
            PRINTF("Unable to set SSID" NL);
            return error;
        }
    }
    else
    {
        QCOM_SSID tmp_ssid = QCOM_SSID_FROM_STR("");
        error              = qcom_set_ssid(enet_device, &tmp_ssid);
        if (error != A_OK)
        {
            PRINTF("Unable to set SSID" NL);
            return error;
        }
    }

    /*Do the actual scan*/
    wmi_set_scan(enet_device, NULL);

    /*Revert to original SSID*/
    error = qcom_set_ssid(enet_device, &orig_ssid);
    if (error != A_OK)
    {
        PRINTF("Unable to set SSID" NL);
        return error;
    }
    return error;
}

#if ENABLE_AP_MODE
static int32_t ap_handler(int32_t index, int32_t argc, char_ptr argv[])
{
    uint32_t error = A_OK, i, ret_val;
    char *p        = 0;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;
    enet_device                 = get_active_device();

    if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
    {
        return A_ERROR;
    }

    if (QCOM_WLAN_DEV_MODE_AP != wifimode)
    {
        PRINTF("Err:Set AP Mode to apply AP settings" NL);
        return A_ERROR;
    }

    if (index + 1 < argc)
    {
        i = index;

        if (ATH_STRCMP(argv[i], "country") == 0)
        {
            PRINTF(NL "Cannot set country code. Use board data file or tuneables instead" NL);
        }
        else if (ATH_STRCMP(argv[i], "inact") == 0)
        {
            ap_config_param.u.wmi_ap_conn_inact_cmd.period = strtoul(argv[i + 1], &p, 0);

            if (ap_config_param.u.wmi_ap_conn_inact_cmd.period == 0)
            {
                PRINTF("incorrect inactive timeout" NL);
                return A_ERROR;
            }

            ret_val = set_ap_params(AP_SUB_CMD_INACT_TIME, (uint8_t *)&ap_config_param.u.wmi_ap_conn_inact_cmd.period);

            if (ret_val != A_OK)
            {
                PRINTF("Err:set inactive_time" NL);
                return A_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[i], "bconint") == 0)
        {
            uint16_t beacon_Interval;
            beacon_Interval = strtoul(argv[i + 1], &p, 0);
            if ((beacon_Interval < 100) || (beacon_Interval > 1000))
            {
                PRINTF("beacon interval has to be within 100-1000 in units of 100" NL);
                return A_ERROR;
            }
            if (qcom_ap_set_beacon_interval(enet_device, beacon_Interval) != A_OK)
            {
                PRINTF("Err:set beacon interval" NL);
                return A_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[i], "dtim") == 0)
        {
            ret_val = strtoul(argv[i + 1], &p, 0);

            if ((ret_val < 1) || (ret_val > 255))
            {
                PRINTF("dtim interval has to be within 1-255" NL);
                return A_ERROR;
            }

            ap_config_param.u.wmi_ap_dtim_cmd.dtimval = ret_val;

            ret_val = set_ap_params(AP_SUB_CMD_DTIM_INT, (uint8_t *)&ap_config_param.u.wmi_ap_dtim_cmd.dtimval);

            if (ret_val != A_OK)
            {
                PRINTF("Err:set dtim interval" NL);
                return A_ERROR;
            }
        }
        else if (ATH_STRCMP(argv[i], "psbuf") == 0)
        {
            ap_config_param.u.wmi_ap_ps_buf_cmd.ps_val[0] = strtoul(argv[i + 1], &p, 0);

            if (ap_config_param.u.wmi_ap_ps_buf_cmd.ps_val[0] == 0)
            {
                ap_config_param.u.wmi_ap_ps_buf_cmd.ps_val[1] = 0x0;

                ret_val = set_ap_params(AP_SUB_CMD_PSBUF, (uint8_t *)ap_config_param.u.wmi_ap_ps_buf_cmd.ps_val);

                if (ret_val != A_OK)
                {
                    PRINTF("Err:set ps buf" NL);
                    return A_ERROR;
                }
            }
            else
            {
                ap_config_param.u.wmi_ap_ps_buf_cmd.ps_val[1] = strtoul(argv[i + 2], &p, 0);

                ret_val = set_ap_params(AP_SUB_CMD_PSBUF, (uint8_t *)ap_config_param.u.wmi_ap_ps_buf_cmd.ps_val);

                if (ret_val != A_OK)
                {
                    PRINTF("Err:set ps buf" NL);
                    return A_ERROR;
                }
            }
        }
        else
        {
            PRINTF("usage:--ap [country <country code>]" NL "      --ap [bconint<intvl>]" NL "      --ap [dtim<val>]" NL
                   "      --ap [inact <minutes>]>" NL
                   "      --ap [psbuf <1/0-enable/disable> <buf_count for enable>]>" NL);
        }
    }
    else
    {
        PRINTF("usage:--ap [country <country code>]" NL "      --ap [bconint<intvl>]" NL "      --ap [dtim<val>]" NL
               "      --ap [inact <minutes>]>" NL "      --ap [psbuf <1/0-enable/disable> <buf_count for enable>]>" NL);
    }
    return error;
}

static int32_t set_ap_params(uint16_t param_cmd, uint8_t *data_val)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    ATH_AP_PARAM_STRUCT ap_param;
    uint32_t error;

    inout_param.cmd_id  = ATH_CONFIG_AP;
    ap_param.cmd_subset = param_cmd;
    ap_param.data       = data_val;
    inout_param.data    = &ap_param;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    error = HANDLE_IOCTL(&inout_param);
    if (A_OK != error)
    {
        return error;
    }

    return A_OK;
}
#endif

#if ENABLE_STACK_OFFLOAD

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ipconfig_query()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Sample function to depict ipconfig functionality.
 *                   Queries IP parameters from target.
 *
 *END*-----------------------------------------------------------------*/
int ipconfig_query(shell_handle_t context, int argc, char **argv)
{
    uint32_t enet_device, addr = 0, mask = 0, gw = 0, num;
    IP6_ADDR_T v6Global;
    IP6_ADDR_T v6GlobalExtd;
    IP6_ADDR_T v6LinkLocal;
    IP6_ADDR_T v6DefGw;
    char ip6buf[48]          = {0};
    char mac_data[6]         = {0};
    char *ip6ptr             = NULL;
    int32_t LinkPrefix       = 0;
    int32_t GlobalPrefix     = 0;
    int32_t DefGwPrefix      = 0;
    int32_t GlobalPrefixExtd = 0;
    uint32_t dnsaddr[MAX_DNSADDRS];
    int i = 0;
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    enet_device = get_active_device();
    qcom_get_bssid(enet_device, (uint8_t *)mac_data);
    PRINTF(NL "mac addr    =   %02x:%02x:%02x:%02x:%02x:%02x" NL, mac_data[0] & 0xFF, mac_data[1] & 0xFF,
           mac_data[2] & 0xFF, mac_data[3] & 0xFF, mac_data[4] & 0xFF, mac_data[5] & 0xFF);

    qcom_ipconfig(enet_device, QCOM_IPCONFIG_QUERY, &addr, &mask, &gw);
    PRINTF("IP:%x Mask:%x, Gateway:%x" NL, (int)addr, (int)mask, (int)gw);
    PRINTF("IP:%d.%d.%d.%d Mask:%d.%d.%d.%d, Gateway:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr),
           getByte(1, addr), getByte(0, addr), getByte(3, mask), getByte(2, mask), getByte(1, mask), getByte(0, mask),
           getByte(3, gw), getByte(2, gw), getByte(1, gw), getByte(0, gw));

    qcom_dns_server_address_get(dnsaddr, &num);
    for (i = 0; i < num; i++)
    {
        addr = A_CPU2LE32(dnsaddr[i]);
        PRINTF("DNS Server:%d.%d.%d.%d" NL, getByte(0, addr), getByte(1, addr), getByte(2, addr), getByte(3, addr));
    }

    memset(&v6LinkLocal, 0, sizeof(IP6_ADDR_T));
    memset(&v6Global, 0, sizeof(IP6_ADDR_T));
    memset(&v6DefGw, 0, sizeof(IP6_ADDR_T));
    memset(&v6GlobalExtd, 0, sizeof(IP6_ADDR_T));
    qcom_ip6_address_get(enet_device, &v6Global, &v6LinkLocal, &v6DefGw, &v6GlobalExtd, &LinkPrefix, &GlobalPrefix,
                         &DefGwPrefix, &GlobalPrefixExtd);

    if (v6LinkLocal.s6_addr[0])
    {
        ip6ptr = print_ip6(&v6LinkLocal, ip6buf);
        if (ip6ptr)
        {
            if (LinkPrefix)
                PRINTF("Link-local IPv6 Address ..... : %s/%d" NL, ip6ptr, (int)LinkPrefix);
            else
                PRINTF("Link-local IPv6 Address ..... : %s" NL, ip6ptr);
        }
        ip6ptr = NULL;
        ip6ptr = print_ip6(&v6Global, ip6buf);
        if (ip6ptr)
        {
            if (GlobalPrefix)
                PRINTF("Global IPv6 Address ......... : %s/%d" NL, ip6ptr, (int)GlobalPrefix);
            else
                PRINTF("Global IPv6 Address ......... : %s" NL, ip6ptr);
        }
        ip6ptr = NULL;
        ip6ptr = print_ip6(&v6DefGw, ip6buf);
        if (ip6ptr)
        {
            if (DefGwPrefix)
                PRINTF("Default Gateway  ............. : %s/%d" NL, ip6ptr, (int)DefGwPrefix);
            else
                PRINTF("Default Gateway  ............. : %s" NL, ip6ptr);
        }
        ip6ptr = NULL;
        ip6ptr = print_ip6(&v6GlobalExtd, ip6buf);
        if (ip6ptr)
        {
            if (GlobalPrefixExtd)
                PRINTF("Global IPv6 Address 2 ......... : %s/%d" NL, ip6ptr, (int)GlobalPrefixExtd);
            else
                PRINTF("Global IPv6 Address 2 ......... : %s" NL, ip6ptr);
        }
    }
    return 0;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ipconfig_static()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Sample function to depict ipconfig functionality.
 *                   Sets static IPv4 parameters.
 *
 *END*-----------------------------------------------------------------*/
int32_t ipconfig_static(int32_t argc, char *argv[])
{
    uint32_t addr, mask, gw;
    char *ip_addr_string;
    char *mask_string;
    char *gw_string;
    int32_t error;
    uint32_t sbits;
    uint32_t enet_device = get_active_device();
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    if (argc < 5)
    {
        PRINTF("incomplete params" NL);
        return A_ERROR;
    }
    ip_addr_string = argv[2];
    mask_string    = argv[3];
    gw_string      = argv[4];

    error = parse_ipv4_ad((unsigned long *)&addr, &sbits, ip_addr_string);
    if (error != 0)
    {
        PRINTF("Invalid IP address" NL);
        return error;
    }
    addr  = A_BE2CPU32(addr);
    error = parse_ipv4_ad((unsigned long *)&mask, &sbits, mask_string);
    if (error != 0)
    {
        PRINTF("Invalid Mask address" NL);
        return error;
    }
    mask  = A_BE2CPU32(mask);
    error = parse_ipv4_ad((unsigned long *)&gw, &sbits, gw_string);
    if (error != 0)
    {
        PRINTF("Invalid Gateway address" NL);
        return error;
    }
    gw = A_BE2CPU32(gw);
    qcom_ipconfig(enet_device, QCOM_IPCONFIG_STATIC, &addr, &mask, &gw);
    PRINTF("IP:%x Mask:%x, Gateway:%x" NL, addr, mask, gw);
    PRINTF("IP:%d.%d.%d.%d Mask:%d.%d.%d.%d, Gateway:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr),
           getByte(1, addr), getByte(0, addr), getByte(3, mask), getByte(2, mask), getByte(1, mask), getByte(0, mask),
           getByte(3, gw), getByte(2, gw), getByte(1, gw), getByte(0, gw));

    return 0;
}

void configure_auto_ipv4_addr(void)
{
    uint32_t addr, mask, gw;
    char *ip_addr_string;
    char *mask_string;
    char *gw_string;
    int32_t error;
    uint32_t sbits;
    uint32_t enet_device = get_active_device();

    ip_addr_string = "169.254.1.1";
    mask_string    = "255.255.0.0";
    gw_string      = "0.0.0.0";

    error = parse_ipv4_ad((unsigned long *)&addr, &sbits, ip_addr_string);
    if (error != 0)
    {
        PRINTF("Invalid IP address" NL);
        return;
    }
    addr  = A_BE2CPU32(addr);
    error = parse_ipv4_ad((unsigned long *)&mask, &sbits, mask_string);
    if (error != 0)
    {
        PRINTF("Invalid Mask address" NL);
        return;
    }
    mask  = A_BE2CPU32(mask);
    error = parse_ipv4_ad((unsigned long *)&gw, &sbits, gw_string);
    if (error != 0)
    {
        PRINTF("Invalid Gateway address" NL);
        return;
    }
    gw = A_BE2CPU32(gw);
    qcom_ipconfig(enet_device, QCOM_IPCONFIG_STATIC, &addr, &mask, &gw);
    PRINTF("Acquired IP address via AutoIPv4" NL);
    PRINTF("IP:%x Mask:%x, Gateway:%x" NL, addr, mask, gw);
    PRINTF("IP:%d.%d.%d.%d Mask:%d.%d.%d.%d, Gateway:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr),
           getByte(1, addr), getByte(0, addr), getByte(3, mask), getByte(2, mask), getByte(1, mask), getByte(0, mask),
           getByte(3, gw), getByte(2, gw), getByte(1, gw), getByte(0, gw));

    return;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ipconfig_dhcp()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Sample function to depict ipconfig functionality.
 *                   Runs DHCP on target (only IPv4).
 *
 *END*-----------------------------------------------------------------*/
int32_t ipconfig_dhcp(int32_t argc, char *argv[])
{
    uint32_t addr, mask, gw;
    int err = 0;
    uint32_t enet_device;
    enet_device = get_active_device();

    if (wifi_connected_flag == 0)
    {
        PRINTF("ERROR: No WiFi connection available, please connect to an Access Point" NL);
        return A_ERROR;
    }
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    set_power_mode(MAX_PERF_POWER, DHCP);

    for (int32_t i = 20; i > 0; i--)
    {
        err = qcom_ipconfig(enet_device, QCOM_IPCONFIG_DHCP, &addr, &mask, &gw);
        A_MDELAY(50);
        err = qcom_ipconfig(enet_device, QCOM_IPCONFIG_QUERY, &addr, &mask, &gw);
        if (addr != 0)
        {
            break;
        }
    }

    if (err == -1)
    {
        PRINTF("dhcp timed out. Configuring auto ip" NL);
        configure_auto_ipv4_addr();
        set_power_mode(REC_POWER, DHCP);
        return 0;
    }
    addr = 0;

    qcom_ipconfig(enet_device, QCOM_IPCONFIG_QUERY, &addr, &mask, &gw);
    if (addr != 0)
    {
        PRINTF("IP:%d.%d.%d.%d Mask:%d.%d.%d.%d, Gateway:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr),
               getByte(1, addr), getByte(0, addr), getByte(3, mask), getByte(2, mask), getByte(1, mask),
               getByte(0, mask), getByte(3, gw), getByte(2, gw), getByte(1, gw), getByte(0, gw));
    }

    set_power_mode(REC_POWER, DHCP);
    return 0;
}

int32_t ipconfig_auto_ipv4(int32_t argc, char *argv[], QCOM_IPCONFIG_MODE mode)
{
    uint32_t addr = 0, mask = 0, gw = 0;
    int err = 0;
    uint32_t sbits;
    char *ip_addr_string;
    uint32_t enet_device = get_active_device();

    if (wifi_connected_flag == 0)
    {
        PRINTF("ERROR: No WiFi connection available, please connect to an Access Point" NL);
        return A_ERROR;
    }
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc == 3)
    {
        ip_addr_string = argv[2];
        err            = parse_ipv4_ad((unsigned long *)&addr, &sbits, ip_addr_string);
        if (err != 0)
        {
            PRINTF("Invalid IP address" NL);
            return err;
        }
        addr = A_BE2CPU32(addr);

        if (addr < dBASE_AUTO_IP_ADDRESS || addr > dMAX_AUTO_IP_ADDRESS)
        {
            PRINTF("Invalid link local IPv4 address" NL);
            return A_ERROR;
        }
    }
    err = qcom_ipconfig(enet_device, mode, &addr, &mask, &gw);

    if (err == -1 && mode == QCOM_IPCONFIG_AUTO)
    {
        PRINTF("Auto IP configure failed" NL);
        return 0;
    }

    addr = 0;
    qcom_ipconfig(enet_device, QCOM_IPCONFIG_QUERY, &addr, &mask, &gw);
    if (addr != 0)
    {
        PRINTF("IP:%d.%d.%d.%d Mask:%d.%d.%d.%d, Gateway:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr),
               getByte(1, addr), getByte(0, addr), getByte(3, mask), getByte(2, mask), getByte(1, mask),
               getByte(0, mask), getByte(3, gw), getByte(2, gw), getByte(1, gw), getByte(0, gw));
    }
    return 0;
}

int32_t block_on_dhcp(int32_t argc, char *argv[])
{
    uint32_t addr = 0, mask = 0, gw = 0;
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    /*Start DHCP*/
    qcom_ipconfig(enet_device, QCOM_IPCONFIG_DHCP, &addr, &mask, &gw);

    /*Wait till we get an IP address, instead of indefinte loop, develop may
      use a timeout logic here*/
    while (addr == 0)
    {
        qcom_ipconfig(enet_device, QCOM_IPCONFIG_QUERY, &addr, &mask, &gw);
        A_MDELAY(100);
    }
    PRINTF("addr 0x%x" NL, addr);
    return A_OK;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : set_tcp_connection_timeout()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : sets TCP Connection Time out
 *
 *END*------------------------------------------------------------------------*/
static int32_t set_tcp_connection_timeout(int32_t argc, char *argv[])
{
    uint32_t timeout_val;
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    if ((argc > 3) || (argc < 2))
    {
        PRINTF("incorrect arguments refer help" NL);
        PRINTF("Usage: wmiconfig --settcptimeout <timeout>" NL);
        return A_ERROR;
    }

    timeout_val = atoi(argv[2]);
    if (A_OK != qcom_tcp_conn_timeout(timeout_val))
    {
        PRINTF("Set TCP Timeout Failed" NL);
        return A_ERROR;
    }

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ip_set_bridgemode()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Function to enable bridge mode.
 *
 *END*-----------------------------------------------------------------*/

int32_t ip_set_bridgemode(int32_t argc, char *argv[])
{
    int err;

    if (wifi_connected_flag == 1)
    {
        PRINTF("Please reboot and execute this as first command" NL);
        return A_ERROR;
    }

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    err = qcom_bridge_mode_enable(1);
    if (err == -1)
    {
        return A_ERROR;
    }

    PRINTF("Please reboot and execute this as first command" NL);

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ipconfig_dhcp_pool()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Sample function to configure dhcp pool.
 *
 *END*-----------------------------------------------------------------*/
int32_t ipconfig_dhcp_pool(int32_t argc, char *argv[])
{
    uint32_t startaddr, endaddr;
    char *start_ip_addr_string;
    char *end_ip_addr_string;
    int32_t error;
    uint32_t sbits;
    int leasetime;
    uint32_t addr = 0, mask = 0, gw = 0;
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    if (argc < 5)
    {
        PRINTF("incomplete params" NL);
        return A_ERROR;
    }
    start_ip_addr_string = argv[2];
    end_ip_addr_string   = argv[3];
    leasetime            = atoi(argv[4]);

    error = parse_ipv4_ad((unsigned long *)&startaddr, &sbits, start_ip_addr_string);
    if (error != 0)
    {
        PRINTF("Invalid start IP address" NL);
        return error;
    }
    startaddr = A_BE2CPU32(startaddr);
    error     = parse_ipv4_ad((unsigned long *)&endaddr, &sbits, end_ip_addr_string);
    if (error != 0)
    {
        PRINTF("Invalid end ip address" NL);
        return error;
    }
    endaddr = A_BE2CPU32(endaddr);
    qcom_ipconfig(enet_device, QCOM_IPCONFIG_QUERY, &addr, &mask, &gw);
    if ((addr & mask) != (startaddr & mask))
    {
        PRINTF("LAN IP address and start ip address are not in the same subnet" NL);
        return error;
    }
    if ((addr & mask) != (endaddr & mask))
    {
        PRINTF("LAN IP address and end ip address are not in the same subnet" NL);
        return error;
    }
    if ((addr >= startaddr) && (addr <= endaddr))
    {
        PRINTF("Please configure pool beyond LAN IP Address" NL);
        return error;
    }
    if (startaddr >= endaddr)
    {
        PRINTF("Staraddr must be less than end addr" NL);
        return error;
    }
    qcom_dhcps_set_pool(enet_device, startaddr, endaddr, leasetime);
    return 0;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ipconfig_dhcps_success_cb()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Sample callback function that is invoked on DHCP Server
 *                   issuing a DHCP client a successful IP.
 *		    The callback is invoked with the client's HW address and the
 *	            IP Address issued to the client.
 *
 *END*-----------------------------------------------------------------*/
uint32_t ipconfig_dhcps_success_cb(uint8_t *mac, uint32_t addr)
{
    PRINTF("DHCP Server issued Client MAC:%02x:%02x:%02x:%02x:%02x:%02x,IP Addr:%d.%d.%d.%d" NL, mac[0], mac[1], mac[2],
           mac[3], mac[4], mac[5], getByte(3, addr), getByte(2, addr), getByte(1, addr), getByte(0, addr));
    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ipconfig_dhcps_cb_enable()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Sample function to configure dhcp server callback
 *                   registration.
 *
 *END*-----------------------------------------------------------------*/
int32_t ipconfig_dhcps_cb_enable(int32_t argc, char *argv[])
{
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("incomplete params:e.g wmiconfig --dhcpscb <start/stop>" NL);
        return A_ERROR;
    }

    if (!ATH_STRCMP(argv[2], "start"))
    {
        qcom_dhcps_register_cb(enet_device, (void *)ipconfig_dhcps_success_cb);
    }
    else if (!ATH_STRCMP(argv[2], "stop"))
    {
        qcom_dhcps_register_cb(enet_device, NULL);
    }
    else
    {
        PRINTF("input parameter should be start or stop!" NL);
        return A_ERROR;
    }
    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ipconfig_dhcpc_success_cb()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Sample callback function that is invoked on DHCP Client
 *                   issuing a DHCP client a successful IP.
 *		    		The callback is invoked with the IP Address,Mask,Gateway address assigned
 *                   to the client.
 *
 *END*-----------------------------------------------------------------*/
uint32_t ipconfig_dhcpc_success_cb(uint32_t addr, uint32_t mask, uint32_t gw)
{
    PRINTF("IP Addr:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr), getByte(1, addr), getByte(0, addr));
    PRINTF("IP Mask:%d.%d.%d.%d" NL, getByte(3, mask), getByte(2, mask), getByte(1, mask), getByte(0, mask));
    PRINTF("IP GW:%d.%d.%d.%d" NL, getByte(3, gw), getByte(2, gw), getByte(1, gw), getByte(0, gw));
    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ipconfig_dhcpc_cb_enable()
 * Returned Value  : A_OK if success else A_ERROR
 *
 * Comments        : Sample function to configure dhcp client callback
 *                   registration.
 *
 *END*-----------------------------------------------------------------*/
int32_t ipconfig_dhcpc_cb_enable(int32_t argc, char *argv[])
{
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("incomplete params:e.g wmiconfig --dhcpccb <start/stop>" NL);
        return A_ERROR;
    }

    if (!ATH_STRCMP(argv[2], "start"))
    {
        qcom_dhcpc_register_cb(enet_device, (void *)ipconfig_dhcpc_success_cb);
    }
    else if (!ATH_STRCMP(argv[2], "stop"))
    {
        qcom_dhcpc_register_cb(enet_device, NULL);
    }
    else
    {
        PRINTF("input parameter should be start or stop!" NL);
        return A_ERROR;
    }
    return A_OK;
}

int32_t ip6_set_router_prefix(int32_t argc, char *argv[])
{
    IP6_ADDR_T v6addr   = {0};
    int32_t retval      = -1;
    int prefixlen       = 0;
    int prefix_lifetime = 0;
    int valid_lifetime  = 0;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 6)
    {
        PRINTF("incomplete params" NL);
        return A_ERROR;
    }

    enet_device = get_active_device();
    if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
    {
        return A_ERROR;
    }
    if (wifimode != QCOM_WLAN_DEV_MODE_AP)
    {
        PRINTF("ipv6 rt prfx support not possible in station" NL);
        return A_ERROR;
    }

    retval = Inet6Pton(argv[2], &v6addr);
    if (retval == 1)
    {
        PRINTF("Invalid ipv6 prefix" NL);
        return (A_ERROR);
    }

    prefixlen       = atoi(argv[3]);
    prefix_lifetime = atoi(argv[4]);
    valid_lifetime  = atoi(argv[5]);
    qcom_ip6config_router_prefix(enet_device, &v6addr, prefixlen, prefix_lifetime, valid_lifetime);
    return 0;
}

static int32_t ip_set_tcp_exponential_backoff_retry(int32_t argc, char *argv[])
{
    int32_t retry        = 0;
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    if (argc < 2)
    {
        PRINTF("incomplete params" NL);
        return A_ERROR;
    }
    retry = atoi(argv[2]);
    if ((retry > 12) || (retry < 4))
    {
        PRINTF("Confiure TCP Max retries between 4 and 12");
        return (A_ERROR);
    }
    qcom_tcp_set_exp_backoff(enet_device, retry);
    return A_OK;
}

static int32_t ip6_set_status(int32_t argc, char *argv[])
{
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    if (argc < 2)
    {
        PRINTF("incomplete params" NL);
        return A_ERROR;
    }

    if (ATH_STRCMP(argv[2], "enable") == 0)
    {
        custom_ipconfig_set_ip6_status((void *)handle, 1);
        return A_OK;
    }
    else if (ATH_STRCMP(argv[2], "disable") == 0)
    {
        custom_ipconfig_set_ip6_status((void *)handle, 0);
        return A_OK;
    }
    PRINTF("Allowed values are enable and disable only");
    return A_OK;
}

static int32_t ipconfig_dhcp_release(int32_t argc, char *argv[])
{
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    qcom_dhcps_release_pool(enet_device);
    return 0;
}

static int32_t ipconfig_tcp_rxbuf(int32_t argc, char *argv[])
{
    int32_t rxbuf = 0;
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 2)
    {
        PRINTF("incomplete params" NL);
        return A_ERROR;
    }
    rxbuf = atoi(argv[2]);
    if (rxbuf > 10)
    {
        PRINTF("Max Allowed RX Buffer is 10");
        return (A_ERROR);
    }
    custom_ipconfig_set_tcp_rx_buffer((void *)handle, rxbuf);
    return A_OK;
}

#if ENABLE_DNS_CLIENT
static int32_t ip_dns_client(int32_t argc, char *argv[])
{
    int32_t command = -1;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (ATH_STRCMP(argv[2], "start") == 0)
    {
        command = 1;
    }
    else if (ATH_STRCMP(argv[2], "stop") == 0)
    {
        command = 0;
    }
    else
    {
        PRINTF("Supported commands: start, stop");
        return (A_ERROR);
    }
    if (A_OK != qcom_dnsc_enable(command))
    {
        PRINTF("Error! DNS Client %s failed" NL, command ? "start" : "stop");
    }
    return A_OK;
}
static int32_t ip_dns_server_addr(int32_t argc, char *argv[])
{
    uint32_t addr        = 0;
    char *ip_addr_string = NULL;
    int32_t error;
    uint32_t sbits;
    int32_t retval    = -1;
    IP6_ADDR_T v6addr = {0};

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }

    ip_addr_string = argv[2];

    error = parse_ipv4_ad((unsigned long *)&addr, &sbits, ip_addr_string);
    if (error == 0)
    {
        if (A_OK != qcom_dnsc_add_server_address((uint8_t *)&addr, ATH_AF_INET))
        {
            PRINTF("Error! DNS Server address conf failed" NL);
            return (A_ERROR);
        }
        return A_OK;
    }

    retval = Inet6Pton(argv[2], &v6addr);
    if (retval == 1)
    {
        PRINTF("Invalid ipv6 prefix" NL);
        return (A_ERROR);
    }

    if (A_OK != qcom_dnsc_add_server_address((uint8_t *)&v6addr, ATH_AF_INET6))
    {
        PRINTF("Error! DNS Server address conf failed" NL);
        return (A_ERROR);
    }

    return A_OK;
}

static int32_t ip_dns_delete_server_addr(int32_t argc, char *argv[])
{
    uint32_t addr        = 0;
    char *ip_addr_string = NULL;
    int32_t error;
    uint32_t sbits;
    int32_t retval    = -1;
    IP6_ADDR_T v6addr = {0};

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    ip_addr_string = argv[2];

    error = parse_ipv4_ad((unsigned long *)&addr, &sbits, ip_addr_string);
    if (error == 0)
    {
        if (A_OK != qcom_dnsc_del_server_address((uint8_t *)&addr, ATH_AF_INET))
        {
            PRINTF("Error! DNS Server address conf failed" NL);
            return (A_ERROR);
        }
        return A_OK;
    }

    retval = Inet6Pton(argv[2], &v6addr);
    if (retval == 1)
    {
        PRINTF("Invalid ipv6 prefix" NL);
        return (A_ERROR);
    }
    if (A_OK != qcom_dnsc_del_server_address((uint8_t *)&v6addr, ATH_AF_INET6))
    {
        PRINTF("Error! DNS Server address conf failed" NL);
        return (A_ERROR);
    }
    return A_OK;
}

static int32_t ip_dns_timeout(int32_t argc, char *argv[])
{
    uint32_t enet_device = get_active_device();
    int32_t to;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    to = atoi(argv[2]);
    custom_ip_set_dns_block_time((void *)handle, to);
    qcom_param_set(enet_device, QCOM_PARAM_GROUP_NETWORK, QCOM_PARAM_GROUP_NETWORK_DNS_TIMEOUT_SECS, &to, 4, TRUE);
    return A_OK;
}

#endif /* ENABLE_DNS_CLIENT*/

#if ENABLE_HTTP_SERVER

/*Callback function called by PHOST when a client posts to HTTP server*/
void swat_http_post_callback(void *cxt, void *buf)
{
    HTTP_POST_EVENT_T *ev = (HTTP_POST_EVENT_T *)buf;
    int numTLV;
    unsigned char *data;

    if (!buf)
        return;

    numTLV = ev->numTLV;
    data   = ev->data;

    while (numTLV)
    {
        short type;
        short length;
        unsigned char *val;

        /*Parse through all TLVs*/
        GET_TLV_TYPE(data, type);
        GET_TLV_LENGTH(data, length);
        val = GET_TLV_VAL(data);
        switch (type)
        {
            case POST_TYPE_URI:
                PRINTF("URI: %s" NL, val);
                break;
            case POST_TYPE_NAME:
                PRINTF("NAME: %s" NL, val);
                break;
            case POST_TYPE_VALUE:
                PRINTF("VALUE: %s" NL, val);
                break;
            default:
                PRINTF("Invalid TLV" NL);
                break;
        }
        data = GET_NEXT_TLV(data, length);
        numTLV--;
    }
}

static int32_t ip_http_server(int32_t argc, char *argv[])
{
    int32_t command = HTTP_UNKNOWN;
    int32_t error;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 2)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (ATH_STRCMP(argv[1], "--ip_http_server") == 0)
    {
        if (ATH_STRCMP(argv[2], "start") == 0)
        {
            command = HTTP_SERVER_START;
            qcom_http_set_post_cb(NULL, (void *)swat_http_post_callback);
        }
        else if (ATH_STRCMP(argv[2], "stop") == 0)
        {
            command = HTTP_SERVER_STOP;
        }
        else
        {
            PRINTF("Supported commands: start, stop");
            return (A_ERROR);
        }
    }
    else if (ATH_STRCMP(argv[1], "--ip_https_server") == 0)
    {
        if (ATH_STRCMP(argv[2], "start") == 0)
        {
            command = HTTPS_SERVER_START;
            qcom_http_set_post_cb(NULL, (void *)swat_http_post_callback);
        }
        else if (ATH_STRCMP(argv[2], "stop") == 0)
        {
            command = HTTPS_SERVER_STOP;
        }
        else
        {
            PRINTF("Supported commands: start, stop");
            return (A_ERROR);
        }
    }

    error = qcom_http_server(command);

    if ((command == HTTP_SERVER_START) || (command == HTTP_SERVER_STOP))
    {
        if (A_OK != error)
            PRINTF("Error! HTTP Server %s" NL, (command == HTTP_SERVER_START) ? "start failed" : "not running");
        else
            PRINTF("HTTP server %s success" NL, (command == HTTP_SERVER_START) ? "start" : "stop");
    }
    else if ((command == HTTPS_SERVER_START) || (command == HTTPS_SERVER_STOP))
    {
        if (A_OK != error)
            PRINTF("Error! HTTPs Server %s" NL, (command == HTTPS_SERVER_START) ? "start failed" : "not running");
        else
            PRINTF("HTTPs server %s success" NL, (command == HTTPS_SERVER_START) ? "start" : "stop");
    }
    return A_OK;
}

static int32_t ip_http_server_post(int32_t argc, char *argv[])
{
    int32_t command = HTTP_POST_METHOD;
    int32_t objtype, objlen;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 7)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }

    objtype = atoi(argv[4]);
    objlen  = atoi(argv[5]);
    if ((objtype != 1) && (objtype != 2) && (objtype != 3))
    {
        PRINTF("Error: <objtype> can be 1 ,2 or 3" NL);
        return A_ERROR;
    }

    if (A_OK != qcom_ip_http_server_method(command, (uint8_t *)argv[2], (uint8_t *)argv[3], objtype, objlen,
                                           (uint8_t *)argv[6]))
    {
        PRINTF("Error! HTTP post failed" NL);
    }
    return A_OK;
}

static int32_t ip_http_server_get(int32_t argc, char *argv[])
{
    int32_t command = HTTP_GET_METHOD;
    int32_t objtype, objlen;
    uint8_t *value;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 4)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    objtype = 0;
    objlen  = 0;
#if 0
    if((objtype != 1) && (objtype != 2) && (objtype != 3))
    {
        PRINTF("Error: <objtype> can be 1 ,2 or 3" NL);
        return A_ERROR;
    }
#endif
    value = A_MALLOC(512, MALLOC_ID_TEMPORARY);
    if (!value)
        return A_ERROR;

    if (A_OK != qcom_ip_http_server_method(command, (uint8_t *)argv[2], (uint8_t *)argv[3], objtype, objlen, value))
    {
        PRINTF("Error! HTTP post failed" NL);
    }
    else
    {
        PRINTF("Value is %s" NL, value);
    }
    A_FREE(value, MALLOC_ID_TEMPORARY);
    return A_OK;
}

#endif // ENABLE_HTTP_SERVER
#if ENABLE_DNS_CLIENT
static int32_t ip_resolve_hostname(int32_t argc, char *argv[])
{
    uint32_t addr, domain;
    char ip6addr[16], ip6buf[48];
    char *ip6ptr = NULL;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 4)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (strlen(argv[2]) > 32)
    {
        PRINTF("host name cannot be more then 32 Bytes" NL);
        return A_ERROR;
    }
    domain = atoi(argv[3]);

    if (A_OK != qcom_dnsc_get_host_by_name2(argv[2], (uint32_t *)&ip6addr[0], domain, RESOLVEHOSTNAME))
    {
        PRINTF("Unable to resolve host name" NL);
        return A_ERROR;
    }
    if (domain == 2)
    {
        // TODO: need to rewrite !
        uint32_t *tmp = (uint32_t *)&ip6addr[0];
        addr          = A_CPU2BE32((uint32_t)*tmp);
        PRINTF("resolved IP:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr), getByte(1, addr), getByte(0, addr));
    }
    if (domain == 3)
    {
        ip6ptr = print_ip6((IP6_ADDR_T *)&ip6addr[0], ip6buf);
        PRINTF("Resolved IP6Addr ......... : %s" NL, ip6ptr);
    }
    return A_OK;
}

int32_t ip_get_addr_for_host_name(char *hostname, uint32_t *v4addr, IP6_ADDR_T *ip6addr, uint32_t domain)
{
    char ip6address[16], ip6buf[48];
    char *ip6ptr = NULL;

    /*Check if driver is loaded*/
    if (A_OK != qcom_dnsc_get_host_by_name2(hostname, (uint32_t *)&ip6address[0], domain, RESOLVEHOSTNAME))
    {
        PRINTF("Unable to resolve host name" NL);
        return A_ERROR;
    }
    if (domain == 2)
    {
        // TODO: rewrite !!
        uint32_t *tmp = (uint32_t *)&ip6address[0];
        *v4addr       = *tmp;
        uint32_t addr = A_CPU2BE32(*v4addr);
        PRINTF("resolved IP:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr), getByte(1, addr), getByte(0, addr));
    }
    if (domain == 3)
    {
        ip6ptr = print_ip6((IP6_ADDR_T *)&ip6address[0], ip6buf);
        PRINTF("Resolved IP6Addr ......... : %s" NL, ip6ptr);
        memcpy(ip6addr, &ip6address[0], 16);
    }
    return A_OK;
}

static int32_t ip_gethostbyname(int32_t argc, char *argv[])
{
    uint32_t addr;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (strlen(argv[2]) > 32)
    {
        PRINTF("host name cannot be more then 32 Bytes" NL);
        return A_ERROR;
    }

    if (A_OK != qcom_dnsc_get_host_by_name(argv[2], &addr))
    {
        PRINTF("Unable to resolve host name" NL);
        return A_ERROR;
    }
    PRINTF("addr:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr), getByte(1, addr), getByte(0, addr));
    return A_OK;
}
static int32_t ip_gethostbyname2(int32_t argc, char *argv[])
{
    int32_t domain;
    uint32_t addr;
    uint8_t ip6addr[16];
    char ip6buf[48];
    char *ip6ptr = NULL;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 4)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (strlen(argv[2]) > 32)
    {
        PRINTF("host name cannot be more then 32 Bytes" NL);
        return A_ERROR;
    }
    domain = atoi(argv[3]);

    if (A_OK != qcom_dnsc_get_host_by_name2(argv[2], (uint32_t *)ip6addr, domain, GETHOSTBYNAME2))
    {
        PRINTF("Unable to resolve host name" NL);
        return A_ERROR;
    }
    if (domain == 2)
    {
        // TODO: rewrite !
        uint32_t *tmp = (uint32_t *)&ip6addr[0];
        addr          = A_CPU2BE32(*tmp);
        PRINTF("addr:%d.%d.%d.%d" NL, getByte(3, addr), getByte(2, addr), getByte(1, addr), getByte(0, addr));
    }
    else if (domain == 3)
    {
        ip6ptr = print_ip6((IP6_ADDR_T *)&ip6addr[0], ip6buf);
        PRINTF("Resolved IP6Addr ......... : %s" NL, ip6ptr);
    }
    return A_OK;
}
#endif /* ENABLE_DNS_CLIENT */

static int32_t iphostname(int32_t argc, char *argv[])
{
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (strlen(argv[2]) > 32)
    {
        PRINTF("domain name cannot be more then 32 Bytes" NL);
        return A_ERROR;
    }
    if (A_OK != custom_ip_hostname((void *)handle, argv[2]))
    {
        PRINTF("Release DHCP, Configure the hostname and renew DHCP" NL);
        return A_ERROR;
    }
    return A_OK;
}

#if ENABLE_DNS_SERVER
static int32_t ip_dns_local_domain(int32_t argc, char *argv[])
{
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (strlen(argv[2]) > 32)
    {
        PRINTF("domain name cannot be more then 32 Bytes" NL);
        return A_ERROR;
    }
    if (A_OK != qcom_dns_local_domain(argv[2]))
    {
        PRINTF("Unable to configure local domain name" NL);
        return A_ERROR;
    }
    return A_OK;
}

static int32_t ipdns(int32_t argc, char *argv[])
{
    uint32_t addr        = 0;
    char *ip_addr_string = NULL;
    int32_t error;
    uint32_t sbits;
    IP46ADDR dnsaddr;
    int32_t retval    = -1;
    IP6_ADDR_T v6addr = {0};
    int32_t command   = 0;
    A_STATUS rtn;

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 5)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (strcmp(argv[2], "add") == 0)
    {
        command = 1;
    }
    if (strcmp(argv[2], "delete") == 0)
    {
        command = 2;
    }
    if (strlen(argv[3]) > 32)
    {
        PRINTF("domain name cannot be more then 32 Bytes" NL);
        return A_ERROR;
    }
    memset(&dnsaddr, 0, sizeof(IP46ADDR));
    ip_addr_string = argv[4];
    error          = parse_ipv4_ad((unsigned long *)&addr, &sbits, ip_addr_string);
    if (error == 0)
    {
        if (command == 1)
            rtn = qcom_dns_entry_create(argv[3], addr);
        else
            rtn = qcom_dns_entry_delete(argv[3], addr);
        if (A_OK != rtn)
        {
            PRINTF("Error! DNS database conf failed" NL);
            return (A_ERROR);
        }
        return A_OK;
    }
    retval = Inet6Pton(argv[4], &v6addr);
    if (retval == 1)
    {
        PRINTF("Invalid ipv6 prefix" NL);
        return (A_ERROR);
    }

    if (command == 1)
        rtn = qcom_dns_6entry_create(argv[3], &v6addr);
    else
        rtn = qcom_dns_6entry_delete(argv[3], &v6addr);

    if (A_OK != rtn)
    {
        return A_ERROR;
    }
    return A_OK;
}

static int32_t ip_dns_server_enable(int32_t argc, char *argv[])
{
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;
    int32_t command             = -1;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    enet_device = get_active_device();
    if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
    {
        return A_ERROR;
    }
    if (wifimode != QCOM_WLAN_DEV_MODE_AP) /*check previous mode*/
    {
        PRINTF("DNS Server can be enabled only in AP Mode" NL);
        return A_ERROR;
    }

    if (argc < 2)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }

    if (ATH_STRCMP(argv[2], "enable") == 0)
    {
        command = 1;
    }
    else if (ATH_STRCMP(argv[2], "disable") == 0)
    {
        command = 0;
    }
    else
    {
        PRINTF("Supported commands: enable, disable");
        return (A_ERROR);
    }
    if (A_OK != qcom_dnss_enable(command))
    {
        PRINTF("Error! DNS Server %s failed" NL, command ? "enable" : "disable ");
    }
    else
    {
        PRINTF("DNS Server %sed successfully" NL, command ? "enabled" : "disabled");
    }
    return A_OK;
}
#endif /* ENABLE_DNS_SERVER */

#if ENABLE_SNTP_CLIENT
static int32_t ip_sntp_srvr(int32_t argc, char *argv[])
{
    int32_t command = 0;

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 4)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (strcmp(argv[2], "add") == 0)
    {
        command = 1;
    }
    if (strcmp(argv[2], "delete") == 0)
    {
        command = 2;
    }
    if (strlen(argv[3]) > 64)
    {
        PRINTF("domain name or address cannot be more then 64 Bytes" NL);
        return A_ERROR;
    }

    qcom_sntp_srvr_addr(command, argv[3]);
    return A_OK;
}

static int32_t ip_sntp_get_time(int32_t argc, char *argv[])
{
    uint32_t enet_device;
    tSntpTime SntpTime;
    char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char *Day[7]     = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    uint8_t m[4];

    memset(&SntpTime, 0, sizeof(tSntpTime));

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    enet_device = get_active_device();
    qcom_sntp_get_time(enet_device, &SntpTime);

    memset(m, 0, 4);
    memcpy(m, &(months[SntpTime.mon][0]), 3);
    PRINTF("TimeStamp: %s %s %d, %d %d:%d:%d" NL, Day[SntpTime.wday], m, SntpTime.yday, SntpTime.year, SntpTime.hour,
           SntpTime.min, SntpTime.Sec);
    return A_OK;
}

static int32_t ip_sntp_get_time_of_day(int32_t argc, char *argv[])
{
    uint32_t enet_device;
    tSntpTM SntpTm;

    memset(&SntpTm, 0, sizeof(tSntpTM));

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    enet_device = get_active_device();
    qcom_sntp_get_time_of_day(enet_device, &SntpTm);

    PRINTF("sntp time of day done" NL "Seconds = %d" NL, SntpTm.tv_sec);
    return A_OK;
}

static int32_t ip_sntp_modify_timezone_dse(int32_t argc, char *argv[])
{
    tSntpTime SntpTime;
    uint8_t hour, min;
    uint8_t dse, add_sub;
    char hr[3], mn[3], parsing_hour_min[10];

    memset(&SntpTime, 0, sizeof(tSntpTime));

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 5)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }

    if (strlen(argv[2]) > 9)
    {
        PRINTF("Error: Invalid UTC string. Valid string(UTC+hour:min).Eg UTC+05:30" NL);
        return A_ERROR;
    }

    if (strlen(argv[3]) > 3)
    {
        PRINTF("Error: Invalid string" NL);
        return A_ERROR;
    }
    if (strlen(argv[4]) > 7)
    {
        PRINTF("Error: Invalid DSE string.Valid string DSE" NL);
        return A_ERROR;
    }

    strncpy(parsing_hour_min, argv[2], sizeof(parsing_hour_min) - 1);

    if (parsing_hour_min[3] == '+')
    {
        add_sub = 1; // time is to be added
    }
    else if (parsing_hour_min[3] == '-')
    {
        add_sub = 0; // time is to be subtracted
    }
    else
    {
        PRINTF("Error: Only + / - operation is allowed" NL);
        return A_ERROR;
    }

    hr[0] = parsing_hour_min[4];
    hr[1] = parsing_hour_min[5];
    hr[2] = '\0';

    hour = strtol(hr, NULL, 10);

    mn[0] = parsing_hour_min[7];
    mn[1] = parsing_hour_min[8];
    mn[2] = '\0';

    min = strtol(mn, NULL, 10);

    if (add_sub == 1)
    {
        if ((hour == 14) && (min > 0))
        {
            PRINTF("SNTP Error:Max hour limit is 14" NL);
            return A_ERROR;
        }
        if (hour > 14)
        {
            PRINTF("SNTP Error:Max hour limit is 14" NL);
            return A_ERROR;
        }
    }
    if (add_sub == 0)
    {
        if ((hour == 12) && (min > 0))
        {
            PRINTF("SNTP Error:Max hour limit is 12" NL);
            return A_ERROR;
        }
        if (hour > 12)
        {
            PRINTF("SNTP Error:Max hour limit is 12" NL);
            return A_ERROR;
        }
    }
    if (ATH_STRCMP(argv[4], "enable") == 0)
    {
        // PRINTF("Enabling dse" NL);
        dse = 1;
    }
    else if (ATH_STRCMP(argv[4], "disable") == 0)
    {
        // PRINTF("disabling dse" NL);
        dse = 0;
    }
    else
    {
        PRINTF("Invalid entry for DSE" NL);
        return A_ERROR;
    }

    qcom_sntp_zone(hour, min, add_sub, dse);

    return A_OK;
}

static int32_t ip_query_sntp_server_address(uint32_t argc, char *argv[])
{
    uint32_t enet_device;
    uint8_t i = 0;
    tSntpDnsAddr SntpDnsAddr[MAX_SNTP_SERVERS];
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (wifi_connected_flag == 0)
    {
        PRINTF("device not connected" NL);
        return A_ERROR;
    }

    memset(&SntpDnsAddr[0], 0, (sizeof(tSntpDnsAddr) * MAX_SNTP_SERVERS));

    enet_device = get_active_device();
    qcom_sntp_query_srvr_address(enet_device, &SntpDnsAddr[0]);

    for (i = 0; i < MAX_SNTP_SERVERS; i++)
    {
        if (SntpDnsAddr[i].resolve != DNS_NAME_NOT_SPECIFIED)
        {
            PRINTF("SNTP SERVER ADDRESS : %s" NL, SntpDnsAddr[i].addr);
        }
    }

    return A_OK;
}

static int32_t ip_sntp_client_start_stop(uint32_t argc, char *argv[])
{
    int32_t command = -1;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    if (ATH_STRCMP(argv[2], "start") == 0)
    {
        command = 1;
    }
    else if (ATH_STRCMP(argv[2], "stop") == 0)
    {
        command = 0;
    }
    else
    {
        PRINTF("Supported commands: start, stop");
        return (A_ERROR);
    }
    qcom_enable_sntp_client(command);
    return A_OK;
}
#endif /* ENABLE_SNTP_CLIENT */

#if ENABLE_SSL
static int32_t ssl_start(int32_t argc, char *argv[])
{
    SSL_INST *ssl;
    SSL_ROLE_T role;

    if (IS_DRIVER_READY != A_OK)
    {
        PRINTF("Driver not loaded!" NL);
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("ERROR: Incomplete params" NL);
        return A_ERROR;
    }

    if (0 == strcmp(argv[2], "server"))
    {
        ssl  = &ssl_inst[SSL_SERVER_INST];
        role = SSL_SERVER;
    }
    else if (0 == strcmp(argv[2], "client"))
    {
        ssl  = &ssl_inst[SSL_CLIENT_INST];
        role = SSL_CLIENT;
    }
    else
    {
        PRINTF("ERROR: Invalid parameter: %s" NL, argv[2]);
        return A_ERROR;
    }

    if (ssl->sslCtx != NULL)
    {
        PRINTF("ERROR: one SSL context only!" NL);
        return A_ERROR;
    }

    // Create SSL context
    memset(ssl, 0, sizeof(SSL_INST));
    ssl->role   = role;
    ssl->sslCtx = qcom_SSL_ctx_new(role, SSL_INBUF_SIZE, SSL_OUTBUF_SIZE, 0);
    if (ssl->sslCtx == NULL)
    {
        PRINTF("ERROR: Unable to create SSL context" NL);
        return A_ERROR;
    }

    // Reset config struct
    memset(&ssl->config, 0, sizeof(SSL_CONFIG));

    // Done
    PRINTF("SSL %s started" NL, argv[2]);
    return A_OK;
}

static int32_t ssl_stop(int32_t argc, char *argv[])
{
    SSL_INST *ssl;
    int32_t res = A_OK;
    SSL_ROLE_T role;

    if (IS_DRIVER_READY != A_OK)
    {
        PRINTF("Driver not loaded!" NL);
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("ERROR: Incomplete params" NL);
        return A_ERROR;
    }

    if (0 == strcmp(argv[2], "server"))
    {
        ssl  = &ssl_inst[SSL_SERVER_INST];
        role = SSL_SERVER;
    }
    else if (0 == strcmp(argv[2], "client"))
    {
        ssl  = &ssl_inst[SSL_CLIENT_INST];
        role = SSL_CLIENT;
    }
    else
    {
        PRINTF("ERROR: Invalid parameter: %s" NL, argv[2]);
        return A_ERROR;
    }

    if (ssl->sslCtx == NULL || role != ssl->role)
    {
        PRINTF("ERROR: SSL %s not started" NL, argv[2]);
        return A_ERROR;
    }

    if (ssl->ssl)
    {
        qcom_SSL_shutdown(ssl->ssl);
        ssl->ssl = NULL;
    }

    if (ssl->sslCtx)
    {
        qcom_SSL_ctx_free(ssl->sslCtx);
        ssl->sslCtx = NULL;
    }

    PRINTF("SSL %s stopped" NL, argv[2]);
    return res;
}

static int32_t ssl_config(int32_t argc, char *argv[])
{
    SSL_INST *ssl;
    int32_t res;
    int argn, cipher_count = 0;

    if (IS_DRIVER_READY != A_OK)
    {
        PRINTF("Driver not loaded!" NL);
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("ERROR: Incomplete params" NL);
    }

    if (0 == strcmp(argv[2], "server"))
    {
        ssl = &ssl_inst[SSL_SERVER_INST];
    }
    else if (0 == strcmp(argv[2], "client"))
    {
        ssl = &ssl_inst[SSL_CLIENT_INST];
    }
    else
    {
        PRINTF("ERROR: Invalid parameter: %s" NL, argv[2]);
        return A_ERROR;
    }

    if (ssl->sslCtx == NULL)
    {
        PRINTF("ERROR: SSL %s not started" NL, argv[2]);
        return A_ERROR;
    }

    argn            = 3;
    ssl->config_set = 0;
    while (argn < argc)
    {
        if (argn == argc - 1)
        {
            PRINTF("ERROR: Incomplete params" NL);
            return A_ERROR;
        }

        if (0 == strcmp("protocol", argv[argn]))
        {
            // Setting of protocol option is supported for SSL client only
            if (strcmp(argv[2], "client"))
            {
                PRINTF("ERROR: Protocol option is not supported for %s" NL, argv[2]);
                return A_ERROR;
            }
            argn++;
            if (0 == strcmp("SSL3", argv[argn]))
            {
                ssl->config.protocol = SSL_PROTOCOL_SSL_3_0;
            }
            else if (0 == strcmp("TLS1.0", argv[argn]))
            {
                ssl->config.protocol = SSL_PROTOCOL_TLS_1_0;
            }
            else if (0 == strcmp("TLS1.1", argv[argn]))
            {
                ssl->config.protocol = SSL_PROTOCOL_TLS_1_1;
            }
            else if (0 == strcmp("TLS1.2", argv[argn]))
            {
                ssl->config.protocol = SSL_PROTOCOL_TLS_1_2;
            }
            else
            {
                PRINTF("ERROR: Invalid protocol: %s" NL, argv[argn]);
                return A_ERROR;
            }
        }

        if (0 == strcmp("time", argv[argn]))
        {
            argn++;
            if (0 == strcmp("1", argv[argn]))
            {
                ssl->config.verify.timeValidity = 1;
            }
            else if (0 == strcmp("0", argv[argn]))
            {
                ssl->config.verify.timeValidity = 0;
            }
            else
            {
                PRINTF("ERROR: Invalid option: %s" NL, argv[argn]);
                return A_ERROR;
            }
        }

        if (0 == strcmp("alert", argv[argn]))
        {
            argn++;
            if (0 == strcmp("1", argv[argn]))
            {
                ssl->config.verify.sendAlert = 1;
            }
            else if (0 == strcmp("0", argv[argn]))
            {
                ssl->config.verify.sendAlert = 0;
            }
            else
            {
                PRINTF("ERROR: Invalid option: %s" NL, argv[argn]);
                return A_ERROR;
            }
        }

        if (0 == strcmp("domain", argv[argn]))
        {
            argn++;
            if (0 == strcmp("0", argv[argn]))
            {
                ssl->config.verify.domain = 0;
                ssl->config.matchName[0]  = '\0';
            }
            else
            {
                ssl->config.verify.domain = 1;
                if (strlen(argv[argn]) >= sizeof(ssl->config.matchName))
                {
                    PRINTF("ERROR: %s too long (max %d chars)" NL, argv[argn], sizeof(ssl->config.matchName));
                    return A_ERROR;
                }
                strncpy(ssl->config.matchName, argv[argn], sizeof(ssl->config.matchName) - 1);
            }
        }

        if (0 == strcmp("cipher", argv[argn]))
        {
            argn++;
            if (cipher_count == 0)
            {
                memset((void *)ssl->config.cipher, 0, sizeof(ssl->config.cipher));
            }
            if (cipher_count == SSL_CIPHERSUITE_LIST_DEPTH)
            {
                PRINTF("ERROR: Too many cipher options (max %d)" NL, SSL_CIPHERSUITE_LIST_DEPTH);
                return A_ERROR;
            }
            if (0 == strcmp("TLS_RSA_WITH_AES_256_GCM_SHA384", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_256_GCM_SHA384;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_256_CBC_SHA256", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_256_CBC_SHA256;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_256_CBC_SHA", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_256_CBC_SHA;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_128_GCM_SHA256", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_128_GCM_SHA256;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_128_CBC_SHA256", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_128_CBC_SHA256;
            }
            else if (0 == strcmp("TLS_RSA_WITH_AES_128_CBC_SHA", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_AES_128_CBC_SHA;
            }
            else if (0 == strcmp("TLS_RSA_WITH_3DES_EDE_CBC_SHA", argv[argn]))
            {
                ssl->config.cipher[cipher_count] = TLS_RSA_WITH_3DES_EDE_CBC_SHA;
            }
            else
            {
                PRINTF("ERROR: Invalid protocol: %s" NL, argv[argn]);
                return A_ERROR;
            }
            cipher_count++;
        }

        argn++;
    }

    if (ssl->ssl == NULL)
    {
        // Create SSL connection object
        ssl->ssl = qcom_SSL_new(ssl->sslCtx);
        if (ssl->ssl == NULL)
        {
            PRINTF("ERROR: SSL configure failed (Unable to create SSL context)" NL);
            return A_ERROR;
            ;
        }
    }

    // configure the SSL connection
    ssl->config_set = 1;
    res             = qcom_SSL_configure(ssl->ssl, &ssl->config);
    if (res < A_OK)
    {
        PRINTF("ERROR: SSL configure failed (%d)" NL, res);
        return A_ERROR;
    }
    PRINTF("SSL %s configuration changed" NL, argv[2]);
    return A_OK;
}

static int32_t ssl_add_cert(int32_t argc, char *argv[])
{
    SSL_INST *ssl;
    SSL_CERT_TYPE_T type;
    char *name = NULL;

    if (IS_DRIVER_READY != A_OK)
    {
        PRINTF("Driver not loaded!" NL);
        return A_ERROR;
    }

    if (argc < 4)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }

    if (0 == strcmp(argv[2], "server"))
    {
        ssl = &ssl_inst[SSL_SERVER_INST];
        if (ssl->sslCtx == NULL)
        {
            PRINTF("ERROR: SSL server not started" NL);
            return A_ERROR;
        }
    }
    else if (0 == strcmp(argv[2], "client"))
    {
        ssl = &ssl_inst[SSL_CLIENT_INST];
        if (ssl->sslCtx == NULL)
        {
            PRINTF("ERROR: SSL client not started" NL);
            return A_ERROR;
        }
    }
    else
    {
        PRINTF("ERROR: Invalid parameter: %s" NL, argv[2]);
        return A_ERROR;
    }

    if (0 == strcmp("certificate", argv[3]))
    {
        type = SSL_CERTIFICATE;
    }
    else if (0 == strcmp("calist", argv[3]))
    {
        type = SSL_CA_LIST;
    }
    else
    {
        PRINTF("ERROR: Invalid parameter: %s" NL, argv[3]);
        return A_ERROR;
    }

    if (argc > 4)
    {
        name = argv[4];
    }

    // Load/add certificate
    if (name != NULL)
    {
        if (qcom_SSL_loadCert(ssl->sslCtx, type, name) < A_OK)
        {
            PRINTF("ERROR: Unable to load %s from FLASH" NL, name);
            return A_ERROR;
        }
        PRINTF("%s loaded from FLASH" NL, name);
    }
    else
    {
        uint8_t *cert     = ssl_cert_data_buf;
        uint16_t cert_len = ssl_cert_data_buf_len;
        if (type == SSL_CERTIFICATE)
        {
            if (cert_len == 0)
            {
                // Load the default certificate
                PRINTF("Note: using the default certificate" NL);
                cert     = (uint8_t *)ssl_default_cert;
                cert_len = ssl_default_cert_len;
            }
            if (qcom_SSL_addCert(ssl->sslCtx, cert, cert_len) < A_OK)
            {
                PRINTF("ERROR: Unable to add certificate" NL);
                return A_OK;
            }
            PRINTF("Certificate added to SSL server" NL);
        }
        else
        {
            if (cert_len == 0)
            {
                // Load the default CA list
                PRINTF("Note: using the default CA list" NL);
                cert     = (uint8_t *)ssl_default_calist;
                cert_len = ssl_default_calist_len;
            }
            if (qcom_SSL_setCaList(ssl->sslCtx, cert, cert_len) < A_OK)
            {
                PRINTF("ERROR: Unable to set CA list" NL);
                return A_ERROR;
            }
            PRINTF("CA list added to SSL client" NL);
        }
    }
    return A_OK;
}

static int32_t ssl_store_cert(int32_t argc, char *argv[])
{
    char *name;

    if (IS_DRIVER_READY != A_OK)
    {
        PRINTF("Driver not loaded!" NL);
        return A_ERROR;
    }

    if (argc < 3)
    {
        PRINTF("Incomplete params" NL);
        return A_ERROR;
    }
    name = argv[2];

    if (ssl_cert_data_buf_len == 0)
    {
        PRINTF("ERROR: no certificate data" NL
               "Hint: Use the wmiconfig --ssl_get_cert to read a certificate from a certificate server." NL);
        return A_ERROR;
    }
    if (A_OK == qcom_SSL_storeCert(name, ssl_cert_data_buf, ssl_cert_data_buf_len))
    {
        PRINTF("%s is stored in FLASH" NL, name);
        return A_OK;
    }
    else
    {
        PRINTF("ERROR: failed to store %s" NL, name);
        return A_ERROR;
    }
}

static int32_t ssl_delete_cert(int32_t argc, char *argv[])
{
    char *name;

    if (IS_DRIVER_READY != A_OK)
    {
        PRINTF("Driver not loaded!" NL);
        return A_ERROR;
    }

    if (argc < 3)
    {
        ssl_cert_data_buf_len = 0;
        PRINTF("Deleted the certificate data stored in RAM." NL);
        return A_OK;
    }
    name = argv[2];

    if (A_OK == qcom_SSL_storeCert(name, NULL, 0))
    {
        PRINTF("Deleted %s from FLASH" NL, name);
        return A_OK;
    }
    else
    {
        PRINTF("ERROR: failed to delete %s" NL, name);
        return A_ERROR;
    }
}

static int32_t ssl_list_cert(int32_t argc, char *argv[])
{
    SSL_FILE_NAME_LIST *fileNames;
    int32_t i, numFiles, namesLen = sizeof(SSL_FILE_NAME_LIST); // 10 files

    if (IS_DRIVER_READY != A_OK)
    {
        PRINTF("Driver not loaded!" NL);
        return A_ERROR;
    }

    fileNames = A_MALLOC(namesLen, MALLOC_ID_TEMPORARY);
    if (fileNames == NULL)
    {
        PRINTF("ERROR: not enough memory" NL);
        return A_ERROR;
    }

    numFiles = SSL_listCert(fileNames);
    if (numFiles < 0)
    {
        PRINTF("ERROR: failed to list files (%d)" NL, numFiles);
        A_FREE(fileNames, MALLOC_ID_TEMPORARY);
        return A_ERROR;
    }

    PRINTF("%d %s stored in FLASH" NL, numFiles, numFiles == 1 ? "file" : "files");
    for (i = 0; i < numFiles; i++)
    {
        char str[21];
        if (fileNames->name[i][0])
        {
            strncpy(str, (char *)fileNames->name[i], sizeof(str) - 1);
            str[sizeof(str) - 1] = '\0';
            PRINTF("%s" NL, str);
        }
    }

    if (NULL != fileNames)
    {
        A_FREE(fileNames, MALLOC_ID_TEMPORARY);
        fileNames = NULL;
    }

    return A_OK;
}
#endif // ENABLE_SSL

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : qcom_get_wps_net_info()
 * Parames:
 *        void
 * Returned Value  : ATH_NETPARAMS * - the structure point to netinfo
 *		    NULL - on any failure.
 * Comments        :
 *                   Not supported on RTCS
 *END*-----------------------------------------------------------------*/
ATH_NETPARAMS *qcom_get_wps_net_info(void)
{
    ATH_NETPARAMS *pNetparams = &wps_context.netparams;
    if (pNetparams->error)
        return NULL;
    else
        return pNetparams;
}
#if ENABLE_HTTP_CLIENT
/*
Command will be
httpc get 192.1.168.1.10 /index.html
*/
/************************************************************************
 * NAME: httpc_get_page
 *
 * DESCRIPTION:
 ************************************************************************/
HTTPC_PARAMS httpc; // Since stack size is less

static int32_t httpc_command_parser(int32_t argc, char_ptr argv[])
{ /* Body */
    int32_t return_code = A_OK;

    if (argc < 3)
    {
        /*Incorrect number of params, exit*/
        return_code = A_ERROR;
    }
    else
    {
        memset((void *)&httpc, 0, sizeof(HTTPC_PARAMS));

        if (strlen((char *)httpc.url) >= 64)
        {
            PRINTF("Maximum 64 bytes supported as argument" NL);
            return A_ERROR;
        }

        if (argc >= 4)
        {
            strncpy((char *)httpc.url, argv[3], sizeof(httpc.url) - 1);
        }

        if (argc >= 5)
        {
            strncpy((char *)httpc.data, argv[4], sizeof(httpc.data));
        }

        if (ATH_STRCMP(argv[2], "connect") == 0)
        {
            httpc.command = HTTPC_CONNECT_CMD;

            if (strncmp(argv[3], "https://", 8) == 0)
            {
                int size = sizeof(httpc.url) - 8;
                memmove(httpc.url, httpc.url + 8, size);
                httpc.command = HTTPC_CONNECT_SSL_CMD;
            }
            else if (strncmp(argv[3], "http://", 7) == 0)
            {
                int size = sizeof(httpc.url) - 7;
                memmove(httpc.url, httpc.url + 7, size);
            }

            if (argc >= 4)
                httpc_connect(&httpc);
            else
                return_code = A_ERROR;
        }
        else if (ATH_STRCMP(argv[2], "get") == 0)
        {
            httpc.command = HTTPC_GET_CMD;
            if (argc >= 4)
                httpc_method(&httpc);
            else
                return_code = A_ERROR;
        }
        else if (ATH_STRCMP(argv[2], "post") == 0)
        {
            httpc.command = HTTPC_POST_CMD;
            if (argc >= 4)
                httpc_method(&httpc);
            else
                return_code = A_ERROR;
        }
        else if (ATH_STRCMP(argv[2], "query") == 0)
        {
            httpc.command = HTTPC_DATA_CMD;
            if (argc >= 5)
                httpc_method(&httpc);
            else
                return_code = A_ERROR;
        }
        else if (ATH_STRCMP(argv[2], "disc") == 0)
        {
            httpc.command = HTTPC_DISCONNECT_CMD;
            httpc_connect(&httpc);
        }
        else
        {
            PRINTF("Unknown Command \"%s\"" NL, argv[2]);
            return_code = A_ERROR;
        }
    }
    if (return_code == A_ERROR)
    {
        PRINTF("USAGE: wmiconfig --ip_http_client [<connect/get/post/query> <data1> <data2>]" NL);
    }
    return return_code;
} /* Endbody */

/************************************************************************
 * NAME: httpc_connect
 *
 * DESCRIPTION: Process a HTTP connect request.
 ************************************************************************/

static void httpc_connect(HTTPC_PARAMS *p_httpc)
{
    int error = A_OK;

    // PRINTF("Sedning command %s %s" NL, p_httpc->url, p_httpc->data);
    error = qcom_http_client_method(p_httpc->command, p_httpc->url, p_httpc->data, NULL);

    PRINTF("HTTPClient cmd %s" NL, (error == A_ERROR) ? "failed" : "succeeded");
}

#define MAX_HTTP_BUF_SIZE 1500
/************************************************************************
 * NAME: httpc_method
 *
 * DESCRIPTION: Process a HTTP connect request.
 *              For request, output will come as last argument
 *              Application must free the buffer using zero_copy_http_free
 ************************************************************************/
static void httpc_method(HTTPC_PARAMS *p_httpc)
{
    uint8_t *output      = NULL;
    uint32_t error       = A_OK;
    HTTPC_RESPONSE *temp = NULL;
    int i;

    output = malloc(MAX_HTTP_BUF_SIZE);
    error  = qcom_http_client_method(p_httpc->command, p_httpc->url, p_httpc->data, output);

    if (error != A_OK)
    {
        PRINTF("HTTPC Command failed" NL);
    }
    else if (output != NULL)
    {
        temp = (HTTPC_RESPONSE *)output;

        /* For GET and POST alone, print the output */
        if ((p_httpc->command == 1) || (p_httpc->command == 2))
        {
            PRINTF("Size:%u Resp_code:%u" NL NL, temp->length, temp->resp_code);
            // PRINTF("Flags %u Packet: %u" NL, temp->flags, strlen(temp->data));
            if (temp->length)
            {
                PRINTF("Packet:" NL);
                for (i = 0; i < temp->length; i++)
                    PRINTF("%c", temp->data[i]);
            }
        }
    }

    if (NULL != output)
    {
        free(output);
    }

    return;
}

#endif /* ENABLE_HTTP_CLIENT */

#if ENABLE_ROUTING_CMDS
/************************************************************************
 * NAME: ipv4_route
 *
 * DESCRIPTION: Add, delete or show IPv4 route table
 *               wmiconfig --ipv4route add <addr> <mask> <gw>
 *               wmiconfig --ipv4route del <addr> <mask> <ifindex>
 *               wmiconfig --ipv4route show
 ************************************************************************/
int32_t ipv4_route(int32_t argc, char_ptr argv[])
{ /* Body */
    int32_t error    = A_OK;
    uint32_t ifIndex = 0;
    IP_ADDR_T addr   = {0};
    IP_ADDR_T mask   = {0};
    IP_ADDR_T gw     = {0};
    uint32_t sbits;
    uint32_t command;
    IPV4_ROUTE_LIST_T routelist;
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    do
    {
        if (argc < 3)
        {
            PRINTF("incomplete params" NL);
            error = A_ERROR;
            break;
        }

        if (ATH_STRCMP(argv[2], "show") != 0)
        {
            error = parse_ipv4_ad((unsigned long *)&addr, &sbits, argv[3]);
            if (error != 0)
            {
                PRINTF("Invalid IP address" NL);
                error = A_ERROR;
                break;
            }
            addr.s_addr = A_BE2CPU32(addr.s_addr);
            error       = parse_ipv4_ad((unsigned long *)&mask, &sbits, argv[4]);
            if (error != 0)
            {
                PRINTF("Invalid Mask address" NL);
                error = A_ERROR;
                break;
            }
            mask.s_addr = A_BE2CPU32(mask.s_addr);
        }
        if (ATH_STRCMP(argv[2], "add") == 0)
        {
            command = ROUTE_ADD;
            error   = parse_ipv4_ad((unsigned long *)&gw, &sbits, argv[5]);
            if (error != 0)
            {
                PRINTF("Invalid Gateway address" NL);
                error = A_ERROR;
                break;
            }
            gw.s_addr = A_BE2CPU32(gw.s_addr);
        }
        else if (ATH_STRCMP(argv[2], "del") == 0)
        {
            command = ROUTE_DEL;
            ifIndex = atoi(argv[5]);
        }
        else if (ATH_STRCMP(argv[2], "show") == 0)
        {
            command = ROUTE_SHOW;
        }
        else
        {
            PRINTF("Unknown Command \"%s\"" NL, argv[2]);
            error = A_ERROR;
            break;
        }

        if (A_OK == qcom_ip4_route(enet_device, command, &addr, &mask, &gw, &ifIndex, &routelist))
        {
            if (command == ROUTE_SHOW)
            {
                int i        = 0;
                char *ip_str = A_MALLOC(sizeof(uint8_t) * 48, MALLOC_ID_TEMPORARY);
                if (NULL == ip_str)
                {
                    PRINTF("Out of Memory error" NL);
                }
                else
                {
                    PRINTF(NL "..IPaddr.......mask.........nexthop...iface.." NL);
                    for (i = 0; i < routelist.rtcount; i++)
                    {
                        PRINTF("%s  %s  %s %d" NL,
                               inet_ntoa(*(uint32_t *)(&routelist.route[i].address), (char *)&ip_str[0]),
                               inet_ntoa(*(uint32_t *)(&routelist.route[i].mask), (char *)&ip_str[16]),
                               inet_ntoa(*(uint32_t *)(&routelist.route[i].gateway), (char *)&ip_str[32]),
                               routelist.route[i].ifIndex);
                    }
                    A_FREE(ip_str, MALLOC_ID_TEMPORARY);
                    ip_str = NULL;
                }
            }
        }
        else
        {
            PRINTF("Route %s failed" NL, (command == 0) ? "addition" : ((command == 1) ? "deletion" : "show"));
        }
    } while (0);
    if (error == A_ERROR)
    {
        PRINTF("USAGE: wmiconfig --ipv4_route add <address> <mask> <gw>" NL);
        PRINTF("       wmiconfig --ipv4_route del <address> <mask> <ifIndex>" NL);
        PRINTF("       wmiconfig --ipv4_route show" NL);
    }
    return error;
} /* Endbody */

/************************************************************************
 * NAME: ipv6_route
 *
 * DESCRIPTION: Add, delete or show IPv4 route table
 *               wmiconfig --ipv4route add <addr> <prfx len> <gw>
 *               wmiconfig --ipv4route del <addr> <ifindex>
 *               wmiconfig --ipv4route show
 ************************************************************************/
int32_t ipv6_route(int32_t argc, char_ptr argv[])
{ /* Body */
    int32_t error    = A_OK;
    uint32_t ifIndex = 0, prefixLen = 0;
    IP6_ADDR_T ip6addr = {0};
    IP6_ADDR_T gateway = {0};
    uint32_t command;
    IPV6_ROUTE_LIST_T routelist;
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    do
    {
        if (argc < 3)
        {
            PRINTF("incomplete params" NL);
            error = A_ERROR;
            break;
        }

        if (ATH_STRCMP(argv[2], "show") != 0)
        {
            error = Inet6Pton(argv[3], &ip6addr);
            if (error != 0)
            {
                PRINTF("Invalid IP address" NL);
                error = A_ERROR;
                break;
            }

            prefixLen = atoi(argv[4]);
        }
        if (ATH_STRCMP(argv[2], "add") == 0)
        {
            command = ROUTE_ADD;
            ifIndex = atoi(argv[6]);

            error = Inet6Pton(argv[5], &gateway);
            if (error != 0)
            {
                PRINTF("Invalid Gateway address" NL);
                error = A_ERROR;
                break;
            }
        }
        else if (ATH_STRCMP(argv[2], "del") == 0)
        {
            command = ROUTE_DEL;
            ifIndex = atoi(argv[5]);
        }
        else if (ATH_STRCMP(argv[2], "show") == 0)
        {
            command = ROUTE_SHOW;
        }
        else
        {
            PRINTF("Unknown Command \"%s\"" NL, argv[2]);
            error = A_ERROR;
            break;
        }

        if (A_OK == qcom_ip6_route(enet_device, command, &ip6addr, &prefixLen, &gateway, &ifIndex, &routelist))
        {
            if (command == ROUTE_SHOW)
            {
                int i        = 0;
                char *ip_str = A_MALLOC(sizeof(uint8_t) * 48, MALLOC_ID_TEMPORARY);
                if (NULL == ip_str)
                {
                    PRINTF("Out of Memory error" NL);
                }
                else
                {
                    PRINTF(NL "..IPaddr.......prefixLen.........nexthop...iface.." NL);
                    for (i = 0; i < routelist.rtcount; i++)
                    {
                        PRINTF("%s  %d ", inet6_ntoa((char *)(&routelist.route[i].address), (char *)ip_str),
                               routelist.route[i].prefixlen);
                        PRINTF("%s  %d" NL, inet6_ntoa((char *)(&routelist.route[i].nexthop), (char *)ip_str),
                               routelist.route[i].ifindex);
                    }
                    A_FREE(ip_str, MALLOC_ID_TEMPORARY);
                    ip_str = NULL;
                }
            }
        }
        else
        {
            PRINTF("Route %s failed" NL, (command == 0) ? "addition" : ((command == 1) ? "deletion" : "show"));
        }
    } while (0);

    if (error == A_ERROR)
    {
        PRINTF("USAGE: wmiconfig --ipv6_route add <address> <prefixLen> <gw>" NL);
        PRINTF("       wmiconfig --ipv6_route del <address> <prefixLen> <ifIndex>" NL);
        PRINTF("       wmiconfig --ipv6_route show" NL);
    }
    return error;
} /* Endbody */

#endif /* ENABLE_ROUTING_CMDS */

static int32_t ota_upgrade(int32_t argc, char *argv[])
{
    uint32_t addr;
    uint32_t sbits;
    char filename[128];
    uint8_t mode;
    uint8_t preserve_last;
    uint8_t protocol;
    int error;
    tOtaUpgradeResp resp;
    uint32_t enet_device;

    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 6)
    {
        PRINTF("Incomplete params" NL);
        PRINTF("Usage: wmiconfig --ota_upgrade <ip> <filename> <mode> <preseve_last> <protocol>" NL);
        return A_ERROR;
    }

    error = parse_ipv4_ad((unsigned long *)&addr, &sbits, argv[2]);
    if (error != 0)
    {
        PRINTF("Invalid IP address" NL);
        return error;
    }
    addr = A_BE2CPU32(addr);

    if (strlen(argv[3]) > 128)
    {
        PRINTF("Error: Invalid Filename Length" NL);
        return A_ERROR;
    }
    memset(filename, '\0', strlen(argv[3]) + 1);
    strncpy(filename, argv[3], strlen(argv[3]));

    mode = strtol(argv[4], NULL, 10);

    if (mode > 2)
    {
        PRINTF("Error: Invalid Mode [valid values are 0 or 1 ]" NL);
        return A_ERROR;
    }

    preserve_last = strtol(argv[5], NULL, 10);

    if (preserve_last > 2)
    {
        PRINTF("Error: Invalid preserve_last [valid values are 0 or 1 ]" NL);
        return A_ERROR;
    }
    protocol = strtol(argv[6], NULL, 10);

    if (protocol > 2)
    {
        PRINTF("Error: Invalid protocol [valid values are 0 or 1 ]" NL);
        return A_ERROR;
    }

    enet_device = get_active_device();

    if (A_OK !=
        qcom_ota_upgrade(enet_device, addr, filename, mode, preserve_last, protocol, &(resp.resp_code), &(resp.size)))
    {
        PRINTF("OTA Download Failed" NL);
        return A_ERROR;
    }

    if (resp.resp_code == 0)
    {
        PRINTF("OTA Image downloaded, Image Size:%d" NL, resp.size);
    }
    else
    {
        PRINTF("OTA Download Failed ERR:%d" NL, resp.resp_code);
    }

    return A_OK;
}

static int32_t ota_read(int32_t argc, char *argv[])
{
    uint32_t offset;
    uint32_t len;
    unsigned char *databuffer;
    uint32_t size;

    if (argc < 3)
    {
        PRINTF("Incomplete Params" NL "Usage: wmiconfig --ota_read <offset> <size>!" NL);
        PRINTF("e.g wmiconfig --ota_read 100 20 to read 20 bytes from and offset 100 w.r.t start of the image!" NL);
        return A_ERROR;
    }

    databuffer = A_MALLOC(sizeof(unsigned char) * MAX_OTA_AREA_READ_SIZE, MALLOC_ID_TEMPORARY);

    offset = atoi(argv[2]);

    len = atoi(argv[3]);

    if (A_OK != qcom_ota_read_area(offset, len, databuffer, &size))
    {
        PRINTF("OTA Download Failed" NL);
        A_FREE(databuffer, MALLOC_ID_TEMPORARY);
        return A_ERROR;
    }
    else
    {
        PRINTF("Read %d bytes from OTA Area" NL, size);
        uint32_t index;
        for (index = 0; index < size; index++)
        {
            PRINTF("%02x ", databuffer[index]);
            if (!((index + 1) % 16))
            {
                PRINTF("" NL);
            }
        }
    }
    A_FREE(databuffer, MALLOC_ID_TEMPORARY);
    return A_OK;
}

static int32_t ota_done(int32_t argc, char *argv[])
{
    bool good_image;

    if (argc < 2)
    {
        PRINTF("Incomplete Params" NL "Usage: wmiconfig --ota_done <good_image>" NL);
        return A_ERROR;
    }
    good_image = atoi(argv[2]);
#ifndef USE_QCOM_REL_3_3_API
    if (A_OK != qcom_ota_done(good_image))
#else
    if (A_OK != qcom_ota_done())
#endif
    {
        PRINTF("OTA Download Complete Command Failed" NL);
        return A_ERROR;
    }
    else
    {
        PRINTF("OTA Download Completed Successfully" NL);
    }

    return A_OK;
}

int32_t ota_cust(int32_t argc, char *argv[])
{
    uint32_t len, i, offset = 0;
    uint32_t cmd, rtn = 0;
    if (argc < 3)
        return A_ERROR;
    cmd = atoi(argv[2]);

    switch (cmd)
    {
        case 0:
            // ota session start
            {
                uint32_t flags, partition_indx;
                if (argc < 5)
                {
                    PRINTF("Invalid Params" NL "Usage: wmiconfig --ota_cust 0 <flags> <partition_index>" NL);
                    return A_ERROR;
                }
                flags          = atoi(argv[3]);
                partition_indx = atoi(argv[4]);
                rtn            = qcom_ota_session_start(flags, partition_indx);
            }
            break;
        case 1:
            // ota partition get size
            rtn = qcom_ota_partition_get_size();
            break;
        case 2:
            // ota partition erase
            rtn = qcom_ota_partition_erase();
            break;
        case 3:
            // ota partion parse image hdr
            {
#define HDR_LEN 24
                uint8_t buf[HDR_LEN];
                if ((argc < 4) || (strlen(argv[3]) != HDR_LEN * 2))
                {
                    PRINTF("Invalid Params" NL "Usage: wmiconfig --ota_cust 3 <xx...xx>" NL);
                    return A_ERROR;
                }
                for (i = 0; i < HDR_LEN; i++)
                {
                    if (mystrtobyte(&argv[3][i * 2], &buf[i]) != 0)
                    {
                        PRINTF("data format is not correct" NL);
                        return A_ERROR;
                    }
                }
                rtn = qcom_ota_parse_image_hdr(buf, &offset);
                PRINTF("offset: %d" NL, offset);
            }
            break;
        case 4:
            // ota partition write data
            {
#define BUF_LEN 50
                uint8_t buf[BUF_LEN];

                if (argc < 6)
                {
                    PRINTF("Incomplete Params" NL "Usage: wmiconfig --ota_cust 4 <offset> <len> <xx...xx>" NL);
                    return A_ERROR;
                }

                offset = atoi(argv[3]);
                len    = atoi(argv[4]);
                if (len > BUF_LEN)
                {
                    PRINTF("data must be less than 50 bytes here" NL);
                    return A_ERROR;
                }
                if (strlen(argv[5]) != len * 2)
                {
                    PRINTF("data format is not correct" NL);
                    return A_ERROR;
                }

                for (i = 0; i < len; i++)
                {
                    if (mystrtobyte(&argv[5][i * 2], &buf[i]) != 0)
                    {
                        PRINTF("data format is not correct" NL);
                        return A_ERROR;
                    }
                }
                qcom_ota_partition_write_data(offset, buf, len, &rtn);
            }
            break;
        case 5:
            // ota partition verify checksum
            rtn = qcom_ota_partition_verify_checksum();
            if (rtn != QCOM_OTA_OK)
            {
                PRINTF("Checksum verification failed:%d" NL, rtn);
                return A_ERROR;
            }
            break;
        case 6:
            // ota read
            {
                uint32_t offset;
                uint32_t len, total_read = 0;
                unsigned char *data;
                uint32_t ret_len = 0;

                if (argc < 5)
                {
                    PRINTF("Incomplete Params" NL "Usage: wmiconfig --ota_custom 6 <offset> <size>!" NL);
                    PRINTF(
                        "e.g wmiconfig --ota_cust 6 100 20 to read 20 bytes from offset 100 w.r.t start of the "
                        "image!" NL);
                    return A_ERROR;
                }

                data = A_MALLOC(sizeof(unsigned char) * MAX_OTA_AREA_READ_SIZE, MALLOC_ID_TEMPORARY);
                if (!data)
                {
                    PRINTF("Insufficient memory" NL);
                    return A_ERROR;
                }

                offset = atoi(argv[3]);
                len    = atoi(argv[4]);

                PRINTF("Partition data:" NL);

                while (total_read < len)
                {
                    uint32_t index;
                    uint32_t bytes_to_read;

                    if (len - total_read > MAX_OTA_AREA_READ_SIZE)
                    {
                        bytes_to_read = MAX_OTA_AREA_READ_SIZE;
                    }
                    else
                    {
                        bytes_to_read = len - total_read;
                    }

                    if ((uint32_t)-1 == (ret_len = qcom_ota_partition_read_data(offset, data, bytes_to_read)))
                    {
                        PRINTF("OTA Read Error" NL);
                        rtn = (uint32_t)A_ERROR;
                        break;
                    }

                    for (index = 0; index < ret_len; index++)
                    {
                        if ((index & 15) == 0)
                        {
                            PRINTF("" NL);
                        }
                        PRINTF("%02x ", data[index]);
                    }

                    total_read += ret_len;
                    offset += ret_len;
                }

                A_FREE(data, MALLOC_ID_TEMPORARY);
                return A_OK;
            }
            // break;

        case 7:
            // ota session end
            {
                uint32_t good_image;
                if (argc < 4)
                {
                    PRINTF("Invalid Params" NL "Usage: wmiconfig --ota_cust 7 <good_image>" NL);
                    return A_ERROR;
                }
                good_image = atoi(argv[3]);
                rtn        = qcom_ota_session_end(good_image ? 1 : 00);
            }
            break;
    }

    PRINTF("%d" NL, rtn);
    return A_OK;
}

int32_t ota_format(int32_t argc, char *argv[])
{
    int32_t rtn;
    uint32_t indx;

    if (argc < 3)
    {
        PRINTF("Invalid Params" NL "Usage: wmiconfig --ota_format <partition_index>" NL);
        return A_ERROR;
    }

    indx = atoi(argv[2]);
    rtn  = qcom_ota_partition_format(indx);
    if (rtn == QCOM_OTA_OK)
    {
        PRINTF("OTA Success");
    }
    else
    {
        PRINTF("OTA Err:%d" NL, rtn);
    }

    return A_OK;
}

/*****************************************************************************************************************************/
/*                                                                                                                           */
/*       OTA_FTP */
/*                                                                                                                           */
/*****************************************************************************************************************************/
#define OTA_FTP_DATA_PORT 1068
#define OTA_FTP_BUF_SIZE MAX_OTA_AREA_READ_SIZE
#define OTA_FTP_INCOMING_CONNECT_TIMEOUT (20 * 1000)
#define OTA_FTP_WAIT_TIME (50)

/*
 * OTA FTP Status codes
 */
typedef enum
{
    QCOM_OTA_ERR_FTP_CONNECT_FAIL = 2000,
    QCOM_OTA_ERR_FTP_BIND_FAIL,
    QCOM_OTA_ERR_FTP_PEER_CLOSED,
    QCOM_OTA_ERR_FTP_SEND_COMMAND,
    QCOM_OTA_ERR_FTP_LISTEN_DATA_PORT,
    QCOM_OTA_ERR_FTP_DATA_CONNECTION_TIMEOUT,
    QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT,
    QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE,
    QCOM_OTA_ERR_FTP_SYST,
    QCOM_OTA_ERR_FTP_LOGIN_INCORRECT,
    QCOM_OTA_ERR_FTP_SET_TYPE,
    QCOM_OTA_ERR_FTP_SET_PORT,
    QCOM_OTA_ERR_FTP_FILE_NOT_FOUND,

} QCOM_OTA_FTP_STATUS_CODE_t;

//#define DEBUG_OTA_FTP_PRINTF
#if defined(DEBUG_OTA_FTP_PRINTF)
#define OTA_FTP_D_PRINTF(args...) PRINTF(args)
#else
#define OTA_FTP_D_PRINTF(args...)
#endif

typedef struct mem_link
{
    struct mem_link *next;
    uint32_t offset;
    uint32_t len;
    uint8_t *buf;
} QCOM_OTA_MEM;

typedef struct
{
    uint32_t data_sock; /* data socket.*/
    uint32_t peer_sock;
    uint32_t control_sock; /* control socket.*/
    _ip_address remote_ip_address;
    _ip_address local_ip_address;
    uint16_t cmd_port;
    uint16_t data_port;
    uint32_t flags;
    uint32_t partition_index;
    QCOM_OTA_MEM *first;
    QCOM_OTA_MEM *last;
    QCOM_OTA_MEM *cur;
    uint32_t parse_hdr_pending;
    uint32_t write_pending;
    uint32_t write_flag;
    uint32_t total_write;
    uint32_t total_write_rsp;
} QCOM_OTA_FTP_SESSION_INFO;

int32_t qcom_ota_ftp_upgrade(_ip_address ip_addr,
                             uint16_t port,
                             char *user_name,
                             char *password,
                             char *filename,
                             uint32_t flags,
                             uint32_t partition_index,
                             uint32_t *size);
int32_t ota_ftp_init(void);
int32_t ota_ftp_finit(void);

int32_t ota_ftp_open_control_sock(_ip_address ip_address, uint16_t port);
int32_t ota_ftp_close_control_sock(void);
int32_t ota_ftp_open_data_sock(_ip_address ip_address, uint16_t port);
int32_t ota_ftp_close_data_sock(void);
int32_t ota_ftp_send_cmd(char *cmd);
int32_t ota_ftp_recv_cmd(int *resp_code);
int32_t ota_ftp_send_cmd_resp(char *cmd, int *resp_code);
int32_t ota_ftp_program_ota(uint32_t *size);
int32_t ota_ftp_free_pkt(QCOM_OTA_MEM *pPkt);
int32_t ota_ftp_clear_pkt(void);

/* OTA FTP Global */
QCOM_OTA_FTP_SESSION_INFO *ota_ftp_sess = NULL;
uint32_t ota_cmd, ota_resp, ota_result;

void ota_response_callback(uint32_t cmd, uint32_t resp_code, uint32_t result)
{
    OTA_FTP_D_PRINTF("ota_cb cmd:%d,resp:%d, result:%d" NL, cmd, resp_code, result);
    ota_cmd    = cmd;
    ota_resp   = resp_code;
    ota_result = result;
    if (ota_ftp_sess != NULL)
    {
        PRINTF(".");
        if (cmd == SOCK_OTA_PARTITION_WRITE_DATA)
        {
            ota_ftp_sess->total_write_rsp += result;
            if (resp_code != QCOM_OTA_OK)
            {
                OTA_FTP_D_PRINTF("ota write resp with error: %d", resp_code);
                ota_ftp_sess->write_flag = resp_code;
            }
        }
    }
}

int32_t ota_ftp_init(void)
{
    uint32_t enet_device, addr = 0, mask = 0, gw = 0;
    uint16_t sock_port;

    OTA_FTP_D_PRINTF("FTP Init...." NL);

    ota_ftp_sess = malloc(sizeof(QCOM_OTA_FTP_SESSION_INFO));
    if (!ota_ftp_sess)
    {
        return A_ERROR;
    }
    memset(ota_ftp_sess, '\0', sizeof(QCOM_OTA_FTP_SESSION_INFO));

    ota_ftp_sess->control_sock = (uint32_t)A_ERROR;
    ota_ftp_sess->peer_sock    = (uint32_t)A_ERROR;
    ota_ftp_sess->data_sock    = (uint32_t)A_ERROR;

    ota_ftp_sess->first = NULL;
    ota_ftp_sess->last  = NULL;
    ota_ftp_sess->cur   = NULL;

    enet_device = get_active_device();
    qcom_ipconfig(enet_device, QCOM_IPCONFIG_QUERY, &addr, &mask, &gw);

    ota_ftp_sess->local_ip_address = addr;

    sock_port = rand() % 0x7fff;
    if (sock_port < OTA_FTP_DATA_PORT)
        sock_port += OTA_FTP_DATA_PORT;
    ota_ftp_sess->data_port = sock_port;

    if ((ota_ftp_sess->control_sock = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
    {
        PRINTF("ERROR: Unable to create ftp control socket" NL);
        goto error_1;
    }

    if ((ota_ftp_sess->data_sock = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
    {
        PRINTF("ERROR: Unable to create ftp data socket" NL);
        goto error_1;
    }

    qcom_ota_set_callback((void *)ota_response_callback);
    return QCOM_OTA_OK;
error_1:
    ota_ftp_finit();
    return A_ERROR;
}

int32_t ota_ftp_finit(void)
{
    ota_ftp_close_data_sock();
    ota_ftp_close_control_sock();

    qcom_ota_set_callback((void *)NULL);
    ota_ftp_clear_pkt();

    /* Clean up and free all resources */
    if (ota_ftp_sess != NULL)
    {
        free(ota_ftp_sess);
        ota_ftp_sess = NULL;
    }

    return QCOM_OTA_OK;
}

QCOM_OTA_MEM *ota_ftp_fetch_pkt(void)
{
    QCOM_OTA_MEM *tmp;
    tmp = ota_ftp_sess->first;
    if (tmp != NULL)
    {
        ota_ftp_sess->first = tmp->next;
        if (ota_ftp_sess->first == NULL)
        {
            ota_ftp_sess->last = NULL;
        }
    }
    return tmp;
}

int32_t ota_ftp_append_pkt(uint8_t *buf, uint32_t len)
{
    QCOM_OTA_MEM *tmp;
    tmp = A_MALLOC(sizeof(QCOM_OTA_MEM) + len, MALLOC_ID_CONTEXT);
    if (tmp == NULL)
    {
        OTA_FTP_D_PRINTF("Out of memory error" NL);
        return A_ERROR;
    }

    tmp->len    = len;
    tmp->offset = 0;
    tmp->next   = NULL;
    tmp->buf    = (uint8_t *)tmp + sizeof(QCOM_OTA_MEM);
    A_MEMCPY(tmp->buf, buf, len);
    if (ota_ftp_sess->last == NULL)
    {
        ota_ftp_sess->first = tmp;
        ota_ftp_sess->last  = tmp;
    }
    else
    {
        ota_ftp_sess->last->next = tmp;
        ota_ftp_sess->last       = tmp;
    }

    return A_OK;
}

int32_t ota_ftp_free_pkt(QCOM_OTA_MEM *pPkt)
{
    A_FREE(pPkt, MALLOC_ID_CONTEXT);
    return A_OK;
}

int32_t ota_ftp_clear_pkt(void)
{
    QCOM_OTA_MEM *tmp;
    while (ota_ftp_sess->first != NULL)
    {
        tmp                 = ota_ftp_sess->first;
        ota_ftp_sess->first = tmp->next;
        A_FREE(tmp, MALLOC_ID_CONTEXT);
    }
    ota_ftp_sess->last = NULL;
    return A_OK;
}

int32_t ota_ftp_close_data_sock(void)
{
    if (ota_ftp_sess->data_sock != (uint32_t)A_ERROR)
    {
        qcom_socket_close(ota_ftp_sess->data_sock);
        ota_ftp_sess->data_sock = (uint32_t)A_ERROR;
    }
    return QCOM_OTA_OK;
}

int32_t ota_ftp_open_data_sock(_ip_address ip_addr, uint16_t port)
{
    SOCKADDR_T local_addr;
    int32_t rtn = QCOM_OTA_OK;

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_addr.s_addr = ip_addr;
    local_addr.sin_port        = port;
    local_addr.sin_family      = ATH_AF_INET;

    OTA_FTP_D_PRINTF("connect ftp data port ip:%xh port:%d" NL, ip_addr, port);

    /* bind to local port.*/
    if (qcom_bind(ota_ftp_sess->data_sock, (struct sockaddr *)(&local_addr), sizeof(local_addr)) == A_ERROR)
    {
        PRINTF("socket bind failed." NL);
        rtn = QCOM_OTA_ERR_FTP_BIND_FAIL;
    }

    return rtn;
}

int32_t ota_ftp_close_control_sock(void)
{
    OTA_FTP_D_PRINTF("close ftp control connection" NL);

    if (ota_ftp_sess->control_sock != (uint32_t)A_ERROR)
    {
        qcom_socket_close(ota_ftp_sess->control_sock);
        ota_ftp_sess->control_sock = (uint32_t)A_ERROR;
    }
    return QCOM_OTA_OK;
}

int32_t ota_ftp_open_control_sock(_ip_address ip_addr, uint16_t port)
{
    SOCKADDR_T foreign_addr;
    int32_t rtn = QCOM_OTA_OK;

    OTA_FTP_D_PRINTF("FTP Connecting ip:%xh port:%d" NL, ip_addr, port);

    ota_ftp_sess->remote_ip_address = ip_addr;
    ota_ftp_sess->cmd_port          = port;

    memset(&foreign_addr, 0, sizeof(foreign_addr));
    foreign_addr.sin_addr.s_addr = ip_addr;
    foreign_addr.sin_port        = port;
    foreign_addr.sin_family      = ATH_AF_INET;

    /* Connect to the server.*/
    if (qcom_connect(ota_ftp_sess->control_sock, (struct sockaddr *)(&foreign_addr), sizeof(foreign_addr)) == A_ERROR)
    {
        PRINTF("Connection failed." NL);
        rtn = QCOM_OTA_ERR_FTP_CONNECT_FAIL;
    }

    return rtn;
}

/* return: send length */
int32_t ota_ftp_send_cmd(char *cmd)
{
    char *buf;
    int len, rtn;

    OTA_FTP_D_PRINTF("send: %s", cmd);
    len = strlen(cmd);
    while ((buf = CUSTOM_ALLOC(len)) == NULL)
    {
        // Wait till we get a buffer
        /*Allow small delay to allow other thread to run*/
        A_MDELAY(SMALL_TX_DELAY);
    }

    A_MEMCPY(buf, cmd, len);
    rtn = qcom_send(ota_ftp_sess->control_sock, buf, len, 0);
#if !NON_BLOCKING_TX
    /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
    if (buf)
        CUSTOM_FREE(buf);
#endif
    return rtn;
}

int32_t ota_ftp_recv_cmd(int *resp_code)
{
    int32_t conn_sock, received = 0;
    int32_t rtn = QCOM_OTA_OK;
#if ZERO_COPY
    char *buffer = NULL;
#else
    char buffer[OTA_FTP_BUF_SZIE] = {0};
#endif
    *resp_code = 0;
    while (1) /* Receiving data.*/
    {
        conn_sock = t_select((void *)handle, ota_ftp_sess->control_sock, UDP_CONNECTION_WAIT_TIME);
        if (conn_sock == A_OK)
        {
/*Packet is available, receive it*/
#if ZERO_COPY
            received = qcom_recv(ota_ftp_sess->control_sock, (char **)(&buffer), OTA_FTP_BUF_SIZE, 0);
#else
            received = qcom_recv(ota_ftp_sess->control_sock, (char *)(&buffer[0]), OTA_FTP_BUF_SIZE, 0);
#endif
            if (received == A_SOCK_INVALID)
            {
                /* Test ended, peer closed connection*/
                rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
            }
            break;
        }

        if (conn_sock == A_SOCK_INVALID)
        {
            /* Test ended, peer closed connection*/
            rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
            break;
        }
    }

    /*Valid packet received*/
    if (rtn == QCOM_OTA_OK)
    {
        buffer[received] = '\0';
        OTA_FTP_D_PRINTF("recv: %s", buffer);

        buffer[3]  = '\0';
        *resp_code = strtol(buffer, NULL, 10);
    }

#if ZERO_COPY
    if (received > 0)
        zero_copy_free(buffer);
#endif
    return rtn;
}

int32_t ota_ftp_send_cmd_resp(char *cmd, int *resp_code)
{
    *resp_code = 0;
    if (ota_ftp_send_cmd(cmd) <= 0)
        return QCOM_OTA_ERR_FTP_SEND_COMMAND;

    return (ota_ftp_recv_cmd(resp_code));
}

int32_t ota_ftp_program_ota(uint32_t *size)
{
    SOCKADDR_T foreign_addr;
    int32_t conn_sock, received = 0;
    int32_t resp_num;
    socklen_t addr_len;
    uint32_t count, is_first, offset, img_offset;
    uint32_t complete = 0;
    uint32_t ret_size;
    int32_t rtn = QCOM_OTA_OK;
    uint8_t *ptr;
    char *buffer = NULL;
#if ZERO_COPY
#else
    uint8_t *mem_buf;

    *size = 0;
    /*Allocate buffer*/
    if ((mem_buf = A_MALLOC(OTA_FTP_BUF_SZIE, MALLOC_ID_CONTEXT)) == NULL)
    {
        OTA_FTP_D_PRINTF("Out of memory error" NL);
        rtn = A_ERROR;
        goto error_1;
    }

    buffer = mem_buf;
#endif

    /* Listen. */
    OTA_FTP_D_PRINTF("Listening data socket" NL);
    if (qcom_listen(ota_ftp_sess->data_sock, 1) == A_ERROR)
    {
        PRINTF("ERROR: Socket listen error." NL);
        rtn = QCOM_OTA_ERR_FTP_LISTEN_DATA_PORT;
        goto error_1;
    }

    count = 0;
    do
    {
        /* block for 50msec or until a packet is received */
        conn_sock = t_select((void *)handle, ota_ftp_sess->data_sock, OTA_FTP_WAIT_TIME);
        if (conn_sock == A_SOCK_INVALID)
        {
            rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
            goto error_1; // Peer closed connection, socket no longer valid
        }
        count++;
    } while ((conn_sock == A_ERROR) && (count < (OTA_FTP_INCOMING_CONNECT_TIMEOUT / OTA_FTP_WAIT_TIME)));

    if (count >= (OTA_FTP_INCOMING_CONNECT_TIMEOUT / OTA_FTP_WAIT_TIME))
    {
        PRINTF("Error:FTP incoming data connection timeout" NL);
        rtn = QCOM_OTA_ERR_FTP_DATA_CONNECTION_TIMEOUT;
        goto error_1;
    }

    /*Accept incoming connection*/
    OTA_FTP_D_PRINTF("Accept incoming data connection" NL);
    addr_len                = sizeof(foreign_addr);
    ota_ftp_sess->peer_sock = qcom_accept(ota_ftp_sess->data_sock, (void *)&foreign_addr, &addr_len);

    if (ota_ftp_sess->peer_sock == (uint32_t)A_ERROR)
    {
        rtn = QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT;
        goto error_2;
    }

    /* -----------------------------------------------------*/
    /* process FTP data                                   */
    /* -----------------------------------------------------*/
    PRINTF("FTP Transfer File" NL);

    count                           = 0;
    is_first                        = 1;
    img_offset                      = 0;
    ota_ftp_sess->write_flag        = 0;
    ota_ftp_sess->total_write       = 0;
    ota_ftp_sess->total_write_rsp   = 0;
    ota_ftp_sess->cur               = NULL;
    ota_ftp_sess->parse_hdr_pending = 0;
    ota_ftp_sess->write_pending     = 0;

    while (count < (OTA_FTP_INCOMING_CONNECT_TIMEOUT / OTA_FTP_WAIT_TIME))
    {
        // increase timeout counter
        count++;

        /*-----------------------------------------------------------*/
        /* Handle OTA */
        {
            if (ota_ftp_sess->cur == NULL) /* currrent pkt is null */
            {
                ota_ftp_sess->cur = ota_ftp_fetch_pkt();
            }

            //           /* we have received FTP Transfer complete command and no pkt at queue */
            //           if((ota_ftp_sess->cur == NULL) && (complete != 0) && (count > (OTA_FTP_INCOMING_CONNECT_TIMEOUT
            //           / OTA_FTP_WAIT_TIME /2)) )
            //           {
            //               //we are done here
            //               if(rtn == A_PENDING)
            //                   rtn = QCOM_OTA_OK;
            //               OTA_FTP_D_PRINTF("all data have been sent out:%d" NL,rtn);
            //                PRINTF("all data have been sent out:%d" NL,rtn);
            //                break;
            //            }

            if (ota_ftp_sess->cur != NULL) // we have valid data
            {
                if (is_first == 1)
                {
                    // send ota parse image hdr command
                    is_first                        = 0;
                    ota_cmd                         = 0;
                    ota_ftp_sess->parse_hdr_pending = 1;
                    qcom_ota_parse_image_hdr(ota_ftp_sess->cur->buf, &offset);
                    OTA_FTP_D_PRINTF("send OTA Parse Image Hdr" NL);
                }

                if (ota_ftp_sess->parse_hdr_pending != 0) // waiting for parse image hdr response
                {
                    if (ota_cmd != 0) // we get parse hdr response
                    {
                        ota_ftp_sess->parse_hdr_pending = 0;
                        if (ota_resp != QCOM_OTA_OK)
                        {
                            OTA_FTP_D_PRINTF("OTA Parse Image Hdr Error" NL);
                            rtn = QCOM_OTA_ERR_IMAGE_HDR_INCORRECT;
                            goto error_2;
                        }

                        OTA_FTP_D_PRINTF("OTA Parse Image Hdr: %d" NL, ota_result);
                        ota_ftp_sess->cur->offset = ota_result;
                        ota_ftp_sess->cur->len -= ota_result;
                        ota_ftp_sess->write_pending = 1;
                    }
                }
                if (ota_ftp_sess->write_pending != 0) // write data
                {
                    count = 0;
                    ptr   = ota_ftp_sess->cur->buf;
                    rtn   = qcom_ota_partition_write_data(img_offset, &ptr[ota_ftp_sess->cur->offset],
                                                        ota_ftp_sess->cur->len, &ret_size);

                    if (rtn == A_PENDING)
                    {
                        OTA_FTP_D_PRINTF("ota_write offset:%d len:%dB" NL, img_offset, ota_ftp_sess->cur->len);
                        ota_ftp_sess->total_write += ota_ftp_sess->cur->len;
                        img_offset += ota_ftp_sess->cur->len;
                        ota_ftp_free_pkt(ota_ftp_sess->cur);
                        ota_ftp_sess->cur = NULL;
                        A_MDELAY(1);
                    }
                    else if (rtn == A_NO_MEMORY)
                    {
                        OTA_FTP_D_PRINTF("ota_write error: no memory, put delay here" NL);
                        A_MDELAY(5);
                    }
                    else
                    {
                        OTA_FTP_D_PRINTF("ota write data error:%d" NL, rtn);
                        goto error_2;
                    }
                }
            }
        }

        /*-----------------------------------------------------------*/
        /* Receiving data from FTP server.*/
        conn_sock = t_select((void *)handle, ota_ftp_sess->peer_sock, OTA_FTP_WAIT_TIME);
        //        if( conn_sock == A_SOCK_INVALID )
        //        {
        //            /* peer data socket is closed the connection*/
        //            ;
        //            PRINTF("data link is closed %d %d" NL, ota_ftp_sess->total_write, ota_ftp_sess->total_write_rsp);
        //        }
        //         else
        if (conn_sock == A_OK)
        {
/*Packet is available, receive it*/
#if ZERO_COPY
            received = qcom_recv(ota_ftp_sess->peer_sock, (char **)(&buffer), OTA_FTP_BUF_SIZE, 0);
#else
            received = qcom_recv(ota_ftp_sess->peer_sock, (char *)(&buffer[0]), OTA_FTP_BUF_SIZE, 0);
#endif
            OTA_FTP_D_PRINTF("recv %dB" NL, received);

            if (received == A_SOCK_INVALID)
            {
                /* Test ended, peer closed connection*/
                rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                goto error_2;
            }

            // reset timeout counter
            count = 0;

            // add data to queue
            if (ota_ftp_append_pkt((uint8_t *)buffer, received) != QCOM_OTA_OK)
            {
#if ZERO_COPY
                zero_copy_free(buffer);
#endif
                rtn = QCOM_OTA_ERR_INSUFFICIENT_MEMORY;
                goto error_2;
            }
#if ZERO_COPY
            zero_copy_free(buffer);
#endif
        }
        else if (complete == 0)
        { // Receiving command
            conn_sock = t_select((void *)handle, ota_ftp_sess->control_sock, OTA_FTP_WAIT_TIME);
            if (conn_sock == A_OK)
            {
/*Packet is available, receive it*/
#if ZERO_COPY
                received = qcom_recv(ota_ftp_sess->control_sock, (char **)(&buffer), OTA_FTP_BUF_SIZE - 1, 0);
#else
                received = qcom_recv(ota_ftp_sess->control_sock, (char *)(char *)(&buffer[0]), OTA_FTP_BUF_SIZE - 1, 0);
#endif
                if (received == A_SOCK_INVALID)
                {
                    /* Test ended, peer closed connection*/
                    rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                    goto error_2;
                }

                /* handle cmd */
                buffer[received] = '\0';
                OTA_FTP_D_PRINTF("recv: %s", buffer);

                buffer[3] = '\0';
                resp_num  = strtol(buffer, NULL, 10);

#if ZERO_COPY
                if (received > 0)
                    zero_copy_free(buffer);
#endif
                // we have received the whole file
                complete = 1;
                count    = 0;
                if (resp_num == 226) // recv 226 Transfer complete
                {
                    rtn = QCOM_OTA_OK;
                    OTA_FTP_D_PRINTF("Transfer Complete received" NL);
                }
                else
                {
                    // get error
                    OTA_FTP_D_PRINTF("Transfer not Complete:%d" NL, resp_num);
                    rtn = QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE;
                }
            }
            else if (conn_sock == A_SOCK_INVALID)
            {
                /* peer closed connection*/
                rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                goto error_2;
            }
        }
        else
        {
            A_MDELAY(1);
        }
    } /* while(  */
error_2:
    if ((rtn == QCOM_OTA_OK) || (complete == 1))
    {
        count = 0;
        // wait up to 5s to get all write response
        while ((ota_ftp_sess->total_write > ota_ftp_sess->total_write_rsp) && (count < 50))
        {
            A_MDELAY(100);
            count++;
        }
        OTA_FTP_D_PRINTF("wait %d ms to get all write response" NL, count * 100);
    }

    if (ota_ftp_sess->peer_sock != (uint32_t)A_ERROR)
    {
        qcom_socket_close(ota_ftp_sess->peer_sock);
        ota_ftp_sess->peer_sock = (uint32_t)A_ERROR;
    }
    ota_ftp_close_data_sock();
    ota_ftp_clear_pkt();

    OTA_FTP_D_PRINTF("Total write: %d %d" NL, ota_ftp_sess->total_write, ota_ftp_sess->total_write_rsp);

    if (complete == 0) // don't receive transfer command yet
    {
        conn_sock = t_select((void *)handle, ota_ftp_sess->control_sock, UDP_CONNECTION_WAIT_TIME);
        if (conn_sock == A_OK)
        {
/*Packet is available, receive it*/
#if ZERO_COPY
            received = qcom_recv(ota_ftp_sess->control_sock, (char **)(&buffer), OTA_FTP_BUF_SIZE - 1, 0);
#else
            received = qcom_recv(ota_ftp_sess->control_sock, (char *)(char *)(&buffer[0]), OTA_FTP_BUF_SIZE - 1, 0);
#endif
            if (received != A_SOCK_INVALID)
            {
                /* handle cmd */
                buffer[received] = '\0';
                OTA_FTP_D_PRINTF("recv: %s", buffer);

                buffer[3] = '\0';
                resp_num  = strtol(buffer, NULL, 10);
            }

#if ZERO_COPY
            if (received > 0)
                zero_copy_free(buffer);
#endif
        }
    }
    *size = ota_ftp_sess->total_write_rsp;
error_1:
    PRINTF("" NL);
#if !ZERO_COPY
    if (mem_buf)
        A_FREE(mem_buf, MALLOC_ID_CONTEXT);
#endif
    if ((complete == 1) && (ota_ftp_sess->total_write_rsp == ota_ftp_sess->total_write) &&
        (ota_ftp_sess->total_write > 0))
    {
        rtn = QCOM_OTA_OK;
    }
    return rtn;
}

int32_t qcom_ota_ftp_upgrade(_ip_address ip_addr,
                             uint16_t port,
                             char *user_name,
                             char *password,
                             char *filename,
                             uint32_t flags,
                             uint32_t partition_index,
                             uint32_t *size)
{
    int32_t rtn = A_OK;
    int resp;
    uint32_t partition_size, good_image = 0;
    char buf[256];

    OTA_FTP_D_PRINTF("Enter qcom_ota_ftp_upgrade" NL);
    OTA_FTP_D_PRINTF("ota_ftp_update: ip:%xH, port:%d" NL, ip_addr, port);
    OTA_FTP_D_PRINTF("                User:%s, Password:%s" NL, user_name, password);
    OTA_FTP_D_PRINTF("                file:%s" NL, filename);
    OTA_FTP_D_PRINTF("                flags:%d, partition_index:%d" NL, flags, partition_index);

    *size = 0;
    if ((flags & QCOM_OTA_TARGET_FIRMWARE_UPGRADE) == 0)
    {
        PRINTF("OTA FTP supports Target Firmware Upgrade only, check flags setting" NL);
        return QCOM_OTA_ERR_UNSUPPORT_PROTOCOL;
    }

    if ((rtn = qcom_ota_session_start(flags, partition_index)) != QCOM_OTA_OK)
    {
        OTA_FTP_D_PRINTF("OTA Session Start Fail" NL);
        return rtn;
    }
    if ((rtn = qcom_ota_partition_erase()) != QCOM_OTA_OK)
    {
        OTA_FTP_D_PRINTF("OTA Partition Erase Fail" NL);
        return rtn;
    }

    /* check partition size */
    partition_size = qcom_ota_partition_get_size();
    OTA_FTP_D_PRINTF("OTA Partition Get Size: %d" NL, partition_size);
    if (partition_size == 0)
    {
        rtn = A_ERROR;
        return rtn;
    }

    if (ota_ftp_init() != QCOM_OTA_OK)
    {
        OTA_FTP_D_PRINTF("Error: ota_ftp_init error" NL);
        goto error_1;
    }

    PRINTF(NL "Connect to FTP Server..." NL);
    // open control sock
    if ((rtn = ota_ftp_open_control_sock(ip_addr, port)) != QCOM_OTA_OK)
        goto error_1;

    // recev welcome
    if ((rtn = ota_ftp_recv_cmd(&resp)) != QCOM_OTA_OK)
    {
        goto error_1;
    }
    if (resp != 220)
    {
        PRINTF("FTP Welcome error: %d" NL, resp);
        goto error_1;
    }

    // send user
    sprintf(buf, "USER %s\r\n", user_name);
    if ((rtn = ota_ftp_send_cmd_resp(buf, &resp)) != QCOM_OTA_OK)
    {
        goto error_1;
    }
    if (resp != 331)
    {
        rtn = QCOM_OTA_ERR_FTP_LOGIN_INCORRECT;
        PRINTF("FTP USER error: %d", resp);
        goto error_1;
    }

    // send password
    sprintf(buf, "PASS %s\r\n", password);
    if ((rtn = ota_ftp_send_cmd_resp(buf, &resp)) != QCOM_OTA_OK)
    {
        goto error_1;
    }
    if (resp != 230)
    {
        rtn = QCOM_OTA_ERR_FTP_LOGIN_INCORRECT;
        PRINTF("FTP PASS error: %d" NL, resp);
        goto error_1;
    }

    PRINTF("FTP Login ..." NL);

    // send syst
    if ((rtn = ota_ftp_send_cmd_resp("SYST" NL, &resp)) != QCOM_OTA_OK)
    {
        goto error_1;
    }
    if (resp != 215)
    {
        rtn = QCOM_OTA_ERR_FTP_SYST;
        PRINTF("FTP SYST error: %d" NL, resp);
        goto error_1;
    }

    // setup BINARY mode
    if ((rtn = ota_ftp_send_cmd_resp("TYPE I" NL, &resp)) != QCOM_OTA_OK)
    {
        goto error_1;
    }
    if (resp != 200)
    {
        rtn = QCOM_OTA_ERR_FTP_SET_TYPE;
        PRINTF("FTP TYPE error: %d" NL, resp);
        goto error_1;
    }

    // open data socket
    if ((rtn = ota_ftp_open_data_sock(ota_ftp_sess->local_ip_address, ota_ftp_sess->data_port)) != QCOM_OTA_OK)
        goto error_1;

    // send port
    sprintf(buf, "PORT %d,%d,%d,%d,%d,%d\r\n", getByte(3, ota_ftp_sess->local_ip_address),
            getByte(2, ota_ftp_sess->local_ip_address), getByte(1, ota_ftp_sess->local_ip_address),
            getByte(0, ota_ftp_sess->local_ip_address), ota_ftp_sess->data_port / 256, ota_ftp_sess->data_port % 256);
    if ((rtn = ota_ftp_send_cmd_resp(buf, &resp)) != QCOM_OTA_OK)
    {
        goto error_1;
    }
    if (resp != 200)
    {
        rtn = QCOM_OTA_ERR_FTP_SET_PORT;
        PRINTF("FTP PORT error: %d" NL, resp);
        goto error_1;
    }

    // send get file
    sprintf(buf, "RETR %s\r\n", filename);
    if ((rtn = ota_ftp_send_cmd_resp(buf, &resp)) != QCOM_OTA_OK)
    {
        goto error_1;
    }
    if (resp != 150)
    {
        rtn = QCOM_OTA_ERR_FTP_FILE_NOT_FOUND;
        PRINTF("FTP RETR error: %d" NL, resp);
        goto error_1;
    }

    // get file and write ota
    rtn = ota_ftp_program_ota(size);
    if (rtn == QCOM_OTA_OK)
    {
        // we are done
        if ((rtn = qcom_ota_partition_verify_checksum()) == QCOM_OTA_OK)
        {
            OTA_FTP_D_PRINTF("OTA Partition Checksum is correct" NL);
            good_image = 1;
        }
        else
        {
            OTA_FTP_D_PRINTF("OTA Partition Verify Checksum is NOT correct" NL);
        }
    }

error_1:
    // send quit and recv goodbye
    ota_ftp_send_cmd_resp("QUIT" NL, &resp);
    qcom_ota_session_end(good_image);
    ota_ftp_finit();
    OTA_FTP_D_PRINTF("Leave qcom_ota_ftp_update %d" NL, rtn);
    return rtn;
}

static int32_t ota_ftp_upgrade(int32_t argc, char *argv[])
{
    uint32_t ip_addr = 0, flags, partition_index;
    uint16_t port;
    uint32_t resp_code;
    uint32_t size;

    if (argc < 9)
    {
        PRINTF("Incomplete params" NL);
        PRINTF("Usage: wmiconfig --ota_ftp <ip> <port> <user name> <password> <filename> <flags> <partition_index>" NL);
        return A_ERROR;
    }

    ath_inet_aton(argv[2], (uint32_t *)&ip_addr);
    port            = strtol(argv[3], NULL, 10);
    flags           = strtol(argv[7], NULL, 10);
    partition_index = strtol(argv[8], NULL, 10);

    resp_code = qcom_ota_ftp_upgrade(ip_addr, port, argv[4], argv[5], argv[6], flags, partition_index, &size);
    if (A_OK != resp_code)
    {
        PRINTF("OTA Image Download Failed ERR:%d" NL, resp_code);
    }
    else
    {
        PRINTF("OTA Image Download Completed successfully,Image Size:%d" NL, size);
    }
    return A_OK;
}

#endif /* ENABLE_STACK_OFFLOAD */

static int32_t get_temperature(int32_t argc, char *argv[])
{
    uint32_t temp_reg = 0;
    float temp_val    = 0;
    qcom_get_temperature(&temp_reg, &temp_val);
    PRINTF(NL "RegVal = %x  Degree = %f deg C" NL, temp_reg, temp_val);
    return A_OK;
}
static int32_t get_country_code(void)
{
    uint8_t country_code[3];

    uint32_t enet_device = get_active_device();
    qcom_get_country_code(enet_device, country_code);
    return A_OK;
}

static int32_t ath_roam_ctrl(int32_t argc, char *argv[])
{
    WMI_SET_ROAM_CTRL_CMD roam_cmd;

    A_MEMZERO(&roam_cmd, sizeof(WMI_SET_ROAM_CTRL_CMD));
    roam_cmd.roamCtrlType = 2;

    if (ATH_STRCMP(argv[2], "enable") == 0)
    {
        roam_cmd.info.roamMode = 1; // enable roam
        // todo add more
    }
    else if (ATH_STRCMP(argv[2], "disable") == 0)
    {
        roam_cmd.info.roamMode = 3; // disable roam
    }
    qcom_roaming_ctrl(&roam_cmd);
    return A_OK;
}

static int32_t wow_set_mode(int32_t argc, char *argv[])
{
    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    int enable = atoi(argv[2]);
    if (enable)
    {
        /*Enable*/
        QCOM_WOW_ENABLE wow_enable;
        wow_enable.enable = enable;
        if (qcom_param_set(enet_device, QCOM_PARAM_GROUP_WLAN, QCOM_PARAM_GROUP_WLAN_WOW_ENABLE, &wow_enable,
                           sizeof(wow_enable), TRUE) != A_OK)
        {
            PRINTF("qcom set wow enable error" NL);
            return A_ERROR;
        }

        /*Set default parameters*/
        /*GPIO*/
        if (qcom_param_set(enet_device, QCOM_PARAM_GROUP_WLAN, QCOM_PARAM_GROUP_WLAN_WOW_GPIO, &default_wow_gpio_config,
                           sizeof(default_wow_gpio_config), TRUE) != A_OK)
        {
            PRINTF("qcom set wow gpio error" NL);
            return A_ERROR;
        }

        /*Pattern*/
        QCOM_WOW_PATTERN wow_pattern;
        wow_pattern.offset        = default_wow_pattern.offset;
        wow_pattern.pattern_index = default_wow_pattern.pattern_index;
        wow_pattern.pattern_size  = default_wow_pattern.pattern_size;
        memcpy(wow_pattern.pattern, default_pattern, default_wow_pattern.pattern_size);
        if (qcom_param_set(enet_device, QCOM_PARAM_GROUP_WLAN, QCOM_PARAM_GROUP_WLAN_WOW_PATTERN, &wow_pattern,
                           sizeof(wow_pattern), TRUE) != A_OK)
        {
            PRINTF("qcom set pattern error" NL);
            return A_ERROR;
        }
    }
    else
    {
        /*Enable*/
        QCOM_WOW_ENABLE wow_enable;
        wow_enable.enable = enable;
        if (qcom_param_set(enet_device, QCOM_PARAM_GROUP_WLAN, QCOM_PARAM_GROUP_WLAN_WOW_ENABLE, &wow_enable,
                           sizeof(wow_enable), TRUE) != A_OK)
        {
            PRINTF("qcom set wow enable error" NL);
            return A_ERROR;
        }
    }
    return A_OK;
}

static int32_t wow_host_sleep(int32_t argc, char *argv[])
{
    int awake = atoi(argv[2]);

    uint32_t enet_device = get_active_device();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    QCOM_WOW_HOST_SLEEP host_sleep;
    host_sleep.awake = awake;
    if (qcom_param_set(enet_device, QCOM_PARAM_GROUP_WLAN, QCOM_PARAM_GROUP_WLAN_WOW_HOSTSLEEP, &host_sleep,
                       sizeof(host_sleep), TRUE) != A_OK)
    {
        PRINTF("qcom set host sleep error" NL);
        return A_ERROR;
    }
    return A_OK;
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : set_gtx
 * Returned Value : A_OK
 * Comments       : Enable/Disable GTX
 *
 *END------------------------------------------------------------------*/
static int32_t set_gtx(char *gtx)
{
    uint8_t b_gtx = atoi(gtx);
    ATH_IOCTL_PARAM_STRUCT inout_param;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_ONOFF_GTX;
    inout_param.data   = &b_gtx;
    inout_param.length = sizeof(bool);

    return HANDLE_IOCTL(&inout_param);
}

/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : set_lpl
 * Returned Value : A_OK
 * Comments       : Enable/Disable LPL
 *
 *END------------------------------------------------------------------*/
static int32_t set_lpl(char *lpl)
{
    uint8_t b_lpl = atoi(lpl);
    ATH_IOCTL_PARAM_STRUCT inout_param;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    inout_param.cmd_id = ATH_ONOFF_LPL;
    inout_param.data   = &b_lpl;
    inout_param.length = sizeof(bool);

    return HANDLE_IOCTL(&inout_param);
}

/*FUNCTION*-----------------------------------------------------------
 *
 * Function Name  : get_active_device
 * Returned Value : A_OK
 * Comments       : Return the current active device in the application
 *
 *END------------------------------------------------------------------*/

int32_t get_active_device(void)
{
    return active_device;
}

static int32_t mac_store(char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT ath_param;
    uint8_t val;
    int32_t j;
    ATH_PROGRAM_MAC_ADDR_PARAM data;

    ath_param.cmd_id = ATH_PROGRAM_MAC_ADDR;
    ath_param.length = sizeof(data);
    /* get mac address from argv */
    if (strlen(argv[0]) != 12)
    {
        PRINTF("Invalid MAC address length" NL);
        return A_ERROR;
    }

    for (j = 0; j < 6; j++)
    {
        data.addr[j] = 0;
    }
    for (j = 0; j < strlen(argv[0]); j++)
    {
        val = ascii_to_hex(argv[0][j]);
        if (val == 0xff)
        {
            PRINTF("Invalid character" NL);
            return A_ERROR;
        }
        else
        {
            if ((j & 1) == 0)
                val <<= 4;

            data.addr[j >> 1] |= val;
        }
    }

    ath_param.data = (uint8_t *)&data;

    if (HANDLE_IOCTL(&ath_param) != A_OK)
    {
        PRINTF("MAC PROGRAM ERROR: unknown driver error" NL);
        return A_ERROR;
    }
    else
    {
        /* macaddr[0] contains result code */
        switch (data.result)
        {
            case ATH_PROGRAM_MAC_RESULT_DRIVER_FAILED:
                PRINTF("MAC PROGRAM ERROR: the driver was unable to complete the request" NL);
                break;
            case ATH_PROGRAM_MAC_RESULT_SUCCESS:
                PRINTF("MAC PROGRAM SUCCESS." NL);
                break;
            case ATH_PROGRAM_MAC_RESULT_DEV_DENIED:
                PRINTF("MAC PROGRAM ERROR: the firmware failed to program the mac address" NL);
                PRINTF("possibly the same MAC address is already programmed" NL);
                break;
            case ATH_PROGRAM_MAC_RESULT_DEV_FAILED:
            default:
                PRINTF("MAC PROGRAM ERROR: Device unknown failure" NL);
                break;
        }
    }

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : gpio_handler()
 * Returned Value  : 1 - successful completion or
 *                   0 - failed.
 * Comments        : Handles GPIO related functionality
 *
 *END*-----------------------------------------------------------------*/
static int32_t gpio_handle(int32_t argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error;
    int32_t index = 2;

    if (ATH_STRCMP(argv[index], "output_set") == 0)
    {
        struct WMIX_GPIO_OUTPUT_SET
        {
            WMIX_CMD_HDR cmd;
            WMIX_GPIO_OUTPUT_SET_CMD setting;
        } gpio_output_set;

        if (argc != 7)
        {
            PRINTF(NL "Incomplete parameter list for gpio output_set" NL);
            PRINTF(NL "Usage : wmiconfig --gpio output_set  <set_mask> <clear_mask> <enable_mask> <disable_mask>" NL);
            return A_ERROR;
        }

        gpio_output_set.cmd.commandId        = WMIX_GPIO_OUTPUT_SET_CMDID;
        gpio_output_set.setting.set_mask     = strtol(argv[3], NULL, 16);
        gpio_output_set.setting.clear_mask   = strtol(argv[4], NULL, 16);
        gpio_output_set.setting.enable_mask  = strtol(argv[5], NULL, 16);
        gpio_output_set.setting.disable_mask = strtol(argv[6], NULL, 16);

        inout_param.cmd_id = ATH_GPIO_CMD;
        inout_param.data   = (void *)&gpio_output_set;
        inout_param.length = sizeof(gpio_output_set);

        error = HANDLE_IOCTL(&inout_param);
        return error;
    }
    else if (ATH_STRCMP(argv[index], "input_get") == 0)
    {
        WMIX_CMD_HDR cmd;
        cmd.commandId = WMIX_GPIO_INPUT_GET_CMDID;

        inout_param.cmd_id = ATH_GPIO_CMD;
        inout_param.data   = (void *)&cmd;
        inout_param.length = sizeof(cmd);

        error = HANDLE_IOCTL(&inout_param);
        return error;
    }
    else if (ATH_STRCMP(argv[index], "register_set") == 0)
    {
        struct WMIX_GPIO_REGISTER_SET
        {
            WMIX_CMD_HDR cmd;
            WMIX_GPIO_REGISTER_SET_CMD setting;
        } gpio_register_set;

        if (argc != 5)
        {
            PRINTF(NL "Incomplete parameter list for gpio output_set" NL);
            PRINTF(NL "Usage : wmiconfig --gpio register_set  <gpioreg_id:hex> <value:hex>" NL);
            return A_ERROR;
        }

        gpio_register_set.cmd.commandId      = WMIX_GPIO_REGISTER_SET_CMDID;
        gpio_register_set.setting.gpioreg_id = strtol(argv[3], NULL, 16);
        gpio_register_set.setting.value      = strtol(argv[4], NULL, 16);

        inout_param.cmd_id = ATH_GPIO_CMD;
        inout_param.data   = (void *)&gpio_register_set;
        inout_param.length = sizeof(gpio_register_set);

        error = HANDLE_IOCTL(&inout_param);
        return error;
    }
    else if (ATH_STRCMP(argv[index], "register_get") == 0)
    {
        struct WMIX_GPIO_REGISTER_GET
        {
            WMIX_CMD_HDR cmd;
            WMIX_GPIO_REGISTER_GET_CMD setting;
        } gpio_register_get;

        if (argc != 4)
        {
            PRINTF(NL "Incomplete parameter list for gpio output_set" NL);
            PRINTF(NL "Usage : wmiconfig --gpio register_get  <gpioreg_id:hex>" NL);
            return A_ERROR;
        }

        gpio_register_get.cmd.commandId      = WMIX_GPIO_REGISTER_GET_CMDID;
        gpio_register_get.setting.gpioreg_id = strtol(argv[3], NULL, 16);

        inout_param.cmd_id = ATH_GPIO_CMD;
        inout_param.data   = (void *)&gpio_register_get;
        inout_param.length = sizeof(gpio_register_get);

        error = HANDLE_IOCTL(&inout_param);
        return error;
    }
    else
    {
        PRINTF(NL "Incorrect Incomplete parameter list" NL);
        return SHELL_EXIT_ERROR;
    }
}

#if ENABLE_KF_PERFORMANCE

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : pfm_handler()
 * Returned Value  : 1 - successful completion or
 *                   0 - failed.
 * Comments        : Handles performance measure related functionality
 *
 *END*-----------------------------------------------------------------*/
static int32_t pfm_handler(int32_t argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error;
    ATH_PFM_PARAM param;

    clear_kf_profile();

    inout_param.cmd_id = ATH_PFM_CMD;
    inout_param.data   = (void *)&param;
    inout_param.length = sizeof(param);

    error = HANDLE_IOCTL(&inout_param);

    //	pDCxt->pfmDone = A_FALSE;
    /* block until scan completes */
    //   	CUSTOM_DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->pfmDone), A_TRUE, 5000);
    output_kf_pfm();

    return error;
}

static int32_t diag_handler(int32_t argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    int32_t error;
    struct diag_data
    {
        WMIX_CMD_HDR cmd;
        WMID_EVENT_SET_CMD setting;
    } data;

    //    data.cmd.commandId = WMID_FSM_EVENT_CMDID;
    data.setting.enable = 1;

    inout_param.cmd_id = ATH_DIAG_CMD;
    inout_param.data   = (void *)&data;
    inout_param.length = sizeof(data);

    error = HANDLE_IOCTL(&inout_param);

    //	pDCxt->pfmDone = A_FALSE;
    /* block until scan completes */
    //   	CUSTOM_DRIVER_WAIT_FOR_CONDITION(pCxt, &(pDCxt->pfmDone), A_TRUE, 5000);

    return error;
}

A_STATUS ath_setpmu(int argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    ATH_PMU_PARAM param;

    if (argc < 3)
    {
        return A_ERROR;
    }
    if (strcmp(argv[2], "timer") == 0)
    {
        param.options   = WAKE_MGR_WAKE_EVENT_TIMER;
        param.wake_msec = atoi(argv[3]);
    }
    else if (strcmp(argv[2], "gpio30") == 0)
    {
        if (atoi(argv[3]) != 0)
            param.options = WAKE_MGR_WAKE_EVENT_GPIO30_HIGH;
        else
            param.options = WAKE_MGR_WAKE_EVENT_GPIO30_LOW;
    }
    else if (strcmp(argv[2], "gpio31") == 0)
    {
        if (atoi(argv[3]) != 0)
            param.options = WAKE_MGR_WAKE_EVENT_GPIO31_HIGH;
        else
            param.options = WAKE_MGR_WAKE_EVENT_GPIO31_LOW;
    }

    inout_param.cmd_id = ATH_PMU_SET_PARAMS;
    inout_param.data   = &param;
    inout_param.length = sizeof(ATH_PMU_PARAM);

    return HANDLE_IOCTL(&inout_param);
}
#endif

A_STATUS ath_dset(int argc, char *argv[])
{
    ATH_IOCTL_PARAM_STRUCT inout_param;
    ATH_DSET_PARAM param;

    if (argc < 5)
    {
        return A_ERROR;
    }
    if (strcmp(argv[2], "write") == 0)
    {
        param.dset_id      = atoi(argv[3]);
        param.offset       = atoi(argv[4]);
        param.length       = atoi(argv[5]);
        inout_param.cmd_id = ATH_DSET_WRITE_CMD;
    }
    else if (strcmp(argv[2], "read") == 0)
    {
        param.dset_id      = atoi(argv[3]);
        param.offset       = atoi(argv[4]);
        param.length       = atoi(argv[5]);
        inout_param.cmd_id = ATH_DSET_READ_CMD;
    }

    inout_param.data   = &param;
    inout_param.length = sizeof(param);

    return HANDLE_IOCTL(&inout_param);
}

uint32_t remote_dset_op(DSET_OP op, HOST_DSET_HANDLE *pDsetHandle)
{
    ATH_IOCTL_PARAM_STRUCT inout_param;

    struct WMIX_GPIO_REGISTER_SET
    {
        WMIX_DSET_CMD_HDR cmd;
        union
        {
            WMIX_DSET_OP_CREATE_PARAM_CMD op_create;
            WMIX_DSET_OP_OPEN_PARAM_CMD op_open;
            WMIX_DSET_OP_WRITE_PARAM_CMD op_write;
            WMIX_DSET_OP_READ_PARAM_CMD op_read;
            WMIX_DSET_OP_COMMIT_PARAM_CMD op_commit;
            WMIX_DSET_OP_CLOSE_PARAM_CMD op_close;
            WMIX_DSET_OP_DELETE_PARAM_CMD op_delete;
        } u;
    } dset_op;

    pDsetHandle->done_flag = 0;

    switch (op)
    {
        case DSET_OP_CREATE:
            dset_op.cmd.commandId = WMIX_DSET_CREATE_CMDID;

            dset_op.u.op_create.dset_id = pDsetHandle->dset_id;
            dset_op.u.op_create.flags   = pDsetHandle->flags | pDsetHandle->media_id;
            dset_op.u.op_create.length  = pDsetHandle->length;

            inout_param.data   = (void *)&dset_op;
            inout_param.length = sizeof(dset_op);

            break;

        case DSET_OP_OPEN:
            dset_op.cmd.commandId     = WMIX_DSET_OPEN_CMDID;
            dset_op.u.op_open.dset_id = pDsetHandle->dset_id;
            dset_op.u.op_open.flags   = pDsetHandle->flags;

            inout_param.data   = (void *)&dset_op;
            inout_param.length = sizeof(WMIX_DSET_CMD_HDR) + sizeof(WMIX_DSET_OP_OPEN_PARAM_CMD);
            break;

        case DSET_OP_READ:
            dset_op.cmd.commandId     = WMIX_DSET_READ_CMDID;
            dset_op.u.op_read.dset_id = pDsetHandle->dset_id;
            dset_op.u.op_read.offset  = pDsetHandle->offset;
            dset_op.u.op_read.length  = pDsetHandle->length;
            pDsetHandle->left_len     = pDsetHandle->length;

            inout_param.data   = (void *)&dset_op;
            inout_param.length = sizeof(WMIX_DSET_CMD_HDR) + sizeof(WMIX_DSET_OP_READ_PARAM_CMD);
            break;
        case DSET_OP_WRITE:
        {
            WMIX_DSET_CMD_HDR *pCmd = (WMIX_DSET_CMD_HDR *)pDsetHandle->data_ptr;

            WMIX_DSET_OP_WRITE_PARAM_CMD *pDsetWrite =
                (WMIX_DSET_OP_WRITE_PARAM_CMD *)(pDsetHandle->data_ptr + sizeof(WMIX_DSET_CMD_HDR));

            pCmd->commandId = WMIX_DSET_WRITE_CMDID;

            pDsetWrite->dset_id = pDsetHandle->dset_id;
            pDsetWrite->flags   = pDsetHandle->flags;
            pDsetWrite->offset  = pDsetHandle->offset;
            pDsetWrite->length  = pDsetHandle->length;

            pDsetHandle->left_len = pDsetHandle->length;

            inout_param.data   = (void *)pDsetHandle->data_ptr;
            inout_param.length = sizeof(WMIX_DSET_CMD_HDR) + sizeof(WMIX_DSET_OP_WRITE_PARAM_CMD) + pDsetHandle->length;

            break;
        }
        case DSET_OP_COMMIT:
        {
            dset_op.cmd.commandId       = WMIX_DSET_COMMIT_CMDID;
            dset_op.u.op_commit.dset_id = pDsetHandle->dset_id;

            inout_param.data   = (void *)&dset_op;
            inout_param.length = sizeof(WMIX_DSET_CMD_HDR) + sizeof(WMIX_DSET_OP_COMMIT_PARAM_CMD);
            break;
        }
        case DSET_OP_CLOSE:
        {
            dset_op.cmd.commandId      = WMIX_DSET_CLOSE_CMDID;
            dset_op.u.op_close.dset_id = pDsetHandle->dset_id;

            inout_param.data   = (void *)&dset_op;
            inout_param.length = sizeof(WMIX_DSET_CMD_HDR) + sizeof(WMIX_DSET_OP_CLOSE_PARAM_CMD);
            break;
        }
        case DSET_OP_DELETE:
        {
            dset_op.cmd.commandId       = WMIX_DSET_DELETE_CMDID;
            dset_op.u.op_delete.dset_id = pDsetHandle->dset_id;
            dset_op.u.op_delete.flags   = pDsetHandle->flags;

            inout_param.data   = (void *)&dset_op;
            inout_param.length = sizeof(WMIX_DSET_CMD_HDR) + sizeof(WMIX_DSET_OP_DELETE_PARAM_CMD);
            break;
        }
        case DSET_OP_SIZE:
        {
            break;
        }
    };

    inout_param.cmd_id = ATH_DSET_OP_CMD;

    return HANDLE_IOCTL(&inout_param);
}

HOST_DSET_HANDLE *pDsetHandle;
#define MAX_BUFF_SIZE 3000
uint8_t buffer[MAX_BUFF_SIZE];

// TODO: fix return codes, the 'qcom_' return 'bool' or A_STATUS as uint32_t !!!
A_STATUS ath_dset_api(int argc, char *argv[])
{
    uint32_t len, i;
    int32_t status = A_ERROR;
    uint32_t dset_id;
    uint32_t flags, length, media_id, offset;

    if (strcmp(argv[2], "create") == 0)
    {
        dset_id  = atoi(argv[3]);
        media_id = atoi(argv[4]);
        flags    = atoi(argv[5]);
        length   = atoi(argv[6]);
        status   = qcom_dset_create(&pDsetHandle, dset_id, media_id, length, flags, NULL, NULL);
    }
    if (strcmp(argv[2], "open") == 0)
    {
        dset_id = atoi(argv[3]);
        flags   = atoi(argv[4]);
        status  = qcom_dset_open(&pDsetHandle, dset_id, flags, NULL, NULL);
    }
    else if (strcmp(argv[2], "read") == 0 && argc >= 5)
    {
        offset = atoi(argv[3]);
        length = atoi(argv[4]);
        if (length > MAX_BUFF_SIZE)
        {
            PRINTF("max buffer size %d" NL, MAX_BUFF_SIZE);
            return (A_OK);
        }
        qcom_dset_read(pDsetHandle, buffer, length, offset, NULL, NULL);
        for (i = 0; i < 10; i++)
            PRINTF("%02X ", buffer[i]);
    }
    else if (strcmp(argv[2], "write") == 0 && argc >= 6)
    {
        flags  = atoi(argv[3]);
        offset = atoi(argv[4]);
        length = atoi(argv[5]);

        len = min(MAX_BUFF_SIZE, length);
        for (i = 0; i < len; i++)
        {
            buffer[i] = (i % 80) + '0';
        }

        status = qcom_dset_write(pDsetHandle, buffer, length, offset, flags, NULL, NULL);
    }
    else if (strcmp(argv[2], "commit") == 0)
    {
        status = qcom_dset_commit(pDsetHandle, NULL, NULL);
    }
    else if (strcmp(argv[2], "close") == 0)
    {
        status = qcom_dset_close(pDsetHandle, NULL, NULL);
    }
    else if (strcmp(argv[2], "size") == 0)
    {
        status = qcom_dset_size(pDsetHandle, NULL, NULL);
    }
    else if (strcmp(argv[2], "delete") == 0 && argc > 4)
    {
        dset_id = atoi(argv[3]);
        flags   = atoi(argv[4]);
        status  = qcom_dset_delete(dset_id, flags, NULL, NULL);
    }

    // TODO: fixme
    return (A_STATUS)status;
}

A_STATUS ath_scratch_dset(int argc, char *argv[])
{
    int32_t status = A_ERROR;
    uint32_t dset_id;
    uint32_t flags, length, media_id, offset, len, i;

    if (strcmp(argv[2], "create") == 0)
    {
        dset_id  = atoi(argv[3]);
        media_id = atoi(argv[4]);
        flags    = atoi(argv[5]);
        length   = atoi(argv[6]);
        flags |= 0x20; // scratch dset
        status = qcom_dset_create(&pDsetHandle, dset_id, media_id, length, flags, NULL, NULL);
    }
    else if (strcmp(argv[2], "open") == 0)
    {
        dset_id = atoi(argv[3]);
        flags   = atoi(argv[4]);
        status  = qcom_dset_open(&pDsetHandle, dset_id, flags, NULL, NULL);
    }
    else if (strcmp(argv[2], "read") == 0 && argc >= 5)
    {
        offset = atoi(argv[3]);
        length = atoi(argv[4]);
        if (length > MAX_BUFF_SIZE)
        {
            PRINTF("max buffer size %d" NL, MAX_BUFF_SIZE);
            return (A_OK);
        }

        status = qcom_dset_read(pDsetHandle, buffer, length, offset, NULL, NULL);

        if (status == A_OK)
        {
            for (i = 0; i < 10; i++)
                PRINTF("%02X ", buffer[i]);
            PRINTF("" NL);
        }
    }
    else if (strcmp(argv[2], "write") == 0 && argc >= 6)
    {
        flags  = atoi(argv[3]);
        offset = atoi(argv[4]);
        length = atoi(argv[5]);

        len = min(MAX_BUFF_SIZE, length);
        for (i = 0; i < len; i++)
        {
            buffer[i] = i % 256;
        }

        status = qcom_dset_write(pDsetHandle, buffer, length, offset, flags, NULL, NULL);
    }
    else if (strcmp(argv[2], "commit") == 0)
    {
        status = qcom_dset_commit(pDsetHandle, NULL, NULL);
    }
    else if (strcmp(argv[2], "close") == 0)
    {
        status = qcom_dset_close(pDsetHandle, NULL, NULL);
    }
    else if (strcmp(argv[2], "size") == 0)
    {
        status = qcom_dset_size(pDsetHandle, NULL, NULL);
    }
    else if (strcmp(argv[2], "delete") == 0 && argc > 4)
    {
        dset_id = atoi(argv[3]);
        flags   = atoi(argv[4]);
        status  = qcom_dset_delete(dset_id, flags, NULL, NULL);
    }

    PRINTF("status :%d" NL, status);

    return A_OK;
}
