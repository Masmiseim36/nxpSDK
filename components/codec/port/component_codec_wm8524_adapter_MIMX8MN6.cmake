if(NOT COMPONENT_CODEC_WM8524_ADAPTER_MIMX8MN6_INCLUDED)
    
    set(COMPONENT_CODEC_WM8524_ADAPTER_MIMX8MN6_INCLUDED true CACHE BOOL "component_codec_wm8524_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wm8524_adapter.c
        ${CMAKE_CURRENT_LIST_DIR}/fsl_codec_adapter.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_wm8524_MIMX8MN6)

    include(driver_codec_MIMX8MN6)

endif()
