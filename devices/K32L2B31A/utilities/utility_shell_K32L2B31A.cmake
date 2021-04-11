if(NOT UTILITY_SHELL_K32L2B31A_INCLUDED)
    
    set(UTILITY_SHELL_K32L2B31A_INCLUDED true CACHE BOOL "utility_shell component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_shell.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(utility_debug_console_K32L2B31A)

    include(component_lists_K32L2B31A)

    include(driver_common_K32L2B31A)

endif()
