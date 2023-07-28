# Add set(CONFIG_USE_middleware_embedded_wizard_template_evkbimxrt1050 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkbimxrt1050/RTE
  ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkbimxrt1050/RGB565
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050
)

