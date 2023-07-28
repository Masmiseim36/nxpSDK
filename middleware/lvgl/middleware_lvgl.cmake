# Add set(CONFIG_USE_middleware_lvgl true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_lvgl_template)

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_disp.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_event.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_group.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_indev.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_indev_scroll.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_obj.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_obj_class.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_obj_draw.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_obj_pos.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_obj_scroll.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_obj_style.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_obj_style_gen.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_obj_tree.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_refr.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/core/lv_theme.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_layer.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_transform.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_arc.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_img.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_label.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_line.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_mask.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_rect.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_draw_triangle.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_img_buf.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_img_cache.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/lv_img_decoder.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/pxp/lv_draw_pxp.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/pxp/lv_draw_pxp_blend.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/pxp/lv_gpu_nxp_pxp.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/pxp/lv_gpu_nxp_pxp_osa.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/vglite/lv_draw_vglite.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/vglite/lv_vglite_buf.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/nxp/vglite/lv_vglite_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_arc.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_bg.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_composite.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_img.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_label.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_mask.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_polygon.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_rect.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_line.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sdl/lv_draw_sdl_layer.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/stm32_dma2d/lv_gpu_stm32_dma2d.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_arc.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_blend.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_dither.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_gradient.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_img.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_letter.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_line.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_polygon.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_rect.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_transform.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/draw/sw/lv_draw_sw_layer.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_dejavu_16_persian_hebrew.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_fmt_txt.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_loader.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_10.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_12.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_12_subpx.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_14.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_16.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_18.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_20.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_22.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_24.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_26.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_28.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_28_compressed.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_30.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_32.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_34.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_36.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_38.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_40.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_42.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_44.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_46.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_48.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_montserrat_8.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_simsun_16_cjk.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_unscii_16.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/font/lv_font_unscii_8.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/hal/lv_hal_disp.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/hal/lv_hal_indev.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/hal/lv_hal_tick.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_anim.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_anim_timeline.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_area.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_async.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_bidi.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_color.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_fs.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_gc.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_ll.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_log.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_math.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_mem.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_printf.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_style.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_style_gen.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_templ.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_timer.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_tlsf.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_txt_ap.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_txt.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_utils.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/misc/lv_lru.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_arc.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_bar.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_btn.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_btnmatrix.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_canvas.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_checkbox.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_dropdown.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_img.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_label.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_line.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_objx_templ.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_roller.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_slider.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_switch.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_table.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/widgets/lv_textarea.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/layouts/flex/lv_flex.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/layouts/grid/lv_grid.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/bmp/lv_bmp.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/freetype/lv_freetype.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/fsdrv/lv_fs_fatfs.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/fsdrv/lv_fs_posix.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/fsdrv/lv_fs_stdio.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/fsdrv/lv_fs_win32.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/gif/gifdec.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/gif/lv_gif.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/png/lodepng.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/png/lv_png.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/qrcode/lv_qrcode.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/qrcode/qrcodegen.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/rlottie/lv_rlottie.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/sjpg/lv_sjpg.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/sjpg/tjpgd.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/libs/ffmpeg/lv_ffmpeg.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/lv_extra.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/others/snapshot/lv_snapshot.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/others/gridnav/lv_gridnav.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/others/monkey/lv_monkey.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/others/fragment/lv_fragment.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/others/fragment/lv_fragment_manager.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/others/ime/lv_ime_pinyin.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/others/imgfont/lv_imgfont.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/others/msg/lv_msg.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/themes/basic/lv_theme_basic.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/themes/default/lv_theme_default.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/themes/mono/lv_theme_mono.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/animimg/lv_animimg.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/calendar/lv_calendar.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/calendar/lv_calendar_header_arrow.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/calendar/lv_calendar_header_dropdown.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/chart/lv_chart.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/colorwheel/lv_colorwheel.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/imgbtn/lv_imgbtn.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/keyboard/lv_keyboard.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/led/lv_led.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/list/lv_list.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/meter/lv_meter.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/msgbox/lv_msgbox.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/span/lv_span.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/spinbox/lv_spinbox.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/spinner/lv_spinner.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/tabview/lv_tabview.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/tileview/lv_tileview.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/win/lv_win.c
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src/extra/widgets/menu/lv_menu.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/lvgl
  ${CMAKE_CURRENT_LIST_DIR}/lvgl/src
  ${CMAKE_CURRENT_LIST_DIR}/.
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  if(CONFIG_TOOLCHAIN STREQUAL iar)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      --diag_suppress Pa089,Go029,Pe188,Pe111,Pe236,Pe546
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL armgcc)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wno-format
    )
  endif()
  if(CONFIG_TOOLCHAIN STREQUAL mcux)
    target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
      -Wno-format
    )
  endif()

endif()

else()

message(SEND_ERROR "middleware_lvgl dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
