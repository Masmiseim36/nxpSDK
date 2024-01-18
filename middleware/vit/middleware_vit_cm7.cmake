# Add set(CONFIG_USE_middleware_vit_cm7 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt1040 OR CONFIG_BOARD STREQUAL evkbimxrt1050 OR CONFIG_BOARD STREQUAL evkmimxrt1060 OR CONFIG_BOARD STREQUAL evkbmimxrt1060 OR CONFIG_BOARD STREQUAL evkcmimxrt1060 OR CONFIG_BOARD STREQUAL evkmimxrt1064 OR CONFIG_BOARD STREQUAL evkmimxrt1160 OR CONFIG_BOARD STREQUAL evkmimxrt1170 OR CONFIG_BOARD STREQUAL evkbmimxrt1170))

if(CONFIG_TOOLCHAIN STREQUAL mcux)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/CortexM7/Lib
  ${CMAKE_CURRENT_LIST_DIR}/CortexM7/Lib/Inc
)
endif()

if(CONFIG_TOOLCHAIN STREQUAL mcux)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/CortexM7/Lib/libVIT_CM7_v04_09_00.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_vit_cm7 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
