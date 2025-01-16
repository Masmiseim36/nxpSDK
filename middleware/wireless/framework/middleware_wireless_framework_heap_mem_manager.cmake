# Add set(CONFIG_USE_middleware_wireless_framework_heap_mem_manager true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/Common/rtos/freertos/heap_mem_manager.c
        )

  

