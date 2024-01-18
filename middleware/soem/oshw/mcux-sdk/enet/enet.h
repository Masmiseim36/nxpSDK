/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
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
