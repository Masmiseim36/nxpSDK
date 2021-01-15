if(NOT COMPONENT_USART_ADAPTER_MIMXRT685S_cm33_INCLUDED)
    
    set(COMPONENT_USART_ADAPTER_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "component_usart_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_usart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT685S_cm33)

    include(driver_flexcomm_usart_MIMXRT685S_cm33)

    include(driver_flexcomm_MIMXRT685S_cm33)

endif()
