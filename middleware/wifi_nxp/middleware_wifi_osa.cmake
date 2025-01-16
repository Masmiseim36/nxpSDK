# Add set(CONFIG_USE_middleware_wifi_osa true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/port/osa/osa.c
          ${CMAKE_CURRENT_LIST_DIR}/port/osa/mem_pool.c
          ${CMAKE_CURRENT_LIST_DIR}/port/osa/mem_pool_config.c
          ${CMAKE_CURRENT_LIST_DIR}/port/osa/slist.c
          ${CMAKE_CURRENT_LIST_DIR}/port/osa/stack_simple.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/incl/port
          ${CMAKE_CURRENT_LIST_DIR}/incl/port/osa
        )

  
