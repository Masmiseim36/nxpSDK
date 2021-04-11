if(NOT COMPONENT_LED_K32L2B31A_INCLUDED)
    
    set(COMPONENT_LED_K32L2B31A_INCLUDED true CACHE BOOL "component_led component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_led.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    #OR Logic component
    if(CONFIG_USE_component_gpio_adapter_K32L2B31A)
         include(component_gpio_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_lpc_gpio_adapter_K32L2B31A)
         include(component_lpc_gpio_adapter_K32L2B31A)
    endif()
    if(CONFIG_USE_component_igpio_adapter_K32L2B31A)
         include(component_igpio_adapter_K32L2B31A)
    endif()
    if(NOT (CONFIG_USE_component_gpio_adapter_K32L2B31A OR CONFIG_USE_component_lpc_gpio_adapter_K32L2B31A OR CONFIG_USE_component_igpio_adapter_K32L2B31A))
        message(WARNING "Since component_gpio_adapter_K32L2B31A/component_lpc_gpio_adapter_K32L2B31A/component_igpio_adapter_K32L2B31A is not included at first or config in config.cmake file, use component_gpio_adapter_K32L2B31A by default.")
        include(component_gpio_adapter_K32L2B31A)
    endif()

    include(driver_common_K32L2B31A)

    include(component_timer_manager_K32L2B31A)

endif()
