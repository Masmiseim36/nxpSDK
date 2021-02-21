if(NOT DRIVER_WM8960_MIMXRT1176_cm7_INCLUDED)
    
    set(DRIVER_WM8960_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "driver_wm8960 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wm8960.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1176_cm7)

    include(component_codec_i2c_MIMXRT1176_cm7)

endif()
