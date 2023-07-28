# Add set(CONFIG_USE_driver_cmsis_enet true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_DEVICE_ID STREQUAL MIMXRT1176xxxxx) AND CONFIG_USE_driver_enet AND CONFIG_USE_CMSIS_Driver_Include_Ethernet_MAC AND CONFIG_USE_CMSIS_Driver_Include_Ethernet_PHY AND CONFIG_USE_CMSIS_Driver_Include_Ethernet AND CONFIG_USE_RTE_Device AND CONFIG_USE_driver_phy-common)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_cmsis.c
  ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_phy_cmsis.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "driver_cmsis_enet.MIMXRT1176 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
