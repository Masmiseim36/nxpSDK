# Add set(CONFIG_USE_middleware_wireless_framework_zephyr_common true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/zephyr/include
          ${CMAKE_CURRENT_LIST_DIR}/zephyr/include/zephyr/fs
          ${CMAKE_CURRENT_LIST_DIR}/zephyr/include/zephyr/settings
          ${CMAKE_CURRENT_LIST_DIR}/zephyr/include/zephyr/sys
          ${CMAKE_CURRENT_LIST_DIR}/zephyr/port/common
        )

  
