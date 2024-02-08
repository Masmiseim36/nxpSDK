# Add set(CONFIG_USE_middleware_edgefast_bluetooth_k32w061_controller true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_edgefast_bluetooth_common_ethermind AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/controller/controller_k32w.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/port/pal/mcux/bluetooth/controller
)

else()

message(SEND_ERROR "middleware_edgefast_bluetooth_k32w061_controller dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
