# Add set(CONFIG_USE_middleware_multicore_rpmsg_lite true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(((CONFIG_DEVICE_ID STREQUAL MIMXRT1181xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1182xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx) OR (CONFIG_BOARD STREQUAL evkmimxrt1180)) AND (CONFIG_USE_middleware_multicore_rpmsg_lite_bm OR CONFIG_USE_middleware_multicore_rpmsg_lite_freertos OR CONFIG_USE_middleware_multicore_rpmsg_lite_xos OR CONFIG_USE_middleware_multicore_rpmsg_lite_azurertos))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/common/llist.c
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/rpmsg_lite.c
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/rpmsg_lite/rpmsg_ns.c
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/virtio/virtqueue.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/rpmsg_lite/lib/include
)

else()

message(SEND_ERROR "middleware_multicore_rpmsg_lite dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
