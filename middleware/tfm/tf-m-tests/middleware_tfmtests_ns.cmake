# Add set(CONFIG_USE_middleware_tfmtests_ns true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/app_broker/os_wrapper_cmsis_rtos_v2.c
          ${CMAKE_CURRENT_LIST_DIR}/app_broker/syscalls_stub.c
          ${CMAKE_CURRENT_LIST_DIR}/app_broker/main_ns.c
          ${CMAKE_CURRENT_LIST_DIR}/lib/log/tfm_log_raw.c
          ${CMAKE_CURRENT_LIST_DIR}/lib/nsid_manager/tfm_nsid_manager.c
          ${CMAKE_CURRENT_LIST_DIR}/lib/nsid_manager/tz_shim_layer.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/app_broker
          ${CMAKE_CURRENT_LIST_DIR}/lib
          ${CMAKE_CURRENT_LIST_DIR}/lib/os_wrapper
          ${CMAKE_CURRENT_LIST_DIR}/lib/log
          ${CMAKE_CURRENT_LIST_DIR}/lib/nsid_manager
        )

  
