include_guard()
message("component_button component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_component_button.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_component_gpio_adapter_MIMXRT1052)
     include(component_gpio_adapter_MIMXRT1052)
endif()
if(CONFIG_USE_component_lpc_gpio_adapter_MIMXRT1052)
     include(component_lpc_gpio_adapter_MIMXRT1052)
endif()
if(CONFIG_USE_component_rt_gpio_adapter_MIMXRT1052)
     include(component_rt_gpio_adapter_MIMXRT1052)
endif()
if(CONFIG_USE_component_igpio_adapter_MIMXRT1052)
     include(component_igpio_adapter_MIMXRT1052)
endif()
if(CONFIG_USE_component_rgpio_adapter_MIMXRT1052)
     include(component_rgpio_adapter_MIMXRT1052)
endif()
if(NOT (CONFIG_USE_component_gpio_adapter_MIMXRT1052 OR CONFIG_USE_component_lpc_gpio_adapter_MIMXRT1052 OR CONFIG_USE_component_rt_gpio_adapter_MIMXRT1052 OR CONFIG_USE_component_igpio_adapter_MIMXRT1052 OR CONFIG_USE_component_rgpio_adapter_MIMXRT1052))
    message(WARNING "Since component_gpio_adapter_MIMXRT1052/component_lpc_gpio_adapter_MIMXRT1052/component_rt_gpio_adapter_MIMXRT1052/component_igpio_adapter_MIMXRT1052/component_rgpio_adapter_MIMXRT1052 is not included at first or config in config.cmake file, use component_gpio_adapter_MIMXRT1052 by default.")
    include(component_gpio_adapter_MIMXRT1052)
endif()

include(driver_common_MIMXRT1052)

include(component_timer_manager_MIMXRT1052)

