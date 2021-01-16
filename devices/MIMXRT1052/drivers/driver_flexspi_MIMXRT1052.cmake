if(NOT DRIVER_FLEXSPI_MIMXRT1052_INCLUDED)
    
    set(DRIVER_FLEXSPI_MIMXRT1052_INCLUDED true CACHE BOOL "driver_flexspi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexspi.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1052)

endif()
