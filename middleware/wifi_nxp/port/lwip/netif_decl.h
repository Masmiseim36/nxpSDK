/*
 *  Copyright 2008-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#include <mlan_api.h>
#include <wm_net.h>
#include <wmlog.h>
#ifndef RW610
#include <wifi-sdio.h>
#endif
#include <wifi-internal.h>

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/sys.h"
#ifdef CONFIG_IPV6
#include "lwip/ethip6.h"
#endif /* CONFIG_IPV6 */
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ethernet.h"
#include "netif/ppp/pppoe.h"

/*------------------------------------------------------*/
/*
 * Packets of this type need o be handled
 * for WPS and Supplicant
 */
#define ETHTYPE_EAPOL 0x888EU /* EAPOL */

PACK_STRUCT_BEGIN
/* This is an Token-Ring LLC structure */
struct eth_llc_hdr
{
    PACK_STRUCT_FIELD(u8_t dsap);      /* destination SAP */
    PACK_STRUCT_FIELD(u8_t ssap);      /* source SAP */
    PACK_STRUCT_FIELD(u8_t llc);       /* LLC control field */
    PACK_STRUCT_FIELD(u8_t protid[3]); /* protocol id */
    PACK_STRUCT_FIELD(u16_t type);     /* ether type field */
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

#define SIZEOF_ETH_LLC_HDR (8U)

/* Define those to better describe your network interface. */
#define IFNAME0 'm'
#define IFNAME1 'l'

/*
 * It was observed that Wi-Fi card does not wakeup
 * immediately after call to wlan_wakeup_card.
 * The code tries to wakeup the card by writing
 * in SDIO register.
 * It waits for 20 ms for AWAKE event from Wi-Fi card,
 * if AWAKE event is not generated or received  by MCU code
 * it returns error and does not send a packet out.
 * This is observed with  8801 Wi-Fi card.
 * So for 8801 based platforms the wait time is now 35 ms.
 */

#define MAX_WAIT_TIME 35
#define MAX_INTERFACES_SUPPORTED 3U

/* The time to block waiting for input. */
#define emacBLOCK_TIME_WAITING_FOR_INPUT ((portTickType)100)
/*------------------------------------------------------*/
extern int wlan_get_mac_address(unsigned char *dest);
extern void wlan_wake_up_card(void);




int wrapper_wlan_handle_rx_packet(t_u16 datalen, RxPD *rxpd, void *p, void *payload);

int wrapper_wlan_handle_amsdu_rx_packet(const t_u8 *rcvdata, const t_u16 datalen);

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif
{
    struct eth_addr *ethaddr;
    /* Interface to bss type identification that tells the FW wherether
       the data is for STA for UAP */
    t_u8 interface;
    /* Add whatever per-interface state that is needed here. */
};
