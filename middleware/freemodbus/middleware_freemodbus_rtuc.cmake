# Add set(CONFIG_USE_middleware_freemodbus_rtuc true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/modbus/mb_m.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfunccoils_m.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncdisc_m.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncholding_m.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncinput_m.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncled_m.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncother.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbutils.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/rtu/mbcrc.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/rtu/mbrtu_m.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/ascii/mbascii.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/modbus
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions
          ${CMAKE_CURRENT_LIST_DIR}/modbus/include
          ${CMAKE_CURRENT_LIST_DIR}/modbus/rtu
          ${CMAKE_CURRENT_LIST_DIR}/modbus/ascii
        )

  
