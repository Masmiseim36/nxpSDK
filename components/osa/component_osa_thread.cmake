# Add set(CONFIG_USE_component_osa_thread true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_common AND CONFIG_USE_component_lists AND CONFIG_USE_component_osa_interface AND (CONFIG_USE_middleware_azure_rtos_tx OR CONFIG_USE_middleware_azure_rtos_tx_sp))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_threadx.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_osa_thread dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
