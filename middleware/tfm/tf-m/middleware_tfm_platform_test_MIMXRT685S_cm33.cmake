include_guard()
message("middleware_tfm_platform_test component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/platform/ext/target/nxp/common/plat_test.c
)


