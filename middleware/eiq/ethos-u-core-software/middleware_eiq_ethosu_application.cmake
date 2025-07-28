# Add set(CONFIG_USE_middleware_eiq_ethosu_application true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/applications/inference_process/src/inference_process.cpp
          ${CMAKE_CURRENT_LIST_DIR}/lib/ethosu_monitor/src/ethosu_monitor.cpp
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/applications/inference_process/include
          ${CMAKE_CURRENT_LIST_DIR}/lib/crc/include
          ${CMAKE_CURRENT_LIST_DIR}/lib/ethosu_log/include
          ${CMAKE_CURRENT_LIST_DIR}/lib/ethosu_monitor/include
        )

  
