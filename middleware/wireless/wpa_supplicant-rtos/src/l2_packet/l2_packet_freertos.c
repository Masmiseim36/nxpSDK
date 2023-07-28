
#include "includes.h"
#include "common.h"
#include "eloop.h"
#include "l2_packet.h"
#include "common/eapol_common.h"

struct l2_packet_data
{
    char ifname[NETIF_NAMESIZE];
    u8 own_addr[ETH_ALEN];
    int ifindex;
    struct netif *iface;
    void (*rx_callback)(void *ctx, const u8 *src_addr, const u8 *buf, size_t len);
    void *rx_callback_ctx;
    int l2_hdr; /* whether to include layer 2 (Ethernet) header data
                 * buffers */
    unsigned short protocol;
};

static struct l2_packet_data gm_l2;
static struct l2_packet_data gu_l2;

static struct l2_packet_data *p_l2;

int l2_packet_get_own_addr(struct l2_packet_data *l2, u8 *addr)
{
    os_memcpy(addr, l2->own_addr, ETH_ALEN);
    return 0;
}

int l2_packet_send(struct l2_packet_data *l2, const u8 *dst_addr, u16 proto, const u8 *buf, size_t len)
{
    unsigned int interface;

    if (strstr(l2->ifname, "ml"))
        interface = WLAN_BSS_TYPE_STA;
    else
        interface = WLAN_BSS_TYPE_UAP;

    if (l2 == NULL)
    {
        return -1;
    }

    if (!l2->l2_hdr)
    {
        int ret;
        struct l2_ethhdr *eth = os_malloc(sizeof(*eth) + len);

        if (eth == NULL)
            return -1;
        os_memcpy(eth->h_dest, dst_addr, ETH_ALEN);
        os_memcpy(eth->h_source, l2->own_addr, ETH_ALEN);
        eth->h_proto = htons(proto);
        os_memcpy(eth + 1, buf, len);
        ret = wifi_supp_inject_frame(interface, (u8 *)eth, len + sizeof(*eth));
        os_free(eth); // TODO Set bss type as per the interface
        return ret;
    }
    else
        return wifi_supp_inject_frame(interface, buf, len); // TODO Set bss type as per the interface
}

int lwip_hook_unknown_eth_proto(struct pbuf *p, struct netif *iface)
{
    return -1;
}

struct l2_packet_data *l2_packet_init(const char *ifname,
                                      const u8 *own_addr,
                                      unsigned short protocol,
                                      void (*rx_callback)(void *ctx, const u8 *src_addr, const u8 *buf, size_t len),
                                      void *rx_callback_ctx,
                                      int l2_hdr)
{
    struct netif *iface = NULL;

    if (strstr(ifname, "ml"))
        p_l2 = (struct l2_packet_data *)&gm_l2;
    else
        p_l2 = (struct l2_packet_data *)&gu_l2;

    memset(p_l2, 0x00, sizeof(struct l2_packet_data));

    LOCK_TCPIP_CORE();
    iface = netif_find(ifname);
    UNLOCK_TCPIP_CORE();

    if (!iface)
    {
        wpa_printf(MSG_ERROR, "Cannot get device for: %s\n", ifname);
        return NULL;
    }

    os_strlcpy(p_l2->ifname, ifname, sizeof(p_l2->ifname));
    p_l2->rx_callback     = rx_callback;
    p_l2->rx_callback_ctx = rx_callback_ctx;
    p_l2->l2_hdr          = l2_hdr;
    p_l2->protocol        = protocol;
    p_l2->iface           = iface;

    p_l2->ifindex = netif_get_index(iface);

    if (!p_l2->ifindex)
    {
        wpa_printf(MSG_ERROR, "Cannot get  interface index for: %s\n", p_l2->ifname);
        return NULL;
    }

    os_memcpy(p_l2->own_addr, iface->hwaddr, iface->hwaddr_len);

    wpa_printf(MSG_DEBUG, "l2_packet_init: iface %s ifindex %d", p_l2->ifname, p_l2->ifindex);

    return p_l2;
}

struct l2_packet_data *l2_packet_init_bridge(
    const char *br_ifname,
    const char *ifname,
    const u8 *own_addr,
    unsigned short protocol,
    void (*rx_callback)(void *ctx, const u8 *src_addr, const u8 *buf, size_t len),
    void *rx_callback_ctx,
    int l2_hdr)
{
    return l2_packet_init(br_ifname, own_addr, protocol, rx_callback, rx_callback_ctx, l2_hdr);
}

void l2_packet_deinit(struct l2_packet_data *l2)
{
    if (l2 == NULL)
    {
        return;
    }
}

int l2_packet_get_ip_addr(struct l2_packet_data *l2, char *buf, size_t len)
{
#ifdef LWIP_IPV4
    memset(buf, 0x00, len);
    os_strlcpy(buf, ipaddr_ntoa(&l2->iface->ip_addr), len);
    return 0;
#else
    return -1;
#endif
}

void l2_packet_notify_auth_start(struct l2_packet_data *l2)
{
    /* This function can be left empty */
}

int l2_packet_set_packet_filter(struct l2_packet_data *l2, enum l2_packet_filter_type type)
{
    /* TODO: Set filter */
    return 0;
}
