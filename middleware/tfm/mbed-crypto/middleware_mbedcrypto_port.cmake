# Add set(CONFIG_USE_middleware_mbedcrypto_port true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_casper.c
          ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_els.c
          ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_hashcrypt.c
          ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_pkc.c
          ${CMAKE_CURRENT_LIST_DIR}/port/mbedcrypto_mcux_sss.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/port
        )

  
