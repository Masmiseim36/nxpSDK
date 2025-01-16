# Add set(CONFIG_USE_middleware_wireless_framework_seclib_rng true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/services/SecLib_RNG/SecLib_aes_mmo.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/services/SecLib_RNG
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  
            if(CONFIG_TOOLCHAIN STREQUAL iar)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
            )
      endif()
      
  endif()

      if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/services/SecLib_RNG/lib_crypto_m33_nodsp.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL NO_FPU AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/services/SecLib_RNG/lib_crypto_m33_nodsp_nofp.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/services/SecLib_RNG/lib_crypto_m33.a
        -Wl,--end-group
    )
    endif()

  