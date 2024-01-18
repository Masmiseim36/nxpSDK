# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND CONFIG_USE_middleware_edgefast_bluetooth_porting_work_queue AND CONFIG_USE_middleware_edgefast_bluetooth_porting_toolchain AND CONFIG_USE_middleware_edgefast_bluetooth_porting_net AND CONFIG_USE_middleware_edgefast_bluetooth_porting_list AND CONFIG_USE_middleware_edgefast_bluetooth_porting_atomic AND CONFIG_USE_component_log AND CONFIG_USE_component_log_backend_debugconsole)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/porting/ring_buffer.c
  ${CMAKE_CURRENT_LIST_DIR}/source/porting/porting.c
  ${CMAKE_CURRENT_LIST_DIR}/source/porting/utf8.c
  ${CMAKE_CURRENT_LIST_DIR}/source/porting/fifo/fifo.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/source/porting
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DLOG_ENABLE_ASYNC_MODE=1
    -DLOG_MAX_ARGUMENT_COUNT=10
    -DLOG_ENABLE_OVERWRITE=0
  )

endif()

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_porting dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
