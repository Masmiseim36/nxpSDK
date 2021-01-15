if(NOT DRIVER_CS42888_MIMXRT1052_INCLUDED)
    
    set(DRIVER_CS42888_MIMXRT1052_INCLUDED true CACHE BOOL "driver_cs42888 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_cs42888.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1052)

    include(component_codec_i2c_MIMXRT1052)

endif()
