# Add set(CONFIG_USE_middleware_wifi true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/dhcpd/dhcp-server-main.c
          ${CMAKE_CURRENT_LIST_DIR}/dhcpd/dhcp-server.c
          ${CMAKE_CURRENT_LIST_DIR}/dhcpd/dns-server.c
          ${CMAKE_CURRENT_LIST_DIR}/port/net/net.c
          ${CMAKE_CURRENT_LIST_DIR}/port/net/wifi_netif.c
          ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan.c
          ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_txpwrlimit_cfg.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/incl
          ${CMAKE_CURRENT_LIST_DIR}/incl/wifidriver
          ${CMAKE_CURRENT_LIST_DIR}/incl/port/net
          ${CMAKE_CURRENT_LIST_DIR}/port/net
          ${CMAKE_CURRENT_LIST_DIR}/incl/port/net/hooks
        )

  
