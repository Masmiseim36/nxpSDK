# Add set(CONFIG_USE_middleware_multicore_erpc_common_two_way_rpc_core0 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx) OR (CONFIG_BOARD STREQUAL evkmimxrt1180)))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_two_way_rpc/service/erpc_two_way_rpc_Core0Interface_client.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_two_way_rpc/service/c_erpc_two_way_rpc_Core0Interface_client.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_two_way_rpc/service/erpc_two_way_rpc_Core0Interface_interface.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_two_way_rpc/service/erpc_two_way_rpc_Core1Interface_server.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_two_way_rpc/service/c_erpc_two_way_rpc_Core1Interface_server.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_two_way_rpc/service/erpc_two_way_rpc_Core1Interface_interface.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_two_way_rpc/service
)

else()

message(SEND_ERROR "middleware_multicore_erpc_common_two_way_rpc_core0 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
