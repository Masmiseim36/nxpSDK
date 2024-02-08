# Add set(CONFIG_USE_driver_netc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_memory AND CONFIG_USE_driver_msgintr AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx) AND CONFIG_USE_driver_common AND (CONFIG_USE_driver_netc_rt1180))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_netc_ierb.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_netc_timer.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_netc_mdio.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_netc_endpoint.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_netc_switch.c
  ${CMAKE_CURRENT_LIST_DIR}/netc_hw/fsl_netc_hw.c
  ${CMAKE_CURRENT_LIST_DIR}/netc_hw/fsl_netc_hw_port.c
  ${CMAKE_CURRENT_LIST_DIR}/netc_hw/fsl_netc_hw_si.c
  ${CMAKE_CURRENT_LIST_DIR}/netc_hw/fsl_netc_hw_enetc.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "driver_netc.MIMXRT1189 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
