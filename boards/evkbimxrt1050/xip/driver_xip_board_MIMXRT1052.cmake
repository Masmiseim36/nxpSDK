if(NOT DRIVER_XIP_BOARD_MIMXRT1052_INCLUDED)
    
    set(DRIVER_XIP_BOARD_MIMXRT1052_INCLUDED true CACHE BOOL "driver_xip_board component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/evkbimxrt1050_flexspi_nor_config.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1052)

endif()
