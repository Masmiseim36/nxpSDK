if(NOT COMPONENT_TFA9896_ADAPTER_MIMXRT595S_cm33_INCLUDED)
    
    set(COMPONENT_TFA9896_ADAPTER_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "component_tfa9896_adapter component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_codec_adapter.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_tfa9896_MIMXRT595S_cm33)

    include(driver_codec_MIMXRT595S_cm33)

endif()
