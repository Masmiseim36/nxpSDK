if(NOT COMPONENT_LPIT_ADAPTER_K32L3A60_cm4_INCLUDED)
    
    set(COMPONENT_LPIT_ADAPTER_K32L3A60_cm4_INCLUDED true CACHE BOOL "component_lpit_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_lpit.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_K32L3A60_cm4)

    include(driver_lpit_K32L3A60_cm4)

endif()
