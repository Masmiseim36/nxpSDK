# Add set(CONFIG_USE_middleware_naturedsp_naturedsp_hifi4 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt685 OR CONFIG_BOARD STREQUAL mimxrt685audevk))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/naturedsp/hifi4/feature.c
  ${CMAKE_CURRENT_LIST_DIR}/naturedsp/hifi4/math_stdlib.c
  ${CMAKE_CURRENT_LIST_DIR}/naturedsp/hifi4/version.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/naturedsp/hifi4/include
  ${CMAKE_CURRENT_LIST_DIR}/naturedsp/hifi4/include_private
)

if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/naturedsp/hifi4/libnaturedsp.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_naturedsp_naturedsp_hifi4 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
