if(NOT COMPONENT_FTM_ADAPTER_MK64F12_INCLUDED)
    
    set(COMPONENT_FTM_ADAPTER_MK64F12_INCLUDED true CACHE BOOL "component_ftm_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_ftm.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK64F12)

    include(driver_ftm_MK64F12)

endif()
