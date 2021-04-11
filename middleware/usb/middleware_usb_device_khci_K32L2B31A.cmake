if(NOT MIDDLEWARE_USB_DEVICE_KHCI_K32L2B31A_INCLUDED)
    
    set(MIDDLEWARE_USB_DEVICE_KHCI_K32L2B31A_INCLUDED true CACHE BOOL "middleware_usb_device_khci component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_khci.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device
        ${CMAKE_CURRENT_LIST_DIR}/include
    )


    include(middleware_usb_device_common_header_K32L2B31A)

endif()
