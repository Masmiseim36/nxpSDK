/*
 * Copyright 2019,2022-2023,2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ETHERNETIF_PRIV_H
#define ETHERNETIF_PRIV_H

#include "fsl_common.h"

#include "lwip/err.h"
#include "lwip/opt.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "ethernetif_mmac.h"

#if ETH_USE_GPIO_ADAPTER
#include "fsl_adapter_gpio.h"
#endif /* ETH_USE_GPIO_ADAPTER */

/* SDK_SIZEALIGN could not be used in some constant expressions */
#define CONSTANT_SIZEALIGN(var, alignbytes) (((var) + ((alignbytes)-1U)) & (~((alignbytes)-1U)))

struct ethernetif;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

err_t ethernetif_init(struct netif *netif_,
                      struct ethernetif *ethernetif,
                      void *enetBase,
                      const ethernetif_config_t *ethernetifConfig);

void ethernetif_plat_init(struct netif *netif,
                          struct ethernetif *ethernetif,
                          const ethernetif_config_t *ethernetifConfig);

void ethernetif_phy_init(struct ethernetif *ethernetif, const ethernetif_config_t *ethernetifConfig);

/**
 * Function to map ENET instance index to its base address
 *
 * IMPORTANT: This function doesn't directly map to array indexes in ENET_BASE_PTRS!
 * It counts only non-NULL entries when determining which hardware instance to use.
 *
 * Examples:
 * - When ENET_BASE_PTRS = [ENET, (ENET_Type *)0u, ENET2]:
 *   - enetIdx 0 => ENET
 *   - enetIdx 1 => ENET2
 * - When ENET_BASE_PTRS = [ENET, ENET_1G]:
 *   - enetIdx 0 => ENET
 *   - enetIdx 1 => ENET_1G
 *
 * @param enetIdx The index of the ENET instance (counts only non-NULL entries)
 * @return Base address of the ENET instance, or NULL if not found
 */
void *ethernetif_get_enet_base(const uint8_t enetIdx);

/**
 * Returns phy handle from netif.
 *
 * @param netif_  the lwip network interface
 * @return a phy driver handle
 */
phy_handle_t *ethernetif_get_phy(struct netif *netif_);

#if ETH_USE_GPIO_ADAPTER
/**
 * Returns a GPIO handle associated with a pin for PHY state interrupts.
 * @param netif_ The lwip network interface.
 * @return The handle.
 */
hal_gpio_handle_t ethernetif_get_int_gpio_hdl(struct netif *netif);
#endif /* ETH_USE_GPIO_ADAPTER */

/**
 * Informs ethernet MAC driver that link is up or the link speed or duplex has changed.
 * Can be called repeatedly with the same values.
 *
 * @param netif_  the lwip network interface
 * @param speed  link speed
 * @param duplex  duplex
 */
void ethernetif_on_link_up(struct netif *netif_, phy_speed_t speed, phy_duplex_t duplex);

/**
 * Informs ethernet MAC driver that link is down.
 * Can be called repeatedly.
 *
 * @param netif_  the lwip network interface
 */
void ethernetif_on_link_down(struct netif *netif_);

#if (defined(FSL_FEATURE_SOC_ENET_QOS_COUNT) && (FSL_FEATURE_SOC_ENET_QOS_COUNT > 0)) || \
    (defined(FSL_FEATURE_SOC_EMAC_COUNT) && (FSL_FEATURE_SOC_EMAC_COUNT > 0))
/**
 * Function to map ENET_QOS instance index to its base address
 *
 * IMPORTANT: This function doesn't directly map to array indexes in ENET_QOS_BASE_PTRS!
 * It counts only non-NULL entries when determining which hardware instance to use.
 *
 * Examples:
 * - When ENET_QOS_BASE_PTRS = [ENET_QOS, (ENET_Type *)0u, ENET_QOS2]:
 *   - enetIdx 0 => ENET_QOS
 *   - enetIdx 1 => ENET_QOS2
 * - When ENET_BASE_PTRS = [ENET_QOS, ENET_QOS2]:
 *   - enetIdx 0 => ENET_QOS
 *   - enetIdx 1 => ENET_QOS2
 *
 * @param enetIdx The index of the ENET_QOS instance (counts only non-NULL entries)
 * @return Base address of the ENET_QOS instance, or NULL if not found
 */
void *ethernetif_get_enet_qos_base(const uint8_t enetIdx);
#endif

void **ethernetif_base_ptr(struct ethernetif *ethernetif);

#if LWIP_IPV4 && LWIP_IGMP
err_t ethernetif_igmp_mac_filter(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action action);
#endif

#if LWIP_IPV6 && LWIP_IPV6_MLD
err_t ethernetif_mld_mac_filter(struct netif *netif, const ip6_addr_t *group, enum netif_mac_filter_action action);
#endif

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
struct pbuf *ethernetif_linkinput(struct netif *netif);

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become available since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
err_t ethernetif_linkoutput(struct netif *netif, struct pbuf *p);

/**
 * Wrapper for pbuf_free/pbuf_free_callback
 * which handles situations like being called from ISR.
 *
 * @param p The pbuf (chain) to be dereferenced.
 */
void ethernetif_pbuf_free_safe(struct pbuf *p);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* ENET_ETHERNETIF_PRIV_H */
