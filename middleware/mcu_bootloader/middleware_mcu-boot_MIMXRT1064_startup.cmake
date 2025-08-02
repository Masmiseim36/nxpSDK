# Add set(CONFIG_USE_middleware_mcu-boot_MIMXRT1064_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1064/src/startup/system_MIMXRT1064.c
        )

        if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1064/src/startup/iar/startup_MIMXRT1064.s
        )
    endif()

        if((CONFIG_TOOLCHAIN STREQUAL mcux OR CONFIG_TOOLCHAIN STREQUAL armgcc))
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1064/src/startup/gcc/startup_MIMXRT1064.S
          ${CMAKE_CURRENT_LIST_DIR}/src/startup/crt0_gcc.S
        )
    endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk)
    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1064/src/startup/arm/startup_MIMXRT1064.s
        )
    endif()

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/../../devices/MIMXRT1064
        )

  
