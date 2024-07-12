# Add set(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_binary true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_eiq_tensorflow_lite_micro_headers)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/tensorflow/lite/micro/debug_log.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/third_party/flatbuffers/include
  ${CMAKE_CURRENT_LIST_DIR}/third_party/gemmlowp
)

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm4f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm4/armgcc/libtflm.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm4f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm4/iar/tflm.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm4f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm4/mdk/tflm.lib
      -Wl,--end-group
  )
endif()

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm7/armgcc/libtflm.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm7/iar/tflm.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm7/mdk/tflm.lib
      -Wl,--end-group
  )
endif()

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm33/armgcc/libtflm.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm33/iar/tflm.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL mdk AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/cm33/mdk/tflm.lib
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL xcc AND CONFIG_CORE_ID STREQUAL hifi4)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/hifi4/xcc/libtflm.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL xcc AND CONFIG_CORE_ID STREQUAL fusionf1)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/lib/fusionf1/xcc/libtflm.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_eiq_tensorflow_lite_micro_binary dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
