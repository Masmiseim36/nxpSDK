if(NOT COMPONENT_WM8524_ADAPTER_MIMX8MM6_INCLUDED)
    
    set(COMPONENT_WM8524_ADAPTER_MIMX8MM6_INCLUDED true CACHE BOOL "component_wm8524_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_codec_adapter.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_wm8524_MIMX8MM6)

    include(driver_codec_MIMX8MM6)

endif()
