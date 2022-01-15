include_guard(GLOBAL)
message("middleware_tfm_ns_mimxrt685audevk component is included.")


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/Device/Include
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/partition
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/mimxrt685audevk/Device/Config
)

include(middleware_tfm_ns_MIMXRT685S_cm33)

