if(NOT COMPONENT_OSA_MIMXRT1052_INCLUDED)
    
    set(COMPONENT_OSA_MIMXRT1052_INCLUDED true CACHE BOOL "component_osa component is included.")

    if(CONFIG_USE_middleware_baremetal_MIMXRT1052)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_bm.c
    )
    elseif(CONFIG_USE_middleware_freertos-kernel_MIMXRT1052)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_free_rtos.c
    )
    else()
        message(WARNING "please config middleware.baremetal_MIMXRT1052 or middleware.freertos-kernel_MIMXRT1052 first.")
    endif()


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1052)

    include(component_lists_MIMXRT1052)

endif()
