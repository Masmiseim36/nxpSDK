if(NOT DRIVER_TRNG_MIMXRT1024_INCLUDED)
    
    set(DRIVER_TRNG_MIMXRT1024_INCLUDED true CACHE BOOL "driver_trng component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_trng.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1024)

endif()
