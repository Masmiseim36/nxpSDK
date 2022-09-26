
/**
 *  \file pan_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "pan_pl.h"

/* --------------------------------------------- Global Definitions */

#ifdef PAN
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
#ifdef CONFIG_AUTOIP
static struct autoip aip;
#endif /* CONFIG_AUTOIP */
static struct netif pan_netif;
static UCHAR pan_nap;

err_t pan_read_pl (struct netif *netif, struct pbuf *p);
static void (* pan_read)(UCHAR *data, UINT16 datalen);

static err_t pan_netif_init(struct netif *netif);

/* --------------------------------------------- Functions */
void pan_init_pl
     (
         BT_DEVICE_ADDR * mac,
         void (* read_cb)(UCHAR *data, UINT16 datalen),
         UCHAR is_nap
     )
{
    struct netif *iface;
    ip4_addr_t btnetif_ipaddr, btnetif_netmask, btnetif_gw;

    /* Register the node type */
    pan_nap = is_nap;

    if ((NULL != pan_read) ||
        (NULL == read_cb))
    {
        return;
    }

    /* Register read */
    pan_read = read_cb;

    niface_stack_setup();

    if (BT_TRUE == is_nap)
    {
        niface_setup();

        // when playing DHCP Server, set address
        IP4_ADDR(&btnetif_ipaddr, 192U, 168U, 7U, 1U);
        IP4_ADDR(&btnetif_netmask, 255U, 255U, 255U, 0U);
    }
    else
    {
        // when using DHCP Client, no address
        IP4_ADDR(&btnetif_ipaddr, 0U, 0U, 0U, 0U);
        IP4_ADDR(&btnetif_netmask, 0U, 0U, 0U, 0U);
    }

    IP4_ADDR(&btnetif_gw, 0U, 0U, 0U, 0U);

    iface = netif_add(&pan_netif, &btnetif_ipaddr, &btnetif_netmask, &btnetif_gw, NULL, pan_netif_init, tcpip_input);

    if (NULL != iface)
    {
        printf ("BTPAN interface added successfully\n");
        pan_netif = *iface;

        /* Set MAC Address to NetIF */
        pan_netif.hwaddr_len = 6U;
        BT_mem_copy(pan_netif.hwaddr, mac->addr, sizeof(mac->addr));
    }
}

void th_cb (void * args, UINT16 size)
{
    UCHAR have_dhcp = *((UCHAR *)args);

    if (0U != have_dhcp)
    {
        niface_dhcp_setup(&pan_netif);
    }
#ifdef CONFIG_AUTOIP
    else
    {
        /* Set AutoIP LinkLocal Address to 169.254.1.3 */
        aip.state = 0U;
        IP4_ADDR(&aip.llipaddr, 169U, 254U, 1U, 3U);
        netif_set_client_data(&pan_netif, LWIP_NETIF_CLIENT_DATA_INDEX_AUTOIP, &aip);

        autoip_start(&pan_netif);
    }
#endif /* CONFIG_AUTOIP */
}

void pan_start_pl (UCHAR have_dhcp)
{
    BT_timer_handle th;

    /* Up the interface */
    netif_set_link_up(&pan_netif);
    netif_set_up(&pan_netif);

    if (BT_TRUE != pan_nap)
    {
        th = BT_TIMER_HANDLE_INIT_VAL;
        (BT_IGNORE_RETURN_VALUE) BT_start_timer(&th, 1U, th_cb, &have_dhcp, 1U);
    }
}

void pan_stop_pl(void)
{
    /* Down the interface */
    netif_set_link_down(&pan_netif);
    netif_set_down(&pan_netif);
}

void pan_write_pl (UCHAR * eth_header, UCHAR * eth_data, UINT16 eth_datalen)
{
    struct pbuf* p;
    UINT16 eth_headerlen = 14U;

    /* Allocate a raw buffer */
    p = pbuf_alloc(PBUF_RAW, (eth_headerlen + eth_datalen), PBUF_RAM);

    if (NULL != p)
    {
        /* Feed the packet from BLE transport to the LwIP stack */
        p->len = (eth_headerlen + eth_datalen);
        BT_mem_copy(p->payload, eth_header, eth_headerlen);
        BT_mem_copy((&((uint8_t *)p->payload)[eth_headerlen]), eth_data, eth_datalen);
        (BT_IGNORE_RETURN_VALUE) pan_netif.input (p, &pan_netif);

        /* Buffer is freed in the LwIP? Because if free here, it crashes */
    }
    else
    {
        printf ("Failed to allocate pbuf\n");
    }
}

err_t pan_read_pl (struct netif *netif, struct pbuf *p)
{
    /* Forward the packet from LwIP to PAN transport */
    pan_read (p->payload, p->len);

    return ERR_OK;
}

static err_t pan_netif_init(struct netif *netif)
{
    // interface short name
    netif->name[0U] = 'b';
    netif->name[1U] = 't';

    // mtu
    netif->mtu = 1600U;

    /* device capabilities */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    netif->output = etharp_output;
#if LWIP_IPV6
    netif->output_ip6 = ethip6_output;
#endif

    netif->linkoutput = pan_read_pl;

    return ERR_OK;
}
#endif /* PAN */

