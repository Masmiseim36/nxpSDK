/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fnet_mdns_port.h"
#include "lwip/sockets.h"
#include "lwip/netif.h"


fnet_socket_t fnet_socket( fnet_address_family_t family, fnet_socket_type_t type, fnet_uint32_t protocol )
{
    return lwip_socket(family, type, protocol);
}


fnet_return_t fnet_socket_bind( fnet_socket_t s, const struct fnet_sockaddr *name, fnet_size_t namelen )
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = name->sa_family;
    addr.sin_port = name->sa_port;
    
    if(lwip_bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        return FNET_ERR;
    }
    
    return FNET_OK;
}


fnet_ssize_t fnet_socket_recvfrom( fnet_socket_t s, void *buf, fnet_size_t len, fnet_flag_t flags, struct fnet_sockaddr *from, fnet_size_t *fromlen )
{
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int ret;
    
    fd_set read_set;

    FD_ZERO(&read_set);
    FD_SET(s, &read_set);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;

    ret = lwip_select(s + 1, &read_set, NULL, NULL, &timeout);
    if (ret > 0)
    {
        ret = lwip_recvfrom(s, buf, len, flags, (struct sockaddr*)&addr, &addr_len);
    }
    else
    {
        /* nothing to receive */
        return 0;
    }
    
    if (from != NULL)
    {
        struct fnet_sockaddr_in* fnet_from = (struct fnet_sockaddr_in*) from;
        fnet_from->sin_family = addr.sin_family;
        fnet_from->sin_port = addr.sin_port;
        fnet_from->sin_addr.s_addr = addr.sin_addr.s_addr;
    }

    return ret;
}


fnet_ssize_t fnet_socket_sendto( fnet_socket_t s, const void *buf, fnet_size_t len, fnet_flag_t flags, const struct fnet_sockaddr *to, fnet_size_t tolen )
{
    struct fnet_sockaddr_in *fnet_to = (struct fnet_sockaddr_in *)to;
  
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = fnet_to->sin_family;
    addr.sin_port = fnet_to->sin_port;
    addr.sin_addr.s_addr = fnet_to->sin_addr.s_addr;
    
    return lwip_sendto(s, buf, len, flags, (struct sockaddr *)&addr, sizeof(addr));
}


fnet_return_t fnet_socket_close( fnet_socket_t s )
{
    if(lwip_close(s) < 0)
    {
        return FNET_ERR;
    }
    
    return FNET_OK;
}


fnet_return_t fnet_socket_setopt( fnet_socket_t s, fnet_protocol_t level, fnet_socket_options_t optname, const void *optval, fnet_size_t optvallen )
{
    if(lwip_setsockopt(s, level, optname, optval, optvallen) < 0)
    {
        return FNET_ERR;
    }
    
    return FNET_OK;
}


fnet_time_t fnet_timer_get_ms( void )
{
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}


fnet_ip4_addr_t fnet_netif_get_ip4_addr( fnet_netif_desc_t netif_desc )
{
    struct netif * lwip_netif = (struct netif *)netif_desc;
    return lwip_netif->ip_addr.addr;
}


fnet_bool_t fnet_netif_get_ip6_addr ( fnet_netif_desc_t netif_desc, fnet_index_t n, fnet_netif_ip6_addr_info_t *addr_info )
{
    return FNET_FALSE;
}
