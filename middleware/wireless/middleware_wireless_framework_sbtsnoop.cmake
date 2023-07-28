# Add set(CONFIG_USE_middleware_wireless_framework_sbtsnoop true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_framework_sbtsnoop_ethermind_port OR CONFIG_USE_middleware_wireless_framework_sbtsnoop_nxp_ble_port)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/DBG/sbtsnoop/sbtsnoop.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/DBG/sbtsnoop
)

else()

message(SEND_ERROR "middleware_wireless_framework_sbtsnoop dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
