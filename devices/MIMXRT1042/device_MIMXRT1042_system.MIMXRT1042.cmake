# Add set(CONFIG_USE_device_MIMXRT1042_system true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_device_MIMXRT1042_CMSIS)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT1042.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "device_MIMXRT1042_system.MIMXRT1042 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
