# Add set(CONFIG_USE_middleware_edgefast_bluetooth_pal_crypto_ethermind true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/crypto/bt_crypto.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/crypto
          ${CMAKE_CURRENT_LIST_DIR}/source/impl/ethermind/host
        )

  
