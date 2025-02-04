# Add set(CONFIG_USE_middleware_mcu-boot_MIMXRT1064_drivers true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1064xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1064xxxxB))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT1064/src
)

else()

message(SEND_ERROR "middleware_mcu-boot_MIMXRT1064_drivers dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
