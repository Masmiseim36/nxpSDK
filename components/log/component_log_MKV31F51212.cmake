if(NOT COMPONENT_LOG_MKV31F51212_INCLUDED)
    
    set(COMPONENT_LOG_MKV31F51212_INCLUDED true CACHE BOOL "component_log component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_log.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MKV31F51212)

    include(utility_debug_console_MKV31F51212)

endif()
