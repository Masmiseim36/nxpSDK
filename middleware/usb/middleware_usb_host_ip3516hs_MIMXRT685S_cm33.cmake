if(NOT MIDDLEWARE_USB_HOST_IP3516HS_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_USB_HOST_IP3516HS_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_usb_host_ip3516hs component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_ip3516hs.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host
        ${CMAKE_CURRENT_LIST_DIR}/include
    )


    include(middleware_usb_phy_MIMXRT685S_cm33)

    include(middleware_usb_host_common_header_MIMXRT685S_cm33)

endif()
