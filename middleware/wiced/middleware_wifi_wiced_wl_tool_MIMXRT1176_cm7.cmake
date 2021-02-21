if(NOT MIDDLEWARE_WIFI_WICED_WL_TOOL_MIMXRT1176_cm7_INCLUDED)
    
    set(MIDDLEWARE_WIFI_WICED_WL_TOOL_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "middleware_wifi_wiced_wl_tool component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/4343WA1/wl/exe/wlu_pipe.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/4343WA1/wl/exe/wlu_server_shared.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/wlu_server.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/4343WA1/common/include/devctrl_if
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/4343WA1/common/include/proto
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/4343WA1/include
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/4343WA1/shared/bcmwifi/include
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/4343WA1/wl/exe
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/dummy/arpa
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/dummy/net
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/dummy
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/dummy/sys
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/libraries/test/wl_tool/4343WA1/common/include
    )


    include(middleware_wifi_wiced_MIMXRT1176_cm7)

endif()
