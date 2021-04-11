if(NOT COMPONENT_LPUART_ADAPTER_K32L3A60_cm0plus_INCLUDED)
    
    set(COMPONENT_LPUART_ADAPTER_K32L3A60_cm0plus_INCLUDED true CACHE BOOL "component_lpuart_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_lpuart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L3A60_cm0plus)

    include(driver_lpuart_K32L3A60_cm0plus)

endif()
