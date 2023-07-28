# Add set(CONFIG_USE_component_cmsis_pack_flash_algo_MIMXRT1024 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1024xxxxx))

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/Sources/fsl_romapi.c
  ${CMAKE_CURRENT_LIST_DIR}/FlashDev.c
  ${CMAKE_CURRENT_LIST_DIR}/FlashPrg.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Sources
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_cmsis_pack_flash_algo_MIMXRT1024.MIMXRT1024 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
