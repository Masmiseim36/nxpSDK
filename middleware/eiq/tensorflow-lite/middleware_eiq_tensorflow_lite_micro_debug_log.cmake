# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_debug_log true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/debug_log.cpp
        )

  

