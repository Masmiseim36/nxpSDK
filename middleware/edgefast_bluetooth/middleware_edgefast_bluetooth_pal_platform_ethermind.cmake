# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal_platform_ethermind true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_pal)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/platform/bt_ble_platform.c
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/platform/bt_ble_settings.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/platform
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_pal_platform_ethermind dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
