# Add set(CONFIG_USE_middleware_mcu-boot_drv_ocotp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ocotp/bl_ocotp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/drivers/ocotp
)

else()

message(SEND_ERROR "middleware_mcu-boot_drv_ocotp dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
