# Add set(CONFIG_USE_component_gpt_time_stamp_adapter true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_BOARD STREQUAL evkmimxrt1060 OR CONFIG_BOARD STREQUAL evkbmimxrt1060 OR CONFIG_BOARD STREQUAL evkcmimxrt1060) AND CONFIG_USE_driver_common AND CONFIG_USE_driver_gpt)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_adapter_gpt_time_stamp.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_gpt_time_stamp_adapter.MIMXRT1062 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
