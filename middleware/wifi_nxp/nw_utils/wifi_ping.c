/** @file wifi_ping.c
 *
 *  @brief  This file provides the support for network utility ping
 *
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

/* wifi_ping.c: This file contains the support for network utility ping */

#include <string.h>
#include <wm_os.h>
#include <wm_net.h>
#include <cli.h>
#include <cli_utils.h>
#include <wlan.h>
#include <wifi_ping.h>

static struct netif *get_netif_up(void)
{
    struct netif *netif = netif_list;
    for (; netif != NULL; netif = netif->next)
    {
        if (netif_is_up(netif) != 0U)
        {
            return netif;
        }
    }
    return NULL;
}

static int addr_af(const ip_addr_t *addr)
{
#ifdef CONFIG_IPV6
    return (addr->type == IPADDR_TYPE_V4) ? AF_INET : AF_INET6;
#else
    return AF_INET;
#endif
}

static const ip_addr_t *get_src_addr(const ip_addr_t *dst)
{
    static ip_addr_t ret;
    const ip_addr_t *addr = NULL;
    struct netif *netif   = get_netif_up();
#ifdef CONFIG_IPV6
    bool is_ip_type_valid = MTRUE;
#endif

    if (netif == NULL)
    {
        return NULL;
    }

#ifdef CONFIG_IPV6
    switch (dst->type)
    {
        case IPADDR_TYPE_V4:
            addr = netif_ip_addr4(netif);
            (void)memcpy(&ret.u_addr.ip4, &addr->u_addr.ip4, sizeof(ret.u_addr.ip4));
            break;
        case IPADDR_TYPE_V6:
            addr = ip6_select_source_address(netif, &addr->u_addr.ip6);
            (void)memcpy(&ret.u_addr.ip6, &addr->u_addr.ip6, sizeof(ret.u_addr.ip6));
            break;
        default:
            is_ip_type_valid = MFALSE;
            break;
    }

    if (is_ip_type_valid == MFALSE)
    {
        return NULL;
    }

    ret.type = dst->type;
#else
    addr = netif_ip_addr4(netif);
    memcpy(&ret, addr, sizeof(ip_addr_t));
#endif
    return &ret;
}

/* Converts IPv4 or IPv6 address into static buffer and returns its pointer */
static const char *ip_to_str(const ip_addr_t *ipaddr)
{
    static char ip_str[50];

    if (inet_ntop(addr_af(ipaddr), ipaddr, ip_str, sizeof(ip_str)) == NULL)
    {
        ip_str[0] = '?';
        ip_str[1] = '\0';
    }

    return ip_str;
}

/* Display the final result of ping */
static void display_ping_result(const ip_addr_t *addr, int total, int recvd)
{
    int dropped = total - recvd;
    (void)PRINTF("\r\n--- %s ping statistics ---\r\n", ip_to_str(addr));
    (void)PRINTF("%d packets transmitted, %d received,", total, recvd);
    if (dropped != 0)
    {
        (void)PRINTF(" +%d errors,", dropped);
    }
    (void)PRINTF(" %d%% packet loss\r\n", (dropped * 100) / total);
}

/* Display the statistics of the current iteration of ping */
static void display_ping_stats(int status, uint32_t size, const ip_addr_t *ipaddr, u16_t seqno, int ttl, uint32_t time)
{
    const char *ip_str = ip_to_str(ipaddr);

    if (status == WM_SUCCESS)
    {
        (void)PRINTF("%u bytes from %s: icmp_req=%u ttl=%u time=%u ms\r\n", size, ip_str, seqno, ttl, time);
    }
    else
    {
        (void)PRINTF("From %s icmp_seq=%u Destination Host Unreachable\r\n", ip_str, seqno);
    }
}

/* Display the usage of ping */
static void display_ping_usage(void)
{
    (void)PRINTF("Usage:\r\n");
    (void)PRINTF(
        "\tping [-s <packet_size>] [-c <packet_count>] "
#ifdef CONFIG_IPV6
        "[-W <timeout in sec>] <ipv4/ipv6 address>\r\n");
#else
        "[-W <timeout in sec>] <ipv4 address>\r\n");
#endif
    (void)PRINTF("Default values:\r\n");
    (void)PRINTF(
        "\tpacket_size: %u\r\n\tpacket_count: %u"
        "\r\n\ttimeout: %u sec\r\n",
        PING_DEFAULT_SIZE, PING_DEFAULT_COUNT, PING_DEFAULT_TIMEOUT_SEC);
}

/* Handle the ICMP echo response and extract required parameters */
static int ping_recv(int s, uint16_t seq_no, int *ttl)
{
    char buf[64];
    int fromlen = 0, len;
    struct sockaddr_in from;
    struct ip_hdr *iphdr;
    struct icmp_echo_hdr *iecho;

    while ((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)(void *)&from,
                                (socklen_t *)(void *)&fromlen)) > 0)
    {
        /* Received length should be greater than size of IP header and
         * size of ICMP header */
        if (len >= (int)(sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr)))
        {
            iphdr = (struct ip_hdr *)(void *)buf;
            /* Calculate the offset of ICMP header */
            iecho = (struct icmp_echo_hdr *)(void *)(buf + (IPH_HL(iphdr) * 4U));

            /* Verify that the echo response is for the echo request
             * we sent by checking PING_ID and sequence number */
            if ((iecho->id == PING_ID) && (iecho->seqno == htons(seq_no)))
            {
                /* Extract TTL and send back so that it can be
                 * displayed in ping statistics */
                *ttl = (int)(iphdr->_ttl);
                return WM_SUCCESS;
            }
        }
    }
    /* Either len < 0 or the echo response verification unsuccessful */
    return -WM_FAIL;
}

#ifdef CONFIG_IPV6
/* Handle the ICMP6 echo response and extract required parameters */
static int ping6_recv(int s, uint16_t seq_no, int *ttl)
{
    char buf[64];
    int fromlen = 0, len;
    struct sockaddr_in6 from;
    struct ip6_hdr *iphdr;
    struct icmp_echo_hdr *iecho;

    while ((len = lwip_recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)(void *)&from,
                                (socklen_t *)(void *)&fromlen)) > 0)
    {
        /* Received length should be greater than size of IP header and
         * size of ICMP header */
        if (len >= (int)(sizeof(struct ip6_hdr) + sizeof(struct icmp_echo_hdr)))
        {
            iphdr = (struct ip6_hdr *)(void *)buf;
            if (IP6H_NEXTH(iphdr) == IP6_NEXTH_ICMP6)
            {
                /* Calculate the offset of ICMP header */
                iecho = (struct icmp_echo_hdr *)(void *)(buf + sizeof(struct ip6_hdr));

                /* Verify that the echo response is for the echo request
                 * we sent by checking PING_ID, sequence number and ICMP type*/
                if ((iecho->id == PING_ID) && (iecho->seqno == htons(seq_no)) && (iecho->type == ICMP6_TYPE_EREP))
                {
                    /* Extract TTL and send back so that it can be
                     * displayed in ping statistics */
                    *ttl = IP6H_HOPLIM(iphdr);
                    return WM_SUCCESS;
                }
            }
        }
    }
    /* Either len < 0 or the echo response verification unsuccessful */
    return -WM_FAIL;
}
#endif

/* Prepare a ICMP echo request */
static void ping_prepare_echo(struct icmp_echo_hdr *iecho, const ip_addr_t *dest, uint16_t len, uint16_t seq_no)
{
    uint32_t i;
    uint32_t data_len = len - sizeof(struct icmp_echo_hdr);

#ifdef CONFIG_IPV6
    ICMPH_TYPE_SET(iecho, (dest->type == IPADDR_TYPE_V4) ? ICMP_ECHO : ICMP6_TYPE_EREQ);
#else
    ICMPH_TYPE_SET(iecho, ICMP_ECHO);
#endif
    ICMPH_CODE_SET(iecho, 0);
    iecho->chksum = 0;
    iecho->id     = PING_ID;
    iecho->seqno  = htons(seq_no);

    /* Fill the additional data buffer with some data */
    for (i = 0; i < data_len; i++)
    {
        ((char *)iecho)[sizeof(struct icmp_echo_hdr) + i] = (char)i;
    }
}

/* Send an ICMP echo request, receive its response and print its statistics and
 * result */
static int ping(u16_t count, unsigned short size, unsigned int r_timeout, ip_addr_t *addr)
{
    int ret = WM_SUCCESS, s, recvd = 0;
    u16_t i = 1;
    struct icmp_echo_hdr *iecho;
    unsigned int ping_time, ping_size;
    const ip_addr_t *ip_addr, *src_ip_addr;
    struct timeval timeout;

#ifdef CONFIG_IPV6
    struct netif *netif     = get_netif_up();
    const unsigned scope_id = netif_get_index(netif);
#endif
    const char *ip_str = ip_to_str(addr);

    (void)PRINTF("PING %s (%s) %u(%u) bytes of data\r\n", ip_str, ip_str, size,
                 size + sizeof(struct ip_hdr) + sizeof(struct icmp_echo_hdr));

    /* Create a raw socket */
#ifdef CONFIG_IPV6
    s = socket(addr_af(addr), SOCK_RAW, (addr->type == IPADDR_TYPE_V4) ? IPPROTO_ICMP : IPPROTO_ICMPV6);
#else
    s              = socket(addr_af(addr), SOCK_RAW, IPPROTO_ICMP);
#endif
    if (s < 0)
    {
        ping_e("Failed to create raw socket for ping %d", s);
        return -WM_FAIL;
    }
/* Convert timeout to milliseconds */
#if defined(__MCUXPRESSO)
    timeout.tv_sec = (time_t)(r_timeout);
#else
    timeout.tv_sec = (long)(r_timeout);
#endif

    timeout.tv_usec = 0;

    /* Set the socket timeout */
    ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (ret < 0)
    {
        ping_e("Failed to set socket options for ping");
        ret = -WM_FAIL;
        goto end;
    }

    /* Get the source IP address */
    src_ip_addr = get_src_addr(addr);

    /* Ping size is: size of ICMP header + size of payload */
    ping_size = sizeof(struct icmp_echo_hdr) + size;

    iecho = (struct icmp_echo_hdr *)os_mem_alloc(ping_size);
    if (iecho == NULL)
    {
        ping_e("Failed to allocate memory for ping packet");
        ret = -WM_FAIL;
        goto end;
    }

    while (i <= count)
    {
        ping_prepare_echo(iecho, addr, (uint16_t)ping_size, i);
#ifdef CONFIG_IPV6
        if (addr->type == IPADDR_TYPE_V4)
#endif
        {
            struct sockaddr_in to;
            to.sin_len    = (u8_t)sizeof(to);
            to.sin_family = AF_INET;
            inet_addr_from_ip4addr(&to.sin_addr, ip_2_ip4(addr));

            iecho->chksum = inet_chksum(iecho, ping_size);

            /* Send the ICMP echo request */
            ret = lwip_sendto(s, iecho, ping_size, 0, (struct sockaddr *)(void *)&to, sizeof(to));
        }
#ifdef CONFIG_IPV6
        else
        {
            struct sockaddr_in6 to;
            to.sin6_len      = sizeof(to);
            to.sin6_family   = AF_INET6;
            to.sin6_scope_id = scope_id;

            inet6_addr_from_ip6addr(&to.sin6_addr, ip_2_ip6(addr));

            /* Send the ICMP6 echo request */
            ret = lwip_sendto(s, iecho, ping_size, 0, (struct sockaddr *)(void *)&to, sizeof(to));
        }
#endif

        /* Get the current ticks as the start time */
        ping_time = os_ticks_get();

        if (ret > 0)
        {
            int ttl = 0;
            /* Receive the ICMP echo response */
#ifdef CONFIG_IPV6
            if (addr->type == IPADDR_TYPE_V4)
#endif
            {
                ret = ping_recv(s, (uint16_t)i, &ttl);
            }
#ifdef CONFIG_IPV6
            else
            {
                ret = ping6_recv(s, (uint16_t)i, &ttl);
            }
#endif

            /* Calculate the round trip time */
            ping_time = os_ticks_get() - ping_time;

            if (ret == WM_SUCCESS)
            {
                /* Increment the receive counter */
                recvd++;
                /* To display successful ping stats, destination
                 * IP address is required */
                ip_addr = addr;
            }
            else
            {
                /* To display unsuccessful ping stats, source
                 * IP address is required */
                ip_addr = src_ip_addr;
            }

            display_ping_stats(ret, ping_size, ip_addr, i, ttl, ping_time);
        }
        else
        {
            ping_e("Failed to send ping echo request");
            os_mem_free(iecho);
            ret = -WM_FAIL;
            goto end;
        }
        i++;
        os_thread_sleep(os_msec_to_ticks(PING_INTERVAL));
    }
    os_mem_free(iecho);
    display_ping_result(src_ip_addr, (int)count, recvd);
end:
    (void)close(s);
    return ret;
}

static void cmd_ping(int argc, char **argv)
{
    ip_addr_t addr;
    int c;
    uint16_t size  = PING_DEFAULT_SIZE;
    uint16_t count = PING_DEFAULT_COUNT;
    uint32_t cnt, temp;
    uint32_t timeout = PING_DEFAULT_TIMEOUT_SEC;
    bool goto_end    = MFALSE;

    (void)memset(&addr, 0, sizeof(addr));

    /* If number of arguments is odd then print error */
    if ((argc & 0x01) != 0)
    {
        goto end;
    }

    cli_optind = 1;
    while ((c = cli_getopt(argc, argv, "c:s:W:")) != -1)
    {
        errno = 0;
        switch (c)
        {
            case 'c':
                cnt = strtoul(cli_optarg, NULL, 10);
                if (cnt > PING_MAX_COUNT)
                {
                    if (errno != 0)
                    {
                        (void)PRINTF("Error during strtoul errno:%d", errno);
                    }
                    (void)PRINTF(
                        "ping: count size too large: %u."
                        " Maximum is %u\r\n",
                        cnt, PING_MAX_COUNT);
                    return;
                }
                count = cnt;
                break;
            case 's':
                temp = strtoul(cli_optarg, NULL, 10);
                if (temp > PING_MAX_SIZE)
                {
                    if (errno != 0)
                    {
                        (void)PRINTF("Error during strtoul errno:%d", errno);
                    }
                    (void)PRINTF(
                        "ping: packet size too large: %u."
                        " Maximum is %u\r\n",
                        temp, PING_MAX_SIZE);
                    return;
                }
                size = (uint16_t)temp;
                break;
            case 'W':
                timeout = strtoul(cli_optarg, NULL, 10);
                break;
            default:
                goto_end = MTRUE;
                break;
        }
        if (goto_end == MTRUE)
        {
            goto end;
        }
        if (errno != 0)
        {
            (void)PRINTF("Error during strtoul errno:%d", errno);
        }
    }
    if (cli_optind == argc)
    {
        goto end;
    }

    /* Extract the destination IP address. This function returns non zero on
     * success, zero on failure */
    if (inet_pton(AF_INET, argv[cli_optind], &addr) != 0)
    {
#ifdef CONFIG_IPV6
        addr.type = IPADDR_TYPE_V4;
#endif
        (void)ping(count, size, timeout, &addr);
        return;
    }
#ifdef CONFIG_IPV6
    else if (inet_pton(AF_INET6, argv[cli_optind], &addr) != 0)
    {
        addr.type = IPADDR_TYPE_V6;
        (void)ping(count, size, timeout, &addr);
        return;
    }
#endif
    else
    {
        /*Do Nothing*/
    }

end:
    (void)PRINTF("Incorrect usage\r\n");
    display_ping_usage();
}

static struct cli_command ping_cli[] = {
#ifdef CONFIG_IPV6
    {"ping", "[-s <packet_size>] [-c <packet_count>] [-W <timeout in sec>] <ipv4/ipv6 address>", cmd_ping},
#else
    {"ping", "[-s <packet_size>] [-c <packet_count>] [-W <timeout in sec>] <ipv4 address>", cmd_ping},
#endif
};

int ping_cli_init(void)
{
    unsigned int i;
    for (i = 0; i < sizeof(ping_cli) / sizeof(struct cli_command); i++)
    {
        if (cli_register_command(&ping_cli[i]) != 0)
        {
            return -WM_FAIL;
        }
    }
    return WM_SUCCESS;
}

int ping_cli_deinit(void)
{
    unsigned int i;

    for (i = 0; i < sizeof(ping_cli) / sizeof(struct cli_command); i++)
    {
        if (cli_unregister_command(&ping_cli[i]) != 0)
        {
            return -WM_FAIL;
        }
    }
    return WM_SUCCESS;
}
