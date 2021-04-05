if(NOT COMPONENT_LPUART_ADAPTER_MCIMX7U5_INCLUDED)
    
    set(COMPONENT_LPUART_ADAPTER_MCIMX7U5_INCLUDED true CACHE BOOL "component_lpuart_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_lpuart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MCIMX7U5)

    include(driver_lpuart_MCIMX7U5)

endif()
