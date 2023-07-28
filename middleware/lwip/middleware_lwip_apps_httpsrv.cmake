# Add set(CONFIG_USE_middleware_lwip_apps_httpsrv true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lwip AND CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_base64.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_fs.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_script.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_sha1.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_supp.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_task.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_tls.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_utf8.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_ws.c
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv/httpsrv_ws_api.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/httpsrv
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DUSE_RTOS=1
  )

endif()

else()

message(SEND_ERROR "middleware_lwip_apps_httpsrv dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
