# Add set(CONFIG_USE_device_system true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_CORE_ID STREQUAL cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT595S_cm33.c
        )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa) AND CONFIG_CORE STREQUAL dsp)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT595S_dsp.c
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
        )

  
