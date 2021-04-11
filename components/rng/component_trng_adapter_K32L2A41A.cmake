if(NOT COMPONENT_TRNG_ADAPTER_K32L2A41A_INCLUDED)
    
    set(COMPONENT_TRNG_ADAPTER_K32L2A41A_INCLUDED true CACHE BOOL "component_trng_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_trng.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L2A41A)

    include(driver_trng_K32L2A41A)

endif()
