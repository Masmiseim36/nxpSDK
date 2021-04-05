if(NOT DRIVER_QSPI_MCIMX7U5_INCLUDED)
    
    set(DRIVER_QSPI_MCIMX7U5_INCLUDED true CACHE BOOL "driver_qspi component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_qspi.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MCIMX7U5)

endif()
