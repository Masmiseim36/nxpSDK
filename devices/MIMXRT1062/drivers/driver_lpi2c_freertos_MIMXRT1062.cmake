if(NOT DRIVER_LPI2C_FREERTOS_MIMXRT1062_INCLUDED)
    
    set(DRIVER_LPI2C_FREERTOS_MIMXRT1062_INCLUDED true CACHE BOOL "driver_lpi2c_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpi2c_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpi2c_MIMXRT1062)

    include(middleware_freertos-kernel_MIMXRT1062)

endif()
