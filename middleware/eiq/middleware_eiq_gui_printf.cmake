# Add set(CONFIG_USE_middleware_eiq_gui_printf true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_worker)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/common/gprintf/chgui.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/common/gprintf
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DEIQ_GUI_PRINTF
  )

endif()

else()

message(SEND_ERROR "middleware_eiq_gui_printf dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
