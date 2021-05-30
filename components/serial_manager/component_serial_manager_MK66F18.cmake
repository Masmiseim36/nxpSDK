if(NOT COMPONENT_SERIAL_MANAGER_MK66F18_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_MK66F18_INCLUDED true CACHE BOOL "component_serial_manager component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_manager.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_serial_manager_uart_MK66F18)
         include(component_serial_manager_uart_MK66F18)
    endif()
    if(CONFIG_USE_component_serial_manager_usb_cdc_MK66F18)
         include(component_serial_manager_usb_cdc_MK66F18)
    endif()
    if(CONFIG_USE_component_serial_manager_virtual_MK66F18)
         include(component_serial_manager_virtual_MK66F18)
    endif()
    if(CONFIG_USE_component_serial_manager_swo_MK66F18)
         include(component_serial_manager_swo_MK66F18)
    endif()
    if(CONFIG_USE_component_serial_manager_rpmsg_MK66F18)
         include(component_serial_manager_rpmsg_MK66F18)
    endif()
    if(NOT (CONFIG_USE_component_serial_manager_uart_MK66F18 OR CONFIG_USE_component_serial_manager_usb_cdc_MK66F18 OR CONFIG_USE_component_serial_manager_virtual_MK66F18 OR CONFIG_USE_component_serial_manager_swo_MK66F18 OR CONFIG_USE_component_serial_manager_rpmsg_MK66F18))
        message(WARNING "Since component_serial_manager_uart_MK66F18/component_serial_manager_usb_cdc_MK66F18/component_serial_manager_virtual_MK66F18/component_serial_manager_swo_MK66F18/component_serial_manager_rpmsg_MK66F18 is not included at first or config in config.cmake file, use component_serial_manager_uart_MK66F18 by default.")
        include(component_serial_manager_uart_MK66F18)
    endif()

    include(driver_common_MK66F18)

    include(component_lists_MK66F18)

endif()
