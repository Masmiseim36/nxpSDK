include_guard()
message("component_wm8904_adapter component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/wm8904/fsl_codec_wm8904_adapter.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/port/wm8904
    ${CMAKE_CURRENT_LIST_DIR}/port
)


include(driver_wm8904_MIMXRT595S_cm33)

include(driver_codec_MIMXRT595S_cm33)

