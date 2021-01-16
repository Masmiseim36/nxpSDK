if(NOT DRIVER_CACHE_ARMV7_M7_MIMXRT1052_INCLUDED)
    
    set(DRIVER_CACHE_ARMV7_M7_MIMXRT1052_INCLUDED true CACHE BOOL "driver_cache_armv7_m7 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_cache.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1052)

endif()
