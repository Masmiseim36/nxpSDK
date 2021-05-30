if(NOT COMPONENT_SERIAL_MANAGER_USB_CDC_MK22F51212_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_USB_CDC_MK22F51212_INCLUDED true CACHE BOOL "component_serial_manager_usb_cdc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_usb.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_cdc_adapter/usb_device_descriptor.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
        ${CMAKE_CURRENT_LIST_DIR}/usb_cdc_adapter
    )


    include(component_serial_manager_MK22F51212)

    include(middleware_usb_device_cdc_external_MK22F51212)

    include(driver_common_MK22F51212)

endif()
