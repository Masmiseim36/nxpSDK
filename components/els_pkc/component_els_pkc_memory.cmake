# Add set(CONFIG_USE_component_els_pkc_memory true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMemory/src/mcuxClMemory.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Clear.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Compare.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Copy.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxCsslMemory/src/mcuxCsslMemory_Set.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMemory/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClMemory/inc/internal
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxCsslMemory/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxCsslMemory/inc/internal
        )

  
