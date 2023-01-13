/** @file wlan_tests.c
 *
 *  @brief  This file provides WLAN Test API
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
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
        return "\tsecurity";
    }
}

static void print_network(struct wlan_network *network)
{
#if SDK_DEBUGCONSOLE != DEBUGCONSOLE_DISABLE
    (void)PRINTF("\"%s\"\r\n\tSSID: %s\r\n\tBSSID: ", network->name,
                 network->ssid[0] != '\0' ? network->ssid : "(hidden)");
    print_mac(network->bssid);
    if (network->channel != 0U)
    {
        (void)PRINTF("\r\n\tchannel: %d", network->channel);
    }
    else
    {
        (void)PRINTF("\r\n\tchannel: %s", "(Auto)");
    }
    (void)PRINTF("\r\n\trole: %s\r\n", print_role(network->role));

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
            (void)PRINTF("\r\n");
            break;
        case WLAN_SECURITY_WPA_WPA2_MIXED:
            (void)PRINTF("%s: WPA/WPA2 Mixed\r\n", sec_tag(network));
            break;
        case WLAN_SECURITY_WPA3_SAE:
            (void)PRINTF("%s: WPA3 SAE", sec_tag(network));
            (void)PRINTF("\r\n");
            break;
        case WLAN_SECURITY_WPA2_WPA3_SAE_MIXED:
            (void)PRINTF("%s: WPA2/WPA3 SAE Mixed\r\n", sec_tag(network));
            break;
        default:
            (void)PRINTF("\r\nUnexpected WLAN SECURITY\r\n");
            break;
    }

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
        return WM_FAIL;
    }

    switch (type)
    {
        case WLAN_SECURITY_WPA:
        case WLAN_SECURITY_WPA2:
            /* copy the PSK phrase */
            sec->psk_len = (uint8_t)strlen(argv[0]);
            if (sec->psk_len < WLAN_PSK_MIN_LENGTH)
            {
                return WM_FAIL;
            }
            if (sec->psk_len < sizeof(sec->psk))
            {
                (void)strcpy(sec->psk, argv[0]);
            }
            else
            {
                return WM_FAIL;
            }
            sec->type = type;
            break;
        default:
            ret = WM_FAIL;
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
        "    wlan-add <profile_name> ssid <ssid> [wpa2 <secret>]"
        "\r\n");
    (void)PRINTF("      If using WPA2 security, set the PMF configuration if required.\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid> [wpa3 sae <secret> mfpc <1> mfpr <0/1>]"
        "\r\n");
    (void)PRINTF("      If using WPA3 SAE security, always set the PMF configuration.\r\n");

    (void)PRINTF("  For static IP address assignment:\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid>\r\n"
        "    ip:<ip_addr>,<gateway_ip>,<netmask>\r\n");
    (void)PRINTF(
        "    [bssid <bssid>] [channel <channel number>]\r\n"
        "    [wpa2 <secret>]"
        "\r\n");

    (void)PRINTF("For Micro-AP interface\r\n");
    (void)PRINTF(
        "    wlan-add <profile_name> ssid <ssid>\r\n"
        "    ip:<ip_addr>,<gateway_ip>,<netmask>\r\n");
    (void)PRINTF(
        "    role uap [bssid <bssid>]\r\n"
        "    [channel <channelnumber>]\r\n");
    (void)PRINTF("    [wpa2 <secret>] [wpa3 sae <secret>]\r\n");
    (void)PRINTF("    [mfpc <0/1>] [mfpr <0/1>]\r\n");
#ifdef CONFIG_11AC
    (void)PRINTF(
        "Note: Setting the channel value greater than or equal to 36 is mandatory,\r\n"
        "      if UAP bandwidth is set to 80MHz.\r\n");
#endif
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
        unsigned role : 1;
        unsigned mfpc : 1;
        unsigned mfpr : 1;
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
        else if ((info.security == 0U) && string_equal("wpa2", argv[arg]))
        {
            if (get_security(argc - arg - 1, argv + arg + 1, WLAN_SECURITY_WPA2, &network.security) != 0)
            {
                (void)PRINTF(
                    "Error: invalid WPA2 security"
                    " argument\r\n");
                return;
            }
            arg += 2;
            info.security++;
        }
        else if ((info.security2 == 0U) && string_equal("wpa3", argv[arg]))
        {
            if (string_equal(argv[arg + 1], "sae") != false)
            {
                network.security.type = WLAN_SECURITY_WPA3_SAE;
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
                arg += 3;
            }
            else
            {
                (void)PRINTF(
                    "Error: invalid WPA3 security"
                    " argument\r\n");
                return;
            }
            info.security2++;
        }
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

    if ((network.security.type == WLAN_SECURITY_WPA2) || (network.security.type == WLAN_SECURITY_WPA3_SAE))
    {
        if ((network.security.psk_len != 0U) && (network.security.password_len != 0U))
        {
            network.security.type = WLAN_SECURITY_WPA2_WPA3_SAE_MIXED;
        }
    }

    network.ip.ipv4.addr_type = (enum address_types)(info.address);

    ret = wlan_add_network(&network);
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
            if (res.wpa3_sae != 0U)
            {
                (void)PRINTF("WPA3 SAE ");
            }
            if (res.wpa2_entp != 0U)
            {
                (void)PRINTF("WPA2 Enterprise ");
            }
        }
        if (!((res.wep != 0U) || (res.wpa != 0U) || (res.wpa2 != 0U) || (res.wpa3_sae != 0U) || (res.wpa2_entp != 0U)))
        {
            (void)PRINTF("OPEN ");
        }
        (void)PRINTF("\r\n");

        (void)PRINTF("\tWMM: %s\r\n", (res.wmm != 0U) ? "YES" : "NO");
    }
#endif

    return 0;
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
            if (strlen(argv[arg + 1]) > IEEEtypes_SSID_SIZE)
            {
                (void)PRINTF("Error: SSID is too long\r\n");
                return;
            }
            (void)memcpy(wlan_scan_param.ssid, argv[arg + 1], strlen(argv[arg + 1]));
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
            (void)PRINTF("ssid \"%s\" ", wlan_scan_param.ssid);
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
    (void)PRINTF(
        "Connecting to network...\r\nUse 'wlan-stat' for "
        "current connection status.\r\n");
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
        (void)PRINTF("If <WNM> <sleep_interval> is specified, fw will enable WNM and use sleep_interval\r\n");
        (void)PRINTF("Example:\r\n");
        (void)PRINTF("	  wlan-ieee-ps 1 WNM 5\r\n");
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
            (void)PRINTF("Turned off IEEE Power Save mode");
        }
        else
        {
            (void)PRINTF("Failed to turn off IEEE Power Save mode, error: %d", ret);
        }
    }
    else if (choice == 1)
    {
        condition = (uint32_t)WAKE_ON_ARP_BROADCAST | (uint32_t)WAKE_ON_UNICAST | (uint32_t)WAKE_ON_MULTICAST |
                    (uint32_t)WAKE_ON_MAC_EVENT;
        ret = wlan_ieeeps_on(condition);
        if (ret == WM_SUCCESS)
        {
            (void)PRINTF("Turned on IEEE Power Save mode");
        }
        else
        {
            (void)PRINTF("Failed to turn on IEEE Power Save mode, error: %d", ret);
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










static void test_wlan_host_sleep(int argc, char **argv)
{
    int choice = -1, wowlan = 0;
    int ret = -WM_FAIL;

    if (argc < 2)
    {
        (void)PRINTF("Usage: %s <0/1> wowlan_test <0/1>\r\n", argv[0]);
        return;
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
            (void)PRINTF("Usage: %s <0/1> wowlan_test <0/1>\r\n", argv[0]);
            return;
        }

        errno  = 0;
        wowlan = (int)strtol(argv[3], NULL, 10);
        if (errno != 0)
        {
            (void)PRINTF("Error during strtoul:wowlan errno:%d\r\n", errno);
        }

        if (string_equal(argv[2], "wowlan_test"))
        {
            if (wowlan == 1)
            {
                ret = wlan_send_host_sleep(HOST_SLEEP_NO_COND);
                if (ret == WM_SUCCESS)
                {
                    (void)PRINTF("Host sleep configuration successs for wowlan test");
                }
                else
                {
                    (void)PRINTF("Failed to host sleep configuration, error: %d", ret);
                }
            }
            else if (wowlan == 0)
            {
                ret = wlan_send_host_sleep((uint32_t)WAKE_ON_ARP_BROADCAST | (uint32_t)WAKE_ON_UNICAST |
                                           (uint32_t)WAKE_ON_MULTICAST | (uint32_t)WAKE_ON_MAC_EVENT);
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
                /*Do Nothing*/
            }
        }
        else
        {
            (void)PRINTF("Usage: %s <0/1> wowlan_test <0/1>\r\n", argv[0]);
            return;
        }
    }
    else
    {
        (void)PRINTF("Usage: %s <0/1> wowlan_test <0/1>\r\n", argv[0]);
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



#ifdef CONFIG_HEAP_STAT
static void test_heap_stat(int argc, char **argv)
{
    os_dump_mem_stats();
}
#endif


static void dump_wlan_eu_crypto(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF("Algorithm AES-WRAP encryption and decryption verification\r\n");
    (void)PRINTF("wlan-eu-crypto <EncDec>\r\n");
    (void)PRINTF("EncDec: 0-Decrypt, 1-Encrypt\r\n");
}
static void test_wlan_eu_crypto(int argc, char **argv)
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
        dump_wlan_eu_crypto();
        (void)PRINTF("Error: invalid number of arguments\r\n");
        return;
    }
    (void)get_uint(argv[1], &EncDec, 1);
    if (EncDec != 0U && EncDec != 1U)
    {
        dump_wlan_eu_crypto();
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


static struct cli_command tests[] = {
    {"wlan-scan", NULL, test_wlan_scan},
    {"wlan-scan-opt", "ssid <ssid> bssid ...", test_wlan_scan_opt},
    {"wlan-add", "<profile_name> ssid <ssid> bssid...", test_wlan_add},
    {"wlan-remove", "<profile_name>", test_wlan_remove},
    {"wlan-list", NULL, test_wlan_list},
    {"wlan-connect", "<profile_name>", test_wlan_connect},
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
    {"wlan-host-sleep", "<0/1> wowlan_test <0/1>", test_wlan_host_sleep},
    {"wlan-send-hostcmd", NULL, test_wlan_send_hostcmd},
#if !defined(SD8801)
#ifdef CONFIG_11AC
    {"wlan-set-uap-bandwidth", "<1/2/3> 1:20 MHz 2:40MHz 3:80MHz", test_wlan_set_uap_bandwidth},
#else
    {"wlan-set-uap-bandwidth", "<1/2> 1:20 MHz 2:40MHz", test_wlan_set_uap_bandwidth},
#endif
#endif
#ifdef SD8801
    {"wlan-8801-enable-ext-coex", NULL, test_wlan_8801_enable_ext_coex},
    {"wlan-8801-get-ext-coex-stats", NULL, test_wlan_8801_ext_coex_stats},
#endif
    {"wlan-eu-crypto", "<EncDec>", test_wlan_eu_crypto},
#ifdef CONFIG_HEAP_STAT
    {"heap-stat", NULL, test_heap_stat},
#endif
#ifdef CONFIG_HEAP_DEBUG
    {"wlan-os-mem-stat", NULL, test_wlan_os_mem_stat},
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
