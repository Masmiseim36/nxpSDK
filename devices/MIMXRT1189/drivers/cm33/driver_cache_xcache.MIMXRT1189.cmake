# Add set(CONFIG_USE_driver_cache_xcache true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_CORE STREQUAL cm33) AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx) AND CONFIG_USE_driver_common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_cache.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "driver_cache_xcache.MIMXRT1189 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
