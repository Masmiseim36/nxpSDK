if(NOT DRIVER_CACHE_CACHE64_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_CACHE_CACHE64_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_cache_cache64 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_cache.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT685S_cm33)

endif()
