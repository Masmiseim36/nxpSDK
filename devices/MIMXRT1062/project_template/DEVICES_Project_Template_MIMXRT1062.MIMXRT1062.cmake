# Add set(CONFIG_USE_DEVICES_Project_Template_MIMXRT1062 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_component_lpuart_adapter AND (CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA OR CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB) AND CONFIG_USE_device_MIMXRT1062_startup AND CONFIG_USE_driver_clock AND CONFIG_USE_driver_common AND CONFIG_USE_driver_igpio AND CONFIG_USE_driver_iomuxc AND CONFIG_USE_driver_lpuart AND CONFIG_USE_driver_nic301 AND ((CONFIG_USE_component_serial_manager AND CONFIG_USE_utility_assert AND CONFIG_USE_utility_debug_console) OR (CONFIG_USE_utility_assert_lite AND CONFIG_USE_utility_debug_console_lite)))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.h "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/board.c "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)

if(CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/clock_config.h "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/clock_config.c "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/pin_mux.h "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/pin_mux.c "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/peripherals.h "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA/peripherals.c "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
endif()

if(CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/clock_config.h "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/clock_config.c "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/pin_mux.h "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/pin_mux.c "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/peripherals.h "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
  add_config_file(${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB/peripherals.c "" DEVICES_Project_Template_MIMXRT1062.MIMXRT1062)
endif()

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxA)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxA
)
endif()

if(CONFIG_DEVICE_ID STREQUAL MIMXRT1062xxxxB)
target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/MIMXRT1062xxxxB
)
endif()

else()

message(SEND_ERROR "DEVICES_Project_Template_MIMXRT1062.MIMXRT1062 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
