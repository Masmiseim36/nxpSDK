# Add set(CONFIG_USE_middleware_wireless_framework_platform_mws_rt1170 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_platform_rt1170)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/rt1170
)

else()

message(SEND_ERROR "middleware_wireless_framework_platform_mws_rt1170 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
