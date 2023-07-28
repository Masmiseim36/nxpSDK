# Add set(CONFIG_USE_driver_dsp true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_power AND CONFIG_USE_driver_common AND (CONFIG_DEVICE_ID STREQUAL MIMXRT685S))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_dsp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "driver_dsp.MIMXRT685S dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
