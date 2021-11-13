include_guard(GLOBAL)
message("middleware_touch component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/controls/nt_control_arotary.c
    ${CMAKE_CURRENT_LIST_DIR}/source/controls/nt_control_aslider.c
    ${CMAKE_CURRENT_LIST_DIR}/source/controls/nt_control_keypad.c
    ${CMAKE_CURRENT_LIST_DIR}/source/controls/nt_control_matrix.c
    ${CMAKE_CURRENT_LIST_DIR}/source/controls/nt_control_proxi.c
    ${CMAKE_CURRENT_LIST_DIR}/source/controls/nt_control_rotary.c
    ${CMAKE_CURRENT_LIST_DIR}/source/controls/nt_control_slider.c
    ${CMAKE_CURRENT_LIST_DIR}/source/controls/nt_controls.c
    ${CMAKE_CURRENT_LIST_DIR}/source/electrodes/nt_electrodes.c
    ${CMAKE_CURRENT_LIST_DIR}/source/filters/nt_filters.c
    ${CMAKE_CURRENT_LIST_DIR}/source/keydetectors/nt_keydetector_afid.c
    ${CMAKE_CURRENT_LIST_DIR}/source/keydetectors/nt_keydetector_safa.c
    ${CMAKE_CURRENT_LIST_DIR}/source/keydetectors/nt_keydetector_usafa.c
    ${CMAKE_CURRENT_LIST_DIR}/source/modules/nt_module_gpio.c
    ${CMAKE_CURRENT_LIST_DIR}/source/modules/nt_module_gpioint.c
    ${CMAKE_CURRENT_LIST_DIR}/source/modules/nt_modules.c
    ${CMAKE_CURRENT_LIST_DIR}/source/nt.c
    ${CMAKE_CURRENT_LIST_DIR}/source/system/nt_system.c
    ${CMAKE_CURRENT_LIST_DIR}/source/system/nt_system_frmstr.c
    ${CMAKE_CURRENT_LIST_DIR}/source/system/nt_system_mem.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/source/controls
    ${CMAKE_CURRENT_LIST_DIR}/source/electrodes
    ${CMAKE_CURRENT_LIST_DIR}/source/filters
    ${CMAKE_CURRENT_LIST_DIR}/source/keydetectors
    ${CMAKE_CURRENT_LIST_DIR}/source/modules
    ${CMAKE_CURRENT_LIST_DIR}/source/system
    ${CMAKE_CURRENT_LIST_DIR}/source/safety
    ${CMAKE_CURRENT_LIST_DIR}/include
)


