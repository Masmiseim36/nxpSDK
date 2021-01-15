if(NOT COMPONENT_OSA_MIMXRT685S_cm33_INCLUDED)
    
    set(COMPONENT_OSA_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "component_osa component is included.")

    if(CONFIG_USE_middleware_baremetal_MIMXRT685S_cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_bm.c
    )
    elseif(CONFIG_USE_middleware_freertos-kernel_MIMXRT685S_cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_free_rtos.c
    )
    else()
        message(WARNING "please config middleware.baremetal_MIMXRT685S_cm33 or middleware.freertos-kernel_MIMXRT685S_cm33 first.")
    endif()


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT685S_cm33)

    include(component_lists_MIMXRT685S_cm33)

endif()
