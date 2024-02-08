# Add set(CONFIG_USE_middleware_sdmmc_osa_bm true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1181xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1182xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1187xxxxx OR CONFIG_DEVICE_ID STREQUAL MIMXRT1189xxxxx) AND CONFIG_USE_component_osa_bm)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/osa/fsl_sdmmc_osa.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/osa
)

else()

message(SEND_ERROR "middleware_sdmmc_osa_bm dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
