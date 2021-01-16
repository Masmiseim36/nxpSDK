if(NOT DRIVER_TFA9XXX_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_TFA9XXX_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_tfa9xxx component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_tfa9xxx.c
        ${CMAKE_CURRENT_LIST_DIR}/fsl_tfa9xxx_IMX.c
        ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa2_container.c
        ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa2_dev.c
        ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa2_haptic.c
        ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa2_init.c
        ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv/tfa_container_crc32.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
        ${CMAKE_CURRENT_LIST_DIR}/vas_tfa_drv
    )


    include(driver_common_MIMXRT685S_cm33)

    include(component_codec_i2c_MIMXRT685S_cm33)

endif()
