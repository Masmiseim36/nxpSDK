# Add set(CONFIG_USE_middleware_wifi_net true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_USE_middleware_wifi_net_free_rtos OR CONFIG_USE_middleware_wifi_net_thread))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/incl/port/osa
)

else()

message(SEND_ERROR "middleware_wifi_net dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
