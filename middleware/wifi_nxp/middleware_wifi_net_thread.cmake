# Add set(CONFIG_USE_middleware_wifi_net_thread true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_osa_thread)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/incl/port/osa
  ${CMAKE_CURRENT_LIST_DIR}/port/osa
  ${CMAKE_CURRENT_LIST_DIR}/incl/port/net
  ${CMAKE_CURRENT_LIST_DIR}/port/net/netxduo
)

else()

message(SEND_ERROR "middleware_wifi_net_thread dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
