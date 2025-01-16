# Add set(CONFIG_USE_driver_rtt true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/RTT/SEGGER_RTT.c
          ${CMAKE_CURRENT_LIST_DIR}/RTT/SEGGER_RTT_ASM_ARMv7M.S
          ${CMAKE_CURRENT_LIST_DIR}/RTT/SEGGER_RTT_printf.c
          ${CMAKE_CURRENT_LIST_DIR}/Syscalls/SEGGER_RTT_Syscalls_GCC.c
          ${CMAKE_CURRENT_LIST_DIR}/Syscalls/SEGGER_RTT_Syscalls_IAR.c
          ${CMAKE_CURRENT_LIST_DIR}/Syscalls/SEGGER_RTT_Syscalls_KEIL.c
          ${CMAKE_CURRENT_LIST_DIR}/Syscalls/SEGGER_RTT_Syscalls_SES.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/RTT
        )

  
