if(NOT DRIVER_XIP_BOARD_MIMXRT1011_INCLUDED)
    
    set(DRIVER_XIP_BOARD_MIMXRT1011_INCLUDED true CACHE BOOL "driver_xip_board component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/evkmimxrt1010_flexspi_nor_config.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1011)

endif()
