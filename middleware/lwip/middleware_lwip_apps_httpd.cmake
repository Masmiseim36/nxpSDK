# Add set(CONFIG_USE_middleware_lwip_apps_httpd true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lwip AND CONFIG_USE_middleware_lwip_apps_httpd_support)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/apps/http/httpd.c
)

else()

message(SEND_ERROR "middleware_lwip_apps_httpd dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
