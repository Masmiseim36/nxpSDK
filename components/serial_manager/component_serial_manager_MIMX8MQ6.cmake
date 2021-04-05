if(NOT COMPONENT_SERIAL_MANAGER_MIMX8MQ6_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_MIMX8MQ6_INCLUDED true CACHE BOOL "component_serial_manager component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_manager.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_serial_manager_uart_MIMX8MQ6)
         include(component_serial_manager_uart_MIMX8MQ6)
    endif()
    if(CONFIG_USE_component_serial_manager_usb_cdc_MIMX8MQ6)
         include(component_serial_manager_usb_cdc_MIMX8MQ6)
    endif()
    if(CONFIG_USE_component_serial_manager_virtual_MIMX8MQ6)
         include(component_serial_manager_virtual_MIMX8MQ6)
    endif()
    if(CONFIG_USE_component_serial_manager_swo_MIMX8MQ6)
         include(component_serial_manager_swo_MIMX8MQ6)
    endif()
    if(CONFIG_USE_component_serial_manager_rpmsg_MIMX8MQ6)
         include(component_serial_manager_rpmsg_MIMX8MQ6)
    endif()
    if(NOT (CONFIG_USE_component_serial_manager_uart_MIMX8MQ6 OR CONFIG_USE_component_serial_manager_usb_cdc_MIMX8MQ6 OR CONFIG_USE_component_serial_manager_virtual_MIMX8MQ6 OR CONFIG_USE_component_serial_manager_swo_MIMX8MQ6 OR CONFIG_USE_component_serial_manager_rpmsg_MIMX8MQ6))
        message(WARNING "Since component_serial_manager_uart_MIMX8MQ6/component_serial_manager_usb_cdc_MIMX8MQ6/component_serial_manager_virtual_MIMX8MQ6/component_serial_manager_swo_MIMX8MQ6/component_serial_manager_rpmsg_MIMX8MQ6 is not included at first or config in config.cmake file, use component_serial_manager_uart_MIMX8MQ6 by default.")
        include(component_serial_manager_uart_MIMX8MQ6)
    endif()

    include(driver_common_MIMX8MQ6)

    include(component_lists_MIMX8MQ6)

endif()
