# Add set(CONFIG_USE_middleware_mcu-boot_src_startup true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/startup/startup.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/src/include
  ${CMAKE_CURRENT_LIST_DIR}/src/startup
)

else()

message(SEND_ERROR "middleware_mcu-boot_src_startup dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
