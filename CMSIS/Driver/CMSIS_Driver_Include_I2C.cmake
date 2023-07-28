# Add set(CONFIG_USE_CMSIS_Driver_Include_I2C true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

# template file
# ${CMAKE_CURRENT_LIST_DIR}/DriverTemplates/Driver_I2C.c

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/Include
)

