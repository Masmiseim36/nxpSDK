/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fnet_mdns_port.h"
#include "qcom_api.h"


fnet_socket_t fnet_socket( fnet_address_family_t family, fnet_socket_type_t type, fnet_uint32_t protocol )
{
    return qcom_socket(family, type, protocol);
}


fnet_return_t fnet_socket_bind( fnet_socket_t s, const struct fnet_sockaddr *name, fnet_size_t namelen )
{
    SOCKADDR_T addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = name->sa_family;
    addr.sin_port = FNET_HTONS(name->sa_port);
    
    if(qcom_bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        return FNET_ERR;
    }
    
    return FNET_OK;
}


fnet_ssize_t fnet_socket_recvfrom( fnet_socket_t s, void *buf, fnet_size_t len, fnet_flag_t flags, struct fnet_sockaddr *from, fnet_size_t *fromlen )
{
    SOCKADDR_T addr;
    socklen_t addr_len = sizeof(SOCKADDR_T);
    
    int ret = 0;
    
    A_STATUS xStatus;
    xStatus = ( A_STATUS ) t_select( Custom_Api_GetDriverCxt(0), (uint32_t ) s, 1 );
    if( xStatus == A_OK )
    {
        char* loc_buf;
        ret = qcom_recvfrom(s, &loc_buf, len, flags, (struct sockaddr*)&addr, &addr_len);
        if( (ret > 0)  && (loc_buf != NULL) )
        {
            memcpy( buf, loc_buf, ret );
            zero_copy_free( loc_buf );
        }
    }
    else
    {
        /* nothing to receive */
        return 0;
    }
    
    if((from != NULL))
    {
        struct fnet_sockaddr_in* fnet_from = (struct fnet_sockaddr_in*) from;
        fnet_from->sin_family = addr.sin_family;
        fnet_from->sin_port = FNET_HTONS(addr.sin_port);
        fnet_from->sin_addr.s_addr = addr.sin_addr.s_addr;
    }

    return ret;
}


fnet_ssize_t fnet_socket_sendto( fnet_socket_t s, const void *buf, fnet_size_t len, fnet_flag_t flags, const struct fnet_sockaddr *to, fnet_size_t tolen )
{
    char* loc_buf = custom_alloc( len );
    if( loc_buf == NULL )
    {
        return FNET_ERR;
    }
  
    struct fnet_sockaddr_in* fnet_to = (struct fnet_sockaddr_in*)to;
    
    SOCKADDR_T addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = fnet_to->sin_family;
    addr.sin_port = FNET_HTONS(fnet_to->sin_port);
    addr.sin_addr.s_addr = /*FNET_HTONL*/(fnet_to->sin_addr.s_addr);
    
    memcpy(loc_buf, buf, len);

    int ret;
    ret = qcom_sendto(s, loc_buf, len, flags, (struct sockaddr*)&addr, sizeof(addr));
    custom_free( loc_buf );
  
    return ret;                         
}


fnet_return_t fnet_socket_close( fnet_socket_t s )
{
    if(qcom_socket_close(s) < 0)
    {
        return FNET_ERR;
    }
    
    return FNET_OK;
}


fnet_return_t fnet_socket_setopt( fnet_socket_t s, fnet_protocol_t level, fnet_socket_options_t optname, const void *optval, fnet_size_t optvallen )
{
    if(qcom_setsockopt(s, level, optname, (void*)optval, optvallen) < 0)
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
    fnet_ip4_addr_t addr = 0;
    
    if (A_OK != qcom_ipconfig(0, QCOM_IPCONFIG_QUERY, (uint32_t *)&addr, NULL, NULL))
    {
        //error
    }
    
    return FNET_HTONL(addr);
}


fnet_bool_t fnet_netif_get_ip6_addr ( fnet_netif_desc_t netif_desc, fnet_index_t n, fnet_netif_ip6_addr_info_t *addr_info )
{
    return FNET_FALSE;
}
  

/* Convert short from host- to network byte order.*/
fnet_uint16_t fnet_htons(fnet_uint16_t short_var)
{
    fnet_uint16_t result = FNET_HTONS(short_var);
    return result;
}


/* Convert long from host- to network byte order.*/
fnet_uint32_t fnet_htonl(fnet_uint32_t long_var)
{
    fnet_uint32_t result = FNET_HTONL(long_var);
    return result;
}
