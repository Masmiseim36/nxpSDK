if(NOT DRIVER_DIALOG7212_MK66F18_INCLUDED)
    
    set(DRIVER_DIALOG7212_MK66F18_INCLUDED true CACHE BOOL "driver_dialog7212 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_dialog7212.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK66F18)

    include(component_codec_i2c_MK66F18)

endif()
