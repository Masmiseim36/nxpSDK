if(NOT COMPONENT_WM8960_ADAPTER_MIMX8QX6_INCLUDED)
    
    set(COMPONENT_WM8960_ADAPTER_MIMX8QX6_INCLUDED true CACHE BOOL "component_wm8960_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_codec_adapter.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_wm8960_MIMX8QX6)

    include(driver_codec_MIMX8QX6)

endif()
