# Add set(CONFIG_USE_middleware_wireless_EEPROM true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_wireless_SPI_Adapter)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/framework/Flash/External/Interface
)

else()

message(SEND_ERROR "middleware_wireless_EEPROM dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
