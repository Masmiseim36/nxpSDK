# Add set(CONFIG_USE_middleware_eiq_deepviewrt_deps_json true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1042xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1052xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1064xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1176xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1166xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/flex.c
  ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/reader.c
  ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/safe.c
  ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/writer.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/deepviewrt/deps/json/include
)

else()

message(SEND_ERROR "middleware_eiq_deepviewrt_deps_json dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
