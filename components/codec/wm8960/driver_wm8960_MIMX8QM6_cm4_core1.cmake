if(NOT DRIVER_WM8960_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_WM8960_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_wm8960 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wm8960.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMX8QM6_cm4_core1)

    include(component_codec_i2c_MIMX8QM6_cm4_core1)

endif()
