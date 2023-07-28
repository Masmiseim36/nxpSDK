# Add set(CONFIG_USE_middleware_openh264_common true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/common_tables.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/copy_mb.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/cpu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/crt_util_safe_x.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/deblocking_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/expand_pic.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/intra_pred_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/mc.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/memory_align.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/sad_common.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/utils.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/welsCodecTrace.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/WelsTaskThread.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/WelsThread.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/WelsThreadLib.cpp
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/src/WelsThreadPool.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/codec/common/inc
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -D__NXP_MSDK__
  )

  if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      --diag_suppress Pe815,Pe111,Pe186,Pe068,Pe177
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL armgcc)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wno-format
      -Wno-strict-aliasing
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL mcux)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wno-format
      -Wno-strict-aliasing
    )
  endif()

endif()

