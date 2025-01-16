# Add set(CONFIG_USE_device_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL mcux)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1062.c "" device_startup.MIMXRT1062)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1062.cpp "" device_startup.MIMXRT1062)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1062.s "" device_startup.MIMXRT1062)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1062.S "" device_startup.MIMXRT1062)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1062.S "" device_startup.MIMXRT1062)
        endif()

  

