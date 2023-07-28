# Add set(CONFIG_USE_middleware_tfmtests_ns true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/app/os_wrapper_cmsis_rtos_v2.c
  ${CMAKE_CURRENT_LIST_DIR}/app/main_ns.c
  ${CMAKE_CURRENT_LIST_DIR}/app/test_app.c
  ${CMAKE_CURRENT_LIST_DIR}/log/tfm_log_raw.c
  ${CMAKE_CURRENT_LIST_DIR}/ns_interface/ns_client_ext/tz_shim_layer.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/app
  ${CMAKE_CURRENT_LIST_DIR}/log
  ${CMAKE_CURRENT_LIST_DIR}/ns_interface
  ${CMAKE_CURRENT_LIST_DIR}/ns_interface/ns_client_ext
)

