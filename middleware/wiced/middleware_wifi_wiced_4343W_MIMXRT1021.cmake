if(NOT MIDDLEWARE_WIFI_WICED_4343W_MIMXRT1021_INCLUDED)
    
    set(MIDDLEWARE_WIFI_WICED_4343W_MIMXRT1021_INCLUDED true CACHE BOOL "middleware_wifi_wiced_4343W component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/resources.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/wwd_ap.c
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W/wwd_chip_specific_functions.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/WICED/WWD/internal/chips/4343W
    )


    include(middleware_wifi_wiced_MIMXRT1021)

endif()
