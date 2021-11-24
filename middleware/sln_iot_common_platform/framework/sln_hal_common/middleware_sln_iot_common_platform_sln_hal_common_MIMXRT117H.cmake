if(NOT MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_HAL_COMMON_MIMXRT117H_INCLUDED)
    
    set(MIDDLEWARE_SLN_IOT_COMMON_PLATFORM_SLN_HAL_COMMON_MIMXRT117H_INCLUDED true CACHE BOOL "middleware_sln_iot_common_platform_sln_hal_common component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/hal_sln_lpm.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_camera_csi_gc0308.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_output_mqs_audio.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_camera_mipi_orbbecU1S.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_input_shell_cdc.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_camera_csi_orbbecU1S.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_output_audio_dump.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_display_usb_cdc.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_display_lpuart.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_graphics_pxp.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_camera_mipi_ov5640.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_output_ir_white_leds.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_camera_3d_sim.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_camera_csi_mt9m114.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_display_usb_uvc.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_sln_timer.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_output_console.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_display_lcdifv2_rk055ahd091.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_flash_littlefs.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_camera_flexio_gc0308.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_sln_platform.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_camera_rgb_sim.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_display_lcdif_rk024hh298.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_output_rgb_led.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_input_shell_uart.c
        ${CMAKE_CURRENT_LIST_DIR}/hal_input_push_buttons.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
        ${CMAKE_CURRENT_LIST_DIR}/inc
    )


endif()
