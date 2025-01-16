# Add set(CONFIG_USE_middleware_wireless_framework_pdum true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/services/PDUM/Include
        )

  
      if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU AND CONFIG_DSP STREQUAL NO_DSP)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/services/PDUM/Library/rw61x/libPDUM.a
        -Wl,--end-group
    )
    endif()

        if(CONFIG_CORE STREQUAL cm33 AND CONFIG_FPU STREQUAL SP_FPU)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
          ${CMAKE_CURRENT_LIST_DIR}/services/PDUM/Library/libPDUM.a
        -Wl,--end-group
    )
    endif()

  