# Add set(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal AND (CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_uart OR CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind_hci_platform))

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_common_ethermind_hci dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
