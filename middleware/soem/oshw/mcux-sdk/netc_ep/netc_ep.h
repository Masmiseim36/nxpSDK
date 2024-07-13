/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/* 
 * Copyright 2023-2024 NXP
 */

#ifndef ____NETC_EP_H___
#define ____NETC_EP_H___
#include "soem_netc_ep.h"
//----------------------------------------------------------------------------//
int netc_ep_init(void *pri, uint8_t *macAddr);
int netc_ep_send(void *pri, const void* payload, uint32_t len);
int netc_ep_recv(void *pri, void* buf, uint32_t len);
int netc_ep_link_status(void *pri);
int netc_ep_close(void *pri);
#endif
