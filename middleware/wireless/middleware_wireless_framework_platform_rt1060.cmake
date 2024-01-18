# Add set(CONFIG_USE_middleware_wireless_framework_platform_rt1060 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_fwk_debug)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/rt1060/fwk_platform.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/include
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/rt1060
  ${CMAKE_CURRENT_LIST_DIR}/framework/platform/rt1060/configs
)

else()

message(SEND_ERROR "middleware_wireless_framework_platform_rt1060 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
