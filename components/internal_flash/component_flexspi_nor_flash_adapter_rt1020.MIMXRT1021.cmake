# Add set(CONFIG_USE_component_flexspi_nor_flash_adapter_rt1020 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt1020) AND CONFIG_USE_driver_common AND CONFIG_USE_driver_flexspi)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_flexspi_nor_flash_adapter_rt1020.MIMXRT1021 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
