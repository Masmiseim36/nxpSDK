if(NOT COMPONENT_SERIAL_MANAGER_MK64F12_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_MK64F12_INCLUDED true CACHE BOOL "component_serial_manager component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_manager.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_serial_manager_uart_MK64F12)
         include(component_serial_manager_uart_MK64F12)
    endif()
    if(CONFIG_USE_component_serial_manager_usb_cdc_MK64F12)
         include(component_serial_manager_usb_cdc_MK64F12)
    endif()
    if(CONFIG_USE_component_serial_manager_virtual_MK64F12)
         include(component_serial_manager_virtual_MK64F12)
    endif()
    if(CONFIG_USE_component_serial_manager_swo_MK64F12)
         include(component_serial_manager_swo_MK64F12)
    endif()
    if(CONFIG_USE_component_serial_manager_rpmsg_MK64F12)
         include(component_serial_manager_rpmsg_MK64F12)
    endif()
    if(NOT (CONFIG_USE_component_serial_manager_uart_MK64F12 OR CONFIG_USE_component_serial_manager_usb_cdc_MK64F12 OR CONFIG_USE_component_serial_manager_virtual_MK64F12 OR CONFIG_USE_component_serial_manager_swo_MK64F12 OR CONFIG_USE_component_serial_manager_rpmsg_MK64F12))
        message(WARNING "Since component_serial_manager_uart_MK64F12/component_serial_manager_usb_cdc_MK64F12/component_serial_manager_virtual_MK64F12/component_serial_manager_swo_MK64F12/component_serial_manager_rpmsg_MK64F12 is not included at first or config in config.cmake file, use component_serial_manager_uart_MK64F12 by default.")
        include(component_serial_manager_uart_MK64F12)
    endif()

    include(driver_common_MK64F12)

    include(component_lists_MK64F12)

endif()
