# Add set(CONFIG_USE_middleware_mcuboot_nxp_encrypted_xip true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/encrypted_xip_mcuboot_support.c
          ${CMAKE_CURRENT_LIST_DIR}/src/encrypted_xip.c
          ${CMAKE_CURRENT_LIST_DIR}/src/encrypted_xip_platform_bee.c
          ${CMAKE_CURRENT_LIST_DIR}/src/encrypted_xip_platform_iped.c
          ${CMAKE_CURRENT_LIST_DIR}/src/encrypted_xip_platform_npx.c
          ${CMAKE_CURRENT_LIST_DIR}/src/encrypted_xip_platform_otfad.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/include
        )

  
