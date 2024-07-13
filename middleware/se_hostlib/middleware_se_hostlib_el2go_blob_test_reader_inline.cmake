# Add set(CONFIG_USE_middleware_se_hostlib_el2go_blob_test_reader_inline true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/nxp_iot_agent/tst/el2go_blob_test/src/el2go_blob_test_reader_inline.c
)

