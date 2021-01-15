if(NOT MIDDLEWARE_EMBEDDED_WIZARD_MIMXRT1064_INCLUDED)
    
    set(MIDDLEWARE_EMBEDDED_WIZARD_MIMXRT1064_INCLUDED true CACHE BOOL "middleware_embedded_wizard component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT/ew_bsp_console.c
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT/ew_bsp_system.c
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT/ew_bsp_clock.c
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT/ew_bsp_event.c
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT/ew_bsp_graphics.c
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT/ew_bsp_display.c
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT/ew_bsp_inout.c
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT/ew_bsp_touch.c
        ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/RT/RGB565/ewextgfx.c
        ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/RT/RTE/ewextrte.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/TargetSpecific/RT
        ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/RT/RGB565
        ${CMAKE_CURRENT_LIST_DIR}/PlatformPackage/RT/RTE
    )


endif()
