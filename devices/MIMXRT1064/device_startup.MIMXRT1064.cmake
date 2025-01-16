# Add set(CONFIG_USE_device_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL mcux)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1064.c "" device_startup.MIMXRT1064)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1064.cpp "" device_startup.MIMXRT1064)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1064.s "" device_startup.MIMXRT1064)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1064.S "" device_startup.MIMXRT1064)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1064.S "" device_startup.MIMXRT1064)
        endif()

  

