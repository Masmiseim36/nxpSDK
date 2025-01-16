# Add set(CONFIG_USE_middleware_audio_voice_maestro_cci_enable true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/cci/src/ccidec.c
          ${CMAKE_CURRENT_LIST_DIR}/src/cci/src/codec_interface.c
          ${CMAKE_CURRENT_LIST_DIR}/src/cci/src/codecextractmetadata.c
          ${CMAKE_CURRENT_LIST_DIR}/src/parsers/cci/cci_codec_type_conversion.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/src/cci/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/parsers/cci
        )

  
