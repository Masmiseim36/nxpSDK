# Add set(CONFIG_USE_component_flexspi_nor_flash_adapter_rt1170 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt1170) AND CONFIG_USE_driver_common AND CONFIG_USE_driver_flexspi)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/mimxrt1170/fsl_adapter_flexspi_nor_flash.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_flexspi_nor_flash_adapter_rt1170.MIMXRT1176 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
