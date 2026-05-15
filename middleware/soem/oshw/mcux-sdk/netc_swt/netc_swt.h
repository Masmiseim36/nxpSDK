/*
 * Licensed under the GNU General Public License version 2 with excepttions. See
 * LICENSE file in the project root for full license information
 */

/* 
 * Copyright 2023-2024 NXP
 */

#ifndef ____NETC_SWT_H___
#define ____NETC_SWT_H___
#include "soem_netc_swt.h"
//----------------------------------------------------------------------------//
int netc_swt_init(void *pri, uint8_t *macAddr);
int netc_swt_send(void *pri, const void* payload, uint32_t len);
int netc_swt_recv(void *pri, void* buf, uint32_t len);
int netc_swt_link_status(void *pri);
int netc_swt_close(void *pri);
#endif
