/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

#include <string.h>

#include "soem_port.h"

struct soem_if_port soem_ports[SOEM_IF_PORT_MAXNUM] = {0};

int register_soem_port(struct soem_if_port *port)
{
	if(strlen(port->ifname) > SOEM_IF_NAME_MAXLEN) {
		EC_PRINT("the length of ifname is too long. (> %d)\n", SOEM_IF_NAME_MAXLEN - 1);
		return -1;
	}

	if(strlen(port->dev_name) > SOEM_DEV_NAME_MAXLEN) {
		EC_PRINT("the length of dev_name is too long. (> %d)\n", SOEM_DEV_NAME_MAXLEN - 1);
		return -1;
	}
	int i;
	for (i = 0; i < SOEM_IF_PORT_MAXNUM; i++) {
		if (soem_ports[i].ifname[0] == '\0') {
			strncpy(soem_ports[i].ifname, port->ifname, SOEM_IF_NAME_MAXLEN);
			strncpy(soem_ports[i].dev_name, port->dev_name, SOEM_DEV_NAME_MAXLEN);
			soem_ports[i].port_pri = port->port_pri;
			soem_ports[i].port_init = port->port_init;
			soem_ports[i].port_send = port->port_send;
			soem_ports[i].port_recv = port->port_recv;
			soem_ports[i].port_link_status = port->port_link_status;
			soem_ports[i].port_close = port->port_close;
			return i;
		}
	}
	return -1;
}
