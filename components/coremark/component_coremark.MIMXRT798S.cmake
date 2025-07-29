# Add set(CONFIG_USE_component_coremark true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/core_list_join.c
          ${CMAKE_CURRENT_LIST_DIR}/core_main.c
          ${CMAKE_CURRENT_LIST_DIR}/core_matrix.c
          ${CMAKE_CURRENT_LIST_DIR}/core_state.c
          ${CMAKE_CURRENT_LIST_DIR}/core_util.c
          ${CMAKE_CURRENT_LIST_DIR}/simple/core_portme.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/simple
        )

  
