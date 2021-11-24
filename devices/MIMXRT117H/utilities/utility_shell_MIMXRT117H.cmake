if(NOT UTILITY_SHELL_MIMXRT117H_INCLUDED)
    
    set(UTILITY_SHELL_MIMXRT117H_INCLUDED true CACHE BOOL "utility_shell component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_shell.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(utility_debug_console_MIMXRT117H)

    include(component_lists_MIMXRT117H)

    include(driver_common_MIMXRT117H)

endif()
