
set(SOURCES
  Source/nvs.c
  Source/crc8.c
  Source/fwk_nvs_flash.c
  Source/fwk_nvs_InternalFlash.c
  Source/fwk_nvs_ExternalFlash.c

)

if(CONNFWK_IGNORE_NVS_INTERNAL_FLASH)
  list(REMOVE_ITEM SOURCES Source/fwk_nvs_InternalFlash.c)
endif()

if(CONNFWK_IGNORE_NVS_EXTERNAL_FLASH)
  list(REMOVE_ITEM SOURCES Source/fwk_nvs_ExternalFlash.c)
endif()

add_library(connfwk-NVS ${SOURCES})

target_include_directories(connfwk-NVS
  PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/Interface
  PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-NVS
  PRIVATE
      connfwk-config
      connfwk-FunctionLib
      ${CONNFWK_MCUX_SDK_LIB}
      ${CONNFWK_PLATFORM_LIB}

)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
  add_library(connfwk-NVS-coverage ${SOURCES})
  target_link_libraries(connfwk-NVS-coverage
    PRIVATE
      connfwk-config
      connfwk-FunctionLib
      ${CONNFWK_MCUX_SDK_LIB}
      ${CONNFWK_PLATFORM_LIB}
    )

  set(COVERAGE_COMPILE_OPTIONS
      --cs-on
      --cs-hit
      --cs-no-execution-time
  )

  target_compile_options(connfwk-NVS-coverage PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
      $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
  )
#    set_target_properties(connfwk-NVM-coverage PROPERTIES LINK_FLAGS ${link_flags})
  set_target_properties(connfwk-NVS-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()
