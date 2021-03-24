if(NOT MIDDLEWARE_LWIP_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_LWIP_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_lwip component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/port/sys_arch.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/sockets.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/netifapi.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/netdb.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/netbuf.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/tcpip.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/api_msg.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/err.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/if_api.c
        ${CMAKE_CURRENT_LIST_DIR}/src/api/api_lib.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/memp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/def.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ip.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/pbuf.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/altcp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/ethip6.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/dhcp6.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/ip6_addr.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/nd6.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/mld6.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/icmp6.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/ip6_frag.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/ip6.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv6/inet6.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/tcp_out.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/raw.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/altcp_tcp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/sys.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/init.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/inet_chksum.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/timeouts.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/tcp_in.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/netif.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/mem.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/udp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/dhcp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/ip4.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/autoip.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/icmp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/acd.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/etharp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/ip4_addr.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/igmp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/ipv4/ip4_frag.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/tcp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/stats.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/dns.c
        ${CMAKE_CURRENT_LIST_DIR}/src/core/altcp_alloc.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/lowpan6.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ethernet.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/bridgeif.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/bridgeif_fdb.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/zepif.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/lowpan6_common.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/lowpan6_ble.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/slipif.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/pppol2tp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/mppe.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/vj.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/pppcrypt.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/ppp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/eap.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/chap-md5.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/lcp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/pppos.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/ipcp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/fsm.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/ccp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/utils.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/chap-new.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/demand.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/pppoe.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/ipv6cp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/lwip_ecp.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/multilink.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/eui64.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/magic.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/upap.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/pppapi.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/chap_ms.c
        ${CMAKE_CURRENT_LIST_DIR}/src/netif/ppp/auth.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/port
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${CMAKE_CURRENT_LIST_DIR}/src/include
    )


    #OR Logic component
    if(CONFIG_USE_middleware_lwip_empty_ethernetif_MIMXRT1064)
         include(middleware_lwip_empty_ethernetif_MIMXRT1064)
    endif()
    if(CONFIG_USE_middleware_lwip_usb_ethernetif_MIMXRT1064)
         include(middleware_lwip_usb_ethernetif_MIMXRT1064)
    endif()
    if(CONFIG_USE_middleware_lwip_enet_ethernetif_MIMXRT1064)
         include(middleware_lwip_enet_ethernetif_MIMXRT1064)
    endif()
    if(CONFIG_USE_middleware_wifi_wiced_MURATA_TYPE1LV_MIMXRT1064)
         include(middleware_wifi_wiced_MURATA_TYPE1LV_MIMXRT1064)
    endif()
    if(CONFIG_USE_middleware_wifi_wiced_MURATA_TYPE1DX_MIMXRT1064)
         include(middleware_wifi_wiced_MURATA_TYPE1DX_MIMXRT1064)
    endif()
    if(CONFIG_USE_middleware_wifi_wiced_AW_NM372SM_MIMXRT1064)
         include(middleware_wifi_wiced_AW_NM372SM_MIMXRT1064)
    endif()
    if(CONFIG_USE_middleware_wifi_MIMXRT1064)
         include(middleware_wifi_MIMXRT1064)
    endif()
    if(NOT (CONFIG_USE_middleware_lwip_empty_ethernetif_MIMXRT1064 OR CONFIG_USE_middleware_lwip_usb_ethernetif_MIMXRT1064 OR CONFIG_USE_middleware_lwip_enet_ethernetif_MIMXRT1064 OR CONFIG_USE_middleware_wifi_wiced_MURATA_TYPE1LV_MIMXRT1064 OR CONFIG_USE_middleware_wifi_wiced_MURATA_TYPE1DX_MIMXRT1064 OR CONFIG_USE_middleware_wifi_wiced_AW_NM372SM_MIMXRT1064 OR CONFIG_USE_middleware_wifi_MIMXRT1064))
        message(WARNING "Since middleware_lwip_empty_ethernetif_MIMXRT1064/middleware_lwip_usb_ethernetif_MIMXRT1064/middleware_lwip_enet_ethernetif_MIMXRT1064/middleware_wifi_wiced_MURATA_TYPE1LV_MIMXRT1064/middleware_wifi_wiced_MURATA_TYPE1DX_MIMXRT1064/middleware_wifi_wiced_AW_NM372SM_MIMXRT1064/middleware_wifi_MIMXRT1064 is not included at first or config in config.cmake file, use middleware_lwip_empty_ethernetif_MIMXRT1064 by default.")
        include(middleware_lwip_empty_ethernetif_MIMXRT1064)
    endif()

    include(utility_debug_console_MIMXRT1064)

endif()
