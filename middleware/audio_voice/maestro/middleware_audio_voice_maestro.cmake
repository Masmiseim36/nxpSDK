# Add set(CONFIG_USE_middleware_audio_voice_maestro true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/core/src/pad.c
          ${CMAKE_CURRENT_LIST_DIR}/src/core/src/pipeline.c
          ${CMAKE_CURRENT_LIST_DIR}/src/core/src/streamer.c
          ${CMAKE_CURRENT_LIST_DIR}/src/core/src/streamer_element.c
          ${CMAKE_CURRENT_LIST_DIR}/src/core/src/streamer_msg.c
          ${CMAKE_CURRENT_LIST_DIR}/src/utils/src/general_utils.c
          ${CMAKE_CURRENT_LIST_DIR}/src/utils/src/maestro_logging.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/src/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/core/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/utils/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/elements
          ${CMAKE_CURRENT_LIST_DIR}/src/devices
        )

  
