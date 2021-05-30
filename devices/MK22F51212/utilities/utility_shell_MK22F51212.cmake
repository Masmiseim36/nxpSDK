if(NOT UTILITY_SHELL_MK22F51212_INCLUDED)
    
    set(UTILITY_SHELL_MK22F51212_INCLUDED true CACHE BOOL "utility_shell component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_shell.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(utility_debug_console_MK22F51212)

    include(component_lists_MK22F51212)

    include(driver_common_MK22F51212)

endif()
