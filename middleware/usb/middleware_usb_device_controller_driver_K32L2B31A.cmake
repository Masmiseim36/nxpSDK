if(NOT MIDDLEWARE_USB_DEVICE_CONTROLLER_DRIVER_K32L2B31A_INCLUDED)
    
    set(MIDDLEWARE_USB_DEVICE_CONTROLLER_DRIVER_K32L2B31A_INCLUDED true CACHE BOOL "middleware_usb_device_controller_driver component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_dci.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/device
    )


    #OR Logic component
    if(CONFIG_USE_middleware_usb_device_khci_K32L2B31A)
         include(middleware_usb_device_khci_K32L2B31A)
    endif()
    if(CONFIG_USE_middleware_usb_device_ehci_K32L2B31A)
         include(middleware_usb_device_ehci_K32L2B31A)
    endif()
    if(CONFIG_USE_middleware_usb_device_ip3511fs_K32L2B31A)
         include(middleware_usb_device_ip3511fs_K32L2B31A)
    endif()
    if(CONFIG_USE_middleware_usb_device_ip3511hs_K32L2B31A)
         include(middleware_usb_device_ip3511hs_K32L2B31A)
    endif()
    if(NOT (CONFIG_USE_middleware_usb_device_khci_K32L2B31A OR CONFIG_USE_middleware_usb_device_ehci_K32L2B31A OR CONFIG_USE_middleware_usb_device_ip3511fs_K32L2B31A OR CONFIG_USE_middleware_usb_device_ip3511hs_K32L2B31A))
        message(WARNING "Since middleware_usb_device_khci_K32L2B31A/middleware_usb_device_ehci_K32L2B31A/middleware_usb_device_ip3511fs_K32L2B31A/middleware_usb_device_ip3511hs_K32L2B31A is not included at first or config in config.cmake file, use middleware_usb_device_khci_K32L2B31A by default.")
        include(middleware_usb_device_khci_K32L2B31A)
    endif()

    include(component_osa_K32L2B31A)

endif()
