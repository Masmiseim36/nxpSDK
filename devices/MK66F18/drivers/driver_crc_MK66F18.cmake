if(NOT DRIVER_CRC_MK66F18_INCLUDED)
    
    set(DRIVER_CRC_MK66F18_INCLUDED true CACHE BOOL "driver_crc component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_crc.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK66F18)

endif()
