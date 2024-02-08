# Add set(CONFIG_USE_middleware_multicore_erpc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_DEVICE_ID STREQUAL MIMXRT1181xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1182xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx) OR (CONFIG_BOARD STREQUAL evkmimxrt1180)))

else()

message(SEND_ERROR "middleware_multicore_erpc dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
