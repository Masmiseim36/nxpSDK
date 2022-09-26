// SPDX-License-Identifier: GPL-2.0+
/* 
 * Copyright 2022 NXP
*/

#ifndef ____ENET_H___
#define ____ENET_H___
#include "soem_enet.h"
//----------------------------------------------------------------------------//
int enet_init(void *pri, uint8_t *macAddr);
int enet_send(void *pri, const void* payload, uint32_t len);
int enet_recv(void *pri, void* buf, uint32_t len);
int enet_link_status(void *pri);
int enet_close(void *pri);
#endif
