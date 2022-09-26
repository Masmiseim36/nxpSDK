
/**
 *  \file niface.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "niface.h"

#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"

#include "netif/ethernet.h"

#include "enet_ethernetif.h"

#include "board.h"
#include "fsl_phy.h"
#include "fsl_enet_mdio.h"

/* --------------------------------------------- Global Definitions */
/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* NIface States */
#define NIFACE_STATE_INVALID            0x00U
#define NIFACE_STATE_SETUP              0x01U

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
static struct netif enetif;

static uint8_t niface_state = NIFACE_STATE_INVALID;
static uint8_t niface_stack_state = NIFACE_STATE_INVALID;

/* --------------------------------------------- Functions */
void get_enet_handles(phy_handle_t *phyHandles, netif_init_fn *enetif);

static void niface_print_dhcp_state(struct netif *netif)
{
    static u8_t dhcp_last_state = DHCP_STATE_OFF;
    struct dhcp *dhcp           = netif_dhcp_data(netif);

    if (dhcp == NULL)
    {
        dhcp_last_state = DHCP_STATE_OFF;
    }
    else if (dhcp_last_state != dhcp->state)
    {
        dhcp_last_state = dhcp->state;

        printf(" DHCP state       : ");
        switch (dhcp_last_state)
        {
            case DHCP_STATE_OFF:
                printf("OFF");
                break;
            case DHCP_STATE_REQUESTING:
                printf("REQUESTING");
                break;
            case DHCP_STATE_INIT:
                printf("INIT");
                break;
            case DHCP_STATE_REBOOTING:
                printf("REBOOTING");
                break;
            case DHCP_STATE_REBINDING:
                printf("REBINDING");
                break;
            case DHCP_STATE_RENEWING:
                printf("RENEWING");
                break;
            case DHCP_STATE_SELECTING:
                printf("SELECTING");
                break;
            case DHCP_STATE_INFORMING:
                printf("INFORMING");
                break;
            case DHCP_STATE_CHECKING:
                printf("CHECKING");
                break;
            case DHCP_STATE_BOUND:
                printf("BOUND");
                break;
            case DHCP_STATE_BACKING_OFF:
                printf("BACKING_OFF");
                break;
            default:
                printf("%u", dhcp_last_state);
                assert(0U);
                break;
        }
        printf("\r\n");

        if (dhcp_last_state == DHCP_STATE_BOUND)
        {
            printf("\r\n IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
            printf(" IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
            printf(" IPv4 Gateway     : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }
}

void niface_stack_setup(void)
{
    /* Validate state */
    if (NIFACE_STATE_INVALID != niface_stack_state)
    {
        return;
    }

    tcpip_init(NULL, NULL);
    niface_stack_state = NIFACE_STATE_SETUP;
}

void niface_setup(void)
{

#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
    static mem_range_t non_dma_memory[] = NON_DMA_MEMORY_ARRAY;
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    phy_handle_t phyHandles;
    netif_init_fn enetintf;

    get_enet_handles(&phyHandles, &enetintf);

    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandles,
        .macAddress = configMAC_ADDR,
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
        .non_dma_memory = non_dma_memory,
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    };

    /* Validate state */
    if (NIFACE_STATE_INVALID != niface_state)
    {
        return;
    }

    IP4_ADDR(&netif_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_gw, 0U, 0U, 0U, 0U);

    (BT_IGNORE_RETURN_VALUE) netifapi_netif_add(&enetif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, enetintf,
                       tcpip_input);
    (BT_IGNORE_RETURN_VALUE) netifapi_netif_set_default(&enetif);
    (BT_IGNORE_RETURN_VALUE) netifapi_netif_set_up(&enetif);

    /* Setup DHCP */
    niface_dhcp_setup(&enetif);

    /* Update state */
    niface_state = NIFACE_STATE_SETUP;

    printf("\nlwip ENetif init complete!\n");
    (BT_IGNORE_RETURN_VALUE) DbgConsole_Flush();
}

void niface_dhcp_setup(struct netif * iface)
{
    struct dhcp *dhcp;

    dhcp = netif_dhcp_data(iface);

    if (DHCP_STATE_BOUND != dhcp->state)
    {
        /* DHCP Start */
        (BT_IGNORE_RETURN_VALUE) dhcp_start(iface);

        printf("\r\n************************************************\r\n");
        printf(" DHCP start\r\n");
        printf("************************************************\r\n");

        do {
            sys_msleep(100U);
            niface_print_dhcp_state(iface);
            dhcp = netif_dhcp_data(iface);
        } while (DHCP_STATE_BOUND != dhcp->state);

        printf("\nDHCP complete!\n");
    }
    else
    {
        printf("\nDHCP complete!\n");
    }

    (BT_IGNORE_RETURN_VALUE) DbgConsole_Flush();
}

void niface_dhcp_down(struct netif * iface)
{
    struct dhcp *dhcp;
    /*get dhcp interface data*/
    dhcp = netif_dhcp_data(iface);

    /*validate dhcp state is initialised*/
    if (DHCP_STATE_BOUND != dhcp->state)
    {
        return;
    }
    else
    {
        /*release dhcp resources and stop*/
        dhcp_stop(iface);
        niface_print_dhcp_state(iface);
        printf("dhcp stop\n");
    }
}

void niface_stack_down(void)
{
    /* de-initialize stack */
    /* to do need tcp_deinit API from LWIP*/
    niface_stack_state = NIFACE_STATE_INVALID;

}

void niface_setup_down(void)
{
    /* Validate state */
    if (NIFACE_STATE_SETUP != niface_state)
    {
        return;
    }

    /*enable and verify these API's when tcpip_deinit is available*/
    /* Set DHCP down*/
    /*niface_dhcp_down(&enetif);

    netifapi_netif_set_down(&enetif);

    netifapi_netif_remove(&enetif);*/

    /* Update state */
    niface_state = NIFACE_STATE_INVALID;

    printf("\nlwip ENetif de-init complete!\n");
 }
