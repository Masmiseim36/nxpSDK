# Add set(CONFIG_USE_middleware_maestro_framework_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/streamer_pcm.c "" middleware_maestro_framework_template)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/template/streamer_pcm.h "" middleware_maestro_framework_template)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/template
)

