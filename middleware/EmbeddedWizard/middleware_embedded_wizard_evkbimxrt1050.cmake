# Add set(CONFIG_USE_middleware_embedded_wizard_evkbimxrt1050 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_embedded_wizard_template_evkbimxrt1050)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050/ew_bsp_clock.c
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050/ew_bsp_console.c
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050/ew_bsp_display.c
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050/ew_bsp_event.c
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050/ew_bsp_graphics.c
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050/ew_bsp_inout.c
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050/ew_bsp_system.c
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050/ew_bsp_touch.c
  ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkbimxrt1050/RGB565/ewextgfx.c
  ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkbimxrt1050/RTE/ewextrte.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkbimxrt1050
  ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkbimxrt1050/RGB565
  ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkbimxrt1050/RTE
)

else()

message(SEND_ERROR "middleware_embedded_wizard_evkbimxrt1050 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
