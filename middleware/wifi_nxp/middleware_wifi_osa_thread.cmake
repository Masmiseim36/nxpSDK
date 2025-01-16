# Add set(CONFIG_USE_middleware_wifi_osa_thread true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/port/osa/osa_threadx.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/incl/port/osa
          ${CMAKE_CURRENT_LIST_DIR}/port/osa
        )

  
