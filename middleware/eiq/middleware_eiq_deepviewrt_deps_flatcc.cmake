# Add set(CONFIG_USE_middleware_eiq_deepviewrt_deps_flatcc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkbmimxrt1170 OR CONFIG_BOARD STREQUAL evkmimxrt1170 OR CONFIG_BOARD STREQUAL evkmimxrt1160 OR CONFIG_BOARD STREQUAL evkbmimxrt1060 OR CONFIG_BOARD STREQUAL evkmimxrt1060 OR CONFIG_BOARD STREQUAL evkbimxrt1050 OR CONFIG_BOARD STREQUAL evkmimxrt1040 OR CONFIG_BOARD STREQUAL evkmimxrt1064))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/flatcc/include
)

else()

message(SEND_ERROR "middleware_eiq_deepviewrt_deps_flatcc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
