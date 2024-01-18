# Add set(CONFIG_USE_middleware_naturedsp_naturedsp_fusionf1 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt595 OR CONFIG_BOARD STREQUAL evkmimx8ulp))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/naturedsp/fusionf1/include
)

if((CONFIG_TOOLCHAIN STREQUAL xcc OR CONFIG_TOOLCHAIN STREQUAL xtensa))
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/naturedsp/fusionf1/libnaturedsp.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_naturedsp_naturedsp_fusionf1 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
