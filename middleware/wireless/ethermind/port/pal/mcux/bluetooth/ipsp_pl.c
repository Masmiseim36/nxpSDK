
/**
 *  \file ipsp_pl.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "ipsp_pl.h"
#include "appl_utils.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static struct netif ipsp_netif;

err_t ipsp_read_pl (struct netif *netif, struct pbuf *p);
static void (* ipsp_read)(UCHAR *data, UINT16 datalen) = NULL;

/* --------------------------------------------- Functions */
#ifdef IPSP_HAVE_6LO_NIFACE
void ipsp_init_pl
     (
         BT_DEVICE_ADDR * local_mac,
         void (* read_cb)(UCHAR *data, UINT16 datalen),
         UCHAR is_router
     )
{
    struct netif *iface;
    ip6_addr_t addr6;
    char *ip6_addr = "2001:db8::1";

    if ((NULL != ipsp_read) ||
        (NULL == read_cb))
    {
        return;
    }

    /* Register read */
    ipsp_read = read_cb;

    niface_stack_setup();

    if (BT_TRUE == is_router)
    {
      /*Since there is no Ethernet interface on the RW610 board, Just disable it for IPSP Profile*/
#if !defined(RW610_SERIES) && !defined(RW612_SERIES)
        niface_setup();
#endif
    }

    /* Add the NetIF to the LwIP */
    iface = netif_add(&ipsp_netif, NULL, NULL, NULL, NULL, rfc7668_if_init, rfc7668_input);

    if (NULL != iface)
    {
        printf ("BtLE 6Lo interface added successfully\n");

        /* Create link local address on NetIF */
        BT_mem_copy(iface->hwaddr, local_mac->addr, sizeof(local_mac->addr));
        netif_create_ip6_linklocal_address(iface, true);
        iface->ip6_addr_state[0U] |= IP6_ADDR_VALID; //TODO: Check if required.

        /* Add a Global Address */
        (BT_IGNORE_RETURN_VALUE) ip6addr_aton(ip6_addr, &addr6);
        netif_ip6_addr_set(iface, 1U, &addr6);
        iface->ip6_addr_state[1U] |= IP6_ADDR_VALID; //TODO: Check if required.

        printf ("NetIF LinkLocal Address: %s\n", ip6addr_ntoa(netif_ip6_addr(iface, 0U)));
        printf ("NetIF Global Address: %s\n", ip6_addr);

        /* Save the interface */
        ipsp_netif = *iface;

        /* Set the linkoutput function to the netif */
        ipsp_netif.linkoutput = ipsp_read_pl;

        /* Update local MAC to LwIP 6Lo */
        (BT_IGNORE_RETURN_VALUE) rfc7668_set_local_addr_mac48(&ipsp_netif, local_mac->addr, 6U, !(local_mac->type));
    }

    return;
}
#endif
void ipsp_start_pl (BT_DEVICE_ADDR * remote_mac)
{
    /* Update Remote MAC to LwIP 6Lo */
    (BT_IGNORE_RETURN_VALUE) rfc7668_set_peer_addr_mac48(&ipsp_netif, remote_mac->addr, 6U, !(remote_mac->type));

    /* Up the interface */
    netif_set_link_up(&ipsp_netif);
    netif_set_up(&ipsp_netif);
}

void ipsp_stop_pl(void)
{
    /* Down the interface */
    netif_set_link_down(&ipsp_netif);
    netif_set_down(&ipsp_netif);
}

void ipsp_write_pl (UCHAR * eth_data, UINT16 eth_datalen)
{
    struct pbuf* p;

    /* Allocate a raw buffer */
    p = pbuf_alloc(PBUF_RAW, eth_datalen, PBUF_RAM);

    if (NULL != p)
    {
        //printf(">> %02X %02X %02X %02X\n", eth_data[0], eth_data[1], eth_data[2], eth_data[3]);

        /* Feed the packet from BLE transport to the LwIP stack */
        BT_mem_copy(p->payload, eth_data, eth_datalen);
        p->len = eth_datalen;
        (BT_IGNORE_RETURN_VALUE) ipsp_netif.input (p, &ipsp_netif);

        /* Buffer is freed in the LwIP? Because if free here, it crashes */
    }
    else
    {
        printf ("Failed to allocate pbuf\n");
    }
}

err_t ipsp_read_pl (struct netif *netif, struct pbuf *p)
{
    //printf("<< %02X %02X %02X %02X\n", edata[0], edata[1], edata[2], edata[3]);

    /* Forward the packet from LwIP to BLE transport */
    ipsp_read (p->payload, p->len);

    return 0U;
}

