include_guard()
message("component_cs42448_adapter component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/cs42448/fsl_codec_cs42448_adapter.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/port/cs42448
    ${CMAKE_CURRENT_LIST_DIR}/port
)


include(driver_cs42448_MIMXRT595S_cm33)

include(driver_codec_MIMXRT595S_cm33)

