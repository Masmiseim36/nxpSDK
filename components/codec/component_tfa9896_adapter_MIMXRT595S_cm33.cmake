include_guard()
message("component_tfa9896_adapter component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/tfa9896/fsl_codec_tfa9896_adapter.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/port/tfa9896
    ${CMAKE_CURRENT_LIST_DIR}/port
)


include(driver_tfa9896_MIMXRT595S_cm33)

include(driver_codec_MIMXRT595S_cm33)

