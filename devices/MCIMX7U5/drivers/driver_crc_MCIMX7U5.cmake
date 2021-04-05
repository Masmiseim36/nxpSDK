if(NOT DRIVER_CRC_MCIMX7U5_INCLUDED)
    
    set(DRIVER_CRC_MCIMX7U5_INCLUDED true CACHE BOOL "driver_crc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_crc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MCIMX7U5)

endif()
