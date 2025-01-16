# Add set(CONFIG_USE_component_els_pkc_examples_hash_modes true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha1_longMsgOneshot_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha1_oneshot_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha1_streaming_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha224_oneshot_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha256_longMsgOneshot_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha256_oneshot_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha256_streaming_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha384_oneshot_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha512_224_oneshot_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha512_256_oneshot_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha512_256_streaming_example.c
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxClHashModes/mcuxClHashModes_sha512_oneshot_example.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxCsslFlowProtection/inc
          ${CMAKE_CURRENT_LIST_DIR}/examples/mcuxCsslMemory/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClExample/inc
          ${CMAKE_CURRENT_LIST_DIR}/src/comps/mcuxClBuffer/inc
        )

  
