if(NOT DRIVER_WM8960_MIMXRT1062_INCLUDED)
    
    set(DRIVER_WM8960_MIMXRT1062_INCLUDED true CACHE BOOL "driver_wm8960 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wm8960.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

    include(component_codec_i2c_MIMXRT1062)

endif()
