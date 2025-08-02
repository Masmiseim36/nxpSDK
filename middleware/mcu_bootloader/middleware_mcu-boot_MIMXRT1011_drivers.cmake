# Add set(CONFIG_USE_middleware_mcu-boot_MIMXRT1011_drivers true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/../../drivers/common
          ${CMAKE_CURRENT_LIST_DIR}/../../devices/RT/RT1010/MIMXRT1011/drivers
          ${CMAKE_CURRENT_LIST_DIR}/../../drivers/wdog01
          ${CMAKE_CURRENT_LIST_DIR}/../../drivers/rtwdog
          ${CMAKE_CURRENT_LIST_DIR}/../../drivers/igpio
          ${CMAKE_CURRENT_LIST_DIR}/../../drivers/lpuart
        )

  
