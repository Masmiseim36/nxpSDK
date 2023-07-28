# Add set(CONFIG_USE_device_MIMXRT1064_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MIMXRT1064_system)

if(CONFIG_TOOLCHAIN STREQUAL armgcc)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1064.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1064.c
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1064.cpp
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1064.s "" device_MIMXRT1064_startup.MIMXRT1064)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1064.S "" device_MIMXRT1064_startup.MIMXRT1064)
endif()

else()

message(SEND_ERROR "device_MIMXRT1064_startup.MIMXRT1064 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
