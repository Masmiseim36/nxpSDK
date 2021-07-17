include_guard(GLOBAL)
message("middleware_embedded_wizard_evkmimxrt1170 component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkmimxrt1170/ew_bsp_clock.c
    ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkmimxrt1170/ew_bsp_console.c
    ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkmimxrt1170/ew_bsp_display.c
    ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkmimxrt1170/ew_bsp_event.c
    ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkmimxrt1170/ew_bsp_inout.c
    ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkmimxrt1170/ew_bsp_system.c
    ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkmimxrt1170/ew_bsp_touch.c
    ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1170/RTE/ewextrte.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/evkmimxrt1170
    ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1170/RGB565
    ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/evkmimxrt1170/RTE
)


