# Add set(CONFIG_USE_middleware_audio_voice_maestro_file_utils_enable true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/utils/src/file_utils.c
        )

  

