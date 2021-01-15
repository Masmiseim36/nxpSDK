if(NOT DRIVER_LPI2C_MIMXRT1024_INCLUDED)
    
    set(DRIVER_LPI2C_MIMXRT1024_INCLUDED true CACHE BOOL "driver_lpi2c component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_lpi2c.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1024)

endif()
