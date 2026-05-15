# Add set(CONFIG_USE_utilities_misc_utilities true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL mdk) AND (CONFIG_CORE STREQUAL cm7f OR CONFIG_CORE STREQUAL cm33))
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/fsl_memcpy.S
        )
    endif()

  

