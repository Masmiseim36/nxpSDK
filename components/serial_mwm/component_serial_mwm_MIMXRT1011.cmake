include_guard()
message("component_serial_mwm component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/serial_mwm.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/.
)


#OR Logic component
if(CONFIG_USE_component_serial_mwm_usart_MIMXRT1011)
     include(component_serial_mwm_usart_MIMXRT1011)
endif()
if(CONFIG_USE_component_serial_mwm_lpuart_MIMXRT1011)
     include(component_serial_mwm_lpuart_MIMXRT1011)
endif()
if(NOT (CONFIG_USE_component_serial_mwm_usart_MIMXRT1011 OR CONFIG_USE_component_serial_mwm_lpuart_MIMXRT1011))
    message(WARNING "Since component_serial_mwm_usart_MIMXRT1011/component_serial_mwm_lpuart_MIMXRT1011 is not included at first or config in config.cmake file, use component_serial_mwm_usart_MIMXRT1011 by default.")
    include(component_serial_mwm_usart_MIMXRT1011)
endif()

