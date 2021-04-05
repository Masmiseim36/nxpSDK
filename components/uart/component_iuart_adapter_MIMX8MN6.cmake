if(NOT COMPONENT_IUART_ADAPTER_MIMX8MN6_INCLUDED)
    
    set(COMPONENT_IUART_ADAPTER_MIMX8MN6_INCLUDED true CACHE BOOL "component_iuart_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_iuart.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8MN6)

    include(driver_iuart_MIMX8MN6)

endif()
