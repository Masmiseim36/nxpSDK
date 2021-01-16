if(NOT DRIVER_DCP_MIMXRT1021_INCLUDED)
    
    set(DRIVER_DCP_MIMXRT1021_INCLUDED true CACHE BOOL "driver_dcp component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dcp.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_cache_armv7_m7_MIMXRT1021)

    include(driver_common_MIMXRT1021)

endif()
