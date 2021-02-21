if(NOT MIDDLEWARE_WIFI_WICED_MURATA_TYPE1DX_MIMXRT1176_cm4_INCLUDED)
    
    set(MIDDLEWARE_WIFI_WICED_MURATA_TYPE1DX_MIMXRT1176_cm4_INCLUDED true CACHE BOOL "middleware_wifi_wiced_MURATA_TYPE1DX component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/43xxx_Wi-Fi/platforms/MURATA_TYPE1DX
    )

    include(middleware_wifi_wiced_4343W_MIMXRT1176_cm4)

endif()
