if(NOT DRIVER_XIP_DEVICE_MIMXRT1062_INCLUDED)
    
    set(DRIVER_XIP_DEVICE_MIMXRT1062_INCLUDED true CACHE BOOL "driver_xip_device component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_flexspi_nor_boot.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

endif()
