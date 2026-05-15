# Add set(CONFIG_USE_middleware_mcu-boot_MIMXRT1186_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_CORE STREQUAL cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1186/src/startup/system_MIMXRT1186_cm33.c
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1186/src/startup/iar/startup_MIMXRT1186_cm33.s
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1186/src/startup/gcc/startup_MIMXRT1186_cm33.S
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/../../devices/RT/RT1180/MIMXRT1186/arm/startup_MIMXRT1186_cm33.S
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT1186
        )

  
