if(NOT MIDDLEWARE_WIFI_CLI_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_WIFI_CLI_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_wifi_cli component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cli/cli.c
        ${CMAKE_CURRENT_LIST_DIR}/cli/cli_mem_simple.c
        ${CMAKE_CURRENT_LIST_DIR}/cli/cli_utils.c
        ${CMAKE_CURRENT_LIST_DIR}/nw_utils/ping.c
        ${CMAKE_CURRENT_LIST_DIR}/nw_utils/iperf.c
        ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_basic_cli.c
        ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_enhanced_tests.c
        ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_tests.c
        ${CMAKE_CURRENT_LIST_DIR}/wlcmgr/wlan_test_mode_tests.c
        ${CMAKE_CURRENT_LIST_DIR}/dhcpd/dhcp-server-cli.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/cli
        ${CMAKE_CURRENT_LIST_DIR}/incl
        ${CMAKE_CURRENT_LIST_DIR}/incl/wlcmgr
    )


    include(middleware_wifi_MIMXRT1064)

endif()
