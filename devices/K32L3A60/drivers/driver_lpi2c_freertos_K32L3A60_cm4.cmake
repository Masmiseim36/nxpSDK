if(NOT DRIVER_LPI2C_FREERTOS_K32L3A60_cm4_INCLUDED)
    
    set(DRIVER_LPI2C_FREERTOS_K32L3A60_cm4_INCLUDED true CACHE BOOL "driver_lpi2c_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpi2c_freertos.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_lpi2c_K32L3A60_cm4)

    include(middleware_freertos-kernel_K32L3A60_cm4)

endif()
