if(NOT COMPONENT_OSA_FREE_RTOS_MK22F51212_INCLUDED)
    
    set(COMPONENT_OSA_FREE_RTOS_MK22F51212_INCLUDED true CACHE BOOL "component_osa_free_rtos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_free_rtos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(middleware_freertos-kernel_MK22F51212)

    include(driver_common_MK22F51212)

    include(component_lists_MK22F51212)

endif()
