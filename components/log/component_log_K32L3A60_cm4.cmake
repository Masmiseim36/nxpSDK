if(NOT COMPONENT_LOG_K32L3A60_cm4_INCLUDED)
    
    set(COMPONENT_LOG_K32L3A60_cm4_INCLUDED true CACHE BOOL "component_log component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_log.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L3A60_cm4)

    include(utility_debug_console_K32L3A60_cm4)

endif()
