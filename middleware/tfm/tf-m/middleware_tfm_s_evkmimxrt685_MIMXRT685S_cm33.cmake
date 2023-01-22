include_guard()
message("middleware_tfm_s_evkmimxrt685 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt685/target_cfg.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt685
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt685/Device/Include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt685/partition
)


include(middleware_tfm_s_MIMXRT685S_cm33)

