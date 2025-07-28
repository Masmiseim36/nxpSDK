# Add set(CONFIG_USE_middleware_wifi_cli true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/cli/cli.c
          ${CMAKE_CURRENT_LIST_DIR}/cli/cli_mem_simple.c
          ${CMAKE_CURRENT_LIST_DIR}/cli/cli_utils.c
          ${CMAKE_CURRENT_LIST_DIR}/cli/wifi_shell.c
          ${CMAKE_CURRENT_LIST_DIR}/port/osa/osa_cli.c
          ${CMAKE_CURRENT_LIST_DIR}/dhcpd/dhcp-server-cli.c
          ${CMAKE_CURRENT_LIST_DIR}/nw_utils/wifi_ping.c
          ${CMAKE_CURRENT_LIST_DIR}/nw_utils/iperf.c
          ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_basic_cli.c
          ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_enhanced_tests.c
          ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_tests.c
          ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_test_mode_tests.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/cli
          ${CMAKE_CURRENT_LIST_DIR}/incl
          ${CMAKE_CURRENT_LIST_DIR}/incl/wlcmgr
          ${CMAKE_CURRENT_LIST_DIR}/nw_utils
        )

  
