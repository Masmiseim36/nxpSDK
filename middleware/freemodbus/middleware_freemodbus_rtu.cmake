# Add set(CONFIG_USE_middleware_freemodbus_rtu true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/modbus/mb.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfunccoils.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncdiag.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncdisc.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncholding.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncinput.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncled.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncother.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbutils.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/rtu/mbcrc.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/rtu/mbrtu.c
          ${CMAKE_CURRENT_LIST_DIR}/modbus/ascii/mbascii.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/modbus
          ${CMAKE_CURRENT_LIST_DIR}/modbus/functions
          ${CMAKE_CURRENT_LIST_DIR}/modbus/include
          ${CMAKE_CURRENT_LIST_DIR}/modbus/rtu
          ${CMAKE_CURRENT_LIST_DIR}/modbus/ascii
        )

  
