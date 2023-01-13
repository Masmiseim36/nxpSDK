// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2022 NXP
 */

#ifndef __SOEM__PORT__
#define __SOEM__PORT__

#include "osal.h"
#include "fsl_phy.h"

#define SOEM_IF_PORT_MAXNUM  2
#define SOEM_IF_NAME_MAXLEN  9
#define SOEM_DEV_NAME_MAXLEN 9

struct soem_if_port
{
    char ifname[SOEM_IF_NAME_MAXLEN + 1];
    char dev_name[SOEM_DEV_NAME_MAXLEN + 1];
    void *port_pri;
};

extern struct soem_if_port soem_ports[SOEM_IF_PORT_MAXNUM];

int register_soem_port(char *ifname, char *dev_name, void *pri);

#endif
