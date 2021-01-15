if(NOT COMPONENT_LOG_BACKEND_DEBUGCONSOLE_MIMXRT1021_INCLUDED)
    
    set(COMPONENT_LOG_BACKEND_DEBUGCONSOLE_MIMXRT1021_INCLUDED true CACHE BOOL "component_log_backend_debugconsole component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_log_backend_debugconsole.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1021)

    include(component_log_MIMXRT1021)

    include(utility_debug_console_MIMXRT1021)

endif()
