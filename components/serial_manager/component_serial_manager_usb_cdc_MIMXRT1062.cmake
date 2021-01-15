if(NOT COMPONENT_SERIAL_MANAGER_USB_CDC_MIMXRT1062_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_USB_CDC_MIMXRT1062_INCLUDED true CACHE BOOL "component_serial_manager_usb_cdc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_port_usb.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_cdc_adapter/usb_device_descriptor.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
        ${CMAKE_CURRENT_LIST_DIR}/usb_cdc_adapter
    )


    include(component_serial_manager_MIMXRT1062)

    include(middleware_usb_device_cdc_external_MIMXRT1062)

    include(driver_common_MIMXRT1062)

endif()
