/** @file wlan_tests.c
 *
 *  @brief  This file provides WLAN Test API
 *
 *  Copyright 2008-2023 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include <wm_os.h>
#include <wm_net.h> /* for net_inet_aton */
#include <wlan.h>
#include <cli.h>
#include <cli_utils.h>
#include <wifi.h>
#include <wlan_tests.h>
#include <wlan_11d.h>

/*
 * NXP Test Framework (MTF) functions
 */



static void print_address(struct wlan_ip_config *addr, enum wlan_bss_role role)
{
#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
    struct ip4_addr ip, gw, nm, dns1, dns2;
    char addr_type[10] = {0};

    /* If the current network role is STA and ipv4 is not connected then do
     * not print the addresses */
    if (role == WLAN_BSS_ROLE_STA && !is_sta_ipv4_connected())
    {
        goto out;
    }
    ip.addr   = addr->ipv4.address;
    gw.addr   = addr->ipv4.gw;
    nm.addr   = addr->ipv4.netmask;
    dns1.addr = addr->ipv4.dns1;
    dns2.addr = addr->ipv4.dns2;
    if (addr->ipv4.addr_type == ADDR_TYPE_STATIC)
    {
        (void)strncpy(addr_type, "STATIC", strlen("STATIC"));
    }
    else if (addr->ipv4.addr_type == ADDR_TYPE_STATIC)
    {
        (void)strncpy(addr_type, "AUTO IP", strlen("AUTO IP"));
    }
    else
    {
        (void)strncpy(addr_type, "DHCP", strlen("DHCP"));
    }

    (void)PRINTF("\r\n\tIPv4 Address\r\n");
    (void)PRINTF("\taddress: %s", addr_type);
    (void)PRINTF("\r\n\t\tIP:\t\t%s", inet_ntoa(ip));
    (void)PRINTF("\r\n\t\tgateway:\t%s", inet_ntoa(gw));
    (void)PRINTF("\r\n\t\tnetmask:\t%s", inet_ntoa(nm));
    (void)PRINTF("\r\n\t\tdns1:\t\t%s", inet_ntoa(dns1));
    (void)PRINTF("\r\n\t\tdns2:\t\t%s", inet_ntoa(dns2));
    (void)PRINTF("\r\n");
out:
#ifdef CONFIG_IPV6
    if (role == WLAN_BSS_ROLE_STA || role == WLAN_BSS_ROLE_UAP)
    {
        int i;
        (void)PRINTF("\r\n\tIPv6 Addresses\r\n");
        for (i = 0; i < CONFIG_MAX_IPV6_ADDRESSES; i++)
        {
            if (addr->ipv6[i].addr_state != (unsigned char)IP6_ADDR_INVALID)
            {
                (void)PRINTF("\t%-13s:\t%s (%s)\r\n", ipv6_addr_type_to_desc(&addr->ipv6[i]),
                             ipv6_addr_addr_to_desc(&addr->ipv6[i]), ipv6_addr_state_to_desc(addr->ipv6[i].addr_state));
            }
        }
        (void)PRINTF("\r\n");
    }
#endif
    return;
#endif
}

#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
static const char *print_role(enum wlan_bss_role role)
{
    if (role == WLAN_BSS_ROLE_STA)
    {
        return "Infra";
    }
    else if (role == WLAN_BSS_ROLE_UAP)
    {
        return "uAP";
    }
    else if (role == WLAN_BSS_ROLE_ANY)
    {
        return "any";
    }
    else
    {
        return "unknown";
    }
}
#endif

static inline const char *sec_tag(struct wlan_network *network)
{
    if (network->security_specific == 0U)
    {
        return "\tsecurity [Wildcard]";
    }
    else
    {
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
        if (is_ep_valid_security(network->security.type))
        {
            if (network->security.wpa3_sb_192)
            {
                return "\tsecurity: WPA3 SuiteB(192)";
            }
            else if (network->security.wpa3_sb)
            {
                return "\tsecurity: WPA3 SuiteB";
            }

            return "\tsecurity: WPA2";
        }
#endif
        return "\tsecurity";
    }
}
#ifdef CONFIG_WIFI_CAPA
static int get_capa(char *arg, uint8_t *wlan_capa)
{
    if (!arg)
        return 1;
#ifdef CONFIG_11AC
        if (string_equal(arg, "11ac") != 0)
    {
        *wlan_capa = (WIFI_SUPPORT_11AC | WIFI_SUPPORT_11N | WIFI_SUPPORT_LEGACY);
        return 0;
    }
    else
#endif
        if (string_equal(arg, "11n") != 0)
    {
        *wlan_capa = (WIFI_SUPPORT_11N | WIFI_SUPPORT_LEGACY);
        return 0;
    }
    else if (string_equal(arg, "legacy") != 0)
    {
        *wlan_capa = WIFI_SUPPORT_LEGACY;
        return 0;
    }
    else
        return 1;
}
#endif

static void print_network(struct wlan_network *network)
{
#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
    (void)PRINTF("\"%s\"\r\n\tSSID: %s\r\n\tBSSID: ", network->name,
                 network->ssid[0] != '\0' ? network->ssid : "(hidden)");
    print_mac(network->bssid);

    if ((network->dot11n != 0U)
#ifdef CONFIG_11AC
        || (network->dot11ac != 0U)
#endif
    )
    {
        (void)PRINTF("\r\n\tmode: ");
#ifdef CONFIG_11AC
            if (network->dot11ac != 0U)
        {
            (void)PRINTF("802.11AC ");
        }
        else
#endif
            if (network->dot11n != 0U)
        {
            (void)PRINTF("802.11N ");
        }
    }

    if (network->channel != 0U)
    {
        (void)PRINTF("\r\n\tchannel: %d", network->channel);
    }
    else
    {
        (void)PRINTF("\r\n\tchannel: %s", "(Auto)");
    }
    (void)PRINTF("\r\n\trole: %s\r\n", print_role(network->role));

    if (network->role == WLAN_BSS_ROLE_STA)
    {
        (void)PRINTF("\r\n\tRSSI: %ddBm\r\n", network->rssi);
    }

    switch (network->security.type)
    {
        case WLAN_SECURITY_NONE:
            (void)PRINTF("%s: none\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_WEP_OPEN:
            (void)PRINTF("%s: WEP (open)\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_WEP_SHARED:
            (void)PRINTF("%s: WEP (shared)\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_WPA:
            (void)PRINTF("%s: WPA\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_WPA2:
            (void)PRINTF("%s: WPA2", sec_tag(network));
#if defined(CONFIG_11R)
            if (network->ft_psk == 1U)
            {
                (void)PRINTF(" with FT_PSK");
            }
#endif
            (void)PRINTF("\r\n");
            break;
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_11R
        case WLAN_SECURITY_WPA2_FT:
            (void)PRINTF("%s: WPA2 FT\r\n", sec_tag(network));
            break;
#endif
        case WLAN_SECURITY_WPA2_SHA256:
            (void)PRINTF("%s: WPA2 SHA256\r\n", sec_tag(network));
            break;
#endif
        case WLAN_SECURITY_WPA_WPA2_MIXED:
            (void)PRINTF("%s: WPA/WPA2 Mixed\r\n", sec_tag(network));
            break;
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
        case WLAN_SECURITY_EAP_TLS:
            (void)PRINTF("%s Enterprise EAP-TLS\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_EAP_TLS_SHA256:
            (void)PRINTF("%s Enterprise EAP-TLS SHA256\r\n", sec_tag(network));
            break;
#ifdef CONFIG_11R
        case WLAN_SECURITY_EAP_TLS_FT:
            (void)PRINTF("%s Enterprise EAP-TLS FT\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_EAP_TLS_FT_SHA384:
            (void)PRINTF("%s Enterprise EAP-TLS FT SHA384\r\n", sec_tag(network));
            break;
#endif
        case WLAN_SECURITY_EAP_TTLS:
            (void)PRINTF("%s Enterprise EAP-TTLSv%d\r\n", sec_tag(network), network->security.eap_ver);
            break;
        case WLAN_SECURITY_EAP_TTLS_MSCHAPV2:
            (void)PRINTF("%s Enterprise EAP-TTLSv%d-MSCHAPV2\r\n", sec_tag(network), network->security.eap_ver);
            break;
        case WLAN_SECURITY_EAP_PEAP_MSCHAPV2:
            (void)PRINTF("%s Enterprise EAP-PEAPv%d-MSCHAPV2\r\n", sec_tag(network), network->security.eap_ver);
            break;
        case WLAN_SECURITY_EAP_PEAP_TLS:
            (void)PRINTF("%s Enterprise EAP-PEAPv%d-TLS\r\n", sec_tag(network), network->security.eap_ver);
            break;
        case WLAN_SECURITY_EAP_PEAP_GTC:
            (void)PRINTF("%s Enterprise EAP-PEAPv%d-GTC\r\n", sec_tag(network), network->security.eap_ver);
            break;
        case WLAN_SECURITY_EAP_FAST_MSCHAPV2:
            (void)PRINTF("%s Enterprise EAP-FAST-MSCHAPV2\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_EAP_FAST_GTC:
            (void)PRINTF("%s Enterprise EAP-FAST-GTC\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_EAP_SIM:
            (void)PRINTF("%s Enterprise EAP SIM\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_EAP_AKA:
            (void)PRINTF("%s Enterprise EAP AKA\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_EAP_AKA_PRIME:
            (void)PRINTF("%s Enterprise EAP AKA PRIME\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_EAP_WILDCARD:
            (void)PRINTF("%s Enterprise EAP WILDCARD\r\n", sec_tag(network));
            break;
#endif
#ifdef CONFIG_OWE
        case WLAN_SECURITY_OWE_ONLY:
            (void)PRINTF("%s: OWE Only\r\n", sec_tag(network));
            break;
#endif
        case WLAN_SECURITY_WPA3_SAE:
            (void)PRINTF("%s: WPA3 SAE", sec_tag(network));
#if defined(CONFIG_11R)
            if (network->ft_sae == 1U)
            {
                (void)PRINTF(" with FT_SAE");
            }
#endif
            (void)PRINTF("\r\n");
            break;
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_11R
        case WLAN_SECURITY_WPA3_SAE_FT:
            (void)PRINTF("%s: WPA3 SAE FT\r\n", sec_tag(network));
            break;
#endif
#endif
        case WLAN_SECURITY_WPA2_WPA3_SAE_MIXED:
            (void)PRINTF("%s: WPA2/WPA3 SAE Mixed\r\n", sec_tag(network));
            break;
        default:
            (void)PRINTF("\r\nUnexpected WLAN SECURITY\r\n");
            break;
    }
#ifdef CONFIG_WIFI_CAPA
    if (network->role == WLAN_BSS_ROLE_UAP)
    {
#ifdef CONFIG_11AC
        uint8_t enable_11ac = false;
#endif
        uint8_t enable_11n = false;

#ifdef CONFIG_11AC
        enable_11ac = wlan_check_11ac_capa(network->channel);
#endif
        enable_11n = wlan_check_11n_capa(network->channel);
#ifdef CONFIG_11AC
            if (network->wlan_capa & WIFI_SUPPORT_11AC)
        {
            if (!enable_11ac)
            {
                (void)PRINTF("\twifi capability: 11n\r\n");
            }
            else
            {
                (void)PRINTF("\twifi capability: 11ac\r\n");
            }

            (void)PRINTF("\tuser configure: 11ac\r\n");
        }
        else
#endif
            if (network->wlan_capa & WIFI_SUPPORT_11N)
        {
            if (!enable_11n)
            {
                (void)PRINTF("\twifi capability: legacy\r\n");
            }
            else
            {
                (void)PRINTF("\twifi capability: 11n\r\n");
            }

            (void)PRINTF("\tuser configure: 11n\r\n");
        }
        else
        {
            (void)PRINTF("\twifi capability: legacy\r\n");
            (void)PRINTF("\tuser configure: legacy\r\n");
        }
    }
#endif
#ifdef CONFIG_WPA_SUPP
    if (network->role == WLAN_BSS_ROLE_STA)
    {
        (void)PRINTF("\tProactive Key Caching: %s\r\n", network->security.pkc == 1 ? "Enabled" : "Disabled");
    }
#endif
    print_address(&network->ip, network->role);
#endif
}

/* Parse the 'arg' string as "ip:ipaddr,gwaddr,netmask,[dns1,dns2]" into
 * a wlan_ip_config data structure */
static int get_address(char *arg, struct wlan_ip_config *ip)
{
    char *ipaddr = NULL, *gwaddr = NULL, *netmask = NULL;
    char *dns1 = NULL, *dns2 = NULL;

    ipaddr = strstr(arg, "ip:");
    if (ipaddr == NULL)
    {
        return -1;
    }
    ipaddr += 3;

    gwaddr = strstr(ipaddr, ",");
    if (gwaddr == NULL)
    {
        return -1;
    }
    *gwaddr++ = (char)0;

    netmask = strstr(gwaddr, ",");
    if (netmask == NULL)
    {
        return -1;
    }
    *netmask++ = (char)0;

    dns1 = strstr(netmask, ",");
    if (dns1 != NULL)
    {
        *dns1++ = (char)0;
        dns2    = strstr(dns1, ",");
    }
    ip->ipv4.address = net_inet_aton(ipaddr);
    ip->ipv4.gw      = net_inet_aton(gwaddr);
    ip->ipv4.netmask = net_inet_aton(netmask);

    if (dns1 != NULL)
    {
        ip->ipv4.dns1 = net_inet_aton(dns1);
    }

    if (dns2 != NULL)
    {
        ip->ipv4.dns2 = net_inet_aton(dns2);
    }

    return 0;
}

static int get_security(int argc, char **argv, enum wlan_security_type type, struct wlan_network_security *sec)
{
    int ret = WM_SUCCESS;
    if (argc < 1)
    {
        return -WM_FAIL;
    }

    switch (type)
    {
        case WLAN_SECURITY_WPA:
        case WLAN_SECURITY_WPA2:
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_11R
        case WLAN_SECURITY_WPA2_FT:
#endif
#endif
        case WLAN_SECURITY_WPA2_SHA256:
            /* copy the PSK phrase */
            sec->psk_len = (uint8_t)strlen(argv[0]);
            if (sec->psk_len < WLAN_PSK_MIN_LENGTH)
            {
                return -WM_FAIL;
            }
            if (sec->psk_len < sizeof(sec->psk))
            {
                (void)strcpy(sec->psk, argv[0]);
            }
            else
            {
                return -WM_FAIL;
            }
            sec->type = type;
            break;
        default:
            ret = -WM_FAIL;
            break;
    }

    return ret;
}

static bool get_role(char *arg, enum wlan_bss_role *role)
{
    if (arg == NULL)
    {
        return true;
    }

    if (string_equal(arg, "sta") != false)
    {
        *role = WLAN_BSS_ROLE_STA;
        return false;
    }
    else if (string_equal(arg, "uap") != false)
    {
        *role = WLAN_BSS_ROLE_UAP;
        return false;
    }
    else
    {
        return true;
    }
}

/*
 * MTF Shell Commands
 */
static void dump_wlan_add_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("For Station interface\r\n");
    (void)PRINTF("  For DHCP IP Address assignment:\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [wpa2"
#ifdef CONFIG_WPA_SUPP
        "/wpa2-sha256"
#ifdef CONFIG_11R
        "/wpa2-ft"
#endif
#endif
        " <secret>] [mfpc <1> mfpr <0/1>] "
        "\r\n");
    (void)PRINTF("      If using WPA2 security, set the PMF configuration if required.\r\n");
#ifdef CONFIG_WPA_SUPP
    (void)PRINTF("If using proactive key caching set pkc as 1, to disable set to 0(default)\r\n");
    (void)PRINTF("If using specific ciphers, set the group, pairwise and group mgmt using gc, pc and gmc options.\r\n");
    (void)PRINTF("supported ciphers: ccmp=0x10, gcmp=0x40, gcmp_256=0x100, ccmp_256=0x200\r\n");
    (void)PRINTF(
        "supported group mgmt ciphers: aes_128_cmac=0x20, bip_gmac_128=0x800, bip_gmac_256=0x1000, "
        "bip_cmac_256=0x2000\r\n");
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [wpa3-sb/wpa3-sb-192] [eap-wild/eap-tls/eap-tls-sha256"
#ifdef CONFIG_11R
        "/eap-tls-ft/eap-tls-ft-sha384"
#endif
        " id <identity> "
        "[key_passwd "
        "<client_key_passwd>][hash <hash>][domain_match <domain_match_string>]] [mfpc <1> mfpr <0/1>] [mc 0x10 uc 0x10 "
        "gc 0x20]"
        "\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [wpa3-sb/wpa3-sb-192] [eap-ttls aid <anonymous identity> [key2_passwd "
        "<client_key2_passwd>]] [mfpc <1> mfpr <0/1>]"
        "\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [wpa3-sb/wpa3-sb-192] [eap-ttls-mschapv2 aid <anonymous identity> id "
        "<identity> pass "
        "<password> [key_passwd <client_key_passwd>]] [mfpc <1> mfpr <0/1>]"
        "\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [wpa3-sb/wpa3-sb-192] [eap-peap-mschapv2/eap-peap-tls/eap-peap-gtc "
        "[ver 0/1] id <identity> pass "
        "<password> [key_passwd <client_key_passwd>]] [mfpc <1> mfpr <0/1>]"
        "\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [wpa3-sb/wpa3-sb-192] [eap-fast-mschapv2/eap-fast-gtc aid <anonymous "
        "identity> id <identity> pass "
        "<password> [key_passwd <client_key_passwd>]] [mfpc <1> mfpr <0/1>]"
        "\r\n");

    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [eap-sim id <identity> pass <password>]"
        "\r\n");

    (void)PRINTF("      If using WPA2/WPA3 Enterprise security, set the PMF configuration as required.\r\n");
#endif
#endif
#ifdef CONFIG_OWE
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [owe_only] mfpc 1 mfpr 1"
        "\r\n");
    (void)PRINTF("      If using OWE only security, always set the PMF configuration.\r\n");
#endif
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [wpa3 sae"
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_11R
        "/sae-ft"
#endif
#endif
        " <secret> [pwe <0/1/2>] mfpc <1> mfpr <0/1>]"
        "\r\n");
    (void)PRINTF("      If using WPA3 SAE security, always set the PMF configuration.\r\n");
    (void)PRINTF("  For static IP address assignment:\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid>\r\n"
        "    ip:<ip_addr>,<gateway_ip>,<netmask>\r\n");
    (void)PRINTF(
        "    [bssid <bssid>] [channel <channel number>]\r\n"
        "    [wpa2"
#ifdef CONFIG_WPA_SUPP
        "/wpa2-sha256"
#ifdef CONFIG_11R
        "/wap2-ft"
#endif
#endif
        " <secret>]"
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
        " [wpa3-sb/wpa3-sb-192] [eap-tls/eap-tls-sha256"
#ifdef CONFIG_11R
        "/eap-tls-ft/eap-tls-ft-sha384"
#endif
        "]"
#endif
#ifdef CONFIG_OWE
        " [owe_only]"
#endif
        " [wpa3 sae"
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_11R
        "/sae-ft"
#endif
#endif
        " <secret>]"
        " [mfpc <0/1> mfpr <0/1>]"
        "\r\n");

    (void)PRINTF("For Micro-AP interface\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid>\r\n"
        "    ip:<ip_addr>,<gateway_ip>,<netmask>\r\n");
    (void)PRINTF(
        "    role uap [bssid <bssid>]\r\n"
        "    [channel <channelnumber>]\r\n");
    (void)PRINTF(
        "    [wpa2"
#ifdef CONFIG_WPA_SUPP
        "/wpa2-sha256"
#endif
        " <secret>] [wpa3 sae <secret> [pwe <0/1/2>] [tr <0/1"
#ifdef CONFIG_WPA_SUPP
        "/2/4/8"
#endif
        ">]]\r\n");
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_HOSTAPD
#ifdef CONFIG_11R
    (void)PRINTF("    [wpa2-ft <secret>] [wpa3 sae-ft <secret>]\r\n");
#endif
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
    (void)PRINTF(
        "    [wpa3-sb/wpa3-sb-192] "
        "[eap-tls/eap-tls-sha256/eap-ttls-mschapv2/eap-peap-mschapv2/eap-peap-tls/eap-peap-gtc/eap-fast-mschapv2/"
        "eap-fast-gtc]\r\n");
#ifdef CONFIG_11R
    (void)PRINTF("    [eap-tls-ft/eap-tls-ft-sha384]\r\n");
#endif
#endif
#endif
#endif
#ifdef CONFIG_OWE
    (void)PRINTF("    [owe_only]\r\n");
#endif
    (void)PRINTF("    [mfpc <0/1>] [mfpr <0/1>]\r\n");
#ifdef CONFIG_11AC
    (void)PRINTF(
        "Note: Setting the channel value greater than or equal to 36 is mandatory,\r\n"
        "      if UAP bandwidth is set to 80MHz.\r\n");
#endif
#ifdef CONFIG_WIFI_CAPA
    (void)PRINTF("\r\n");
#if   defined CONFIG_11AC
    (void)PRINTF("    [capa <11ac/11n/legacy>]\r\n");
#else
    (void)PRINTF("    [capa <11n/legacy>]\r\n");
#endif
#endif
    (void)PRINTF("If Set channel to 0, set acs_band to 0 1.\r\n");
    (void)PRINTF("0: 2.4GHz channel   1: 5GHz channel  Not support to select dual band automatically.\r\n");
}

static void test_wlan_add(int argc, char **argv)
{
    struct wlan_network network;
    int ret    = 0;
    int arg    = 1;
    size_t len = 0U;
    struct
    {
        unsigned ssid : 1;
        unsigned bssid : 1;
        unsigned channel : 1;
        unsigned address : 2;
        unsigned security : 1;
        unsigned security2 : 1;
        unsigned security3 : 1;
        unsigned role : 1;
        unsigned mfpc : 1;
        unsigned mfpr : 1;
#ifdef CONFIG_WPA_SUPP
        unsigned pkc : 1;
        unsigned gcipher : 1;
        unsigned pcipher : 1;
        unsigned gmcipher : 1;
#endif
#ifdef CONFIG_WIFI_CAPA
        unsigned wlan_capa : 1;
#endif
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
        unsigned wpa3_sb : 1;
#endif
        unsigned acs_band : 1;
    } info;

    (void)memset(&info, 0, sizeof(info));
    (void)memset(&network, 0, sizeof(struct wlan_network));

    if (argc < 4)
    {
        dump_wlan_add_usage();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }

    len = strlen(argv[arg]);
    if (len >= WLAN_NETWORK_NAME_MAX_LENGTH)
    {
        (void)PRINTF("Error: network name too long\r\n");
        return;
    }

    (void)memcpy(network.name, argv[arg], len);
    arg++;
    info.address = (u8_t)ADDR_TYPE_DHCP;
    do
    {
        if ((info.ssid == 0U) && string_equal("ssid", argv[arg]))
        {
            len = strlen(argv[arg + 1]);
            if (len > IEEEtypes_SSID_SIZE)
            {
                (void)PRINTF("Error: SSID is too long\r\n");
                return;
            }
            (void)memcpy(network.ssid, argv[arg + 1], len);
            arg += 2;
            info.ssid = 1;
        }
        else if ((info.bssid == 0U) && string_equal("bssid", argv[arg]))
        {
            if (get_mac(argv[arg + 1], network.bssid, ':') != false)
            {
                (void)PRINTF(
                    "Error: invalid BSSID argument"
                    "\r\n");
                return;
            }
            arg += 2;
            info.bssid = 1;
        }
        else if ((info.channel == 0U) && string_equal("channel", argv[arg]))
        {
            if (arg + 1 >= argc || get_uint(argv[arg + 1], &network.channel, strlen(argv[arg + 1])))
            {
                (void)PRINTF(
                    "Error: invalid channel"
                    " argument\n");
                return;
            }
            arg += 2;
            info.channel = 1;
        }
        else if (strncmp(argv[arg], "ip:", 3) == 0)
        {
            if (get_address(argv[arg], &network.ip) != 0)
            {
                (void)PRINTF(
                    "Error: invalid address"
                    " argument\n");
                return;
            }
            arg++;
            info.address = (u8_t)ADDR_TYPE_STATIC;
        }
        else if ((info.security == 0U) && string_equal("wpa", argv[arg]))
        {
            network.security.type = WLAN_SECURITY_WPA;

            if (get_security(argc - arg - 1, argv + arg + 1, WLAN_SECURITY_WPA, &network.security) != 0)
            {
                (void)PRINTF(
                    "Error: invalid WPA security"
                    " argument\r\n");
                return;
            }
            arg += 2;
            info.security++;
        }
        else if ((info.security2 == 0U) && (string_equal("wpa2", argv[arg]) || string_equal("wpa2-sha256", argv[arg])
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_11R
                                            || string_equal("wpa2-ft", argv[arg])
#endif
#endif
                                                ))
        {
            if (string_equal("wpa2", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_WPA2;
            }
#ifdef CONFIG_WPA_SUPP
            else if (string_equal("wpa2-sha256", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_WPA2_SHA256;
            }
#ifdef CONFIG_11R
            else if (string_equal("wpa2-ft", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_WPA2_FT;
            }
#endif
#endif
            if (get_security(argc - arg - 1, argv + arg + 1, network.security.type, &network.security) != 0)
            {
                (void)PRINTF(
                    "Error: invalid WPA2/WPA2_FT security"
                    " argument\r\n");
                return;
            }
            arg += 2;
            info.security2++;
        }
#ifdef CONFIG_OWE
        else if (!info.security && string_equal("owe_only", argv[arg]))
        {
            network.security.type = WLAN_SECURITY_OWE_ONLY;
            arg += 1;
            info.security++;
        }
#endif
        else if ((info.security3 == 0U) && string_equal("wpa3", argv[arg]))
        {
            if ((string_equal(argv[arg + 1], "sae") != false)
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_11R
                || (string_equal(argv[arg + 1], "sae-ft") != false)
#endif
#endif
            )
            {
                if (string_equal(argv[arg + 1], "sae") != false)
                {
                    network.security.type = WLAN_SECURITY_WPA3_SAE;
                }
#ifdef CONFIG_WPA_SUPP
#ifdef CONFIG_11R
                else if (string_equal(argv[arg + 1], "sae-ft") != false)
                {
                    network.security.type = WLAN_SECURITY_WPA3_SAE_FT;
                }
#endif
#endif
                /* copy the PSK phrase */
                network.security.password_len = strlen(argv[arg + 2]);
                if (network.security.password_len == 0U)
                {
                    (void)PRINTF(
                        "Error: invalid WPA3 security"
                        " argument\r\n");
                    return;
                }
                if (network.security.password_len < sizeof(network.security.password))
                {
                    (void)strcpy(network.security.password, argv[arg + 2]);
                }
                else
                {
                    (void)PRINTF(
                        "Error: invalid WPA3 security"
                        " argument\r\n");
                    return;
                }
                arg += 2;

                if (string_equal(argv[arg + 1], "pwe") != false)
                {
                    errno                           = 0;
                    network.security.pwe_derivation = strtol(argv[arg + 2], NULL, 10);
                    if (errno != 0)
                    {
                        (void)PRINTF("Error during strtoul:pwe errno:%d\r\n", errno);
                    }
                    if (arg + 2 >= argc ||
                        (network.security.pwe_derivation != 0 && network.security.pwe_derivation != 1 &&
                         network.security.pwe_derivation != 2))
                    {
                        (void)PRINTF(
                            "Error: invalid wireless"
                            " network pwe derivation\r\n");
                        return;
                    }
                    arg += 2;

                    if (string_equal(argv[arg + 1], "tr") != false)
                    {
                        errno                               = 0;
                        network.security.transition_disable = strtol(argv[arg + 2], NULL, 10);
                        if (errno != 0)
                        {
                            (void)PRINTF("Error during strtoul:pwe errno:%d\r\n", errno);
                        }
                        if (arg + 2 >= argc ||
                            (network.security.transition_disable != 0 && network.security.transition_disable != 1
#ifdef CONFIG_WPA_SUPP
                             && network.security.transition_disable != 2 && network.security.transition_disable != 4 &&
                             network.security.transition_disable != 8
#endif
                             ))
                        {
                            (void)PRINTF(
                                "Error: invalid wireless"
                                " network transition state\r\n");
                            return;
                        }
                        arg += 2;
                    }
                }
            }
            else
            {
                (void)PRINTF(
                    "Error: invalid WPA3 security"
                    " argument\r\n");
                return;
            }
            arg += 1;
            info.security3++;
        }
#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
        else if ((info.wpa3_sb == 0U) && string_equal("wpa3-sb", argv[arg]))
        {
            network.security.wpa3_sb = 1;
            arg += 1;
            info.wpa3_sb = 1;
        }
        else if ((info.wpa3_sb == 0U) && string_equal("wpa3-sb-192", argv[arg]))
        {
            network.security.wpa3_sb_192 = 1;
            arg += 1;
            info.wpa3_sb = 1;
        }
        else if ((info.security2 == 0U) &&
                 (string_equal("eap-wild", argv[arg]) || string_equal("eap-tls", argv[arg]) ||
                  string_equal("eap-tls-sha256", argv[arg]) ||
#ifdef CONFIG_11R
                  string_equal("eap-tls-ft", argv[arg]) || string_equal("eap-tls-ft-sha384", argv[arg]) ||
#endif
                  string_equal("eap-ttls", argv[arg]) || string_equal("eap-ttls-mschapv2", argv[arg]) ||
                  string_equal("eap-peap-mschapv2", argv[arg]) || string_equal("eap-peap-tls", argv[arg]) ||
                  string_equal("eap-peap-gtc", argv[arg]) || string_equal("eap-fast-mschapv2", argv[arg]) ||
                  string_equal("eap-fast-gtc", argv[arg]) || string_equal("eap-sim", argv[arg]) ||
                  string_equal("eap-aka", argv[arg]) || string_equal("eap-aka-prime", argv[arg])))
        {
            if (string_equal("eap-wild", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_WILDCARD;
            }
            else if (string_equal("eap-tls", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_TLS;
            }
            else if (string_equal("eap-tls-sha256", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_TLS_SHA256;
            }
#ifdef CONFIG_11R
            else if (string_equal("eap-tls-ft", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_TLS_FT;
            }
            else if (string_equal("eap-tls-ft-sha384", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_TLS_FT_SHA384;
            }
            else
#endif
                if (string_equal("eap-ttls", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_TTLS;
            }
            else if (string_equal("eap-ttls-mschapv2", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_TTLS_MSCHAPV2;
            }
            else if (string_equal("eap-peap-mschapv2", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_PEAP_MSCHAPV2;
            }
            else if (string_equal("eap-peap-tls", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_PEAP_TLS;
            }
            else if (string_equal("eap-peap-gtc", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_PEAP_GTC;
            }
            else if (string_equal("eap-fast-mschapv2", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_FAST_MSCHAPV2;
            }
            else if (string_equal("eap-fast-gtc", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_FAST_GTC;
            }
            else if (string_equal("eap-sim", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_SIM;
            }
            else if (string_equal("eap-aka", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_AKA;
            }
            else if (string_equal("eap-aka_prime", argv[arg]))
            {
                network.security.type = WLAN_SECURITY_EAP_AKA_PRIME;
            }

            network.security.eap_ver = 1;
            if (string_equal(argv[arg + 1], "ver") != false)
            {
                errno                    = 0;
                network.security.eap_ver = (bool)strtol(argv[arg + 2], NULL, 10);
                if (errno != 0)
                {
                    (void)PRINTF("Error during strtoul:eap_ver errno:%d\r\n", errno);
                }
                if (arg + 1 >= argc || (network.security.eap_ver != 0U && network.security.eap_ver != 1U))
                {
                    (void)PRINTF(
                        "Error: invalid wireless"
                        " network peap version\r\n");
                    return;
                }
                arg += 2;
            }

            if (string_equal(argv[arg + 1], "label") != false)
            {
                errno                       = 0;
                network.security.peap_label = (bool)strtol(argv[arg + 2], NULL, 10);
                if (errno != 0)
                {
                    (void)PRINTF("Error during strtoul:peap_label errno:%d\r\n", errno);
                }
                if (arg + 1 >= argc || (network.security.peap_label != 0U && network.security.peap_label != 1U))
                {
                    (void)PRINTF(
                        "Error: invalid wireless"
                        " network peap label\r\n");
                    return;
                }
                arg += 2;
            }

            if (string_equal(argv[arg + 1], "aid") != false)
            {
                /* Set Client Anonymous Identity */
                strcpy(network.security.anonymous_identity, argv[arg + 2]);
                arg += 2;
            }

            while (string_equal(argv[arg + 1], "id") != false)
            {
                /* For Station Set Client Identity */
                strcpy(network.security.identity, argv[arg + 2]);
                /* For uAP Set External Client Identity */
                strcpy(network.security.identities[network.security.nusers], argv[arg + 2]);

                arg += 2;

                if (string_equal(argv[arg + 1], "pass") != false)
                {
                    /* Set Client Password */
                    strcpy(network.security.eap_password, argv[arg + 2]);
                    /* For uAP Set External Client Identity */
                    strcpy(network.security.passwords[network.security.nusers], argv[arg + 2]);
                    arg += 2;
                }

                network.security.nusers += 1;
            }

            if (string_equal(argv[arg + 1], "key_passwd") != false)
            {
                /* Set Client/Server Key password */
                strcpy(network.security.client_key_passwd, argv[arg + 2]);
                strcpy(network.security.server_key_passwd, argv[arg + 2]);
                arg += 2;
            }

            if (string_equal(argv[arg + 1], "hash") != false)
            {
                /* CA Cert hash */
                strcpy(network.security.ca_cert_hash, argv[arg + 2]);
                arg += 2;
            }

            if (string_equal(argv[arg + 1], "domain_match") != false)
            {
                /* Domain match */
                strcpy(network.security.domain_match, argv[arg + 2]);
                arg += 2;
            }

            info.security2++;
            arg += 1;
        }
#endif
        else if ((info.role == 0U) && string_equal("role", argv[arg]))
        {
            if (arg + 1 >= argc || get_role(argv[arg + 1], &network.role))
            {
                (void)PRINTF(
                    "Error: invalid wireless"
                    " network role\r\n");
                return;
            }
            arg += 2;
            info.role++;
        }
        else if ((info.mfpc == 0U) && string_equal("mfpc", argv[arg]))
        {
            errno                 = 0;
            network.security.mfpc = (bool)strtol(argv[arg + 1], NULL, 10);
            if (errno != 0)
            {
                (void)PRINTF("Error during strtoul:mfpc errno:%d\r\n", errno);
            }
            if (arg + 1 >= argc || (network.security.mfpc != false && network.security.mfpc != true))
            {
                (void)PRINTF(
                    "Error: invalid wireless"
                    " network mfpc\r\n");
                return;
            }
            arg += 2;
            info.mfpc++;
        }
        else if ((info.mfpr == 0U) && string_equal("mfpr", argv[arg]))
        {
            errno                 = 0;
            network.security.mfpr = (bool)strtol(argv[arg + 1], NULL, 10);
            if (errno != 0)
            {
                (void)PRINTF("Error during strtoul:mfpr errno:%d\r\n", errno);
            }
            if (arg + 1 >= argc || (network.security.mfpr != false && network.security.mfpr != true))
            {
                (void)PRINTF(
                    "Error: invalid wireless"
                    " network mfpr\r\n");
                return;
            }
            arg += 2;
            info.mfpr++;
        }
        else if (strncmp(argv[arg], "autoip", 6) == 0)
        {
            info.address = (u8_t)ADDR_TYPE_LLA;
            arg++;
        }
#ifdef CONFIG_WPA_SUPP
        else if (!info.pkc && string_equal("pkc", argv[arg]))
        {
            unsigned short pkc;
            if (arg + 1 >= argc)
            {
                (void)PRINTF(
                    "Error: invalid proactive key caching"
                    " argument \r\n");
                return;
            }
            pkc = a2hex_or_atoi(argv[arg + 1]);

            network.security.pkc = pkc;
            arg += 2;
            info.pkc = 1;
        }
        else if (!info.gcipher && string_equal("gc", argv[arg]))
        {
            unsigned short gcipher;
            if (arg + 1 >= argc)
            {
                (void)PRINTF(
                    "Error: invalid Group cipher"
                    " argument \r\n");
                return;
            }
            gcipher = a2hex_or_atoi(argv[arg + 1]);

            network.security.group_cipher = gcipher;
            arg += 2;
            info.gcipher = 1;
        }
        else if (!info.pcipher && string_equal("pc", argv[arg]))
        {
            unsigned short pcipher;
            if (arg + 1 >= argc)
            {
                (void)PRINTF(
                    "Error: invalid Pairwise cipher"
                    " argument \r\n");
                return;
            }
            pcipher = a2hex_or_atoi(argv[arg + 1]);

            network.security.pairwise_cipher = pcipher;
            arg += 2;
            info.pcipher = 1;
        }
        else if (!info.gmcipher && string_equal("gmc", argv[arg]))
        {
            unsigned short gmcipher;
            if (arg + 1 >= argc)
            {
                (void)PRINTF(
                    "Error: invalid Group Mgmt cipher"
                    " argument \r\n");
                return;
            }
            gmcipher = a2hex_or_atoi(argv[arg + 1]);

            network.security.group_mgmt_cipher = gmcipher;
            arg += 2;
            info.gmcipher = 1;
        }
#endif
#ifdef CONFIG_WIFI_CAPA
        else if (!info.wlan_capa && network.role == WLAN_BSS_ROLE_UAP && string_equal("capa", argv[arg]))
        {
            if (arg + 1 >= argc || get_capa(argv[arg + 1], &network.wlan_capa))
            {
                (void)PRINTF(
                    "Error: invalid wireless"
                    " capability\r\n");
                return;
            }
            arg += 2;
            info.wlan_capa++;
        }
#endif
        else if (!info.acs_band && string_equal("acs_band", argv[arg]))
        {
            unsigned int ACS_band = 0;
            if (arg + 1 >= argc || get_uint(argv[arg + 1], &ACS_band, strlen(argv[arg + 1])))
            {
                (void)PRINTF("Error: invalid acs_band\r\n");
                return;
            }
            if (ACS_band != 0 && ACS_band != 1)
            {
                (void)PRINTF("Pls Set acs_band to 0 or 1.\r\n");
                (void)PRINTF(
                    "0: 2.4GHz channel   1: 5GHz channel\r\n"
                    "Not support to select dual band automatically.\r\n");
                return;
            }
            network.acs_band = (uint16_t)ACS_band;
            arg += 2;
            info.acs_band = 1;
        }
        else
        {
            dump_wlan_add_usage();
            (void)PRINTF("Error: argument %d is invalid\r\n", arg);
            return;
        }
    } while (arg < argc);

    if ((info.ssid == 0U) && (info.bssid == 0U))
    {
        dump_wlan_add_usage();
        (void)PRINTF("Error: specify at least the SSID or BSSID\r\n");
        return;
    }

    if ((info.security && info.security2 && info.security3) ||
        ((network.security.type == WLAN_SECURITY_WPA) && info.security && !info.security2))
    {
        dump_wlan_add_usage();
        (void)PRINTF("Error: not support WPA or WPA/WPA2/WPA3 Mixed\r\n");
        return;
    }

    if ((network.security.type == WLAN_SECURITY_WPA) || (network.security.type == WLAN_SECURITY_WPA2))
    {
        if (network.security.psk_len && info.security && info.security2)
            network.security.type = WLAN_SECURITY_WPA_WPA2_MIXED;
    }

    if ((network.security.type == WLAN_SECURITY_WPA2) || (network.security.type == WLAN_SECURITY_WPA3_SAE))
    {
        if ((network.security.psk_len != 0U) && (network.security.password_len != 0U))
        {
            network.security.type = WLAN_SECURITY_WPA2_WPA3_SAE_MIXED;
        }
    }

    network.ip.ipv4.addr_type         = (enum address_types)(info.address);
    network.ssid[IEEEtypes_SSID_SIZE] = '\0';
    ret                               = wlan_add_network(&network);
    switch (ret)
    {
        case WM_SUCCESS:
            (void)PRINTF("Added \"%s\"\r\n", network.name);
            break;
        case -WM_E_INVAL:
            (void)PRINTF("Error: network already exists or invalid arguments\r\n");
            break;
        case -WM_E_NOMEM:
            (void)PRINTF("Error: network list is full\r\n");
            break;
        case WLAN_ERROR_STATE:
            (void)PRINTF("Error: can't add networks in this state\r\n");
            break;
        default:
            (void)PRINTF(
                "Error: unable to add network for unknown"
                " reason\r\n");
            break;
    }
}

#ifdef CONFIG_WPA_SUPP_WPS
/** Enum that indicates type of WPS session
 *   either a push button or a PIN based session is
 *   determined by value fo this enum
 */
enum wps_session_types
{
    /** WPS session is not active */
    WPS_SESSION_INACTIVE = 0xffff,
    /** WPS Push Button session active */
    WPS_SESSION_PBC = 0x0004,
    /** WPS PIN session active */
    WPS_SESSION_PIN = 0x0000,
};
#endif

static int __scan_cb(unsigned int count)
{
#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
    struct wlan_scan_result res;
    unsigned int i;
    int err;

    if (count == 0U)
    {
        (void)PRINTF("no networks found\r\n");
        return 0;
    }

    (void)PRINTF("%d network%s found:\r\n", count, count == 1U ? "" : "s");

    for (i = 0; i < count; i++)
    {
        err = wlan_get_scan_result(i, &res);
        if (err != 0)
        {
            (void)PRINTF("Error: can't get scan res %d\r\n", i);
            continue;
        }

        print_mac(res.bssid);

        if (res.ssid[0] != '\0')
        {
            (void)PRINTF(" \"%s\" %s\r\n", res.ssid, print_role(res.role));
        }
        else
        {
            (void)PRINTF(" (hidden) %s\r\n", print_role(res.role));
        }
        (void)PRINTF("\tmode: ");
#ifdef CONFIG_11AC
            if (res.dot11ac != 0U)
        {
            (void)PRINTF("802.11AC ");
        }
        else
#endif
            if (res.dot11n != 0U)
        {
            (void)PRINTF("802.11N ");
        }
        else
        {
            (void)PRINTF("802.11BG ");
        }
        (void)PRINTF("\r\n");

        (void)PRINTF("\tchannel: %d\r\n", res.channel);
        (void)PRINTF("\trssi: -%d dBm\r\n", res.rssi);
        (void)PRINTF("\tsecurity: ");
        if (res.wep != 0U)
        {
            (void)PRINTF("WEP ");
        }
        if ((res.wpa != 0U) && (res.wpa2 != 0U))
        {
            (void)PRINTF("WPA/WPA2 Mixed ");
        }
        else if ((res.wpa2 != 0U) && (res.wpa3_sae != 0U))
        {
            (void)PRINTF("WPA2/WPA3 SAE Mixed ");
        }
        else
        {
            if (res.wpa != 0U)
            {
                (void)PRINTF("WPA ");
            }
            if (res.wpa2 != 0U)
            {
                (void)PRINTF("WPA2 ");
            }
            if (res.wpa2_sha256 != 0U)
            {
                (void)PRINTF("WPA2 SHA256");
            }
            if (res.wpa3_sae != 0U)
            {
                (void)PRINTF("WPA3 SAE ");
            }
#ifdef CONFIG_OWE
            if (res.owe != 0U)
            {
                (void)PRINTF("OWE ");
            }
#endif
            if (res.wpa2_entp != 0U)
            {
                (void)PRINTF("WPA2 Enterprise ");
            }
            if (res.wpa2_entp_sha256 != 0U)
            {
                (void)PRINTF("WPA2 SHA256 Enterprise ");
            }
            if (res.wpa3_1x_sha256 != 0U)
            {
                (void)PRINTF("WPA3 SHA256 Enterprise ");
            }
            if (res.wpa3_1x_sha384 != 0U)
            {
                (void)PRINTF("WPA3 SHA384 Enterprise ");
            }
        }
#if defined(CONFIG_11R)
        if (res.ft_1x != 0U)
        {
            (void)PRINTF("with FT_802.1x");
        }
        if (res.ft_psk != 0U)
        {
            (void)PRINTF("with FT_PSK");
        }
        if (res.ft_sae != 0U)
        {
            (void)PRINTF("with FT_SAE");
        }
        if (res.ft_1x_sha384 != 0U)
        {
            (void)PRINTF("with FT_802.1x SHA384");
        }
#endif
        if (!((res.wep != 0U) || (res.wpa != 0U) || (res.wpa2 != 0U) || (res.wpa3_sae != 0U) || (res.wpa2_entp != 0U) ||
              (res.wpa2_sha256 != 0U) ||
#ifdef CONFIG_OWE
              (res.owe != 0U) ||
#endif
              (res.wpa2_entp_sha256 != 0U) || (res.wpa3_1x_sha256 != 0U) || (res.wpa3_1x_sha384 != 0U)))
        {
            (void)PRINTF("OPEN ");
        }
        (void)PRINTF("\r\n");

        (void)PRINTF("\tWMM: %s\r\n", (res.wmm != 0U) ? "YES" : "NO");

#ifdef CONFIG_11K
        if (res.neighbor_report_supported == true)
        {
            (void)PRINTF("\t802.11K: YES\r\n");
        }
#endif
#ifdef CONFIG_11V
        if (res.bss_transition_supported == true)
        {
            (void)PRINTF("\t802.11V: YES\r\n");
        }
#endif
        if ((res.ap_mfpc == true) && (res.ap_mfpr == true))
        {
            (void)PRINTF("\t802.11W: Capable, Required\r\n");
        }
        if ((res.ap_mfpc == true) && (res.ap_mfpr == false))
        {
            (void)PRINTF("\t802.11W: Capable\r\n");
        }
        if ((res.ap_mfpc == false) && (res.ap_mfpr == false))
        {
            (void)PRINTF("\t802.11W: NA\r\n");
        }
#ifdef CONFIG_WPA_SUPP_WPS
        if (res.wps)
        {
            if (res.wps_session == WPS_SESSION_PBC)
                (void)PRINTF("\tWPS: %s, Session: %s\r\n", "YES", "Push Button");
            else if (res.wps_session == WPS_SESSION_PIN)
                (void)PRINTF("\tWPS: %s, Session: %s\r\n", "YES", "PIN");
            else
                (void)PRINTF("\tWPS: %s, Session: %s\r\n", "YES", "Not active");
        }
        else
            (void)PRINTF("\tWPS: %s \r\n", "NO");
#endif
#ifdef CONFIG_OWE
        if (res.trans_ssid_len != 0U)
        {
            (void)PRINTF("\tOWE BSSID: ");
            print_mac(res.trans_bssid);
            (void)PRINTF("\r\n\tOWE SSID:");
            if (res.trans_ssid_len != 0U)
            {
                (void)PRINTF(" \"%s\"\r\n", res.trans_ssid);
            }
        }
#endif
    }
#endif

    return 0;
}

static void test_wlan_thread_info(int argc, char **argv)
{
    os_dump_threadinfo(NULL);
}

static void test_wlan_net_stats(int argc, char **argv)
{
    net_stat();
}

static void test_wlan_scan(int argc, char **argv)
{
    if (wlan_scan(__scan_cb) != 0)
    {
        (void)PRINTF("Error: scan request failed\r\n");
    }
    else
    {
        (void)PRINTF("Scan scheduled...\r\n");
    }
}

static void dump_wlan_scan_opt_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF(
        "    wlan-scan-opt ssid <ssid> bssid <bssid> "
        "channel <channel> probes <probes>"
        "\r\n");
}

static void test_wlan_scan_opt(int argc, char **argv)
{
    wlan_scan_params_v2_t wlan_scan_param;
    int arg = 1;
    int num_ssid = 0;
    struct
    {
        unsigned ssid : 1;
        unsigned bssid : 1;
        unsigned channel : 1;
        unsigned probes : 1;
    } info;

    (void)memset(&info, 0, sizeof(info));
    (void)memset(&wlan_scan_param, 0, sizeof(wlan_scan_params_v2_t));

    if (argc < 3)
    {
        dump_wlan_scan_opt_usage();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    do
    {
        if ((info.ssid == 0U) && string_equal("ssid", argv[arg]))
        {
            if (num_ssid > MAX_NUM_SSID)
            {
                (void)PRINTF("Error: the number of SSID is more than 2\r\n");
                return;
            }
            if (strlen(argv[arg + 1]) > IEEEtypes_SSID_SIZE)
            {
                (void)PRINTF("Error: SSID is too long\r\n");
                return;
            }
            (void)memcpy(wlan_scan_param.ssid[num_ssid], argv[arg + 1], strlen(argv[arg + 1]));
            num_ssid++;
            arg += 2;
            info.ssid = 1;
        }
        else if ((info.bssid == 0U) && string_equal("bssid", argv[arg]))
        {
            if (get_mac(argv[arg + 1], (char *)wlan_scan_param.bssid, ':') != false)
            {
                (void)PRINTF(
                    "Error: invalid BSSID argument"
                    "\r\n");
                return;
            }
            arg += 2;
            info.bssid = 1;
        }
        else if ((info.channel == 0U) && string_equal("channel", argv[arg]))
        {
            if (arg + 1 >= argc ||
                get_uint(argv[arg + 1], (unsigned int *)(void *)&wlan_scan_param.chan_list[0].chan_number,
                         strlen(argv[arg + 1])))
            {
                (void)PRINTF(
                    "Error: invalid channel"
                    " argument\n");
                return;
            }
            wlan_scan_param.num_channels           = 1;
            wlan_scan_param.chan_list[0].scan_type = MLAN_SCAN_TYPE_ACTIVE;
            wlan_scan_param.chan_list[0].scan_time = 120;
            arg += 2;
            info.channel = 1;
        }
        else if ((info.probes == 0U) && string_equal("probes", argv[arg]))
        {
            if (arg + 1 >= argc ||
                get_uint(argv[arg + 1], (unsigned int *)(void *)&wlan_scan_param.num_probes, strlen(argv[arg + 1])))
            {
                (void)PRINTF(
                    "Error: invalid probes"
                    " argument\n");
                return;
            }
            if (wlan_scan_param.num_probes > 4U)
            {
                (void)PRINTF(
                    "Error: invalid number of probes"
                    "\r\n");
                return;
            }
            arg += 2;
            info.probes = 1;
        }
        else
        {
            dump_wlan_scan_opt_usage();
            (void)PRINTF("Error: argument %d is invalid\r\n", arg);
            return;
        }
    } while (arg < argc);

    if ((info.ssid == 0U) && (info.bssid == 0U))
    {
        dump_wlan_scan_opt_usage();
        (void)PRINTF("Error: specify at least the SSID or BSSID\r\n");
        return;
    }

    wlan_scan_param.cb = __scan_cb;

    if (wlan_scan_with_opt(wlan_scan_param) != 0)
    {
        (void)PRINTF("Error: scan request failed\r\n");
    }
    else
    {
        (void)PRINTF("Scan for ");
        if (info.ssid != 0U)
        {
            (void)PRINTF("ssid \"%s\" ", wlan_scan_param.ssid[0]);
        }
        if (info.bssid != 0U)
        {
            (void)PRINTF("bssid ");
            print_mac((const char *)wlan_scan_param.bssid);
        }
        if (info.probes != 0U)
        {
            (void)PRINTF("with %d probes ", wlan_scan_param.num_probes);
        }
        (void)PRINTF("scheduled...\r\n");
    }
}

static void test_wlan_remove(int argc, char **argv)
{
    int ret;

    if (argc < 2)
    {
        (void)PRINTF("Usage: %s <profile_name>\r\n", argv[0]);
        (void)PRINTF("Error: specify network to remove\r\n");
        return;
    }

    ret = wlan_remove_network(argv[1]);
    switch (ret)
    {
        case WM_SUCCESS:
            (void)PRINTF("Removed \"%s\"\r\n", argv[1]);
            break;
        case -WM_E_INVAL:
            (void)PRINTF("Error: network not found\r\n");
            break;
        case WLAN_ERROR_STATE:
            (void)PRINTF("Error: can't remove network in this state\r\n");
            break;
        default:
            (void)PRINTF("Error: unable to remove network\r\n");
            break;
    }
}

static void test_wlan_connect(int argc, char **argv)
{
    (void)PRINTF(
        "Connecting to network...\r\nUse 'wlan-stat' for "
        "current connection status.\r\n");

    int ret = wlan_connect(argc >= 2 ? argv[1] : NULL);

    if (ret == WLAN_ERROR_STATE)
    {
        (void)PRINTF("Error: connect manager not running\r\n");
        return;
    }

    if (ret == -WM_E_INVAL)
    {
        (void)PRINTF("Usage: %s <profile_name>\r\n", argv[0]);
        (void)PRINTF("Error: specify a network to connect\r\n");
        return;
    }
}

static void test_wlan_reassociate(int argc, char **argv)
{
    (void)PRINTF(
        "Reassociating to network...\r\nUse 'wlan-stat' for "
        "current connection status.\r\n");

    int ret = wlan_reassociate();

    if (ret == WLAN_ERROR_STATE)
    {
        (void)PRINTF("Error: connect manager not running or not connected\r\n");
        return;
    }

    if (ret == -WM_E_INVAL)
    {
        (void)PRINTF("Usage: %s \r\n", argv[0]);
        return;
    }
}

static void test_wlan_start_network(int argc, char **argv)
{
    int ret;

    if (argc < 2)
    {
        (void)PRINTF("Usage: %s <profile_name>\r\n", argv[0]);
        (void)PRINTF("Error: specify a network to start\r\n");
        return;
    }

    ret = wlan_start_network(argv[1]);
    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Error: unable to start network\r\n");
    }
}

static void test_wlan_stop_network(int argc, char **argv)
{
    int ret;
    struct wlan_network network;

    (void)memset(&network, 0x00, sizeof(struct wlan_network));
    (void)wlan_get_current_uap_network(&network);
    ret = wlan_stop_network(network.name);
    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Error: unable to stop network\r\n");
    }
}

static void test_wlan_disconnect(int argc, char **argv)
{
    if (wlan_disconnect() != WM_SUCCESS)
    {
        (void)PRINTF("Error: unable to disconnect\r\n");
    }
}

static void test_wlan_stat(int argc, char **argv)
{
    enum wlan_connection_state state;
    enum wlan_ps_mode ps_mode;
    char ps_mode_str[30];

    if (wlan_get_ps_mode(&ps_mode) != 0)
    {
        (void)PRINTF(
            "Error: unable to get power save"
            " mode\r\n");
        return;
    }

    switch (ps_mode)
    {
        case WLAN_IEEE:
            (void)strcpy(ps_mode_str, "IEEE ps");
            break;
        case WLAN_DEEP_SLEEP:
            (void)strcpy(ps_mode_str, "Deep sleep");
            break;
        case WLAN_IEEE_DEEP_SLEEP:
            (void)strcpy(ps_mode_str, "IEEE ps and Deep sleep");
            break;
        case WLAN_ACTIVE:
        default:
            (void)strcpy(ps_mode_str, "Active");
            break;
    }

    if (wlan_get_connection_state(&state) != 0)
    {
        (void)PRINTF(
            "Error: unable to get STA connection"
            " state\r\n");
    }
    else
    {
        switch (state)
        {
            case WLAN_DISCONNECTED:
                (void)PRINTF("Station disconnected (%s)\r\n", ps_mode_str);
                break;
            case WLAN_SCANNING:
                (void)PRINTF("Station scanning (%s)\r\n", ps_mode_str);
                break;
            case WLAN_ASSOCIATING:
                (void)PRINTF("Station associating (%s)\r\n", ps_mode_str);
                break;
            case WLAN_ASSOCIATED:
                (void)PRINTF("Station associated (%s)\r\n", ps_mode_str);
                break;
            case WLAN_CONNECTING:
                (void)PRINTF("Station connecting (%s)\r\n", ps_mode_str);
                break;
            case WLAN_CONNECTED:
                (void)PRINTF("Station connected (%s)\r\n", ps_mode_str);
                break;
            default:
                (void)PRINTF(
                    "Error: invalid STA state"
                    " %d\r\n",
                    state);
                break;
        }
    }
    if (wlan_get_uap_connection_state(&state) != 0)
    {
        (void)PRINTF(
            "Error: unable to get uAP connection"
            " state\r\n");
    }
    else
    {
        switch (state)
        {
            case WLAN_UAP_STARTED:
                (void)strcpy(ps_mode_str, "Active");
                (void)PRINTF("uAP started (%s)\r\n", ps_mode_str);
                break;
            case WLAN_UAP_STOPPED:
                (void)PRINTF("uAP stopped\r\n");
                break;
            default:
                (void)PRINTF(
                    "Error: invalid uAP state"
                    " %d\r\n",
                    state);
                break;
        }
    }
}

static void test_wlan_list(int argc, char **argv)
{
    struct wlan_network network;
    unsigned int count;
    unsigned int i;

    if (wlan_get_network_count(&count) != 0)
    {
        (void)PRINTF("Error: unable to get number of networks\r\n");
        return;
    }

    (void)PRINTF("%d network%s%s\r\n", count, count == 1U ? "" : "s", count > 0U ? ":" : "");
    for (i = 0; i < WLAN_MAX_KNOWN_NETWORKS; i++)
    {
        if (wlan_get_network(i, &network) == WM_SUCCESS)
        {
            print_network(&network);
        }
    }
}

static void test_wlan_info(int argc, char **argv)
{
    enum wlan_connection_state state;
    struct wlan_network sta_network;
    struct wlan_network uap_network;
    int sta_found = 0;

    if (wlan_get_connection_state(&state) != 0)
    {
        (void)PRINTF(
            "Error: unable to get STA connection"
            " state\r\n");
    }
    else
    {
        switch (state)
        {
            case WLAN_CONNECTED:
                if (wlan_get_current_network(&sta_network) == WM_SUCCESS)
                {
                    (void)PRINTF("Station connected to:\r\n");
                    print_network(&sta_network);
                    sta_found = 1;
                }
                else
                {
                    (void)PRINTF("Station not connected\r\n");
                }
                break;
            default:
                (void)PRINTF("Station not connected\r\n");
                break;
        }
    }

    if (wlan_get_current_uap_network(&uap_network) != 0)
    {
        (void)PRINTF("uAP not started\r\n");
    }
    else
    {
        /* Since uAP automatically changes the channel to the one that
         * STA is on */
        if (sta_found == 1)
        {
            uap_network.channel = sta_network.channel;
        }
        if (uap_network.role == WLAN_BSS_ROLE_UAP)
        {
            (void)PRINTF("uAP started as:\r\n");
        }

        print_network(&uap_network);
    }
}

static void test_wlan_address(int argc, char **argv)
{
    struct wlan_network network;

    if (wlan_get_current_network(&network) != 0)
    {
        (void)PRINTF("not connected\r\n");
        return;
    }
    print_address(&network.ip, network.role);
}

static void test_wlan_get_uap_channel(int argc, char **argv)
{
    int channel;
    int rv = wlan_get_uap_channel(&channel);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get channel: %d\r\n", rv);
    }
    else
    {
        (void)PRINTF("uAP channel: %d\r\n", channel);
    }
}

static void test_wlan_get_uap_sta_list(int argc, char **argv)
{
#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
    int i;
    wifi_sta_list_t *sl = NULL;

    (void)wifi_uap_bss_sta_list(&sl);

    if (sl == NULL)
    {
        (void)PRINTF("Failed to get sta list\n\r");
        return;
    }

    wifi_sta_info_t *si = (wifi_sta_info_t *)(void *)(&sl->count + 1);

    (void)PRINTF("Number of STA = %d \r\n\r\n", sl->count);
    for (i = 0; i < sl->count; i++)
    {
        (void)PRINTF("STA %d information:\r\n", i + 1);
        (void)PRINTF("=====================\r\n");
        (void)PRINTF("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\r\n", si[i].mac[0], si[i].mac[1], si[i].mac[2],
                     si[i].mac[3], si[i].mac[4], si[i].mac[5]);
        (void)PRINTF("Power mfg status: %s\r\n", (si[i].power_mgmt_status == 0U) ? "active" : "power save");
        (void)PRINTF("Rssi : %d dBm\r\n\r\n", (signed char)si[i].rssi);
    }

    os_mem_free(sl);
#endif
}

static void test_wlan_ieee_ps(int argc, char **argv)
{
    int choice             = -1;
    int ret                = -WM_FAIL;
    unsigned int condition = 0;

    if (argc < 2)
    {
        (void)PRINTF("Usage: %s <0/1>\r\n", argv[0]);
        (void)PRINTF("Error: Specify 0 to Disable or 1 to Enable\r\n");
        return;
    }

    errno  = 0;
    choice = strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul:mfpc errno:%d\r\n", errno);
    }

    if (choice == 0)
    {
        ret = wlan_ieeeps_off();
        if (ret == WM_SUCCESS)
        {
            (void)PRINTF("Turned off IEEE Power Save mode\r\n");
        }
        else
        {
            (void)PRINTF("Failed to turn off IEEE Power Save mode, error: %d\r\n", ret);
        }
    }
    else if (choice == 1)
    {
        condition = (uint32_t)WAKE_ON_ARP_BROADCAST | (uint32_t)WAKE_ON_UNICAST | (uint32_t)WAKE_ON_MULTICAST |
                    (uint32_t)WAKE_ON_MAC_EVENT;
        ret = wlan_ieeeps_on(condition);
        if (ret == WM_SUCCESS)
        {
            (void)PRINTF("Turned on IEEE Power Save mode\r\n");
        }
        else
        {
            (void)PRINTF("Failed to turn on IEEE Power Save mode, error: %d\r\n", ret);
        }
    }
    else
    {
        (void)PRINTF("Error: Specify 0 to Disable or 1 to Enable\r\n");
    }
}


static void test_wlan_deep_sleep_ps(int argc, char **argv)
{
    int choice = -1;
    int ret    = -WM_FAIL;

    if (argc < 2)
    {
        (void)PRINTF("Usage: %s <0/1>\r\n", argv[0]);
        (void)PRINTF("Error: Specify 0 to Disable or 1 to Enable\r\n");
        return;
    }

    errno  = 0;
    choice = strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul:deep_sleep_ps errno:%d\r\n", errno);
    }

    if (choice == 0)
    {
        ret = wlan_deepsleepps_off();
        if (ret == WM_SUCCESS)
        {
            (void)PRINTF("Turned off Deep Sleep Power Save mode");
        }
        else
        {
            (void)PRINTF("Failed to turn off Deep Sleep Power Save mode, error: %d", ret);
        }
    }
    else if (choice == 1)
    {
        ret = wlan_deepsleepps_on();
        if (ret == WM_SUCCESS)
        {
            (void)PRINTF("Turned on Deep Sleep Power Save mode");
        }
        else
        {
            (void)PRINTF("Failed to turn on Deep Sleep Power Save mode, error: %d", ret);
        }
    }
    else
    {
        (void)PRINTF("Error: Specify 0 to Disable or 1 to Enable\r\n");
    }
}



#ifdef CONFIG_ROAMING
static void dump_wlan_roaming_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF(
        "    wlan-roaming <0/1> <rssi_threshold>"
        "\r\n");
    (void)PRINTF("Example:\r\n");
    (void)PRINTF("    wlan-roaming 1 40\r\n");
}

static void test_wlan_roaming(int argc, char **argv)
{
    int enable                 = 0;
    uint8_t rssi_low_threshold = 0;

    if ((argc != 2) && (argc != 3))
    {
        dump_wlan_roaming_usage();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }

    errno  = 0;
    enable = (int)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan roaming errno:%d\r\n", errno);
    }

    if (argc == 3)
    {
        errno              = 0;
        rssi_low_threshold = (uint8_t)strtol(argv[2], NULL, 10);
        if (errno != 0)
        {
            (void)PRINTF("Error during strtoul:rssi_threshold errno:%d\r\n", errno);
            return;
        }
    }

    wlan_set_roaming(enable, rssi_low_threshold);
    return;
}
#endif





#ifdef CONFIG_11K
static void test_wlan_host_11k_cfg(int argc, char **argv)
{
    int enable_11k;
    int ret;

    if (argc != 2)
    {
        (void)PRINTF("Usage: %s <0/1> < 0--disable host 11k; 1---enable host 11k>\r\n", argv[0]);
        return;
    }

    errno      = 0;
    enable_11k = (int)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan_host_11k errno:%d\r\n", errno);
    }

    ret = wlan_host_11k_cfg(enable_11k);

    if (ret == -WM_E_PERM)
    {
        (void)PRINTF("Please disable fw base 11k.(wlan-host-11k-enable 0)\r\n");
    }
    else if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Failed to set 11k config\r\n");
    }
    else
    {
        /* Do nothing */
    }
}

static void test_wlan_host_11k_neighbor_request(int argc, char **argv)
{
    int ret;
    int len                            = 0;
    t_u8 ssid[IEEEtypes_SSID_SIZE + 1] = {0};

    if ((argc != 1 && argc != 3) || (argc == 3 && !string_equal("ssid", argv[1])))
    {
        (void)PRINTF("Usage: %s\r\n", argv[0]);
        (void)PRINTF("or     %s ssid <ssid>\r\n", argv[0]);
        return;
    }

    if (argc == 3)
    {
        if (strlen(argv[2]) > IEEEtypes_SSID_SIZE)
        {
            (void)PRINTF("Error: ssid too long\r\n");
            return;
        }
        else
        {
            (void)memcpy((void *)ssid, (const void *)argv[2], (size_t)strlen(argv[2]));
            len       = (int)strlen(argv[2]);
            ssid[len] = (t_u8)'\0';
        }
    }

    ret = wlan_host_11k_neighbor_req(ssid);
    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Error: send neighbor report request fail\r\n");
        return;
    }
}
#endif

#ifdef CONFIG_11V
static void test_wlan_host_11v_bss_trans_query(int argc, char **argv)
{
    int ret;
    int query_reason;

    if (argc != 2)
    {
        (void)PRINTF("Usage: %s <query_reason[0..16]>\r\n", argv[0]);
        return;
    }

    errno        = 0;
    query_reason = (int)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan_host_11v_bss_trans_query errno:%d\r\n", errno);
    }

    if (query_reason < 0 || query_reason > 16)
    {
        (void)PRINTF("Usage: %s <query_reason[0..16]>\r\n", argv[0]);
        return;
    }

    ret = wlan_host_11v_bss_trans_query((t_u8)query_reason);
    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Error: send bss transition query failed\r\n");
        return;
    }
}
#endif

#ifdef CONFIG_MBO
static void test_wlan_mbo_cfg(int argc, char **argv)
{
    int enable_mbo;
    int ret;

    if (argc != 2)
    {
        (void)PRINTF("Usage: %s <0/1> < 0--disable MBO; 1---enable MBO>\r\n", argv[0]);
        return;
    }

    errno      = 0;
    enable_mbo = (int)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan mbo cfg:%d\r\n", errno);
    }

    ret = wlan_host_mbo_cfg(enable_mbo);

    if (ret == -WM_E_PERM)
    {
        (void)PRINTF("Please disable MBO.(wlan-mbo-enable 0)\r\n");
    }
    else if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Failed to config MBO\r\n");
    }
    else
    {
        /* Do nothing */
    }
}

static void test_wlan_mbo_non_prefer_chs(int argc, char **argv)
{
    int ret;
    uint8_t ch0, ch1, preference0, preference1;

    if (argc != 5)
    {
        (void)PRINTF(
            "Usage: %s <ch0> <Preference0: 0/1/255> <ch1> <Preference1: 0/1/255> < 0--non-operable; 1--prefers not to "
            "operate; 255--prefers to operate>\r\n",
            argv[0]);
        return;
    }

    errno = 0;
    ch0   = (uint8_t)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan mbo non prefer chs:%d\r\n", errno);
    }

    errno       = 0;
    preference0 = (uint8_t)strtol(argv[2], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan mbo non prefer chs:%d\r\n", errno);
    }

    errno = 0;
    ch1   = (uint8_t)strtol(argv[3], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan mbo non prefer chs:%d\r\n", errno);
    }

    errno       = 0;
    preference1 = (uint8_t)strtol(argv[4], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan mbo non prefer chs:%d\r\n", errno);
    }

    ret = wlan_mbo_peferch_cfg(ch0, preference0, ch1, preference1);

    if (ret == -WM_E_PERM)
    {
        (void)PRINTF("Please add pefer or non-pefer channels.\r\n");
    }
    else if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Failed to add pefer or non-pefer channels.\r\n");
    }
    else
    {
        /* Do nothing */
    }
}

#endif

#ifdef CONFIG_WPA_SUPP

static void test_wlan_set_okc(int argc, char **argv)
{
    int ret;
    uint8_t okc;

    if (argc != 2)
    {
        (void)PRINTF("Usage: %s <okc: 0(default)/1>\r\n", argv[0]);
        (void)PRINTF(
            "\tOpportunistic Key Caching (also known as Proactive Key Caching) default\r\n"
            "\tThis parameter can be used to set the default behavior for the\r\n"
            "\tBy default, OKC is disabled unless enabled with the global okc=1 parameter \r\n"
            "\tor with the per-network proactive_key_caching=1 parameter.\r\n"
            "With okc=1, OKC is enabled by default, but can be disabled with per-network proactive_key_caching=0 "
            "parameter.\r\n"
            "\t# 0 = Disable OKC (default)\r\n"
            "\t# 1 = Enable OKC\r\n");
        return;
    }

    errno = 0;
    okc   = (uint8_t)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan okc:%d\r\n", errno);
    }

    ret = wlan_set_okc(okc);

    if (ret == -WM_E_PERM)
    {
        (void)PRINTF("Please set correct okc.\r\n");
    }
    else if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Failed to set okc.\r\n");
    }
    else
    {
        /* Do nothing */
    }
}

static void test_wlan_pmksa_list(int argc, char **argv)
{
    int ret;
    char *buf     = NULL;
    size_t buflen = 512;

    if (argc != 1)
    {
        (void)PRINTF("Usage: %s \r\n", argv[0]);
        return;
    }

    buf = (char *)os_mem_calloc(buflen);

    if (buf == NULL)
    {
        (void)PRINTF("Failed to alloc buffer\r\n");
        return;
    }

    ret = wlan_pmksa_list(buf, buflen);

    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("PMKSA list is empty\r\n");
    }
    else
    {
        (void)PRINTF("PMKSA list\r\n");
        (void)PRINTF("%s\r\n", buf);
    }
}

static void test_wlan_pmksa_flush(int argc, char **argv)
{
    int ret;

    if (argc != 1)
    {
        (void)PRINTF("Usage: %s \r\n", argv[0]);
        return;
    }

    ret = wlan_pmksa_flush();

    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("failed to flush pmksa\r\n");
    }
    else
    {
        (void)PRINTF("Flushed PMKSA cache\r\n");
    }
}

static void test_wlan_set_scan_interval(int argc, char **argv)
{
    int ret, scan_int;

    if (argc != 2)
    {
        (void)PRINTF("Usage: %s <scan_int: in seconds>\r\n", argv[0]);
        return;
    }

    errno    = 0;
    scan_int = (uint8_t)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:wlan scan int:%d\r\n", errno);
    }

    ret = wlan_set_scan_interval(scan_int);

    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Failed to set scan interval.\r\n");
    }
    else
    {
        (void)PRINTF("Scan interval set successfully.\r\n");
    }
}

#endif



static void test_wlan_host_sleep(int argc, char **argv)
{
    int choice = -1, wowlan = 0;
    int ret = -WM_FAIL;
#ifdef CONFIG_HOST_SLEEP
    bool is_mef    = MFALSE;
    bool is_manual = MFALSE;
#endif

#ifdef CONFIG_HOST_SLEEP
#if !defined(CONFIG_WIFI_BLE_COEX_APP) || (CONFIG_WIFI_BLE_COEX_APP == 0)
    if (argc < 3 || argc > 5)
#else
    if (argc < 3 || argc > 4)
#endif
#else
    if (argc < 2)
#endif
    {
        goto done;
    }

    errno  = 0;
    choice = (int)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul:host_sleep errno:%d\r\n", errno);
    }

    if (choice == 0)
    {
        ret = wlan_send_host_sleep(HOST_SLEEP_CFG_CANCEL);
        if (ret == WM_SUCCESS)
        {
            (void)PRINTF("Cancel Previous configured Host sleep configuration");
        }
        else
        {
            (void)PRINTF("Failed to Cancel Previous configured Host sleep configuration, error: %d", ret);
        }
    }
    else if (choice == 1)
    {
        if (argc < 4)
        {
            goto done;
        }

#ifdef CONFIG_HOST_SLEEP
        if (string_equal("mef", argv[2]))
        {
            is_mef = MTRUE;
        }
        else if (string_equal("wowlan", argv[2]))
        {
            if (argc < 4)
            {
                (void)PRINTF("wake_up_conds need be specified\r\n");
                return;
            }
            if (!ISDIGIT(argv[3]) && !ISHEXSTRING(argv[3]))
            {
                (void)PRINTF("wake_up_conds need be a number\r\n");
                return;
            }
            wowlan = a2hex_or_atoi(argv[3]);
        }
        else
        {
            (void)PRINTF("wowlan/mef need be specified\r\n");
            goto done;
        }

#if !defined(CONFIG_WIFI_BLE_COEX_APP) || (CONFIG_WIFI_BLE_COEX_APP == 0)
        if ((is_mef && argc == 4) || (!is_mef && argc == 5))
        {
            if (string_equal("manual", argv[argc - 1]))
            {
                is_manual = MTRUE;
            }
            else
            {
                (void)PRINTF("Invalid input! Only <manual> is allowed if you want to put host to sleep manually\r\n");
                (void)PRINTF("Usage:\r\n");
                (void)PRINTF("    wlan-host-sleep <wowlan [wake_up_conds]>/mef> <manual>\r\n");
                (void)PRINTF("If you want to configure MEF entries, please use command wlan-multi-mef first\r\n");
                return;
            }
        }
        (void)PRINTF("%s is selected for host sleep\r\n", is_manual ? "Manual mode" : "Power Manager");
#endif
        wlan_config_host_sleep(is_mef, wowlan, is_manual);
#else
        if (string_equal(argv[2], "wowlan"))
        {
            errno = 0;
            wowlan = (int)strtol(argv[3], NULL, 10);
            if (errno != 0)
            {
                (void)PRINTF("Error during strtoul:wowlan errno:%d\r\n", errno);
            }

            ret = wlan_send_host_sleep(wowlan);
            if (ret == WM_SUCCESS)
            {
                (void)PRINTF("Host sleep configuration successs with regular condition");
            }
            else
            {
                (void)PRINTF("Failed to host sleep configuration, error: %d", ret);
            }
        }

        else
        {
            goto done;
        }
#endif
    }
    else
    {
    done:
        (void)PRINTF("Error: invalid number of arguments\r\n");
        (void)PRINTF("Usage:\r\n");
#if defined(CONFIG_HOST_SLEEP) && (!defined(CONFIG_WIFI_BLE_COEX_APP) || (CONFIG_WIFI_BLE_COEX_APP == 0))
        (void)PRINTF("    wlan-host-sleep <1/0> <wowlan [wake_up_conds]/mef> <manual>\r\n");
#else
        (void)PRINTF("    wlan-host-sleep <1/0> <wowlan [wake_up_conds]/mef>\r\n");
#endif
        (void)PRINTF("    [wake_up_conds] -- value for host wakeup conditions\r\n");
        (void)PRINTF("	       bit 0: WAKE_ON_ALL_BROADCAST\r\n");
        (void)PRINTF("	       bit 1: WAKE_ON_UNICAST\r\n");
        (void)PRINTF("	       bit 2: WAKE_ON_MAC_EVENT\r\n");
        (void)PRINTF("	       bit 3: WAKE_ON_MULTICAST\r\n");
        (void)PRINTF("	       bit 4: WAKE_ON_ARP_BROADCAST\r\n");
        (void)PRINTF("	       bit 6: WAKE_ON_MGMT_FRAME\r\n");
        (void)PRINTF("	       All bit 0 discard and not wakeup host\r\n");
#ifndef CONFIG_HOST_SLEEP
        (void)PRINTF("	       All bit 1 cancel host sleep configuration\r\n");
#endif
        (void)PRINTF("    wlan-host-sleep <1/0> wowlan 0x1e\r\n");
        return;
    }
}


#define HOSTCMD_RESP_BUFF_SIZE 1024
static u8_t host_cmd_resp_buf[HOSTCMD_RESP_BUFF_SIZE] = {0};
/* Command taken from Robust_btc.conf*/
static u8_t host_cmd_buf[] = {0xe0, 0,    0x18, 0, 0x29, 0, 0,    0, 0x01, 0,    0, 0,
                              0x38, 0x02, 0x08, 0, 0x05, 0, 0x01, 0, 0x02, 0x01, 0, 0x01};

static void test_wlan_send_hostcmd(int argc, char **argv)
{
    int ret           = -WM_FAIL;
    uint32_t reqd_len = 0;
    uint32_t len;

    ret = wlan_send_hostcmd(host_cmd_buf, sizeof(host_cmd_buf) / sizeof(u8_t), host_cmd_resp_buf,
                            HOSTCMD_RESP_BUFF_SIZE, &reqd_len);

    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Hostcmd success, response is");
        for (len = 0; len < reqd_len; len++)
        {
            (void)PRINTF("%x\t", host_cmd_resp_buf[len]);
        }
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}

#ifdef SD8801
static void test_wlan_8801_enable_ext_coex(int argc, char **argv)
{
    int ret = -WM_FAIL;
    wlan_ext_coex_config_t ext_coex_config;

    ext_coex_config.Enabled                        = 1;
    ext_coex_config.IgnorePriority                 = 0;
    ext_coex_config.DefaultPriority                = 0;
    ext_coex_config.EXT_RADIO_REQ_ip_gpio_num      = 3;
    ext_coex_config.EXT_RADIO_REQ_ip_gpio_polarity = 1;
    ext_coex_config.EXT_RADIO_PRI_ip_gpio_num      = 2;
    ext_coex_config.EXT_RADIO_PRI_ip_gpio_polarity = 1;
    ext_coex_config.WLAN_GRANT_op_gpio_num         = 1;
    ext_coex_config.WLAN_GRANT_op_gpio_polarity    = 0;
    ext_coex_config.reserved_1                     = 0x28;
    ext_coex_config.reserved_2                     = 0x3c;

    ret = wlan_set_ext_coex_config(ext_coex_config);

    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("8801 External Coex Config set successfully");
    }
    else
    {
        (void)PRINTF("8801 External Coex Config error: %d", ret);
    }
}

static void test_wlan_8801_ext_coex_stats(int argc, char **argv)
{
    int ret = -WM_FAIL;
    wlan_ext_coex_stats_t ext_coex_stats;

    ret = wlan_get_ext_coex_stats(&ext_coex_stats);

    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get external Coex statistics\r\n");
    }
    else
    {
        (void)PRINTF("BLE_EIP: %d, BLE_PRI: %d, WLAN_EIP: %d\r\n", ext_coex_stats.ext_radio_req_count,
                     ext_coex_stats.ext_radio_pri_count, ext_coex_stats.wlan_grant_count);
    }
}
#endif

#if !defined(SD8801)
static void test_wlan_set_uap_bandwidth(int argc, char **argv)
{
    uint8_t bandwidth;
    int ret = -WM_FAIL;

    if (argc < 2)
    {
#ifdef CONFIG_11AC
        (void)PRINTF("Usage: %s <1/2/3>\r\n", argv[0]);
        (void)PRINTF("Error: Specify 1 to set bandwidth 20MHz or 2 for 40MHz or 3 for 80MHz\r\n");
#else
        (void)PRINTF("Usage: %s <1/2>\r\n", argv[0]);
        (void)PRINTF("Error: Specify 1 to set bandwidth 20MHz or 2 for 40MHz\r\n");
#endif
        return;
    }

    errno     = 0;
    bandwidth = (uint8_t)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul:uap_bandwidth errno:%d\r\n", errno);
    }

    ret = wlan_uap_set_bandwidth(bandwidth);

    if (ret != WM_SUCCESS)
    {
#ifdef CONFIG_11AC
        (void)PRINTF("Usage: %s <1/2/3>\r\n", argv[0]);
        (void)PRINTF("Error: Specify 1 to set bandwidth 20MHz or 2 for 40MHz or 3 for 80MHz\r\n");
#else
        (void)PRINTF("Usage: %s <1/2>\r\n", argv[0]);
        (void)PRINTF("Error: Specify 1 to set bandwidth 20MHz or 2 for 40MHz\r\n");
#endif
    }
    else
    {
        (void)PRINTF("bandwidth set successfully\r\n");
    }
}
#endif

static void dump_hidden_ssid_usage()
{
    (void)PRINTF("Usage: wlan-set-uap-hidden-ssid <0/1/2>\r\n");
    (void)PRINTF(
        "Error: 0: broadcast SSID in beacons.\r\n"
        "1: send empty SSID (length=0) in beacons.\r\n"
        "2: clear SSID (ACSII 0), but keep the original length\r\n");
}

static void test_wlan_set_uap_hidden_ssid(int argc, char **argv)
{
    uint8_t hidden_ssid;
    int ret = -WM_FAIL;

    if (argc < 2)
    {
        dump_hidden_ssid_usage();
        return;
    }

    errno       = 0;
    hidden_ssid = (uint8_t)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul:uap_bandwidth errno:%d\r\n", errno);
    }

    ret = wlan_uap_set_hidden_ssid(hidden_ssid);

    if (ret != WM_SUCCESS)
    {
        dump_hidden_ssid_usage();
    }
    else
    {
        (void)PRINTF("SSID broadcast control set successfully\r\n");
    }
}


#if defined(CONFIG_11R)
static void dump_wlan_ft_roam_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Roam to new AP using FT:\r\n");
    (void)PRINTF("    wlan-ft-roam <bssid> <channel>\r\n");
}

static void test_wlan_ft_roam(int argc, char **argv)
{
    int ret;
    t_u8 bssid[IEEEtypes_ADDRESS_SIZE] = {0};
    t_u8 channel                       = 0;
    if (argc != 3)
    {
        dump_wlan_ft_roam_usage();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }

    if (get_mac(argv[1], (char *)bssid, ':') != false)
    {
        (void)PRINTF(
            "Error: invalid BSSID argument"
            "\r\n");
        dump_wlan_ft_roam_usage();
        return;
    }

    errno   = 0;
    channel = (t_u8)strtol(argv[2], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtol:channel errno:%d\r\n", errno);
        dump_wlan_ft_roam_usage();
        return;
    }

    ret = wlan_ft_roam(bssid, channel);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Started FT roaming\r\n");
    }
    else
    {
        (void)PRINTF("Failed to start FT roaming\r\n");
    }
}
#endif

#ifdef CONFIG_HEAP_STAT
static void test_heap_stat(int argc, char **argv)
{
    os_dump_mem_stats();
}
#endif


#ifdef CONFIG_WIFI_EU_CRYPTO
static void dump_wlan_eu_crypto_rc4(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Algorithm RC4 encryption and decryption verification\r\n");
    (void)PRINTF("wlan-eu-crypto-rc4 <EncDec>\r\n");
    (void)PRINTF("EncDec: 0-Decrypt, 1-Encrypt\r\n");
}
static void test_wlan_eu_crypto_rc4(int argc, char **argv)
{
    unsigned int EncDec = 0U;
    t_u8 DATA[80]       = {0};
    t_u16 Length;
    int ret;
    t_u16 Dec_DataLength;
    t_u16 Enc_DataLength;
    t_u16 KeyLength;
    t_u16 KeyIVLength;
    if (argc != 2)
    {
        dump_wlan_eu_crypto_rc4();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    (void)get_uint(argv[1], &EncDec, 1);
    if (EncDec != 0U && EncDec != 1U)
    {
        dump_wlan_eu_crypto_rc4();
        (void)PRINTF("Error: invalid EncDec\r\n");
        return;
    }
    /*Algorithm: RC4*/
    t_u8 Key[16]     = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
    KeyLength        = 16;
    t_u8 EncData[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12};
    Enc_DataLength   = 16;
    t_u8 DecData[16] = {0xd9, 0x90, 0x42, 0xad, 0x51, 0xab, 0x11, 0x3f, 0x24, 0x46, 0x69, 0xe6, 0xf1, 0xac, 0x49, 0xf5};
    Dec_DataLength   = 16;
    t_u8 KeyIV[8]    = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
    KeyIVLength      = 8;

    if (EncDec == 0U)
    {
        (void)memcpy(DATA, DecData, Dec_DataLength);
        Length = Dec_DataLength;
        ret    = wlan_set_crypto_RC4_decrypt(Key, KeyLength, KeyIV, KeyIVLength, DATA, &Length);
    }
    else
    {
        (void)memcpy(DATA, EncData, Enc_DataLength);
        Length = Enc_DataLength;
        ret    = wlan_set_crypto_RC4_encrypt(Key, KeyLength, KeyIV, KeyIVLength, DATA, &Length);
    }
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Raw Data:\r\n");
        if (EncDec == 0U)
        {
            dump_hex((t_u8 *)DecData, Dec_DataLength);
            (void)PRINTF("Decrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
        else
        {
            dump_hex((t_u8 *)EncData, Enc_DataLength);
            (void)PRINTF("Encrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}

static void dump_wlan_eu_crypto_aes_ecb(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Algorithm AES-ECB encryption and decryption verification\r\n");
    (void)PRINTF("wlan-eu-crypto <EncDec>\r\n");
    (void)PRINTF("EncDec: 0-Decrypt, 1-Encrypt\r\n");
}
static void test_wlan_eu_crypto_aes_ecb(int argc, char **argv)
{
    unsigned int EncDec = 0U;
    t_u8 DATA[80]       = {0};
    t_u16 Length;
    int ret;
    t_u16 Dec_DataLength;
    t_u16 Enc_DataLength;
    t_u16 KeyLength;
    t_u16 KeyIVLength;
    if (argc != 2)
    {
        dump_wlan_eu_crypto_aes_ecb();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    (void)get_uint(argv[1], &EncDec, 1);
    if (EncDec != 0U && EncDec != 1U)
    {
        dump_wlan_eu_crypto_aes_ecb();
        (void)PRINTF("Error: invalid EncDec\r\n");
        return;
    }
    /*Algorithm: AES_ECB*/
    t_u8 Key[16]     = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
    KeyLength        = 16;
    t_u8 EncData[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12};
    Enc_DataLength   = 16;
    t_u8 DecData[16] = {0xc6, 0x93, 0x9d, 0xaa, 0xd1, 0xd0, 0x68, 0x28, 0xfe, 0x88, 0x52, 0x75, 0xa9, 0x43, 0xf9, 0xc0};
    Dec_DataLength   = 16;
    t_u8 KeyIV[8]    = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
    KeyIVLength      = 8;

    if (EncDec == 0U)
    {
        (void)memcpy(DATA, DecData, Dec_DataLength);
        Length = Dec_DataLength;
        ret    = wlan_set_crypto_AES_ECB_decrypt(Key, KeyLength, KeyIV, KeyIVLength, DATA, &Length);
    }
    else
    {
        (void)memcpy(DATA, EncData, Enc_DataLength);
        Length = Enc_DataLength;
        ret    = wlan_set_crypto_AES_ECB_encrypt(Key, KeyLength, KeyIV, KeyIVLength, DATA, &Length);
    }
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Raw Data:\r\n");
        if (EncDec == 0U)
        {
            dump_hex((t_u8 *)DecData, Dec_DataLength);
            (void)PRINTF("Decrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
        else
        {
            dump_hex((t_u8 *)EncData, Enc_DataLength);
            (void)PRINTF("Encrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}

static void dump_wlan_eu_crypto_aes_wrap(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Algorithm AES-WRAP encryption and decryption verification\r\n");
    (void)PRINTF("wlan-eu-crypto <EncDec>\r\n");
    (void)PRINTF("EncDec: 0-Decrypt, 1-Encrypt\r\n");
}
static void test_wlan_eu_crypto_aes_wrap(int argc, char **argv)
{
    unsigned int EncDec = 0U;
    t_u8 DATA[80]       = {0};
    t_u16 Length;
    int ret;
    t_u16 Dec_DataLength;
    t_u16 Enc_DataLength;
    t_u16 KeyLength;
    t_u16 KeyIVLength;
    if (argc != 2)
    {
        dump_wlan_eu_crypto_aes_wrap();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    (void)get_uint(argv[1], &EncDec, 1);
    if (EncDec != 0U && EncDec != 1U)
    {
        dump_wlan_eu_crypto_aes_wrap();
        (void)PRINTF("Error: invalid EncDec\r\n");
        return;
    }
    /*Algorithm: AES_WRAP*/
    t_u8 Key[16]     = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
    KeyLength        = 16;
    t_u8 EncData[16] = {0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12};
    Enc_DataLength   = 16;
    t_u8 DecData[24] = {0xfa, 0xda, 0x96, 0x53, 0x30, 0x97, 0x4b, 0x61, 0x77, 0xc6, 0xd4, 0x3c,
                        0xd2, 0x0e, 0x1f, 0x6d, 0x43, 0x8a, 0x0a, 0x1c, 0x4f, 0x6a, 0x1a, 0xd7};
    Dec_DataLength   = 24;
    t_u8 KeyIV[8]    = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
    KeyIVLength      = 8;

    if (EncDec == 0U)
    {
        (void)memcpy(DATA, DecData, Dec_DataLength);
        Length = Dec_DataLength;
        ret    = wlan_set_crypto_AES_WRAP_decrypt(Key, KeyLength, KeyIV, KeyIVLength, DATA, &Length);
    }
    else
    {
        (void)memcpy(DATA, EncData, Enc_DataLength);
        Length = Enc_DataLength;
        ret    = wlan_set_crypto_AES_WRAP_encrypt(Key, KeyLength, KeyIV, KeyIVLength, DATA, &Length);
    }
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Raw Data:\r\n");
        if (EncDec == 0U)
        {
            dump_hex((t_u8 *)DecData, Dec_DataLength);
            (void)PRINTF("Decrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
        else
        {
            dump_hex((t_u8 *)EncData, Enc_DataLength);
            (void)PRINTF("Encrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}

static void dump_wlan_eu_crypto_ccmp_128(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Algorithm AES-CCMP-128 encryption and decryption verification\r\n");
    (void)PRINTF("wlan-eu-crypto <EncDec>\r\n");
    (void)PRINTF("EncDec: 0-Decrypt, 1-Encrypt\r\n");
}
static void test_wlan_eu_crypto_ccmp_128(int argc, char **argv)
{
    unsigned int EncDec = 0U;
    t_u8 DATA[80]       = {0};
    t_u16 Length;
    int ret;
    t_u16 Dec_DataLength;
    t_u16 Enc_DataLength;
    t_u16 KeyLength;
    t_u16 NonceLength;
    t_u16 AADLength;
    if (argc != 2)
    {
        dump_wlan_eu_crypto_ccmp_128();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    (void)get_uint(argv[1], &EncDec, 1);
    if (EncDec != 0U && EncDec != 1U)
    {
        dump_wlan_eu_crypto_ccmp_128();
        (void)PRINTF("Error: invalid EncDec\r\n");
        return;
    }
    /*Algorithm: AES_CCMP_128*/
    t_u8 Key[16]     = {0xc9, 0x7c, 0x1f, 0x67, 0xce, 0x37, 0x11, 0x85, 0x51, 0x4a, 0x8a, 0x19, 0xf2, 0xbd, 0xd5, 0x2f};
    KeyLength        = 16;
    t_u8 EncData[20] = {0xf8, 0xba, 0x1a, 0x55, 0xd0, 0x2f, 0x85, 0xae, 0x96, 0x7b,
                        0xb6, 0x2f, 0xb6, 0xcd, 0xa8, 0xeb, 0x7e, 0x78, 0xa0, 0x50};
    Enc_DataLength   = 20;
    t_u8 DecData[28] = {0xf3, 0xd0, 0xa2, 0xfe, 0x9a, 0x3d, 0xbf, 0x23, 0x42, 0xa6, 0x43, 0xe4, 0x32, 0x46,
                        0xe8, 0x0c, 0x3c, 0x04, 0xd0, 0x19, 0x78, 0x45, 0xce, 0x0b, 0x16, 0xf9, 0x76, 0x23};
    Dec_DataLength   = 28;
    t_u8 Nonce[13]   = {0x00, 0x50, 0x30, 0xf1, 0x84, 0x44, 0x08, 0xb5, 0x03, 0x97, 0x76, 0xe7, 0x0c};
    NonceLength      = 13;
    t_u8 AAD[22]     = {0x08, 0x40, 0x0f, 0xd2, 0xe1, 0x28, 0xa5, 0x7c, 0x50, 0x30, 0xf1,
                    0x84, 0x44, 0x08, 0xab, 0xae, 0xa5, 0xb8, 0xfc, 0xba, 0x00, 0x00};
    AADLength        = 22;

    if (EncDec == 0U)
    {
        (void)memcpy(DATA, DecData, Dec_DataLength);
        Length = Dec_DataLength;
        ret    = wlan_set_crypto_AES_CCMP_decrypt(Key, KeyLength, AAD, AADLength, Nonce, NonceLength, DATA, &Length);
    }
    else
    {
        (void)memcpy(DATA, EncData, Enc_DataLength);
        Length = Enc_DataLength;
        ret    = wlan_set_crypto_AES_CCMP_encrypt(Key, KeyLength, AAD, AADLength, Nonce, NonceLength, DATA, &Length);
    }
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Raw Data:\r\n");
        if (EncDec == 0U)
        {
            dump_hex((t_u8 *)DecData, Dec_DataLength);
            (void)PRINTF("Decrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
        else
        {
            dump_hex((t_u8 *)EncData, Enc_DataLength);
            (void)PRINTF("Encrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}

static void dump_wlan_eu_crypto_ccmp_256(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Algorithm AES-CCMP-256 encryption and decryption verification\r\n");
    (void)PRINTF("wlan-eu-crypto <EncDec>\r\n");
    (void)PRINTF("EncDec: 0-Decrypt, 1-Encrypt\r\n");
}
static void test_wlan_eu_crypto_ccmp_256(int argc, char **argv)
{
    unsigned int EncDec = 0U;
    t_u8 DATA[80]       = {0};
    t_u16 Length;
    int ret;
    t_u16 Dec_DataLength;
    t_u16 Enc_DataLength;
    t_u16 KeyLength;
    t_u16 NonceLength;
    t_u16 AADLength;
    if (argc != 2)
    {
        dump_wlan_eu_crypto_ccmp_256();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    (void)get_uint(argv[1], &EncDec, 1);
    if (EncDec != 0U && EncDec != 1U)
    {
        dump_wlan_eu_crypto_ccmp_256();
        (void)PRINTF("Error: invalid EncDec\r\n");
        return;
    }
    /*Algorithm: AES_WRAP*/
    t_u8 Key[32]     = {0xc9, 0x7c, 0x1f, 0x67, 0xce, 0x37, 0x11, 0x85, 0x51, 0x4a, 0x8a, 0x19, 0xf2, 0xbd, 0xd5, 0x2f,
                    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    KeyLength        = 32;
    t_u8 EncData[20] = {0xf8, 0xba, 0x1a, 0x55, 0xd0, 0x2f, 0x85, 0xae, 0x96, 0x7b,
                        0xb6, 0x2f, 0xb6, 0xcd, 0xa8, 0xeb, 0x7e, 0x78, 0xa0, 0x50};
    Enc_DataLength   = 20;
    t_u8 DecData[36] = {0x6d, 0x15, 0x5d, 0x88, 0x32, 0x66, 0x82, 0x56, 0xd6, 0xa9, 0x2b, 0x78,
                        0xe1, 0x1d, 0x8e, 0x54, 0x49, 0x5d, 0xd1, 0x74, 0x80, 0xaa, 0x56, 0xc9,
                        0x49, 0x2e, 0x88, 0x2b, 0x97, 0x64, 0x2f, 0x80, 0xd5, 0x0f, 0xe9, 0x7b};
    Dec_DataLength   = 36;
    t_u8 Nonce[13]   = {0x00, 0x50, 0x30, 0xf1, 0x84, 0x44, 0x08, 0xb5, 0x03, 0x97, 0x76, 0xe7, 0x0c};
    NonceLength      = 13;
    t_u8 AAD[22]     = {0x08, 0x40, 0x0f, 0xd2, 0xe1, 0x28, 0xa5, 0x7c, 0x50, 0x30, 0xf1,
                    0x84, 0x44, 0x08, 0xab, 0xae, 0xa5, 0xb8, 0xfc, 0xba, 0x00, 0x00};
    AADLength        = 22;

    if (EncDec == 0U)
    {
        (void)memcpy(DATA, DecData, Dec_DataLength);
        Length = Dec_DataLength;
        ret    = wlan_set_crypto_AES_CCMP_decrypt(Key, KeyLength, AAD, AADLength, Nonce, NonceLength, DATA, &Length);
    }
    else
    {
        (void)memcpy(DATA, EncData, Enc_DataLength);
        Length = Enc_DataLength;
        ret    = wlan_set_crypto_AES_CCMP_encrypt(Key, KeyLength, AAD, AADLength, Nonce, NonceLength, DATA, &Length);
    }
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Raw Data:\r\n");
        if (EncDec == 0U)
        {
            dump_hex((t_u8 *)DecData, Dec_DataLength);
            (void)PRINTF("Decrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
        else
        {
            dump_hex((t_u8 *)EncData, Enc_DataLength);
            (void)PRINTF("Encrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}

static void dump_wlan_eu_crypto_gcmp_128(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Algorithm AES-GCMP-128 encryption and decryption verification\r\n");
    (void)PRINTF("wlan-eu-crypto <EncDec>\r\n");
    (void)PRINTF("EncDec: 0-Decrypt, 1-Encrypt\r\n");
}
static void test_wlan_eu_crypto_gcmp_128(int argc, char **argv)
{
    unsigned int EncDec = 0U;
    t_u8 DATA[80]       = {0};
    t_u16 Length;
    int ret;
    t_u16 Dec_DataLength;
    t_u16 Enc_DataLength;
    t_u16 KeyLength;
    t_u16 NonceLength;
    t_u16 AADLength;
    if (argc != 2)
    {
        dump_wlan_eu_crypto_gcmp_128();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    (void)get_uint(argv[1], &EncDec, 1);
    if (EncDec != 0U && EncDec != 1U)
    {
        dump_wlan_eu_crypto_gcmp_128();
        (void)PRINTF("Error: invalid EncDec\r\n");
        return;
    }
    /*Algorithm: AES_WRAP*/
    t_u8 Key[16]     = {0xc9, 0x7c, 0x1f, 0x67, 0xce, 0x37, 0x11, 0x85, 0x51, 0x4a, 0x8a, 0x19, 0xf2, 0xbd, 0xd5, 0x2f};
    KeyLength        = 16;
    t_u8 EncData[40] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
                        0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
                        0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
    Enc_DataLength   = 40;
    t_u8 DecData[56] = {0x60, 0xe9, 0x70, 0x0c, 0xc4, 0xd4, 0x0a, 0xc6, 0xd2, 0x88, 0xb2, 0x01, 0xc3, 0x8f,
                        0x5b, 0xf0, 0x8b, 0x80, 0x74, 0x42, 0x64, 0x0a, 0x15, 0x96, 0xe5, 0xdb, 0xda, 0xd4,
                        0x1d, 0x1f, 0x36, 0x23, 0xf4, 0x5d, 0x7a, 0x12, 0xdb, 0x7a, 0xfb, 0x23, 0xe9, 0x04,
                        0x97, 0xa1, 0xec, 0x9c, 0x5e, 0x8b, 0x85, 0x5b, 0x9d, 0xc3, 0xa8, 0x16, 0x91, 0xa3};
    Dec_DataLength   = 56;
    t_u8 Nonce[12]   = {0x50, 0x30, 0xf1, 0x84, 0x44, 0x08, 0x00, 0x89, 0x5f, 0x5f, 0x2b, 0x08};
    NonceLength      = 12;
    t_u8 AAD[24]     = {0x88, 0x48, 0x0f, 0xd2, 0xe1, 0x28, 0xa5, 0x7c, 0x50, 0x30, 0xf1, 0x84,
                    0x44, 0x08, 0x50, 0x30, 0xf1, 0x84, 0x44, 0x08, 0x80, 0x33, 0x03, 0x00};
    AADLength        = 24;

    if (EncDec == 0U)
    {
        (void)memcpy(DATA, DecData, Dec_DataLength);
        Length = Dec_DataLength;
        ret    = wlan_set_crypto_AES_GCMP_decrypt(Key, KeyLength, AAD, AADLength, Nonce, NonceLength, DATA, &Length);
    }
    else
    {
        (void)memcpy(DATA, EncData, Enc_DataLength);
        Length = Enc_DataLength;
        ret    = wlan_set_crypto_AES_GCMP_encrypt(Key, KeyLength, AAD, AADLength, Nonce, NonceLength, DATA, &Length);
    }
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Raw Data:\r\n");
        if (EncDec == 0U)
        {
            dump_hex((t_u8 *)DecData, Dec_DataLength);
            (void)PRINTF("Decrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
        else
        {
            dump_hex((t_u8 *)EncData, Enc_DataLength);
            (void)PRINTF("Encrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}

static void dump_wlan_eu_crypto_gcmp_256(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Algorithm AES-GCMP-256 encryption and decryption verification\r\n");
    (void)PRINTF("wlan-eu-crypto <EncDec>\r\n");
    (void)PRINTF("EncDec: 0-Decrypt, 1-Encrypt\r\n");
}
static void test_wlan_eu_crypto_gcmp_256(int argc, char **argv)
{
    unsigned int EncDec = 0U;
    t_u8 DATA[80]       = {0};
    t_u16 Length;
    int ret;
    t_u16 Dec_DataLength;
    t_u16 Enc_DataLength;
    t_u16 KeyLength;
    t_u16 NonceLength;
    t_u16 AADLength;
    if (argc != 2)
    {
        dump_wlan_eu_crypto_gcmp_256();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    (void)get_uint(argv[1], &EncDec, 1);
    if (EncDec != 0U && EncDec != 1U)
    {
        dump_wlan_eu_crypto_gcmp_256();
        (void)PRINTF("Error: invalid EncDec\r\n");
        return;
    }
    /*Algorithm: AES_WRAP*/
    t_u8 Key[32]     = {0xc9, 0x7c, 0x1f, 0x67, 0xce, 0x37, 0x11, 0x85, 0x51, 0x4a, 0x8a, 0x19, 0xf2, 0xbd, 0xd5, 0x2f,
                    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    KeyLength        = 32;
    t_u8 EncData[40] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
                        0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
                        0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
    Enc_DataLength   = 40;
    t_u8 DecData[56] = {0x65, 0x83, 0x43, 0xc8, 0xb1, 0x44, 0x47, 0xd9, 0x21, 0x1d, 0xef, 0xd4, 0x6a, 0xd8,
                        0x9c, 0x71, 0x0c, 0x6f, 0xc3, 0x33, 0x33, 0x23, 0x6e, 0x39, 0x97, 0xb9, 0x17, 0x6a,
                        0x5a, 0x8b, 0xe7, 0x79, 0xb2, 0x12, 0x66, 0x55, 0x5e, 0x70, 0xad, 0x79, 0x11, 0x53,
                        0x9a, 0x0e, 0x22, 0xc1, 0x26, 0x0c, 0xbb, 0xe8, 0x35, 0x93, 0x35, 0xf3, 0x37, 0x65};
    Dec_DataLength   = 56;
    t_u8 Nonce[12]   = {0x50, 0x30, 0xf1, 0x84, 0x44, 0x08, 0x00, 0x89, 0x5f, 0x5f, 0x2b, 0x08};
    NonceLength      = 12;
    t_u8 AAD[24]     = {0x88, 0x48, 0x0f, 0xd2, 0xe1, 0x28, 0xa5, 0x7c, 0x50, 0x30, 0xf1, 0x84,
                    0x44, 0x08, 0x50, 0x30, 0xf1, 0x84, 0x44, 0x08, 0x80, 0x33, 0x03, 0x00};
    AADLength        = 24;

    if (EncDec == 0U)
    {
        (void)memcpy(DATA, DecData, Dec_DataLength);
        Length = Dec_DataLength;
        ret    = wlan_set_crypto_AES_GCMP_decrypt(Key, KeyLength, AAD, AADLength, Nonce, NonceLength, DATA, &Length);
    }
    else
    {
        (void)memcpy(DATA, EncData, Enc_DataLength);
        Length = Enc_DataLength;
        ret    = wlan_set_crypto_AES_GCMP_encrypt(Key, KeyLength, AAD, AADLength, Nonce, NonceLength, DATA, &Length);
    }
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Raw Data:\r\n");
        if (EncDec == 0U)
        {
            dump_hex((t_u8 *)DecData, Dec_DataLength);
            (void)PRINTF("Decrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
        else
        {
            dump_hex((t_u8 *)EncData, Enc_DataLength);
            (void)PRINTF("Encrypted Data:\r\n");
            dump_hex((t_u8 *)DATA, Length);
        }
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}
#endif

#ifdef CONFIG_HEAP_DEBUG
int os_mem_alloc_cnt = 0;
int os_mem_free_cnt  = 0;

static void test_wlan_os_mem_stat(int argc, char **argv)
{
    (void)PRINTF("os_mem_alloc_cnt: %d \r\n", os_mem_alloc_cnt);
    (void)PRINTF("os_mem_free_cnt : %d \r\n", os_mem_free_cnt);
    (void)PRINTF("FreeHeapSize    : %d \r\n\r\n", xPortGetFreeHeapSize());
    wlan_show_os_mem_stat();
}
#endif



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
    (void)PRINTF("\t           default value is 6000 milli seconds\r\n");
}

static void wlan_antcfg_set(int argc, char *argv[])
{
    int ret;
    uint32_t ant_mode;
    uint16_t evaluate_time = 0;

    if (!(argc >= 2 && argc <= 3))
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
        evaluate_time = (uint16_t)strtol(argv[2], NULL, 10);
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
    int ret                  = -WM_FAIL;
    uint32_t ant_mode        = 0;
    uint16_t evaluate_time   = 0;
    uint16_t current_antenna = 0;

    if (argc != 1)
    {
        dump_wlan_get_antcfg_usage();
        return;
    }

    ret = wlan_get_antcfg(&ant_mode, &evaluate_time, &current_antenna);
    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Mode of Tx/Rx path is : %x\r\n", ant_mode);
        if (ant_mode == 0XFFFFU)
        {
            (void)PRINTF("Evaluate time : %d\r\n", evaluate_time);
        }
        if (current_antenna > 0)
        {
            (void)PRINTF("Current antenna is %d\n", current_antenna);
        }
    }
    else
    {
        (void)PRINTF("antcfg configuration read failed\r\n");
        dump_wlan_get_antcfg_usage();
    }
}

static void test_wlan_set_scan_channel_gap(int argc, char **argv)
{
    unsigned scan_chan_gap;
    if (argc != 2)
    {
        (void)PRINTF("Invalid arguments\r\n");
        (void)PRINTF("Usage:\r\n");
        (void)PRINTF("wlan-scan-channel-gap <scan_gap_value>\r\n");
        (void)PRINTF("scan_gap_value: [2,500]\r\n");
        return;
    }
    scan_chan_gap = a2hex_or_atoi(argv[1]);
    if (scan_chan_gap < 2 || scan_chan_gap > 500)
    {
        (void)PRINTF("Invaild scan_gap value!\r\n");
        (void)PRINTF("Usage:\r\n");
        (void)PRINTF("wlan-scan-channel-gap <scan_gap_value>\r\n");
        (void)PRINTF("scan_gap_value: [2,500]\r\n");
        return;
    }
    wlan_set_scan_channel_gap(scan_chan_gap);
}

#ifdef CONFIG_WMM
static void test_wlan_wmm_tx_stats(int argc, char **argv)
{
    int bss_type = atoi(argv[1]);

    wlan_wmm_tx_stats_dump(bss_type);
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
    (void)PRINTF("0x50 : China\r\n");
    (void)PRINTF("0xFF : Japan Special\r\n");
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
    int rv = wlan_set_region_code(region_code);
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
    int rv            = wlan_get_region_code(&region_code);
    if (rv != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get region code: 0x%x\r\n", region_code);
    }
    else
    {
        (void)PRINTF("Region code: 0x%x\r\n", region_code);
    }
}
static void test_wlan_set_mac_address(int argc, char **argv)
{
    int ret;
    uint8_t raw_mac[MLAN_MAC_ADDR_LENGTH];

    if (argc != 2)
    {
        (void)PRINTF("Usage: %s MAC_Address\r\n", argv[0]);
        return;
    }

    ret = get_mac(argv[1], (char *)raw_mac, ':');
    if (ret != 0)
    {
        (void)PRINTF("Error: invalid MAC argument\r\n");
        return;
    }

    wlan_set_mac_addr(raw_mac);
}








static void dump_wlan_tx_ampdu_prot_mode_usage()
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("    wlan-tx-ampdu-prot-mode <mode>\r\n");
    (void)PRINTF("    <mode>: 0 - Set RTS/CTS mode \r\n");
    (void)PRINTF("            1 - Set CTS2SELF mode \r\n");
    (void)PRINTF("            2 - Disable Protection mode \r\n");
    (void)PRINTF("            3 - Set Dynamic RTS/CTS mode \r\n");
    (void)PRINTF("Example:\r\n");
    (void)PRINTF("    wlan-tx-ampdu-prot-mode\r\n");
    (void)PRINTF("    - Get currently set protection mode for TX AMPDU.\r\n");
    (void)PRINTF("    wlan-tx-ampdu-prot-mode 1\r\n");
    (void)PRINTF("    - Set protection mode for TX AMPDU to CTS2SELF.\r\n");
}

static void test_wlan_tx_ampdu_prot_mode(int argc, char **argv)
{
    tx_ampdu_prot_mode_para data;

    if (argc > 2)
    {
        (void)PRINTF("Error: invalid number of arguments\r\n");
        dump_wlan_tx_ampdu_prot_mode_usage();
        return;
    }

    /* GET */
    if (argc == 1)
    {
        dump_wlan_tx_ampdu_prot_mode_usage();
        wlan_tx_ampdu_prot_mode(&data, ACTION_GET);
        (void)PRINTF("\r\nTx AMPDU protection mode: ");
        switch (data.mode)
        {
            case TX_AMPDU_RTS_CTS:
                (void)PRINTF("RTS/CTS\r\n");
                break;
            case TX_AMPDU_CTS_2_SELF:
                (void)PRINTF("CTS-2-SELF\r\n");
                break;
            case TX_AMPDU_DISABLE_PROTECTION:
                (void)PRINTF("Disabled\r\n");
                break;
            case TX_AMPDU_DYNAMIC_RTS_CTS:
                (void)PRINTF("DYNAMIC RTS/CTS\r\n");
                break;
            default:
                (void)PRINTF("Invalid protection mode\r\n");
                break;
        }
    }
    else /* SET */
    {
        data.mode = atoi(argv[1]);
        if (data.mode < 0 || data.mode > 3)
        {
            (void)PRINTF("Error: invalid protection mode\r\n");
            dump_wlan_tx_ampdu_prot_mode_usage();
            return;
        }
        wlan_tx_ampdu_prot_mode(&data, ACTION_SET);
    }
}


#if defined(CONFIG_11K) || defined(CONFIG_11V) || defined(CONFIG_11R) || defined(CONFIG_ROAMING)
static void test_wlan_rssi_low_threshold(int argc, char **argv)
{
    uint8_t rssi_threshold;

    if (argc < 2)
    {
        (void)PRINTF("Usage: %s <rssi threshold value>\r\n", argv[0]);
        (void)PRINTF("Error: Default value is 70. Specify the value you want to set as threshold.\r\n");
        return;
    }

    errno          = 0;
    rssi_threshold = (uint8_t)strtol(argv[1], NULL, 10);
    if (errno != 0)
    {
        (void)PRINTF("Error during strtoul:rssi_threshold errno:%d\r\n", errno);
        return;
    }

    wlan_set_rssi_low_threshold(rssi_threshold);

    (void)PRINTF("rssi threshold set successfully.\r\n");
}
#endif




#ifdef CONFIG_CLOUD_KEEP_ALIVE

#include "lwip/priv/tcp_priv.h"
#include "lwip/stats.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"

#define DATA_LEN 1460

static void dump_wlan_tcp_client_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("    wlan-tcp-client dst_ip <dst_ip> src_port <src_port> dst_port <dst_port>\r\n");
    (void)PRINTF("        <dst_ip> Destination IP\r\n");
    (void)PRINTF("        <src_port> Source port\r\n");
    (void)PRINTF("        <dst_port> Destination port\r\n");
    (void)PRINTF("        Please specify dst_ip src_port and dst_port\r\n");
    (void)PRINTF("    wlan-tcp-client dst_ip 192.168.1.50 src_port 54236 dst_port 9526\r\n");
}

static err_t wlan_tcp_client_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
    char clientString[] = "hello\r\n";
    t_u8 count          = 5;
    char *clientData    = NULL;

    clientData = os_mem_alloc(DATA_LEN);
    (void)memset((t_u8 *)clientData, 0, DATA_LEN);
    (void)memcpy(clientData, clientString, sizeof(clientString));

    while (count--)
    {
        tcp_write(pcb, clientData, DATA_LEN, TCP_WRITE_FLAG_COPY);
        tcp_output(pcb);
    }

    return ERR_OK;
}

static void test_wlan_tcp_client(int argc, char **argv)
{
    struct tcp_pcb *tcp_client_pcb;
    ip_addr_t ipaddr;
    int arg           = 0;
    int dst_ip_set    = 0;
    int src_port_set  = 0;
    int dst_port_set  = 0;
    t_u32 dst_ip      = 0;
    t_u16 src_port    = 0;
    t_u16 dst_port    = 0;
    unsigned int port = 0;

    if (argc < 2)
    {
        (void)PRINTF("Error: invalid number of arguments\r\n");
        dump_wlan_tcp_client_usage();
        return;
    }

    if (!is_sta_connected())
    {
        (void)PRINTF("Can not start wlan_tcp_client in disconnected state\r\n");
        return;
    }

    arg += 1;
    do
    {
        if (string_equal("dst_ip", argv[arg]))
        {
            dst_ip     = net_inet_aton(argv[arg + 1]);
            dst_ip_set = 1;
            arg += 2;
        }
        else if (string_equal("src_port", argv[arg]))
        {
            if (arg + 1 >= argc || get_uint(argv[arg + 1], (unsigned int *)&port, strlen(argv[arg + 1])))
            {
                (void)PRINTF("Error: invalid src_port argument\r\n");
                return;
            }
            src_port     = (uint16_t)(port & 0XFFFF);
            src_port_set = 1;
            arg += 2;
        }
        else if (string_equal("dst_port", argv[arg]))
        {
            if (arg + 1 >= argc || get_uint(argv[arg + 1], (unsigned int *)&port, strlen(argv[arg + 1])))
            {
                (void)PRINTF("Error: invalid dst_port argument\r\n");
                return;
            }
            dst_port     = (uint16_t)(port & 0XFFFF);
            dst_port_set = 1;
            arg += 2;
        }
        else
        {
            (void)PRINTF("Error: argument %d is invalid\r\n", arg);
            dump_wlan_tcp_client_usage();
            return;
        }
    } while (arg < argc);

    if (!dst_ip_set || !src_port_set || !dst_port_set)
    {
        dump_wlan_tcp_client_usage();
        (void)PRINTF("Error: please specify dst_ip src_port and dst_port\r\n");
        return;
    }

    if (!wlan_cloud_keep_alive_enabled(dst_ip, dst_port))
    {
        (void)PRINTF("Cloud keep alive not started for given destination ip and port\r\n");
        return;
    }

    (void)memcpy(&ipaddr.u_addr.ip4, &dst_ip, sizeof(ip4_addr_t));
#ifdef CONFIG_IPV6
    ipaddr.type = IPADDR_TYPE_V4;
#endif

    tcp_client_pcb = tcp_new();
    tcp_bind(tcp_client_pcb, IP_ADDR_ANY, src_port);

    if (tcp_client_pcb != NULL)
    {
        tcp_connect(tcp_client_pcb, &ipaddr, dst_port, wlan_tcp_client_connected);
    }
}

static void dump_wlan_cloud_keep_alive_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("    wlan-cloud-keep-alive start id <id> dst_mac <dst_mac> dst_ip <dst_ip> dst_port <dst_port>\r\n");
    (void)PRINTF("        <id> Keep alive id from 0 to 3\r\n");
    (void)PRINTF("        <dst_mac> Destination MAC address\r\n");
    (void)PRINTF("        <dst_ip> Destination IP\r\n");
    (void)PRINTF("        <dst_port> Destination port\r\n");
    (void)PRINTF("        Please specify dst_mac, dst_ip and dst_port\r\n");
    (void)PRINTF("    wlan-cloud-keep-alive stop\r\n");
    (void)PRINTF("    wlan-cloud-keep-alive reset\r\n");
}

static void test_wlan_cloud_keep_alive(int argc, char **argv)
{
    int ret          = -WM_FAIL;
    int arg          = 1;
    int id_set       = 0;
    int dst_mac_set  = 0;
    int dst_ip_set   = 0;
    int dst_port_set = 0;

    wlan_cloud_keep_alive_t cloud_keep_alive;

    /* Period to send keep alive packet, set the default value to 55s(The unit is milliseconds) */
    t_u32 send_interval_default = 55000;

    /* Period to send retry packet, set the default value to 20s(The unit is milliseconds) */
    t_u16 retry_interval_default = 20000;

    /* Count to send retry packet, set the default value to 3 */
    t_u16 retry_count_default = 3;

    if (argc < 2)
    {
        (void)PRINTF("Error: invalid number of arguments\r\n");
        dump_wlan_cloud_keep_alive_usage();
        return;
    }

    memset(&cloud_keep_alive, 0x00, sizeof(wlan_cloud_keep_alive_t));

    if (string_equal("start", argv[1]))
    {
        /* Period to send keep alive packet, set the default value to 55s(The unit is milliseconds) */
        cloud_keep_alive.send_interval = send_interval_default;
        /* Period to send retry packet, set the default value to 20s(The unit is milliseconds) */
        cloud_keep_alive.retry_interval = retry_interval_default;
        /* Count to send retry packet, set the default value to 3 */
        cloud_keep_alive.retry_count = retry_count_default;

        arg += 1;
        do
        {
            if (string_equal("id", argv[arg]))
            {
                errno                           = 0;
                cloud_keep_alive.mkeep_alive_id = strtol(argv[arg + 1], NULL, 10);
                if (errno != 0)
                {
                    (void)PRINTF("Error during strtoul:id errno:%d\r\n", errno);
                }

                id_set = 1;
                arg += 2;
            }
            else if (string_equal("dst_mac", argv[arg]))
            {
                ret = get_mac(argv[arg + 1], (char *)&cloud_keep_alive.dst_mac, ':');
                if (ret != 0)
                {
                    (void)PRINTF("Error: invalid dst_mac argument\r\n");
                    return;
                }
                dst_mac_set = 1;
                arg += 2;
            }
            else if (string_equal("dst_ip", argv[arg]))
            {
                cloud_keep_alive.dst_ip = net_inet_aton(argv[arg + 1]);
                dst_ip_set              = 1;
                arg += 2;
            }
            else if (string_equal("dst_port", argv[arg]))
            {
                unsigned int dst_port;

                if (arg + 1 >= argc || get_uint(argv[arg + 1], (unsigned int *)&dst_port, strlen(argv[arg + 1])))
                {
                    (void)PRINTF("Error: invalid dst_port argument\r\n");
                    return;
                }
                cloud_keep_alive.dst_port = (uint16_t)(dst_port & 0XFFFF);
                dst_port_set              = 1;
                arg += 2;
            }
            else
            {
                (void)PRINTF("Error: argument %d is invalid\r\n", arg);
                dump_wlan_cloud_keep_alive_usage();
                return;
            }
        } while (arg < argc);
        if (!id_set || !dst_mac_set || !dst_ip_set || !dst_port_set)
        {
            dump_wlan_cloud_keep_alive_usage();
            (void)PRINTF("Error: please specify dst_mac, dst_ip and dst_port\r\n");
            return;
        }

        cloud_keep_alive.enable = MTRUE;
    }
    else if (string_equal("stop", argv[1]))
    {
        cloud_keep_alive.enable = MFALSE;
        cloud_keep_alive.reset  = MFALSE;
    }
    else if (string_equal("reset", argv[1]))
    {
        cloud_keep_alive.enable = MFALSE;
        cloud_keep_alive.reset  = MTRUE;
    }
    else
    {
        (void)PRINTF("Error: invalid [%d] argument, give start/stop/reset\r\n", arg + 1);
        dump_wlan_cloud_keep_alive_usage();
        return;
    }
    if (cloud_keep_alive.enable == MTRUE)
    {
        ret = wlan_save_cloud_keep_alive_params(&cloud_keep_alive, 0, 0, 0, 0, MTRUE);
    }
    else
    {
        ret = wlan_stop_cloud_keep_alive(&cloud_keep_alive);
    }
}
#endif

static void test_wlan_get_signal(int argc, char **argv)
{
    wlan_rssi_info_t signal;
    int ret = WM_SUCCESS;

    if (!is_sta_connected())
    {
        (void)PRINTF("Can not get RSSI information in disconnected state\r\n");
        return;
    }

    (void)memset(&signal, 0, sizeof(wlan_rssi_info_t));

    ret = wlan_get_signal_info(&signal);
    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Unable to get RSSI information\r\n");
        return;
    }
    (void)PRINTF("\tBeaconLast\tBeacon Average\tData Last\tData Average\r\n");
    (void)PRINTF("RSSI\t%-10d \t%-10d \t%-10d \t%-10d\r\n", (int)signal.bcn_rssi_last, (int)signal.bcn_rssi_avg,
                 (int)signal.data_rssi_last, (int)signal.data_rssi_avg);
    (void)PRINTF("SNR \t%-10d \t%-10d \t%-10d \t%-10d\r\n", (int)signal.bcn_snr_last, (int)signal.bcn_snr_avg,
                 (int)signal.data_snr_last, (int)signal.data_snr_avg);
    (void)PRINTF("NF  \t%-10d \t%-10d \t%-10d \t%-10d\r\n", (int)signal.bcn_nf_last, (int)signal.bcn_nf_avg,
                 (int)signal.data_nf_last, (int)signal.data_nf_avg);
}

#ifdef CONFIG_SET_SU
static void dump_wlan_set_su_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("    wlan-set-su <0/1>\r\n");
    (void)PRINTF("    <start/stop>: 1 -- stop su\r\n");
    (void)PRINTF("                  0 -- start su\r\n");
    (void)PRINTF("Example:\r\n");
    (void)PRINTF("    wlan-set-su\r\n");
    (void)PRINTF("    - Get current su state.\r\n");
    (void)PRINTF("    wlan-set-su 1\r\n");
    (void)PRINTF("    - stop su\r\n");
}

static void test_wlan_set_su(int argc, char **argv)
{
    int ret           = -WM_FAIL;
    uint32_t reqd_len = 0;
    uint8_t state;
    u8_t debug_resp_buf[64] = {0};

    (void)memset(debug_resp_buf, 0, sizeof(debug_resp_buf));
    /**
     * Command taken from debug.conf
     *  start_su={
     *        CmdCode=0x008b
     *        Action:2=1
     *        SUBID:2=0x101
     *        Value:4=1           # 1 -- stop_su;
     *                            # 0 -- start_su;
     */
    uint8_t debug_cmd_buf[] = {0x8b, 0, 0x10, 0, 0, 0, 0, 0, 0x01, 0, 0x01, 0x01, 0x01, 0, 0, 0};

    if (argc > 2)
    {
        (void)PRINTF("Error: invalid number of arguments\r\n");
        dump_wlan_set_su_usage();
        return;
    }

    /* SET */
    if (argc == 2)
    {
        state             = atoi(argv[1]);
        debug_cmd_buf[12] = state;
    }
    else /* GET */
    {
        debug_cmd_buf[8] = 0;
    }

    ret = wlan_send_hostcmd(debug_cmd_buf, sizeof(debug_cmd_buf) / sizeof(u8_t), debug_resp_buf, sizeof(debug_resp_buf),
                            &reqd_len);

    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Hostcmd success, response is\r\n");
        for (ret = 0; ret < reqd_len; ret++)
            (void)PRINTF("%x\t", debug_resp_buf[ret]);
    }
    else
    {
        (void)PRINTF("Hostcmd failed error: %d", ret);
    }
}
#endif







#ifdef CONFIG_WPA_SUPP_WPS
static void test_wlan_start_wps_pbc(int argc, char **argv)
{
    int ret;

    ret = wlan_start_wps_pbc();

    if (ret == -WM_FAIL)
    {
        PRINTF("Start WPS PBC failed\r\n");
    }
    else if (ret == -2)
    {
        PRINTF("FAIL-PBC-OVERLAP\r\n");
    }
}

static void test_wlan_start_wps_pin(int argc, char **argv)
{
    int ret = -WM_FAIL;

    PRINTF("Start WPS PIN session with %s pin\r\n", argv[1]);

    ret = wlan_start_wps_pin(argv[1]);

    if (ret != WM_SUCCESS)
    {
        PRINTF("Invalid PIN entered\r\n");
    }
}

static void test_wlan_wps_generate_pin(int argc, char **argv)
{
    uint32_t pin = 0;

    wlan_wps_generate_pin((unsigned int *)&pin);
    PRINTF("WPS PIN is: %d\r\n", pin);
}

static void test_wlan_wps_cancel(int argc, char **argv)
{
    int ret;

    ret = wlan_wps_cancel();

    if (ret != WM_SUCCESS)
    {
        PRINTF("Cancel WPS failed\r\n");
    }
}

#ifdef CONFIG_WPA_SUPP_AP
static void test_wlan_start_ap_wps_pbc(int argc, char **argv)
{
    int ret;

    ret = wlan_start_ap_wps_pbc();

    if (ret != WM_SUCCESS)
    {
        PRINTF("Start AP WPS PBC failed\r\n");
    }
}

static void test_wlan_start_ap_wps_pin(int argc, char **argv)
{
    int ret = -WM_FAIL;

    PRINTF("Start AP WPS PIN session with %s pin\r\n", argv[1]);

    ret = wlan_start_ap_wps_pin(argv[1]);

    if (ret != WM_SUCCESS)
    {
        PRINTF("Start AP WPS PIN failed\r\n");
    }
}

static void test_wlan_wps_ap_cancel(int argc, char **argv)
{
    int ret;

    ret = wlan_wps_ap_cancel();

    if (ret != WM_SUCCESS)
    {
        PRINTF("Cancel WPS failed\r\n");
    }
}
#endif
#endif

static void dump_wlan_11d_enable_usage()
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("wlan-11d-enable <sta/uap> <0/1>, 0: disable, 1: enable\r\n");
    (void)PRINTF("This command is only used to enable/disable 11D\r\n");
    (void)PRINTF("Please use wlan-set-regioncode command to set region\r\n");
}

static void test_wlan_11d_enable(int argc, char **argv)
{
    int state;

    if (argc != 3)
    {
        dump_wlan_11d_enable_usage();
        return;
    }

    state = atoi(argv[2]);
    if (state != 0 && state != 1)
    {
        dump_wlan_11d_enable_usage();
        return;
    }

    if (string_equal("sta", argv[1]))
        wlan_set_11d_state(WLAN_BSS_TYPE_STA, state);
    else if (string_equal("uap", argv[1]))
        wlan_set_11d_state(WLAN_BSS_TYPE_UAP, state);
    else
        dump_wlan_11d_enable_usage();
}

static void dump_wlan_country_code(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Set country code:\r\n");
    (void)PRINTF("    wlan-set-country <country_code_str 3 bytes>\r\n");
    (void)PRINTF("First two octets are used as the\r\n");
    (void)PRINTF("first two octets of the Country String\r\n");
    (void)PRINTF("For example:\r\n");
    (void)PRINTF("    wlan-set-country US\r\n");
    (void)PRINTF("    wlan-set-country EU\r\n");
    (void)PRINTF("Country Code Options: \r\n");
    (void)PRINTF("    WW  (World Wide Safe)\r\n");
    (void)PRINTF("    US  (US FCC)\r\n");
    (void)PRINTF("    CA  (IC Canada)\r\n");
    (void)PRINTF("    SG  (Singapore)\r\n");
    (void)PRINTF("    EU  (ETSI)\r\n");
    (void)PRINTF("    AU  (Australia)\r\n");
    (void)PRINTF("    KR  (Republic Of Korea)\r\n");
    (void)PRINTF("    FR  (France)\r\n");
    (void)PRINTF("    JP  (Japan)\r\n");
    (void)PRINTF("    CN  (China)\r\n");
    (void)PRINTF("The third octet of the Country String as below\r\n");
    (void)PRINTF("All environments of the current frequency band and country (default)\r\n");
    (void)PRINTF("country3=0x20\r\n");
    (void)PRINTF("Outdoor environment only\r\n");
    (void)PRINTF("country3=0x4f\r\n");
    (void)PRINTF("Indoor environment only\r\n");
    (void)PRINTF("country3=0x49\r\n");
    (void)PRINTF("Noncountry entity (country_code=XX)\r\n");
    (void)PRINTF("country3=0x58\r\n");
    (void)PRINTF("IEEE 802.11 standard Annex E table indication: 0x01 .. 0x1f\r\n");
    (void)PRINTF("Annex E, Table E-4 (Global operating classes)\r\n");
    (void)PRINTF("country3=0x04\r\n");
}

static void test_wlan_set_country_code(int argc, char **argv)
{
    int ret;
    char country_code[3] = {0};

    if (argc != 2)
    {
        (void)PRINTF("Invalid arguments\r\n");
        dump_wlan_country_code();
        return;
    }

    country_code[0] = argv[1][0];
    country_code[1] = argv[1][1];

    ret = wlan_set_country_code(country_code);
    if (ret != WM_SUCCESS)
    {
        (void)PRINTF("Set country code %s is failed\r\n", country_code);
    }
    else
    {
        (void)PRINTF("Set country code %s is successful\r\n", country_code);
    }
}

#ifdef CONFIG_TURBO_MODE
static void test_wlan_get_turbo_mode(int argc, char **argv)
{
    int ret = -WM_FAIL;
    uint8_t mode;
    int bss_type = MLAN_BSS_TYPE_ANY;
    if (argc != 2)
    {
        (void)PRINTF("Error: invalid number of arguments\r\n");
        (void)PRINTF("Usage: wlan-get-turbo-mode STA/UAP\r\n");
        return;
    }

    if (string_equal("STA", argv[1]))
    {
        bss_type = MLAN_BSS_TYPE_STA;
        ret      = wlan_get_turbo_mode(&mode);
    }
    else if (string_equal("UAP", argv[1]))
    {
        bss_type = MLAN_BSS_TYPE_UAP;
        ret      = wlan_get_uap_turbo_mode(&mode);
    }
    else
    {
        (void)PRINTF("Error: invalid BSS type\r\n");
        (void)PRINTF("Usage: wlan-get-turbo-mode STA/UAP\r\n");
        return;
    }

    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("%s Turbo mode: %d\r\n", bss_type == MLAN_BSS_TYPE_STA ? "STA" : "UAP", mode);
    }
    else
    {
        (void)PRINTF("Failed to get %s turbo mode\r\n", bss_type == MLAN_BSS_TYPE_STA ? "STA" : "UAP");
    }

    return;
}

static void dump_wlan_set_turbo_mode_usage()
{
    (void)PRINTF("Usage: wlan-set-turbo-mode <STA/UAP> <mode>\r\n");
    (void)PRINTF("          <STA/UAP>  'STA'  or 'UAP' \r\n");
    (void)PRINTF("          <mode> can be 0,1,2,3\r\n");
}

static void test_wlan_set_turbo_mode(int argc, char **argv)
{
    int ret = -WM_FAIL;
    unsigned int value;
    uint8_t mode;
    int bss_type;
    if (argc != 3)
    {
        (void)PRINTF("Error: invalid number of arguments\r\n");
        dump_wlan_set_turbo_mode_usage();
        return;
    }

    if (string_equal("STA", argv[1]))
        bss_type = MLAN_BSS_TYPE_STA;
    else if (string_equal("UAP", argv[1]))
        bss_type = MLAN_BSS_TYPE_UAP;
    else
    {
        (void)PRINTF("Error: invalid BSS type\r\n");
        dump_wlan_set_turbo_mode_usage();
        return;
    }

    if (get_uint(argv[2], &value, strlen(argv[2])) && value > 3)
    {
        (void)PRINTF("Invalid mode argument\r\n");
        dump_wlan_set_turbo_mode_usage();
        return;
    }
    mode = value & 0xFF;

    if (bss_type == MLAN_BSS_TYPE_STA)
    {
        ret = wlan_set_turbo_mode(mode);
    }
    else
    {
        ret = wlan_set_uap_turbo_mode(mode);
    }

    if (ret == WM_SUCCESS)
    {
        (void)PRINTF("Set %s turbo mode to %d\r\n", bss_type == MLAN_BSS_TYPE_STA ? "STA" : "UAP", mode);
    }
    else
    {
        (void)PRINTF("Failed to set %s turbo mode\r\n", bss_type == MLAN_BSS_TYPE_STA ? "STA" : "UAP");
    }

    return;
}
#endif

static struct cli_command tests[] = {
    {"wlan-thread-info", NULL, test_wlan_thread_info},
    {"wlan-net-stats", NULL, test_wlan_net_stats},
    {"wlan-set-mac", "<MAC_Address>", test_wlan_set_mac_address},
    {"wlan-scan", NULL, test_wlan_scan},
    {"wlan-scan-opt", "ssid <ssid> bssid ...", test_wlan_scan_opt},
    {"wlan-add", "<profile_name> ssid <ssid> bssid...", test_wlan_add},
    {"wlan-remove", "<profile_name>", test_wlan_remove},
    {"wlan-list", NULL, test_wlan_list},
    {"wlan-connect", "<profile_name>", test_wlan_connect},
    {"wlan-reassociate", NULL, test_wlan_reassociate},
    {"wlan-start-network", "<profile_name>", test_wlan_start_network},
    {"wlan-stop-network", NULL, test_wlan_stop_network},
    {"wlan-disconnect", NULL, test_wlan_disconnect},
    {"wlan-stat", NULL, test_wlan_stat},
    {"wlan-info", NULL, test_wlan_info},
    {"wlan-address", NULL, test_wlan_address},
    {"wlan-get-uap-channel", NULL, test_wlan_get_uap_channel},
    {"wlan-get-uap-sta-list", NULL, test_wlan_get_uap_sta_list},
    {"wlan-ieee-ps", "<0/1>", test_wlan_ieee_ps},
    {"wlan-deep-sleep-ps", "<0/1>", test_wlan_deep_sleep_ps},
#ifdef CONFIG_11K
    {"wlan-host-11k-enable", "<0/1>", test_wlan_host_11k_cfg},
    {"wlan-host-11k-neighbor-req", "[ssid <ssid>]", test_wlan_host_11k_neighbor_request},
#endif
#ifdef CONFIG_11V
    {"wlan-host-11v-bss-trans-query", "<0..16>", test_wlan_host_11v_bss_trans_query},
#endif
#ifdef CONFIG_MBO
    {"wlan-mbo-enable", "<0/1>", test_wlan_mbo_cfg},
    {"wlan-mbo-nonprefer-ch", "<ch0> <Preference0: 0/1/255> <ch1> <Preference1: 0/1/255>",
     test_wlan_mbo_non_prefer_chs},
#endif
#ifdef CONFIG_WPA_SUPP
    {"wlan-set-okc", "<okc: 0(default)/1>", test_wlan_set_okc},
    {"wlan-pmksa-list", NULL, test_wlan_pmksa_list},
    {"wlan-pmksa-flush", NULL, test_wlan_pmksa_flush},
    {"wlan-set-scan-interval", "<scan_int: in seconds>", test_wlan_set_scan_interval},
#endif
#ifdef CONFIG_ROAMING
    {"wlan-roaming", "<0/1> <rssi_threshold>", test_wlan_roaming},
#endif
    {"wlan-host-sleep", "<0/1> wowlan <wake_up_conds>", test_wlan_host_sleep},
    {"wlan-send-hostcmd", NULL, test_wlan_send_hostcmd},
#if !defined(SD8801)
#ifdef CONFIG_11AC
    {"wlan-set-uap-bandwidth", "<1/2/3> 1:20 MHz 2:40MHz 3:80MHz", test_wlan_set_uap_bandwidth},
#else
    {"wlan-set-uap-bandwidth", "<1/2> 1:20 MHz 2:40MHz", test_wlan_set_uap_bandwidth},
#endif
#endif
    {"wlan-set-uap-hidden-ssid", "<0/1/2>", test_wlan_set_uap_hidden_ssid},
#ifdef SD8801
    {"wlan-8801-enable-ext-coex", NULL, test_wlan_8801_enable_ext_coex},
    {"wlan-8801-get-ext-coex-stats", NULL, test_wlan_8801_ext_coex_stats},
#endif
#ifdef CONFIG_WIFI_EU_CRYPTO
    {"wlan-eu-crypto-rc4", "<EncDec>", test_wlan_eu_crypto_rc4},
    {"wlan-eu-crypto-aes-wrap", "<EncDec>", test_wlan_eu_crypto_aes_wrap},
    {"wlan-eu-crypto-aes-ecb", "<EncDec>", test_wlan_eu_crypto_aes_ecb},
    {"wlan-eu-crypto-ccmp-128", "<EncDec>", test_wlan_eu_crypto_ccmp_128},
    {"wlan-eu-crypto-ccmp-256", "<EncDec>", test_wlan_eu_crypto_ccmp_256},
    {"wlan-eu-crypto-gcmp-128", "<EncDec>", test_wlan_eu_crypto_gcmp_128},
    {"wlan-eu-crypto-gcmp-256", "<EncDec>", test_wlan_eu_crypto_gcmp_256},
#endif
#ifdef CONFIG_HEAP_STAT
    {"heap-stat", NULL, test_heap_stat},
#endif
#ifdef CONFIG_HEAP_DEBUG
    {"wlan-os-mem-stat", NULL, test_wlan_os_mem_stat},
#endif
#if defined(CONFIG_11R)
    {"wlan-ft-roam", "<bssid> <channel>", test_wlan_ft_roam},
#endif
    {"wlan-set-antcfg", "<ant mode> [evaluate_time]", wlan_antcfg_set},
    {"wlan-get-antcfg", NULL, wlan_antcfg_get},
    {"wlan-scan-channel-gap", "<channel_gap_value>", test_wlan_set_scan_channel_gap},
#ifdef CONFIG_WMM
    {"wlan-wmm-stat", "<bss_type>", test_wlan_wmm_tx_stats},
#endif
    {"wlan-set-regioncode", "<region-code>", test_wlan_set_regioncode},
    {"wlan-get-regioncode", NULL, test_wlan_get_regioncode},
    {"wlan-11d-enable", "<sta/uap> <0/1>", test_wlan_11d_enable},
    {"wlan-tx-ampdu-prot-mode", "<mode>", test_wlan_tx_ampdu_prot_mode},
#if defined(CONFIG_11K) || defined(CONFIG_11V) || defined(CONFIG_11R) || defined(CONFIG_ROAMING)
    {"wlan-rssi-low-threshold", "<threshold_value>", test_wlan_rssi_low_threshold},
#endif
#ifdef CONFIG_WPA_SUPP_WPS
    {"wlan-generate-wps-pin", NULL, test_wlan_wps_generate_pin},
    {"wlan-start-wps-pbc", NULL, test_wlan_start_wps_pbc},
    {"wlan-start-wps-pin", "<8 digit pin>", test_wlan_start_wps_pin},
    {"wlan-wps-cancel", NULL, test_wlan_wps_cancel},
#ifdef CONFIG_WPA_SUPP_AP
    {"wlan-start-ap-wps-pbc", NULL, test_wlan_start_ap_wps_pbc},
    {"wlan-start-ap-wps-pin", "<8 digit pin>", test_wlan_start_ap_wps_pin},
    {"wlan-wps-ap-cancel", NULL, test_wlan_wps_ap_cancel},
#endif
#endif
    {"wlan-get-signal", NULL, test_wlan_get_signal},
#ifdef CONFIG_SET_SU
    {"wlan-set-su", "<0/1>", test_wlan_set_su},
#endif
#ifdef CONFIG_TURBO_MODE
    {"wlan-get-turbo-mode", "<STA/UAP>", test_wlan_get_turbo_mode},
    {"wlan-set-turbo-mode", "<STA/UAP> <mode>", test_wlan_set_turbo_mode},
#endif
#ifdef CONFIG_CLOUD_KEEP_ALIVE
    {"wlan-cloud-keep-alive", "<start/stop/reset>", test_wlan_cloud_keep_alive},
    {"wlan_tcp_client", "dst_ip <dst_ip> src_port <src_port> dst_port <dst_port>", test_wlan_tcp_client},
#endif
    {"wlan-set-country", "<country_code_str>", test_wlan_set_country_code},
#ifdef CONFIG_COEX_DUTY_CYCLE
    {"wlan-single-ant-duty-cycle", "<enable/disable> [<Ieee154Duration> <TotalDuration>]",
     test_wlan_single_ant_duty_cycle},
    {"wlan-dual-ant-duty-cycle", "<enable/disable> [<Ieee154Duration> <TotalDuration> <Ieee154FarRangeDuration>]",
     test_wlan_dual_ant_duty_cycle},
#endif
};

/* Register our commands with the MTF. */
int wlan_cli_init(void)
{
    int i;

    i = wlan_basic_cli_init();
    if (i != WLAN_ERROR_NONE)
    {
        return i;
    }

    if (cli_register_commands(tests, (int)(sizeof(tests) / sizeof(struct cli_command))) != 0)
    {
        return -WM_FAIL;
    }

    return WM_SUCCESS;
}
