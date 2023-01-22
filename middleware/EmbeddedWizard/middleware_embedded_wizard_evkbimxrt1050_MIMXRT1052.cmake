include_guard()
message("middleware_embedded_wizard_evkbimxrt1050 component is included.")

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


