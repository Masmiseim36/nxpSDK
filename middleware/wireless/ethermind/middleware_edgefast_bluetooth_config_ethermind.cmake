# Add set(CONFIG_USE_middleware_edgefast_bluetooth_config_ethermind true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/bluetooth/private/lib/mcux/default/config
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_config_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
