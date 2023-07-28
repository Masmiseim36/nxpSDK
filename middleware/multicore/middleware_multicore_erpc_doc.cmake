# Add set(CONFIG_USE_middleware_multicore_erpc_doc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_multicore_erpc_common)

else()

message(SEND_ERROR "middleware_multicore_erpc_doc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
