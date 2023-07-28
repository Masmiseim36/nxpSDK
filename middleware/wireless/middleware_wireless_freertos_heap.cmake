# Add set(CONFIG_USE_middleware_wireless_freertos_heap true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_freertos-kernel)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/framework/Common/rtos/freertos/heap_mem_manager.c
)

else()

message(SEND_ERROR "middleware_wireless_freertos_heap dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
