# Add set(CONFIG_USE_component_flexspi_hyper_flash_adapter_rt1050 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkbimxrt1050) AND CONFIG_USE_driver_common AND CONFIG_USE_driver_flexspi)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/hyper_flash/fsl_adapter_flexspi_hyper_nor_flash.c
  ${CMAKE_CURRENT_LIST_DIR}/hyper_flash/RT1050/fsl_adapter_flexspi_hyper_flash_config.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/hyper_flash/RT1050
)

else()

message(SEND_ERROR "component_flexspi_hyper_flash_adapter_rt1050.MIMXRT1052 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
