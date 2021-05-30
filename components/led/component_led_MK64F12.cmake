if(NOT COMPONENT_LED_MK64F12_INCLUDED)
    
    set(COMPONENT_LED_MK64F12_INCLUDED true CACHE BOOL "component_led component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_led.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_gpio_adapter_MK64F12)
         include(component_gpio_adapter_MK64F12)
    endif()
    if(CONFIG_USE_component_lpc_gpio_adapter_MK64F12)
         include(component_lpc_gpio_adapter_MK64F12)
    endif()
    if(CONFIG_USE_component_igpio_adapter_MK64F12)
         include(component_igpio_adapter_MK64F12)
    endif()
    if(NOT (CONFIG_USE_component_gpio_adapter_MK64F12 OR CONFIG_USE_component_lpc_gpio_adapter_MK64F12 OR CONFIG_USE_component_igpio_adapter_MK64F12))
        message(WARNING "Since component_gpio_adapter_MK64F12/component_lpc_gpio_adapter_MK64F12/component_igpio_adapter_MK64F12 is not included at first or config in config.cmake file, use component_gpio_adapter_MK64F12 by default.")
        include(component_gpio_adapter_MK64F12)
    endif()

    include(driver_common_MK64F12)

    include(component_timer_manager_MK64F12)

endif()
