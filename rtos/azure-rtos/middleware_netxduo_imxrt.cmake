# Add set(CONFIG_USE_middleware_netxduo_imxrt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_azure_rtos_nxd)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/ports/netxduo/nx_driver_imxrt_irq.c
  ${CMAKE_CURRENT_LIST_DIR}/ports/netxduo/nx_driver_imxrt.c
)

else()

message(SEND_ERROR "middleware_netxduo_imxrt dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
