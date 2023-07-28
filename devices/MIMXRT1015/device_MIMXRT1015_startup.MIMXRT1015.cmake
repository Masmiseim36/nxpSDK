# Add set(CONFIG_USE_device_MIMXRT1015_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MIMXRT1015_system)

if(CONFIG_TOOLCHAIN STREQUAL armgcc)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1015.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1015.c
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1015.cpp
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1015.s "" device_MIMXRT1015_startup.MIMXRT1015)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1015.S "" device_MIMXRT1015_startup.MIMXRT1015)
endif()

else()

message(SEND_ERROR "device_MIMXRT1015_startup.MIMXRT1015 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
