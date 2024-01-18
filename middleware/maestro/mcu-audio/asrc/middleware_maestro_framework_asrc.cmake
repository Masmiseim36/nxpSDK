# Add set(CONFIG_USE_middleware_maestro_framework_asrc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_CMSIS_DSP_Include)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/PL_InstAlloc.c
  ${CMAKE_CURRENT_LIST_DIR}/src/PL_MEM_Copy.c
  ${CMAKE_CURRENT_LIST_DIR}/src/PL_Mat_Utils.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/src
)

if(CONFIG_CORE STREQUAL cm7f AND (CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/../../libs/cm7f/armgcc/release/libasrc.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_maestro_framework_asrc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
