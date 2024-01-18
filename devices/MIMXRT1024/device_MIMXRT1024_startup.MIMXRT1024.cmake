# Add set(CONFIG_USE_device_MIMXRT1024_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MIMXRT1024_system)

if(CONFIG_TOOLCHAIN STREQUAL iar)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1024.s "" device_MIMXRT1024_startup.MIMXRT1024)
endif()

if(CONFIG_TOOLCHAIN STREQUAL armgcc)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1024.S "" device_MIMXRT1024_startup.MIMXRT1024)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1024.S "" device_MIMXRT1024_startup.MIMXRT1024)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1024.c "" device_MIMXRT1024_startup.MIMXRT1024)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1024.cpp "" device_MIMXRT1024_startup.MIMXRT1024)
endif()

else()

message(SEND_ERROR "device_MIMXRT1024_startup.MIMXRT1024 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
