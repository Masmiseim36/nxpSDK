# Add set(CONFIG_USE_middleware_multicore_erpc_common_multicore_matrix_multiply_client true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx) OR (CONFIG_BOARD STREQUAL evkmimxrt1180)))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_client.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_interface.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_matrix_multiply/service/c_erpc_matrix_multiply_client.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/example/multicore_examples/erpc_common/erpc_matrix_multiply/service
)

else()

message(SEND_ERROR "middleware_multicore_erpc_common_multicore_matrix_multiply_client dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
