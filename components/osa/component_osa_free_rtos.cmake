# Add set(CONFIG_USE_component_osa_free_rtos true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel AND CONFIG_USE_driver_common AND CONFIG_USE_component_lists AND CONFIG_USE_component_osa_interface)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_os_abstraction_free_rtos.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_osa_free_rtos dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
