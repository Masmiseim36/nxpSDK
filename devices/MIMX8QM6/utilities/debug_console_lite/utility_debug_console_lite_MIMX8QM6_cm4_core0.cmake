if(NOT UTILITY_DEBUG_CONSOLE_LITE_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(UTILITY_DEBUG_CONSOLE_LITE_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "utility_debug_console_lite component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_debug_console.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(component_lpuart_adapter_MIMX8QM6_cm4_core0)

    include(driver_common_MIMX8QM6_cm4_core0)

endif()
