# Add set(CONFIG_USE_driver_lpspi_edma true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_edma AND CONFIG_USE_driver_lpspi AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1021xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_lpspi_edma.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "driver_lpspi_edma.MIMXRT1021 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
