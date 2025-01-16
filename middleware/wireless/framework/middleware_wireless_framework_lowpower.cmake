# Add set(CONFIG_USE_middleware_wireless_framework_lowpower true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/services/LowPower/PWR.c
        )

        if(CONFIG_USE_middleware_freertos-kernel)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/services/LowPower/PWR_systicks.c
        )
    endif()

        if(CONFIG_USE_component_osa_bm)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/services/LowPower/PWR_systicks_bm.c
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/services/LowPower
        )

  
