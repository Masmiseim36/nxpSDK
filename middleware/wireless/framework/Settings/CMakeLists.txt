
set(SOURCES
  Source/settings_init.c
  Source/settings.c
  Source/settings_store.c
  Source/settings_nvs.c
  Source/settings_runtime.c
)

add_library(connfwk-SettingsNvs ${SOURCES})

target_include_directories(connfwk-SettingsNvs
  PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/Interface
  PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Get common configs from the connfwk-config interface
target_link_libraries(connfwk-SettingsNvs
  PRIVATE
      connfwk-config
      connfwk-FunctionLib
      connfwk-NVS
      ${CONNFWK_MCUX_SDK_LIB}
      ${CONNFWK_PLATFORM_LIB}

)

# If the selected build type is for coverage, we build the same lib but with coverage instrumentation
if(${CMAKE_BUILD_TYPE} MATCHES "Coverage")
  add_library(connfwk-SettingsNvs-coverage ${SOURCES})
  target_link_libraries(connfwk-SettingsNvs-coverage PRIVATE connfwk-config connfwk-NVS)

  set(COVERAGE_COMPILE_OPTIONS
      --cs-on
      --cs-hit
      --cs-no-execution-time
  )

  target_compile_options(connfwk-SettingsNvs-coverage PRIVATE
      $<$<COMPILE_LANGUAGE:C>:${COVERAGE_COMPILE_OPTIONS}>
      $<$<COMPILE_LANGUAGE:CXX>:${COVERAGE_COMPILE_OPTIONS}>
  )
#    set_target_properties(connfwk-NVM-coverage PROPERTIES LINK_FLAGS ${link_flags})
  set_target_properties(connfwk-SettingsNvs-coverage PROPERTIES STATIC_LIBRARY_OPTIONS --cs-on)
endif()