if(NOT COMPONENT_SERIAL_MANAGER_MIMXRT117H_INCLUDED)
    
    set(COMPONENT_SERIAL_MANAGER_MIMXRT117H_INCLUDED true CACHE BOOL "component_serial_manager component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_serial_manager.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_serial_manager_uart_MIMXRT117H)
         include(component_serial_manager_uart_MIMXRT117H)
    endif()
    if(CONFIG_USE_component_serial_manager_usb_cdc_MIMXRT117H)
         include(component_serial_manager_usb_cdc_MIMXRT117H)
    endif()
    if(CONFIG_USE_component_serial_manager_virtual_MIMXRT117H)
         include(component_serial_manager_virtual_MIMXRT117H)
    endif()
    if(CONFIG_USE_component_serial_manager_swo_MIMXRT117H)
         include(component_serial_manager_swo_MIMXRT117H)
    endif()
    if(CONFIG_USE_component_serial_manager_rpmsg_MIMXRT117H)
         include(component_serial_manager_rpmsg_MIMXRT117H)
    endif()
    if(NOT (CONFIG_USE_component_serial_manager_uart_MIMXRT117H OR CONFIG_USE_component_serial_manager_usb_cdc_MIMXRT117H OR CONFIG_USE_component_serial_manager_virtual_MIMXRT117H OR CONFIG_USE_component_serial_manager_swo_MIMXRT117H OR CONFIG_USE_component_serial_manager_rpmsg_MIMXRT117H))
        message(WARNING "Since component_serial_manager_uart_MIMXRT117H/component_serial_manager_usb_cdc_MIMXRT117H/component_serial_manager_virtual_MIMXRT117H/component_serial_manager_swo_MIMXRT117H/component_serial_manager_rpmsg_MIMXRT117H is not included at first or config in config.cmake file, use component_serial_manager_uart_MIMXRT117H by default.")
        include(component_serial_manager_uart_MIMXRT117H)
    endif()

    include(driver_common_MIMXRT117H)

    include(component_lists_MIMXRT117H)

endif()
