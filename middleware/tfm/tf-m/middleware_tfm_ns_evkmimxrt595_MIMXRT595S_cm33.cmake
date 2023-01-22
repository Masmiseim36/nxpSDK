include_guard()
message("middleware_tfm_ns_evkmimxrt595 component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt595
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt595/partition
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt595/Device/Include
)

include(middleware_tfm_ns_MIMXRT595S_cm33)

