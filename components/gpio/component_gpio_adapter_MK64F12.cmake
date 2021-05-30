if(NOT COMPONENT_GPIO_ADAPTER_MK64F12_INCLUDED)
    
    set(COMPONENT_GPIO_ADAPTER_MK64F12_INCLUDED true CACHE BOOL "component_gpio_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_gpio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_gpio_MK64F12)

    include(driver_port_MK64F12)

endif()
