# Add set(CONFIG_USE_middleware_motor_control_pmsm_float_imcat true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/freemaster/pmsm_float/mcat
)

else()

message(SEND_ERROR "middleware_motor_control_pmsm_float_imcat dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
