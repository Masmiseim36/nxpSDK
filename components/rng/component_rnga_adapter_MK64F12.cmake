if(NOT COMPONENT_RNGA_ADAPTER_MK64F12_INCLUDED)
    
    set(COMPONENT_RNGA_ADAPTER_MK64F12_INCLUDED true CACHE BOOL "component_rnga_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_rnga.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK64F12)

    include(driver_rnga_MK64F12)

endif()
