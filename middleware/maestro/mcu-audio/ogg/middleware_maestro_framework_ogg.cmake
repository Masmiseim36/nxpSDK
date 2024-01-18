# Add set(CONFIG_USE_middleware_maestro_framework_ogg true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_osa_free_rtos AND CONFIG_USE_middleware_maestro_framework_opus)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/framing.c
  ${CMAKE_CURRENT_LIST_DIR}/src/memory.c
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

else()

message(SEND_ERROR "middleware_maestro_framework_ogg dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
