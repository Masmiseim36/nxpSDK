# Add set(CONFIG_USE_middleware_mcuboot_tinycrypt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/aes_decrypt.c
          ${CMAKE_CURRENT_LIST_DIR}/source/aes_encrypt.c
          ${CMAKE_CURRENT_LIST_DIR}/source/cbc_mode.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ccm_mode.c
          ${CMAKE_CURRENT_LIST_DIR}/source/cmac_mode.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ctr_mode.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ctr_prng.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ecc.c
          ${CMAKE_CURRENT_LIST_DIR}/source/ecc_dsa.c
          ${CMAKE_CURRENT_LIST_DIR}/source/hmac.c
          ${CMAKE_CURRENT_LIST_DIR}/source/hmac_prng.c
          ${CMAKE_CURRENT_LIST_DIR}/source/sha256.c
          ${CMAKE_CURRENT_LIST_DIR}/source/utils.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/include
        )

  
