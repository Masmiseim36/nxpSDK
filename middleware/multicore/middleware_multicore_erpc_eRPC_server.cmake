# Add set(CONFIG_USE_middleware_multicore_erpc_eRPC_server true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_multicore_erpc_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_simple_server.cpp
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_server.cpp
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_server_setup.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup
)

else()

message(SEND_ERROR "middleware_multicore_erpc_eRPC_server dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
