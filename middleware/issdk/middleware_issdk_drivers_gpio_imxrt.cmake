# Add set(CONFIG_USE_middleware_issdk_drivers_gpio_imxrt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1181xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1182xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx/gpio_driver.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio/i.mx
  ${CMAKE_CURRENT_LIST_DIR}/drivers/gpio
)

else()

message(SEND_ERROR "middleware_issdk_drivers_gpio_imxrt dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
