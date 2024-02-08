# Add set(CONFIG_USE_device_MIMXRT1189_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MIMXRT1189_system)

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1189_cm7.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT1189_cm33.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1189_cm7.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL armgcc AND CONFIG_CORE STREQUAL cm33)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT1189_cm33.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1189_cm7.s
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT1189_cm33.s
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm7f)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1189_cm7.c
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1189_cm7.cpp
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux AND CONFIG_CORE STREQUAL cm33)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1189_cm33.c
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt1189_cm33.cpp
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/boot_multicore_slave.c
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso
)
endif()

else()

message(SEND_ERROR "device_MIMXRT1189_startup.MIMXRT1189 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
