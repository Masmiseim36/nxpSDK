
/**
 *  \file niface.h
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

#ifndef _H_NIFACE_
#define _H_NIFACE_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

#include "lwip/opt.h"
#include "lwip/netif.h"
#include "lwip/netifapi.h"
#include "lwip/tcpip.h"
#include "lwip/ethip6.h"
#include "netif/lowpan6_ble.h"
#include "lwip/netdb.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void niface_stack_setup(void);
void niface_setup (void);
void niface_dhcp_setup (struct netif * iface);

void niface_stack_down(void);
void niface_setup_down (void);
void niface_dhcp_down (struct netif * iface);

#endif /* _H_NIFACE_*/
