# Add set(CONFIG_USE_middleware_vglite_elementary true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_buffer.c
  ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_draw.c
  ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_init.c
  ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_object.c
  ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_os.c
  ${CMAKE_CURRENT_LIST_DIR}/elementary/src/elm_text.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/elementary/inc
  ${CMAKE_CURRENT_LIST_DIR}/elementary/src
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DVG_RENDER_TEXT=1
    -DRTOS
  )

endif()

