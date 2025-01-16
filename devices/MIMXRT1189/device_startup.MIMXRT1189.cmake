# Add set(CONFIG_USE_device_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE_ID STREQUAL cm33)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/image_hash_tool.py "" device_startup.MIMXRT1189)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/readme.txt "" device_startup.MIMXRT1189)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1189_cm33.c "" device_startup.MIMXRT1189)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1189_cm33.cpp "" device_startup.MIMXRT1189)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/boot_multicore_slave.c "" device_startup.MIMXRT1189)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/boot_multicore_slave.h "" device_startup.MIMXRT1189)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE_ID STREQUAL cm33)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1189_cm33.s "" device_startup.MIMXRT1189)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE_ID STREQUAL cm33)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1189_cm33.S "" device_startup.MIMXRT1189)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE_ID STREQUAL cm33)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1189_cm33.S "" device_startup.MIMXRT1189)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE_ID STREQUAL cm7)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1189_cm7.c "" device_startup.MIMXRT1189)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1189_cm7.cpp "" device_startup.MIMXRT1189)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE_ID STREQUAL cm7)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1189_cm7.s "" device_startup.MIMXRT1189)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE_ID STREQUAL cm7)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1189_cm7.S "" device_startup.MIMXRT1189)
        endif()

        if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE_ID STREQUAL cm7)
          add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1189_cm7.S "" device_startup.MIMXRT1189)
        endif()

  

