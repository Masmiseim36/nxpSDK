// SPDX-License-Identifier: GPL-2.0+
/* 
 * Copyright 2022 NXP
*/

#include <string.h>

#include "soem_port.h"

struct soem_if_port soem_ports[SOEM_IF_PORT_MAXNUM] = {0};

int register_soem_port(char *ifname, char *dev_name, void *pri)
{
	if(strlen(ifname) > SOEM_IF_NAME_MAXLEN) {
		EC_PRINT("the length of ifname is too long. (> %d)\n", SOEM_IF_NAME_MAXLEN - 1);
		return -1;
	}

	if(strlen(dev_name) > SOEM_DEV_NAME_MAXLEN) {
		EC_PRINT("the length of dev_name is too long. (> %d)\n", SOEM_DEV_NAME_MAXLEN - 1);
		return -1;
	}
	int i;
	for (i = 0; i < SOEM_IF_PORT_MAXNUM; i++) {
		if (soem_ports[i].ifname[0] == '\0') {
			strncpy(soem_ports[i].ifname, ifname, SOEM_IF_NAME_MAXLEN);
			strncpy(soem_ports[i].dev_name, dev_name, SOEM_DEV_NAME_MAXLEN);
			soem_ports[i].port_pri = pri;
			return i;
		}
	}
	return -1;
}
