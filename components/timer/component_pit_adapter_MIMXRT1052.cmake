if(NOT COMPONENT_PIT_ADAPTER_MIMXRT1052_INCLUDED)
    
    set(COMPONENT_PIT_ADAPTER_MIMXRT1052_INCLUDED true CACHE BOOL "component_pit_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_pit.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1052)

    include(driver_pit_MIMXRT1052)

endif()
