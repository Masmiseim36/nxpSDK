# Add set(CONFIG_USE_device_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE_ID STREQUAL cm4)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1166_cm4.c "" device_startup.MIMXRT1166)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1166_cm4.cpp "" device_startup.MIMXRT1166)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE_ID STREQUAL cm4)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1166_cm4.s "" device_startup.MIMXRT1166)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE_ID STREQUAL cm4)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1166_cm4.S "" device_startup.MIMXRT1166)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE_ID STREQUAL cm4)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1166_cm4.S "" device_startup.MIMXRT1166)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE_ID STREQUAL cm7)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1166_cm7.c "" device_startup.MIMXRT1166)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1166_cm7.cpp "" device_startup.MIMXRT1166)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE_ID STREQUAL cm7)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1166_cm7.s "" device_startup.MIMXRT1166)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE_ID STREQUAL cm7)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1166_cm7.S "" device_startup.MIMXRT1166)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE_ID STREQUAL cm7)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1166_cm7.S "" device_startup.MIMXRT1166)
        endif()

  

