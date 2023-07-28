# Add set(CONFIG_USE_middleware_openvg true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/inc
)

if((CONFIG_TOOLCHAIN STREQUAL iar OR CONFIG_TOOLCHAIN STREQUAL armgcc))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/libEGL.a
      ${CMAKE_CURRENT_LIST_DIR}/lib/libGAL.a
      ${CMAKE_CURRENT_LIST_DIR}/lib/libOpenVG.a
      -Wl,--end-group
  )
endif()

