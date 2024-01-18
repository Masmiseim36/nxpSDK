/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
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
    int (*port_init)(void *, uint8_t *);
    int (*port_send)(void *, const void *, uint32_t);
    int (*port_recv)(void *, void *, uint32_t);
    int (*port_link_status)(void *);
    int (*port_close)(void *);
    void *port_pri;
};

extern struct soem_if_port soem_ports[SOEM_IF_PORT_MAXNUM];

// int register_soem_port(char *ifname, char *dev_name, void *pri);
int register_soem_port(struct soem_if_port *port);

#endif
