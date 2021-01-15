if(NOT UTILITY_SHELL_MIMXRT1052_INCLUDED)
    
    set(UTILITY_SHELL_MIMXRT1052_INCLUDED true CACHE BOOL "utility_shell component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_shell.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(utility_debug_console_MIMXRT1052)

    include(component_lists_MIMXRT1052)

    include(driver_common_MIMXRT1052)

endif()
