# Add set(CONFIG_USE_driver_xip_board_evkmimxrt1020 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_common AND (CONFIG_BOARD STREQUAL evkmimxrt1020))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/evkmimxrt1020_flexspi_nor_config.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "driver_xip_board_evkmimxrt1020 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
