include_guard(GLOBAL)
message("component_cs42888_adapter component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/cs42888/fsl_codec_cs42888_adapter.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/cs42888
    ${CMAKE_CURRENT_LIST_DIR}/port
)


include(driver_cs42888_MIMXRT595S_cm33)

include(driver_codec_MIMXRT595S_cm33)

