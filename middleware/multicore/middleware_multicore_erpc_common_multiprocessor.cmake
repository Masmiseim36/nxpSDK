# Add set(CONFIG_USE_middleware_multicore_erpc_common_multiprocessor true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx) OR (CONFIG_BOARD STREQUAL evkmimxrt1180)))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/example/multiprocessor_examples/erpc_common/erpc_error_handler.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/example/multiprocessor_examples/erpc_common
)

else()

message(SEND_ERROR "middleware_multicore_erpc_common_multiprocessor dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
