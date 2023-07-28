# Add set(CONFIG_USE_middleware_crank_evkbmimxrt1060 true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if(CONFIG_USE_middleware_crank_template_evkbmimxrt1060 AND CONFIG_USE_middleware_freertos-kernel AND (CONFIG_USE_middleware_crank_freetype OR CONFIG_USE_middleware_crank_sbfont))

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/cortexM7/include/freertos
  ${CMAKE_CURRENT_LIST_DIR}/cortexM7/include
)

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/libgre.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/libgreal.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/libsbimage.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/libsbvfs.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-animation.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-canvas.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-capture-playback.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-ccallback.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-circle.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-greio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-logger.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-lua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-luagredom.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-metrics.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-poly.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-rtext.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-screen-fade.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-screen-path.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-timer.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/libgre-plugin-sbimage-soil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/gcc/librender-plugin-swrender.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/liblogging-util.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/libgreio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/libswrender.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/libsblua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/gcc/libsoil.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/libgre.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/libgreal.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/libsbimage.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/libsbvfs.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-animation.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-canvas.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-capture-playback.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-ccallback.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-circle.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-greio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-logger.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-lua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-luagredom.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-metrics.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-poly.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-rtext.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-screen-fade.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-screen-path.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-timer.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/libgre-plugin-sbimage-soil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/plugins/iar/librender-plugin-swrender.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/liblogging-util.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/libgreio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/libswrender.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/libsblua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM7/lib/iar/libsoil.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_crank_evkbmimxrt1060 dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
