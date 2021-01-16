if(NOT MIDDLEWARE_USB_HOST_EHCI_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_USB_HOST_EHCI_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_usb_host_ehci component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_ehci.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host
        ${CMAKE_CURRENT_LIST_DIR}/include
    )


    include(middleware_usb_phy_MIMXRT1052)

    include(middleware_usb_host_common_header_MIMXRT1052)

endif()
