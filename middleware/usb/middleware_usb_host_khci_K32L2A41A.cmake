if(NOT MIDDLEWARE_USB_HOST_KHCI_K32L2A41A_INCLUDED)
    
    set(MIDDLEWARE_USB_HOST_KHCI_K32L2A41A_INCLUDED true CACHE BOOL "middleware_usb_host_khci component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_khci.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host
        ${CMAKE_CURRENT_LIST_DIR}/include
    )


    include(middleware_usb_host_common_header_K32L2A41A)

endif()
