# Add set(CONFIG_USE_board_project_template true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/board.c
          ${CMAKE_CURRENT_LIST_DIR}/clock_config.c
          ${CMAKE_CURRENT_LIST_DIR}/peripherals.c
        )

        if(CONFIG_CORE STREQUAL cm7f)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/cm7/pin_mux.c
        )
    endif()

        if(CONFIG_CORE STREQUAL cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/cm33/pin_mux.c
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

  
