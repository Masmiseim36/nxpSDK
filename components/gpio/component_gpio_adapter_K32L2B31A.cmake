if(NOT COMPONENT_GPIO_ADAPTER_K32L2B31A_INCLUDED)
    
    set(COMPONENT_GPIO_ADAPTER_K32L2B31A_INCLUDED true CACHE BOOL "component_gpio_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_gpio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_gpio_K32L2B31A)

    include(driver_port_K32L2B31A)

endif()
