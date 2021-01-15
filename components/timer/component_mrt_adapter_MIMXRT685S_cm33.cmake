if(NOT COMPONENT_MRT_ADAPTER_MIMXRT685S_cm33_INCLUDED)
    
    set(COMPONENT_MRT_ADAPTER_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "component_mrt_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_mrt.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT685S_cm33)

    include(driver_mrt_MIMXRT685S_cm33)

endif()
