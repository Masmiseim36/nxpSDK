# Add set(CONFIG_USE_board_project_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/board.c
        )

        if(CONFIG_CORE_ID STREQUAL cm33_core0)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/clock_config.h ${CMAKE_CURRENT_LIST_DIR}/. board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/clock_config.c "" board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/pin_mux.h ${CMAKE_CURRENT_LIST_DIR}/. board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/pin_mux.c "" board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/peripherals.h ${CMAKE_CURRENT_LIST_DIR}/. board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core0/peripherals.c "" board_project_template)
        endif()

        if(CONFIG_CORE_ID STREQUAL cm33_core1)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/clock_config.h ${CMAKE_CURRENT_LIST_DIR}/. board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/clock_config.c "" board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/pin_mux.h ${CMAKE_CURRENT_LIST_DIR}/. board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/pin_mux.c "" board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/peripherals.h ${CMAKE_CURRENT_LIST_DIR}/. board_project_template)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/cm33_core1/peripherals.c "" board_project_template)
        endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1
              )
  
  
  endif()

