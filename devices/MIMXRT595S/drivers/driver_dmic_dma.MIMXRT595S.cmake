# Add set(CONFIG_USE_driver_dmic_dma true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_driver_lpc_dma AND CONFIG_USE_driver_dmic)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_dmic_dma.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "driver_dmic_dma.MIMXRT595S dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
