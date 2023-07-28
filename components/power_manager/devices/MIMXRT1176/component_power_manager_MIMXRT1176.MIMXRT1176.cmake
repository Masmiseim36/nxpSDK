# Add set(CONFIG_USE_component_power_manager_MIMXRT1176 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_power_manager_core AND CONFIG_USE_driver_dcdc_soc AND CONFIG_USE_driver_gpc_3 AND CONFIG_USE_driver_pgmc AND CONFIG_USE_driver_soc_src AND CONFIG_USE_driver_pmu_1 AND CONFIG_USE_driver_ssarc AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1176xxxxx) AND CONFIG_USE_driver_clock)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/fsl_pm_device.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

else()

message(SEND_ERROR "component_power_manager_MIMXRT1176.MIMXRT1176 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
