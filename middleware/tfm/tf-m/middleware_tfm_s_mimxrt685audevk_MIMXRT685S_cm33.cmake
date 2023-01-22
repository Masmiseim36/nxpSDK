include_guard()
message("middleware_tfm_s_mimxrt685audevk component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/target_cfg.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/Device/Include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/partition
)


include(middleware_tfm_s_MIMXRT685S_cm33)

