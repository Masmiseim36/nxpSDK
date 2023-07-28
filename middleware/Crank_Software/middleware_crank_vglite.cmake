# Add set(CONFIG_USE_middleware_crank_vglite true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

if((CONFIG_USE_middleware_crank_template_evkmimxrt1040 OR CONFIG_USE_middleware_crank_template_evkbimxrt1050 OR CONFIG_USE_middleware_crank_template_evkmimxrt1060 OR CONFIG_USE_middleware_crank_template_evkbmimxrt1060 OR CONFIG_USE_middleware_crank_template_evkmimxrt1064 OR CONFIG_USE_middleware_crank_template_evkmimxrt595 OR CONFIG_USE_middleware_crank_template_evkmimxrt1170 OR CONFIG_USE_middleware_crank_template_evkbmimxrt1170) AND CONFIG_USE_middleware_freertos-kernel AND CONFIG_USE_middleware_vglite)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/include/freertos
  ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/include
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DCUSTOM_VGLITE_MEMORY_CONFIG=1
  )

endif()

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libgre.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libgreal.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libsbimage.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libsbvfs.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-animation.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-gesture.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-circle.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-greio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-logger.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-lua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-metrics.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-poly.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-trace.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-screen-drag.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-screen-path.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-timer.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libgre-plugin-sbimage-soil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/libfont-plugin-freetype.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-gcc/librender-plugin-vglite.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/liblogging-util.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libgreio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libswrender.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libsbfreetype.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libsblua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libsoil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libpng.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-gcc/libsbz.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm33)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libgre.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libgreal.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libsbimage.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libsbvfs.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-animation.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-gesture.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-circle.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-greio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-logger.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-lua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-metrics.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-poly.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-trace.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-screen-drag.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-screen-path.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-timer.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libgre-plugin-sbimage-soil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/libfont-plugin-freetype.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM33-iar/librender-plugin-vglite.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/liblogging-util.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libgreio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libswrender.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libsbfreetype.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libsblua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libsoil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libpng.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM33-iar/libsbz.a
      -Wl,--end-group
  )
endif()

if((CONFIG_TOOLCHAIN STREQUAL armgcc OR CONFIG_TOOLCHAIN STREQUAL mcux) AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libgre.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libgreal.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libsbimage.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libsbvfs.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-animation.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-circle.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-greio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-logger.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-lua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-metrics.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-poly.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-trace.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-screen-drag.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-screen-path.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-timer.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libgre-plugin-sbimage-soil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/libfont-plugin-freetype.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-gcc/librender-plugin-vglite.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/liblogging-util.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libgreio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libswrender.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libsbfreetype.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libsblua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libsoil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libpng.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-gcc/libsbz.a
      -Wl,--end-group
  )
endif()

if(CONFIG_TOOLCHAIN STREQUAL iar AND CONFIG_CORE STREQUAL cm7f)
  target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE
    -Wl,--start-group
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libgre.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libgreal.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libsbimage.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libsbvfs.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-animation.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-circle.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-greio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-logger.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-lua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-metrics.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-poly.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-trace.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-screen-drag.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-screen-path.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-timer.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libgre-plugin-sbimage-soil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/libfont-plugin-freetype.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/plugins/CM7-iar/librender-plugin-vglite.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/liblogging-util.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libgreio.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libswrender.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libsbfreetype.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libsblua.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libsoil.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libpng.a
      ${CMAKE_CURRENT_LIST_DIR}/cortexM-vglite/lib/CM7-iar/libsbz.a
      -Wl,--end-group
  )
endif()

else()

message(SEND_ERROR "middleware_crank_vglite dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()
