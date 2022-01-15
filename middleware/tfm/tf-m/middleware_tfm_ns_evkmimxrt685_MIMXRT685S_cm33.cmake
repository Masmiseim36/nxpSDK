include_guard(GLOBAL)
message("middleware_tfm_ns_evkmimxrt685 component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt685/Device/Include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt685
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt685/partition
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt685/Device/Config
)

include(middleware_tfm_ns_MIMXRT685S_cm33)

