include_guard()
message("component_tfa9xxx_adapter component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/tfa9xxx/fsl_codec_tfa9xxx_adapter.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/port/tfa9xxx
    ${CMAKE_CURRENT_LIST_DIR}/port
)


include(driver_tfa9xxx_MIMXRT685S_cm33)

include(driver_codec_MIMXRT685S_cm33)

