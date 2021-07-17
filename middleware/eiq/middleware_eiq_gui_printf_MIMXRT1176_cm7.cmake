include_guard(GLOBAL)
message("middleware_eiq_gui_printf component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/common/gprintf/chgui.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/common/gprintf
)


include(middleware_eiq_worker_MIMXRT1176_cm7)

