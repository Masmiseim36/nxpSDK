# Add set(CONFIG_USE_device_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE_ID STREQUAL cm33)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt595s_cm33.c "" device_startup.MIMXRT595S)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt595s_cm33.cpp "" device_startup.MIMXRT595S)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE_ID STREQUAL cm33)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT595S_cm33.s "" device_startup.MIMXRT595S)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE_ID STREQUAL cm33)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT595S_cm33.S "" device_startup.MIMXRT595S)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE_ID STREQUAL cm33)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT595S_cm33.S "" device_startup.MIMXRT595S)
        endif()

  

