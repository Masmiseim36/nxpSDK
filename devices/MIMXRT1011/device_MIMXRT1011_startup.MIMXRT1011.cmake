# Add set(CONFIG_USE_device_MIMXRT1011_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MIMXRT1011_system)

if(CONFIG_TOOLCHAIN STREQUAL iar)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1011.s "" device_MIMXRT1011_startup.MIMXRT1011)
endif()

if(CONFIG_TOOLCHAIN STREQUAL armgcc)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1011.S "" device_MIMXRT1011_startup.MIMXRT1011)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1011.S "" device_MIMXRT1011_startup.MIMXRT1011)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1011.c "" device_MIMXRT1011_startup.MIMXRT1011)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1011.cpp "" device_MIMXRT1011_startup.MIMXRT1011)
endif()

else()

message(SEND_ERROR "device_MIMXRT1011_startup.MIMXRT1011 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
