# Add set(CONFIG_USE_middleware_multicore_erpc_common_multiprocessor_matrix_multiply_client true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_DEVICE_ID STREQUAL MIMXRT1165xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1173xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1175xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MCXN236 OR CONFIG_DEVICE_ID STREQUAL MCXN947 OR CONFIG_DEVICE_ID STREQUAL MCXN547 OR CONFIG_DEVICE_ID STREQUAL MCXC444 OR CONFIG_DEVICE_ID STREQUAL MCXC242) OR (CONFIG_BOARD STREQUAL frdmk22f OR CONFIG_BOARD STREQUAL frdmk32l2b OR CONFIG_BOARD STREQUAL evkmcimx7ulp OR CONFIG_BOARD STREQUAL frdmk32l3a6 OR CONFIG_BOARD STREQUAL lpcxpresso55s69 OR CONFIG_BOARD STREQUAL lpcxpresso55s36 OR CONFIG_BOARD STREQUAL evkmimxrt1160 OR CONFIG_BOARD STREQUAL evkmimxrt1170 OR CONFIG_BOARD STREQUAL evkbmimxrt1170 OR CONFIG_BOARD STREQUAL evkmimxrt685 OR CONFIG_BOARD STREQUAL evkmimxrt1060 OR CONFIG_BOARD STREQUAL evkmimxrt1180 OR CONFIG_BOARD STREQUAL mcxn9xxevk OR CONFIG_BOARD STREQUAL mcxn5xxevk OR CONFIG_BOARD STREQUAL frdmmcxn236 OR CONFIG_BOARD STREQUAL frdmmcxn947 OR CONFIG_BOARD STREQUAL frdmmcxc444 OR CONFIG_BOARD STREQUAL frdmmcxc242)))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/example/multiprocessor_examples/erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_client.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multiprocessor_examples/erpc_common/erpc_matrix_multiply/service/erpc_matrix_multiply_interface.cpp
  ${CMAKE_CURRENT_LIST_DIR}/example/multiprocessor_examples/erpc_common/erpc_matrix_multiply/service/c_erpc_matrix_multiply_client.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/example/multiprocessor_examples/erpc_common/erpc_matrix_multiply/service
)

else()

message(SEND_ERROR "middleware_multicore_erpc_common_multiprocessor_matrix_multiply_client dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
