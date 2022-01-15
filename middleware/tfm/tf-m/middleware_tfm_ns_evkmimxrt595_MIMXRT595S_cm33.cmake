include_guard(GLOBAL)
message("middleware_tfm_ns_evkmimxrt595 component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt595/Device/Include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt595
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt595/partition
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/evkmimxrt595/Device/Config
)

include(middleware_tfm_ns_MIMXRT595S_cm33)

