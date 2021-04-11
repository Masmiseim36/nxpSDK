if(NOT COMPONENT_PANIC_K32L2B31A_INCLUDED)
    
    set(COMPONENT_PANIC_K32L2B31A_INCLUDED true CACHE BOOL "component_panic component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_panic.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2B31A)

    include(utility_debug_console_K32L2B31A)

endif()
