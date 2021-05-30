if(NOT COMPONENT_DA7212_ADAPTER_MK66F18_INCLUDED)
    
    set(COMPONENT_DA7212_ADAPTER_MK66F18_INCLUDED true CACHE BOOL "component_da7212_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_codec_adapter.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_dialog7212_MK66F18)

    include(driver_codec_MK66F18)

endif()
