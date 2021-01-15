if(NOT COMPONENT_IGPIO_ADAPTER_MIMXRT1052_INCLUDED)
    
    set(COMPONENT_IGPIO_ADAPTER_MIMXRT1052_INCLUDED true CACHE BOOL "component_igpio_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_igpio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_igpio_MIMXRT1052)

endif()
