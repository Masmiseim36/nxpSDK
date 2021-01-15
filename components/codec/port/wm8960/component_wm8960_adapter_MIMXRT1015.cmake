if(NOT COMPONENT_WM8960_ADAPTER_MIMXRT1015_INCLUDED)
    
    set(COMPONENT_WM8960_ADAPTER_MIMXRT1015_INCLUDED true CACHE BOOL "component_wm8960_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_codec_adapter.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_wm8960_MIMXRT1015)

    include(driver_codec_MIMXRT1015)

endif()
