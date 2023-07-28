# Add set(CONFIG_USE_middleware_multicore_erpc_eRPC_rpmsg_tty_rtos_transport true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_multicore_erpc_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/transports/erpc_rpmsg_tty_rtos_transport.cpp
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/setup/erpc_setup_mbf_rpmsg_tty.cpp
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port/erpc_threading_freertos.cpp
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra/erpc_framed_transport.cpp
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/transports
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/port
  ${CMAKE_CURRENT_LIST_DIR}/erpc/erpc_c/infra
)

else()

message(SEND_ERROR "middleware_multicore_erpc_eRPC_rpmsg_tty_rtos_transport dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
