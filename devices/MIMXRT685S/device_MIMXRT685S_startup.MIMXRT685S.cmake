# Add set(CONFIG_USE_device_MIMXRT685S_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT685S_cm33.c
)

if(CONFIG_TOOLCHAIN STREQUAL iar)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/iar/startup_MIMXRT685S_cm33.s
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL armgcc)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/gcc/startup_MIMXRT685S_cm33.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/arm/startup_MIMXRT685S_cm33.S
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt685s.c
      ${CMAKE_CURRENT_LIST_DIR}/mcuxpresso/startup_mimxrt685s.cpp
  )
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

