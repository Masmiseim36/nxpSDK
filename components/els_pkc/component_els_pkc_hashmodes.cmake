# Add set(CONFIG_USE_component_els_pkc_hashmodes true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClHashModes/src/mcuxClHashModes_Core_c_sha1.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClHashModes/src/mcuxClHashModes_Core_els_sha2.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClHashModes/src/mcuxClHashModes_Internal_c_sha1.c
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClHashModes/src/mcuxClHashModes_Internal_els_sha2.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClHashModes/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClHashModes/inc/internal
        )

  
