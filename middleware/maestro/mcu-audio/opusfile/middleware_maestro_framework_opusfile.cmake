# Add set(CONFIG_USE_middleware_maestro_framework_opusfile true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/info.c
  ${CMAKE_CURRENT_LIST_DIR}/src/internal.c
  ${CMAKE_CURRENT_LIST_DIR}/src/opusfile.c
  ${CMAKE_CURRENT_LIST_DIR}/src/stream.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/src
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DHAVE_CONFIG_H
  )

endif()

